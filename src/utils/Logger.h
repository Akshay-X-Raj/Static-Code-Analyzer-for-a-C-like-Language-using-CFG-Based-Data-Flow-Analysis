#pragma once
#include <iostream>
#include <string>

class Logger {
public:
    static void info(const std::string& msg) {
        std::cout << "[INFO] " << msg << "\n";
    }
    static void error(const std::string& msg) {
        std::cerr << "[ERROR] " << msg << "\n";
    }
    static void warning(const std::string& msg) {
        std::cerr << "[WARN] " << msg << "\n";
    }
};
