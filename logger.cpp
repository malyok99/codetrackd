#include <filesystem>
#include <map>
#include <ctime>
#include <fstream>

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

