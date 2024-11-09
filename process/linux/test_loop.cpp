#include <iostream>
#include <string>

#include <array>
#include <fstream>

#include <unistd.h>

void protectSelf()
{
    int fd;
    ssize_t bytes_written;
    const char *device_path = "/dev/chardev_example"; // 设备文件路径
    pid_t pid = getpid();

    std::string data_to_write = "";
    data_to_write += std::to_string(pid);

    // 打开设备文件
    fd = open(device_path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open the device file");
        return 1;
    }

    // 写入数据到设备文件
    bytes_written = write(fd, data_to_write.c_str(), data_to_write.size() + 1);
    if (bytes_written == -1) {
        perror("Failed to write to the device file");
        close(fd);
    }

    printf("Data written to the device file: %s", data_to_write.c_str());
    // 关闭设备文件
    close(fd);
}

void AddToTestDriverFile(int pid, std::array<long long, 2> & address)
{
    // Open the file in append mode. If the file does not exist, it will be created.
    std::ofstream ofile("./test_driver.txt", std::ios::app);

    // Check if the file was opened successfully
    if (!ofile)
    {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Write the data to the file
    ofile << pid << " " << std::to_string(address[0]) << " " << std::to_string(address[1]) << std::endl;

    // Close the file
    ofile.close();

}

int main()
{
    // Get the current process ID
    pid_t pid = getpid();
    // Print the PID
    std::cout << "PID: " << pid << std::endl;

    std::array<int, 3> offsets = {0x0, 0x4, 0x8};
    int test = 114514;
    // test_loop text will be change?
    char *test_another = new char[128];
    std::fill(test_another, test_another + 128, 0);
    // fill test_another
    std::string copy = "test_loop text will be change?--------------------------";
    std::copy(copy.begin(), copy.end(), test_another);

    std::array<long long, 2> address = {(long long)&test, (long long)test_another};
    AddToTestDriverFile(pid, address);
    protectSelf();

    // sleep loop
    while (true)
    {
        std::cout << "Sleeping: "  << std::dec << test << ":" << test_another <<std::endl;
        sleep(10);
    }

    delete[] test_another;
    return 0;
}
