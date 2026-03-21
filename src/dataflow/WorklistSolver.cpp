#include "WorklistSolver.h"

void WorklistSolver::solve(DataFlowAnalysis* analysis) {
    if (analysis) analysis->analyze();
}
