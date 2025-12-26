
#include "logger.h"

#include <iostream>
#include <utils/time.h>

#include "boostbiscuit.h"


logger::logger() : log_file_opened(bb::debug_mode) {
    if constexpr (!bb::debug_mode)  return;

    DeleteFile(log_file_path.c_str());  // Delete the log file if it exists
    log_file.open(log_file_path, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
    }
}


logger::~logger() {
    std::lock_guard lock(log_mutex);
    if (log_file_opened)
        log_file.close();
}


logger& logger::get() {
    static logger instance;
    return instance;
}


void logger::info(const std::string &message) {
    const std::string time = getCurrentTimestamp();

    std::cout << "[" << time << "] " << "[INFO] " << message << std::endl;

    std::lock_guard lock(log_mutex);
    if (log_file_opened) {
        log_file << "[" << time << "] " << "[INFO] " << message << std::endl;
        log_file.flush();
    }
}


void logger::warn(const std::string &message) {
    const std::string time = getCurrentTimestamp();

    std::cout << "[" << time << "] " << "[WARN] " << message << std::endl;

    std::lock_guard lock(log_mutex);
    if (log_file_opened) {
        log_file << "[" << time << "] " << "[WARN] " << message << std::endl;
        log_file.flush();
    }
}


void logger::error(const std::string &message) {
    const std::string time = getCurrentTimestamp();

    std::cerr << "[" << time << "] " << "[ERROR] " << message << std::endl;

    std::lock_guard lock(log_mutex);
    if (log_file_opened) {
        log_file << "[" << time << "] " << "[ERROR] " << message << std::endl;
        log_file.flush();
    }
}
