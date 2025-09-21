#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>

#include "logger.h"

const std::vector<std::string> editors = {
    "nvim", "vim", "emacs", "nano", "clion", "pycharm", 
    "pycharm64", "code", "code-oss", "idea", "idea64"
};

bool IsDigits(const std::string& str) {
  for (char c : str) {
    if (!isdigit(c)) return false;
  }
  return true;
}

std::vector<int> FindEditorPids() {
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
    std::string process_name;

    if (comm_file >> process_name) {
      // Check if process name matches any editor
      for (const auto& editor : editors) {
        if (process_name.find(editor) != std::string::npos) {
          pids.push_back(std::stoi(pid_str));
          break;
        }
      }
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
  std::chrono::steady_clock::time_point start;
  double accumulatedTime = 0.0;

  while (true) {
    std::vector<int> pids = FindEditorPids();

    if (pids.empty()) {
      if (editorOpened) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = end - start;
        accumulatedTime += duration.count();
        
        std::cout << "Editor closed. Time in editor: " << accumulatedTime / 60.0 << " minutes" << std::endl;
        SaveTimeToLog(GetTodayDate(), accumulatedTime / 60.0);
        
        editorOpened = false;
        accumulatedTime = 0.0;
      }
    } else {
      if (!editorOpened) {
        start = std::chrono::steady_clock::now();
        editorOpened = true;
        std::cout << "Editor processes detected:" << std::endl;
        for (int pid : pids) {
          std::cout << " - PID: " << pid << std::endl;
        }
      } else {
        // Update the time if editor is still open
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<double> duration = now - start;
        start = now;
        accumulatedTime += duration.count();
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Check every 5 seconds
  }

  return 0;
}
