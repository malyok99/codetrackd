#include <iostream>
#include <filesystem>
#include <map>
#include <ctime>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>

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
      std::getline(infile, line);
      double minutes = std::stod(line.substr(line.find('-') + 1));
      log[logDate] = minutes;
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
      outfile << "coding time - " << hrs << " hour " << min << " minutes\n";
    else
      outfile << "coding time - " << min << " minutes\n";
  }
}

void PrintLastNDaysLog(int days) {
  std::ifstream infile(GetLogPath());
  if (!infile) {
    std::cerr << "Failed to open log file." << std::endl;
    return;
  }

  std::map<std::string, std::string> logs;
  std::map<std::string, double> minutesMap;

  std::string line;
  std::string currentDate;

  while (std::getline(infile, line)) {
    if (line.starts_with("-") && line.ends_with("-")) {
      currentDate = line.substr(1, line.size() - 2);
    } else if (!currentDate.empty()) {
      logs[currentDate] = line;

      // parse minutes
      size_t dash = line.find("coding time - ");
      if (dash != std::string::npos) {
        std::istringstream iss(line.substr(dash + 14));
        int hrs = 0, mins = 0;
        std::string temp;

        iss >> hrs >> temp >> mins; // "hour"/"hours" gets skipped

        if (temp == "hour" || temp == "hours") {
          iss >> temp; // skip "minutes"
        } else {
          mins = hrs;
          hrs = 0;
        }

        minutesMap[currentDate] = hrs * 60 + mins;
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

    auto it = logs.find(day);
    if (it != logs.end()) {
      std::cout << day << ": " << it->second << "\n";
      totalMinutes += minutesMap[day];
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
    std::cout << totalHrs << " hours ";
  std::cout << totalMin << " minutes\n";
}

