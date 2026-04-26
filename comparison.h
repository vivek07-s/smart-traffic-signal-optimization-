#ifndef COMPARISON_H
#define COMPARISON_H

#include <string>
#include <vector>
using namespace std;

struct AlgorithmResult {
    string algorithmName;
    double avgWaitingTime;
    int totalVehiclesProcessed;
    int totalSimulationTime;
    double throughput;
};

class ComparisonAnalysis {
private:
    vector<AlgorithmResult> results;
    
public:
    ComparisonAnalysis();
    
    void addResult(AlgorithmResult result);
    
    void displayComparison() const;
    void displayBarChart() const;
    void displayImprovementTable() const;
    
    double calculateImprovement() const;
};

#endif
