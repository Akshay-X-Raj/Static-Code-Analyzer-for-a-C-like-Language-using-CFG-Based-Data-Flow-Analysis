#pragma once
#include "CFG.h"

class DataFlowAnalysis {
public:
    virtual ~DataFlowAnalysis() = default;
    virtual void analyze() = 0;
};
