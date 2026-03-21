#pragma once
#include <string>
#include <vector>

class BasicBlock {
public:
    int id;
    std::vector<std::string> statements;
    std::vector<int> next;
};
