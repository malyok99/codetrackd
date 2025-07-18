#pragma once

#include <string>

std::string GetTodayDate();
std::string GetLogPath();
void SaveTimeToLog(const std::string& date, double timeMinutes);

