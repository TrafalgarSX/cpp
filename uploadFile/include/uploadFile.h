#ifndef _UPLOAD_FILE_H_
#define _UPLOAD_FILE_H_
#include<iostream>
#include<string>
#include<curl/curl.h>
#include<sys/stat.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#ifdef _WIN32
#include <io.h>
#else
include<unistd.h>
#endif // _WIN32

using namespace std;

#define OPEN_FILE_FAILED  50000001
#define FILE_INFO_FAILED  50000002

//test

#ifndef CURL_OFF_T_MAX
/* https://github.com/curl/curl/pull/6704#issuecomment-796566199 */
#define CURL_OFF_T_MAX ((curl_off_t)(((unsigned CURL_TYPEOF_CURL_OFF_T)-1)/2))
#define CURL_OFF_T_MIN ((curl_off_t)(-CURL_OFF_T_MAX - 1))
#endif

enum progress_type {
	PROGRESS_NONE = 0,  /* NONE must stay at 0 */
	PROGRESS_STARTED,
	PROGRESS_DONE
};
/* to initialize zero out the struct then set session */
struct progress_data {
	CURL* session;             /* curl easy_handle to the calling session */
	int percent;               /* last percent output to stderr */
	time_t time;               /* last time progress was output to stderr */
	enum progress_type type;
};

struct Response{
	char* reponse;
	size_t size;
};

int PostUpload(const string& filePath, const string& url);
int PutUpload(const string & filePath, const string& url);
int multiUpload(const string& filePath, const string& url);
int mimeUpload(const string& filePath, const string& url);

size_t write_callback(void* buffer, size_t sz, size_t nmemb, void* ResInfo);
size_t cb(void* buffer, size_t size, size_t nmemb, void* clientp);
size_t read_callback(char* buffer, size_t size, size_t nitems, void* instream);
int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
	curl_off_t ultotal, curl_off_t ulnow);


#endif //_UPLOAD_FILE_H_
