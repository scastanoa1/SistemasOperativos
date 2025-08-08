#include "Logger.h"
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>

std::ofstream Logger::ofs;
static std::mutex logger_mutex;

void Logger::init(const std::string& filename) {
    ofs.open(filename, std::ios::app);
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logger_mutex);
    auto now     = std::chrono::system_clock::now();
    auto in_time = std::chrono::system_clock::to_time_t(now);
    ofs << std::put_time(std::localtime(&in_time), "%Y-%m-%d %H:%M:%S")
        << " " << message;
    if (message.empty() || message.back()!='\n') ofs << "\n";
}
