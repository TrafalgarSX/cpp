#include"uploadFile.h"
#include<thread>
#include<sstream>

int PutUpload(const string& filePath, const string& url) {
	int result;
	CURL* curl;
	CURLcode res;
	struct stat file_info;
	curl_off_t speed_upload, total_time;
	FILE* fd;

	result = fopen_s(&fd, filePath.c_str(), "rb");
	if (!fd) {
		return OPEN_FILE_FAILED;
	}

	/* to get the file size */
	if (fstat(_fileno(fd), &file_info) != 0) {
		return FILE_INFO_FAILED;
	}

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		
		/* tall it to "upload" to the URL*/
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		/* set where to read from (on Windows you need to use READFUNCTION too)*/
		curl_easy_setopt(curl, CURLOPT_READDATA, fd);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &fread);
		/* and give the size of the upload (optional)*/
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
		/* enable verbose for easier tracing*/
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			return res;
		}
		else {
			/* now extract transfer info */
			curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);

			fprintf(stderr, "Speed: %lu bytes/sec during %1u.%06lu seconds\n",
				(unsigned long)speed_upload,
				(unsigned long)(total_time / 1000000),
				(unsigned long)(total_time % 1000000));
		}
		curl_easy_cleanup(curl);
	}
	fclose(fd);
	return 0;
}
int PostUpload(const string& filePath, const string& url) {
	int result = -1;
	CURL* curl = NULL;
	CURLcode res = CURLE_FAILED_INIT;
	char errbuf[CURL_ERROR_SIZE] = { 0 };
	FILE* fp = NULL;
	struct stat file_info;
	struct progress_data progress_data = { 0 };
	double average_speed = 0;
	double bytes_uploaded = 0;
	double total_upload_time = 0;
	long response_code = 0;
	struct curl_slist* header_list = NULL;
	
	if (filePath.empty()) {
		fprintf(stderr, "Error: post_data_filename parameter is missing.\n");
		goto cleanup;
	}

	if (url.empty()) {
		fprintf(stderr, "Error: url parameter is missing.\n");
		goto cleanup;
	}

	result = fopen_s(&fp, filePath.c_str(), "rb");
	if (!fp) {
		fprintf(stderr, "Error: failed to open file \"%s\"\n", filePath.c_str());
		goto cleanup;
	}

	/* to get the file size*/
	if (fstat(_fileno(fp), &file_info) != 0) {
		fprintf(stderr, "Error: unknown file size \"%s\"\n", filePath.c_str());
		return FILE_INFO_FAILED;
	}
	/*
	if (!(0 <= file_info.st_size && file_info.st_size <= CURL_OFF_T_MAX)) {
		fprintf(stderr, "Error: file size too large \"%s\"\n", filePath);
		//goto cleanup;
	}*/
	
	curl = curl_easy_init();
	
	if (!curl) {
		fprintf(stderr, "Error: curl_easy_init failed.\n");
		goto cleanup;
	}

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
		(curl_off_t)file_info.st_size);
	
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(curl, CURLOPT_READDATA, (void*)fp);

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progress_data);
	progress_data.session = curl;

	header_list = curl_slist_append(header_list, "Expect:");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

	res = curl_easy_perform(curl);
	
	if (res != CURLE_OK) {
		size_t len = strlen(errbuf);
		fprintf(stderr, "\nError: libcurl: (%d) ", res);
		if (len)
			fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
		fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
		goto cleanup;
	}

	curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &average_speed);
	curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &bytes_uploaded);
	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_upload_time);

	fprintf(stderr, "\nTransfer rate: %.0f KB/sec"
		" (%.0f bytes in %.0f seconds)\n",
		average_speed / 1024, bytes_uploaded, total_upload_time);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

	if (response_code != 200) {
		fprintf(stderr, "Error: HTTP response code is %ld.\n", response_code);
		result = response_code;
	}

cleanup:
	curl_easy_cleanup(curl);
	curl_slist_free_all(header_list);
	if (NULL != fp) {
		fclose(fp);
	}
	return result;
}

//formadd此功能curl已弃用，可实现功能，但不推荐使用，请改用curl_mime_init()
int multiUpload(const string& filePath, const string& url) {
	int result = -1;
	CURL* curl = NULL;
	CURLcode res = CURLE_FAILED_INIT;
	char errbuf[CURL_ERROR_SIZE] = { 0 };
	long response_code = 0;
	string response;
	struct curl_slist* header_list = NULL;
	curl_httppost *form_post = NULL;
	curl_httppost* lastptr = NULL;
	
	if (filePath.empty()) {
		fprintf(stderr, "Error: post_data_filename parameter is missing.\n");
		goto cleanup;
	}

	if (url.empty()) {
		fprintf(stderr, "Error: url parameter is missing.\n");
		goto cleanup;
	}

	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Error: curl_easy_init failed.\n");
		goto cleanup;
	}

#ifdef _DEBUG
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif // _DEBUG
	
	/* Add simple name/content section*/
	curl_formadd(&form_post,
		&lastptr,
		CURLFORM_COPYNAME, "name",
		CURLFORM_COPYCONTENTS, "guoyawen",
		CURLFORM_END);

	curl_formadd(&form_post,
		&lastptr,
		CURLFORM_COPYNAME, "logFile",
		CURLFORM_FILE, filePath.c_str(),
		CURLFORM_CONTENTTYPE, "file",
		CURLFORM_END);


	header_list = curl_slist_append(NULL, "Except:");
	
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, form_post);

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

	//server return data
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	res = curl_easy_perform(curl);
	
	if (res != CURLE_OK) {
		size_t len = strlen(errbuf);
		fprintf(stderr, "\nError: libcurl: (%d) ", res);
		if (len)
			fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
		fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
		goto cleanup;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	fprintf(stderr, "%s", response.c_str());
	if (response_code != 200) {
		fprintf(stderr, "Error: HTTP response code is %ld.\n", response_code);
		result = response_code;
	}
cleanup:
	curl_easy_cleanup(curl);
	curl_formfree(form_post);
	curl_slist_free_all(header_list);
	return result;
}

/* HTTP Multipart formpost with file upload and two additional parts. */
int mimeUpload(const string& filePath, const string& url) {
	int result = -1;
	CURL* curl = NULL;
	CURLcode res = CURLE_FAILED_INIT;
	char errbuf[CURL_ERROR_SIZE] = { 0 };
	long response_code = 0;
	string response;
	struct curl_slist* header_list = NULL;
	curl_mime* form = NULL;
	curl_mimepart* field = NULL;
	ostringstream thread_id;
	
	if (filePath.empty()) {
		fprintf(stderr, "Error: post_data_filename parameter is missing.\n");
		goto cleanup;
	}

	if (url.empty()) {
		fprintf(stderr, "Error: url parameter is missing.\n");
		goto cleanup;
	}

	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "Error: curl_easy_init failed.\n");
		goto cleanup;
	}

#ifdef _DEBUG
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 0);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif // _DEBUG
	/* create the form */
	form = curl_mime_init(curl);

	/* Fill in the file upload field */
	field = curl_mime_addpart(form);
	curl_mime_name(field, "logFile");
	curl_mime_filedata(field, filePath.c_str());

	/* Fill in the name field */

	thread_id << this_thread::get_id();
	field = curl_mime_addpart(form);
	curl_mime_name(field, "fileName");
	curl_mime_data(field, thread_id.str().c_str(), CURL_ZERO_TERMINATED);

	/* Fill in the userId field */
	field = curl_mime_addpart(form);
	curl_mime_name(field, "userId");
	curl_mime_data(field, "Trafalgar", CURL_ZERO_TERMINATED);

	curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

	/* initialize custom header list (stating that Expect: 100-continue is not wanted) */
	header_list = curl_slist_append(header_list, "Expect:");


	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

	//server return data
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


	res = curl_easy_perform(curl);
	
	if (res != CURLE_OK) {
		size_t len = strlen(errbuf);
		fprintf(stderr, "\nError: libcurl: (%d) ", res);
		if (len)
			fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
		fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
		goto cleanup;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	fprintf(stderr, "%s", response.c_str());
	if (response_code != 200) {
		fprintf(stderr, "Error: HTTP response code is %ld.\n", response_code);
		result = response_code;
	}
cleanup:
	curl_easy_cleanup(curl);
	curl_mime_free(form);
	curl_slist_free_all(header_list);
	return result;
}

int main(void) {
	int result = -1;
	string url = string("http://localhost:8088/upload");
	string file_path = string("D:/workspace/dev/include/gtest/gtest-spi.h");
	string file_path_other = string("D:/workspace/dev/include/gtest/gtest.h");
	std::thread thread_group[10];
	int thread_count = 10;

	curl_global_init(CURL_GLOBAL_ALL);
	//result = PostUpload(filePath, url); //binary  only upload one file;
	//result = PutUpload(filePath, url);  //binary  only upload one file;
	url = "http://localhost:9458/multiUpload";
	//result = multiUpload(filePath, url); // 服务端使用MultipartFile接收文件, curl已弃用该方法

	
	for (int i = 0; i < thread_count; i++) {
		thread_group[i] = std::thread(mimeUpload, file_path, url);
	}
	for (int i = 0; i < thread_count; i++) {
		thread_group[i].join();
	}

	//result = mimeUpload(filePath, url);   //服务端使用MultipartFile接收文件, 可上传多个文件
	printf("test marco %d", CURLE_OK);
	if (0x00 != result) {
		fprintf(stderr, "Fatal: PostUpload failed.\n");
		return EXIT_FAILURE;
	}
	curl_global_cleanup();
	return result;
}

size_t read_callback(char* buffer, size_t size, size_t nitems, void* instream)
{
	size_t bytes_read;

	/* I'm doing it this way to get closer to what the reporter is doing.
	   Technically we don't need to do this, we could just use the default read
	   callback which is fread. Also, 'size' param is always set to 1 by libcurl
	   so it's fine to pass as buffer, size, nitems, instream. */
	bytes_read = fread(buffer, 1, (size * nitems), (FILE*)instream);

	return bytes_read;
}

int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
	curl_off_t ultotal, curl_off_t ulnow) {
	int percent;
	time_t timenow;
	struct progress_data* d = (struct progress_data*)clientp;

	(void)dltotal;
	(void)dlnow;

	if(d->type == PROGRESS_DONE || ultotal <= 0 || ulnow < 0) {
		return 0;
	}else if(!ulnow) {
		percent = 0;
	}else if(ulnow >= ultotal){
		percent = 100;
	}else if(ultotal < 10000) {
		percent = (int)(ulnow * 100 / ultotal);
	}else{
		percent = (int)(ulnow /(ultotal /100));
	}

	if(d->type == PROGRESS_STARTED && d->percent == percent) {
		return 0;
	}

	timenow = time(NULL);

	if(timenow == d->time && percent != 100) {
		return 0;
	}

	fprintf(stderr, "\r%3d%% uploaded...", percent) ;

	d->percent = percent;
	d->time = timenow;
	if(percent == 100) {
		fprintf(stderr, "\n\n") ;
		d->type = PROGRESS_DONE;
	}else{
		d->type = PROGRESS_STARTED;
	}

	return 0;
}

/* 使用C++的string 类 */
size_t write_callback(void* buffer, size_t sz, size_t nmemb, void *ResInfo) {
	string* response = (string*)ResInfo; //强制转换
	response->append((char*)buffer, sz * nmemb); //sz * nmemb表示接受数据的多少
	return sz * nmemb;
}

/* 自定义结构体，可自动扩容，动态增长，接收任意长度的数据 */
size_t cb(void* buffer, size_t size, size_t nmemb, void* clientp) {
	size_t realsize = size * nmemb; //接受数据的大小
	struct Response* resp = (struct Response*)clientp; //强制转换

	char* ptr = (char*)realloc(resp ->reponse, resp->size + realsize + 1);
	if (NULL == ptr) {
		return 0; /* out of memory */
	}

	resp->reponse = ptr;
	memcpy(&(resp->reponse[resp->size]), buffer, realsize);
	resp->size += realsize;
	resp->reponse[resp->size] = 0;

	return realsize;
}
/**/
