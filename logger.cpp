#include <iostream>
#include <filesystem>
#include <map>
#include <ctime>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#include "logger.h"

std::string GetTodayDate() {
  time_t now = time(nullptr);
  char buf[16];
  strftime(buf, sizeof(buf), "%d.%m.%y", localtime(&now));
  return std::string(buf);
}

std::string GetLogPath() {
  std::string home = std::getenv("HOME");
  std::string dir = home + "/.programming_tracker";
  std::filesystem::create_directories(dir);
  return dir + "/coding_log.txt";
}

void SaveTimeToLog(const std::string& date, double timeMinutes) {
  std::ifstream infile(GetLogPath());
  std::map<std::string, double> log;

  std::string line;
  while (std::getline(infile, line)) {
    if (line.starts_with("-") && line.ends_with("-")) {
      std::string logDate = line.substr(1, line.size() - 2);
      if (std::getline(infile, line)) {
        size_t dashPos = line.find("coding time - ");
        if (dashPos != std::string::npos) {
          std::string timeStr = line.substr(dashPos + 14);
          size_t hourPos = timeStr.find(" hour");
          size_t minPos = timeStr.find(" minute");
          
          double minutes = 0;
          if (hourPos != std::string::npos) {
            int hours = std::stoi(timeStr.substr(0, hourPos));
            size_t minStart = timeStr.find(" ", hourPos + 5);
            if (minStart != std::string::npos && minPos != std::string::npos) {
              int mins = std::stoi(timeStr.substr(minStart + 1, minPos - minStart - 1));
              minutes = hours * 60 + mins;
            } else {
              minutes = hours * 60;
            }
          } else if (minPos != std::string::npos) {
            minutes = std::stoi(timeStr.substr(0, minPos));
          }
          log[logDate] = minutes;
        }
      }
    }
  }

  infile.close();

  log[date] += timeMinutes;

  std::ofstream outfile(GetLogPath(), std::ios::trunc);
  for (const auto& [day, minutes] : log) {
    outfile << "-" << day << "-" << "\n";
    int hrs = static_cast<int>(minutes) / 60;
    int min = static_cast<int>(minutes) % 60;

    if (hrs > 0)
      outfile << "coding time - " << hrs << " hour" << (hrs != 1 ? "s " : " ") << min << " minute" << (min != 1 ? "s" : "") << "\n";
    else
      outfile << "coding time - " << min << " minute" << (min != 1 ? "s" : "") << "\n";
  }
}

void PrintLastNDaysLog(int days) {
  std::ifstream infile(GetLogPath());
  if (!infile) {
    std::cerr << "No log file found." << std::endl;
    return;
  }

  std::map<std::string, double> minutesMap;

  std::string line;
  std::string currentDate;

  while (std::getline(infile, line)) {
    if (line.starts_with("-") && line.ends_with("-")) {
      currentDate = line.substr(1, line.size() - 2);
    } else if (!currentDate.empty()) {
      size_t dashPos = line.find("coding time - ");
      if (dashPos != std::string::npos) {
        std::string timeStr = line.substr(dashPos + 14);
        size_t hourPos = timeStr.find(" hour");
        size_t minPos = timeStr.find(" minute");
        
        double minutes = 0;
        if (hourPos != std::string::npos) {
          int hours = std::stoi(timeStr.substr(0, hourPos));
          size_t minStart = timeStr.find(" ", hourPos + 5);
          if (minStart != std::string::npos && minPos != std::string::npos) {
            int mins = std::stoi(timeStr.substr(minStart + 1, minPos - minStart - 1));
            minutes = hours * 60 + mins;
          } else {
            minutes = hours * 60;
          }
        } else if (minPos != std::string::npos) {
          minutes = std::stoi(timeStr.substr(0, minPos));
        }
        minutesMap[currentDate] = minutes;
      }
    }
  }

  // print last N days
  time_t now = time(nullptr);
  std::tm* now_tm = localtime(&now);

  double totalMinutes = 0.0;

  std::cout << "=== Last " << days << " Days of Coding ===\n";

  for (int i = days - 1; i >= 0; --i) {
    std::tm temp_tm = *now_tm;
    temp_tm.tm_mday -= i;
    mktime(&temp_tm); // normalize

    char buf[16];
    strftime(buf, sizeof(buf), "%d.%m.%y", &temp_tm);
    std::string day(buf);

    auto it = minutesMap.find(day);
    if (it != minutesMap.end()) {
      int hrs = static_cast<int>(it->second) / 60;
      int min = static_cast<int>(it->second) % 60;
      
      if (hrs > 0)
        std::cout << day << ": coding time - " << hrs << " hour" << (hrs != 1 ? "s " : " ") << min << " minute" << (min != 1 ? "s" : "") << "\n";
      else
        std::cout << day << ": coding time - " << min << " minute" << (min != 1 ? "s" : "") << "\n";
      
      totalMinutes += it->second;
    } else {
      std::cout << day << ": no data\n";
    }
  }

  // summary
  int totalHrs = static_cast<int>(totalMinutes) / 60;
  int totalMin = static_cast<int>(totalMinutes) % 60;

  std::cout << "\n== SUMMARY ==\n";
  std::cout << "coding hours - ";
  if (totalHrs > 0)
    std::cout << totalHrs << " hour" << (totalHrs != 1 ? "s " : " ");
  std::cout << totalMin << " minute" << (totalMin != 1 ? "s" : "") << "\n";
}
