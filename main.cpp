#include <cerrno>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <algorithm>

#include "logger.h"

const std::vector<std::string> editors = {
    "nvim",
    "vim",
    "emacs",
    "nano",
    "clion",
    "pycharm",
    "pycharm64",
    "code",
    "code-oss",
    "idea",
    "idea64"
};

bool IsDigits(const std::string& str) {
  for (char c : str) {
    if (!isdigit(c)) return false;
  }
  return true;
}

std::vector<int> FindNvimPids() {
  std::vector<int> pids;
  DIR* proc_dir = opendir("/proc");

  if (proc_dir == nullptr) {
    perror("Opendir /proc failed");
    return pids;
  }

  struct dirent* entry;
  while ((entry = readdir(proc_dir)) != nullptr) {
    std::string pid_str(entry->d_name);

    if (!IsDigits(pid_str)) {
      continue;
    }

    std::ifstream comm_file("/proc/" + pid_str + "/comm");
    std::string proccess_name;

    if (comm_file >> proccess_name && std::find(editors.begin(), editors.end(), process_name) != editors.end()) {
      pids.push_back(std::stoi(pid_str));
    }
  }

  closedir(proc_dir);
  return pids;
}

int main(int argc, char* argv[]) {
  if (argc == 3 && std::string(argv[1]) == "--show") {
    int days = std::stoi(argv[2]);
    if (days <= 0 || days > 365) {
      std::cerr << "Invalid number of days (1-365 allowed)." << std::endl;
      return 1;
    }
    PrintLastNDaysLog(days);
    return 0;
  }

  bool editorOpened = false;

  std::chrono::high_resolution_clock::time_point start;

  while (true) {
    std::vector<int> pids = FindNvimPids();

    if (pids.empty()) {
      std::cout << "No nvim processes found" << std::endl;

      if (editorOpened) {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        std::cout << "Time in Nvim: " << duration.count() / 60.0 << "minutes" << std::endl;

        SaveTimeToLog(GetTodayDate(), duration.count() / 60.0);

        editorOpened = false;
      }
    } else {
      if (!editorOpened) {
        start = std::chrono::high_resolution_clock::now();
        editorOpened = true;
      }

      std::cout << "Nvim processes:" << std::endl;
      for (int pid : pids) {
        std::cout << " - PID: " << pid << std::endl;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(30000));
  }

  return 0;
}
