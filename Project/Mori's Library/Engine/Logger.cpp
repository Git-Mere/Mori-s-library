
#include <iostream>
#include "Logger.h"

Logger::Logger(Logger::Severity severity, bool use_console, std::chrono::system_clock::time_point start_time) : min_level(severity), out_stream("Trace.log"), start_time(start_time) {
    if (use_console == true) {
        out_stream.set_rdbuf(std::cout.rdbuf());
    }
}

Logger::~Logger() {
    out_stream.flush();
    out_stream.close();
}

void Logger::log(Logger::Severity severity, std::string message) {

    const char* severity_str[] = { "Verbos\t", "Debug\t", "Event\t", "Error\t" };
    const char* severity_level_str = severity_str[static_cast<int>(severity)];

    bool should_log = (static_cast<int>(severity) >= static_cast<int>(min_level));

    if (should_log) { 
        out_stream.precision(4);
        out_stream << '[' << std::fixed << seconds_since_start() << "]\t";
        out_stream << severity_level_str + message + '\n'; }

}

double Logger::seconds_since_start() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> actual_time = now - start_time;
    return actual_time.count();
}