
#ifndef TIME_H
#define TIME_H

#include <string>
#include <chrono>


inline std::string getCurrentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm bt{};

    localtime_s(&bt, &in_time_t);

    std::ostringstream oss;
    oss << std::put_time(&bt, "%H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}


#endif //TIME_H
