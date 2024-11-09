#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>


void run_exe(const char* exePath, int count)
{
    for(int i = 0; i < count; ++i){
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            return;
        }

        if (pid == 0) {
            // This is the child process
            execl(exePath, exePath, (char*)NULL);
            // If execl returns, it must have failed
            perror("execl failed");
            return;
        } else {
            // This is the parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}


bool readDriverFile(std::unordered_map<int, std::pair<long long, long long>> & addressMap)
{
    std::ifstream ifile("./test_driver.txt");
    if (!ifile)
    {
        std::cerr << "Failed to open the file." << std::endl;
        return false;
    }

    int pid;
    std::string address1, address2;
    while (ifile >> pid >> address1 >> address2)
    {
        addressMap[pid] = std::make_pair(std::stoll(address1), std::stoll(address2));
    }

    ifile.close();
    return true;
}

int read_mem_file(std::string pid, std::array<uintptr_t, 2> address) {
  std::string process_mem{"/proc/" + pid + "/mem"};
  std::ifstream mem(process_mem.c_str(), std::ios::binary);
  if (!mem) {
    std::cerr << "Failed to open " << process_mem << "\n";
    return 1;
  }

  int read_int = -1;
  mem.seekg(address[0]);
  mem.read(reinterpret_cast<char *>(&read_int), sizeof(read_int));
  if (mem.fail()) {
    std::cout << "read mem failed" << std::endl;
  }

  char read_string[256]{0};
  mem.seekg(address[1]);
  // mem read char util meet '\0'
  char ch;
  int i = 0;
  do {
    mem.read(&ch, 1);
    read_string[i++] = ch;
  } while (ch != '\0');

  std::cout << "read memory from other process, read int:" << std::dec
            << read_int << ", read_string:" << read_string << std::endl;

  return 0;
}


int read_mem_api(std::string pid, std::array<uintptr_t, 2> address) {
  // process_vm_readv
  struct iovec local[2];
  struct iovec remote[2];

  int read_int = 0;
  char read_string[256]{0};

  for (int i = 0; i < address.size(); ++i) {
    remote[i].iov_base = reinterpret_cast<void *>(address[i]);
    remote[i].iov_len = sizeof(int);
  }

  local[0].iov_base = static_cast<void *>(&read_int);
  local[0].iov_len = sizeof(int);

  local[1].iov_base = static_cast<void *>(read_string);
  local[1].iov_len = sizeof(int);

  int num_read = 0;
  num_read = process_vm_readv(std::stoi(pid), local, 2, remote, 2, 0);

  std::cout << "read bytes count is " << num_read << std::endl;

  std::cout << "read memory from other process, read int:" << std::dec
            << read_int << ", read_string:" << read_string << std::endl;

  return 0;
}

int write_mem(std::string pid, std::array<uintptr_t, 2> address) {
  std::string process_mem{"/proc/" + pid + "/mem"};
  std::ofstream mem(process_mem.c_str(), std::ios::binary);
  if (!mem) {
    std::cerr << "Failed to open " << process_mem << "\n";
    return 1;
  }

  int write_int = 7777;

  // write int to address[0];
  mem.seekp(address[0]);
  mem.write(reinterpret_cast<char *>(&write_int), sizeof(write_int));

  std::string write_str = "guoyawen linux change other process memory fourth";
  mem.seekp(address[1]);
  mem.write(write_str.c_str(), write_str.size());

  return 0;
}

void test_rw_mem(const std::string& pid, const std::array<uintptr_t, 2>& address)
{
    std::cout << "select change process memory method: " << std::endl;
    std::cout << "1: use process_vm_readv; 2: use /proc/${pid}/mem" << std::endl;

    int choice;
    std::cin >> choice;

    if(choice == 1){
        read_mem_api(pid, address);

        write_mem(pid, address);

        read_mem_api(pid, address);
    }else{
        int ret = 0;
        std::cout << "use ptrace" << std::endl;
        ret = ptrace(PTRACE_ATTACH, std::stoi(pid), nullptr, nullptr);
        std::cout << "attach use ptrace ret is :"<< ret << std::endl;
        waitpid(std::stoi(pid), nullptr, 0);

        read_mem_file(pid, address);

        write_mem(pid, address);

        read_mem_file(pid, address);

        ret = ptrace(PTRACE_CONT, std::stoi(pid), nullptr, nullptr);
        std::cout << "attach use ptrace ret is :"<< ret << std::endl;
        ret = ptrace(PTRACE_DETACH, std::stoi(pid), nullptr, nullptr);
        std::cout << "attach use ptrace ret is :"<< ret << std::endl;
    }
}

int main(int argc, char **argv) {

  std::array<uintptr_t, 2> address;
  std::string processId;
  char ch;

  do{
        std::cout << "select access process id: " << std::endl;
        std::cin >> processId;

        std::unordered_map<int, std::pair<long long, long long>> addressMap;
        if(!readDriverFile(addressMap)){
            std::cout << "Failed to read driver file" << std::endl;
        }else{
            address[0] = addressMap[std::stoi(processId)].first;
            address[1] = addressMap[std::stoi(processId)].second;
            test_rw_mem(processId, address);
        }

        std::cout << "q to quit, other key to continue:";
        std::cin >> ch;

  }while(ch != 'q');

  return 0;
}
