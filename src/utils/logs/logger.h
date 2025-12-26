
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>


class logger {
    const std::string log_file_path = R"(C:\Users\niila\CLionProjects\boostbiscuit\cmake-build-debug\boostbiscuit.log)";
    std::ofstream log_file;
    std::mutex log_mutex;
    bool log_file_opened;

    logger();
    ~logger();

public:
    static logger& get();

    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
};


#endif //LOGGER_H
