#include <iostream>
#include <limits>
#include "simulation.h"
#include "comparison.h"
using namespace std;

void displayMenu() {
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║   TRAFFIC SIGNAL OPTIMIZATION SYSTEM              ║" << endl;
    cout << "║   Using Greedy Algorithm (DAA Project)            ║" << endl;
    cout << "╚═══════════════════════════════════════════════════╝" << endl;
    cout << "\n1. Run Quick Demo (5 cycles, Morning Rush)" << endl;
    cout << "2. Morning Rush Hour Simulation" << endl;
    cout << "3. Evening Rush Hour Simulation" << endl;
    cout << "4. Midday Regular Traffic" << endl;
    cout << "5. Night Time Traffic" << endl;
    cout << "6. Emergency Vehicle Scenario" << endl;
    cout << "7. Accident Simulation (Lane Blockage)" << endl;
    cout << "8. Compare with Fixed-Time Signals" << endl;
    cout << "9. Custom Simulation" << endl;
    cout << "10. Algorithm Information" << endl;
    cout << "11. Exit" << endl;
    cout << "\nEnter choice: ";
}

void displayAlgorithmInfo() {
    cout << "\n" << string(65, '=') << endl;
    cout << "GREEDY ALGORITHM INFORMATION" << endl;
    cout << string(65, '=') << endl;
    
    cout << "\n📌 Strategy:" << endl;
    cout << "  At each cycle, select the lane with HIGHEST priority" << endl;
    cout << "  Priority = (Vehicles × Average Waiting Time) / Capacity" << endl;
    
    cout << "\n🔧 Data Structures:" << endl;
    cout << "  • Priority Queue (Max-Heap) for lane selection" << endl;
    cout << "  • Queue (FIFO) for vehicle management" << endl;
    
    cout << "\n⏱️  Complexity:" << endl;
    cout << "  • Time: O(n log n) per cycle" << endl;
    cout << "  • Space: O(n + m)" << endl;
    cout << "    where n = lanes, m = total vehicles" << endl;
    
    cout << "\n✅ Greedy Choice Property:" << endl;
    cout << "  • Locally optimal: Choose highest priority NOW" << endl;
    cout << "  • Globally optimal: Minimizes total waiting time" << endl;
    
    cout << "\n🚨 Emergency Override:" << endl;
    cout << "  • Emergency vehicles bypass greedy algorithm" << endl;
    cout << "  • Safety takes priority over optimization" << endl;
    
    cout << "\n" << string(65, '=') << "\n" << endl;
}

void runQuickDemo() {
    cout << "\n🚀 Running Quick Demo (5 cycles, Morning Rush)...\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(MORNING_RUSH);
    sim.runSimulation(5);
}

void runMorningRush() {
    cout << "\n🌅 Morning Rush Hour Simulation (7-9 AM)" << endl;
    cout << "Heavy traffic from North & East (toward city)\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(MORNING_RUSH);
    
    int cycles;
    cout << "Enter number of cycles (recommended: 10-15): ";
    cin >> cycles;
    
    sim.runSimulation(cycles);
}

void runEveningRush() {
    cout << "\n🌆 Evening Rush Hour Simulation (5-7 PM)" << endl;
    cout << "Heavy traffic from South & West (leaving city)\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(EVENING_RUSH);
    
    int cycles;
    cout << "Enter number of cycles (recommended: 10-15): ";
    cin >> cycles;
    
    sim.runSimulation(cycles);
}

void runMidday() {
    cout << "\n☀️ Midday Regular Traffic (12-2 PM)" << endl;
    cout << "Moderate traffic in all directions\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(MIDDAY);
    
    int cycles;
    cout << "Enter number of cycles (recommended: 10): ";
    cin >> cycles;
    
    sim.runSimulation(cycles);
}

void runNight() {
    cout << "\n🌙 Night Time Traffic (10 PM-6 AM)" << endl;
    cout << "Very light traffic\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(NIGHT);
    
    int cycles;
    cout << "Enter number of cycles (recommended: 8): ";
    cin >> cycles;
    
    sim.runSimulation(cycles);
}

void runEmergencyScenario() {
    cout << "\n🚨 Emergency Vehicle Scenario" << endl;
    cout << "Watch the greedy algorithm prioritize emergency vehicles!\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(MIDDAY);
    
    cout << "Adding emergency vehicle to South lane...\n" << endl;
    sim.addEmergencyVehicle("South");
    
    sim.runSimulation(8);
}

void runAccidentScenario() {
    cout << "\n🚧 Accident Simulation - Lane Blockage" << endl;
    cout << "East lane blocked at 50% due to accident\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(15, 60, 5);
    sim.setTrafficPattern(MIDDAY);
    
    sim.blockLane("East", 0.5);
    
    sim.runSimulation(10);
}

void runComparison() {
    cout << "\n📊 Comparing Greedy vs Fixed-Time Signals\n" << endl;
    cout << "Running both algorithms with same traffic pattern..." << endl;
    
    ComparisonAnalysis comparison;
    
    cout << "\n1️⃣  Running GREEDY Algorithm..." << endl;
    Simulation greedySim;
    greedySim.initializeLanes();
    greedySim.setSignalTimings(15, 60, 5);
    greedySim.setTrafficPattern(MORNING_RUSH);
    greedySim.runSimulation(10);
    
    AlgorithmResult greedyResult;
    greedyResult.algorithmName = "Greedy Algorithm";
    greedyResult.avgWaitingTime = greedySim.getAverageWaitingTime();
    greedyResult.totalVehiclesProcessed = greedySim.getTotalProcessed();
    greedyResult.totalSimulationTime = 10 * 40;
    greedyResult.throughput = (double)greedyResult.totalVehiclesProcessed / (greedyResult.totalSimulationTime / 60.0);
    
    comparison.addResult(greedyResult);
    
    cout << "\n2️⃣  Running FIXED-TIME Algorithm..." << endl;
    cout << "(Each lane gets 30 seconds in round-robin order)\n" << endl;
    
    Simulation fixedSim;
    fixedSim.initializeLanes();
    fixedSim.setSignalTimings(15, 60, 5);
    fixedSim.setTrafficPattern(MORNING_RUSH);
    
    for (int i = 0; i < 10; i++) {
        Lane* lane = fixedSim.getLane(i % 4);
        fixedSim.runFixedCycle(lane, 30);
    }
    
    AlgorithmResult fixedResult;
    fixedResult.algorithmName = "Fixed-Time Signal";
    fixedResult.avgWaitingTime = fixedSim.getAverageWaitingTime();
    fixedResult.totalVehiclesProcessed = fixedSim.getTotalProcessed();
    fixedResult.totalSimulationTime = 10 * 35;
    fixedResult.throughput = (double)fixedResult.totalVehiclesProcessed / (fixedResult.totalSimulationTime / 60.0);
    
    comparison.addResult(fixedResult);
    
    comparison.displayComparison();
}

void runCustomSimulation() {
    int cycles, minGreen, maxGreen, yellow, pattern;
    
    cout << "\n⚙️  Custom Simulation Setup" << endl;
    
    cout << "\nEnter number of cycles: ";
    cin >> cycles;
    
    cout << "Enter minimum green signal duration (seconds): ";
    cin >> minGreen;
    
    cout << "Enter maximum green signal duration (seconds): ";
    cin >> maxGreen;
    
    cout << "Enter yellow signal duration (seconds): ";
    cin >> yellow;
    
    cout << "\nSelect Traffic Pattern:" << endl;
    cout << "1. Morning Rush" << endl;
    cout << "2. Evening Rush" << endl;
    cout << "3. Midday" << endl;
    cout << "4. Night" << endl;
    cout << "Enter choice: ";
    cin >> pattern;
    
    if (cycles <= 0 || minGreen <= 0 || maxGreen <= minGreen || yellow <= 0) {
        cout << "\n❌ Invalid parameters!\n" << endl;
        return;
    }
    
    TimeOfDay period;
    switch(pattern) {
        case 1: period = MORNING_RUSH; break;
        case 2: period = EVENING_RUSH; break;
        case 3: period = MIDDAY; break;
        case 4: period = NIGHT; break;
        default: period = MIDDAY;
    }
    
    cout << "\n🚀 Starting custom simulation...\n" << endl;
    
    Simulation sim;
    sim.initializeLanes();
    sim.setSignalTimings(minGreen, maxGreen, yellow);
    sim.setTrafficPattern(period);
    sim.runSimulation(cycles);
}

int main() {
    int choice;
    bool running = true;
    
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║                                                           ║\n";
    cout << "║     TRAFFIC SIGNAL OPTIMIZATION SYSTEM                    ║\n";
    cout << "║     Design and Analysis of Algorithms Project             ║\n";
    cout << "║                                                           ║\n";
    cout << "║     Algorithm: Greedy (Priority-based Selection)          ║\n";
    cout << "║     Complexity: O(n log n)                                ║\n";
    cout << "║                                                           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";
    
    while (running) {
        displayMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n❌ Invalid input! Please enter a number.\n" << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                runQuickDemo();
                break;
            
            case 2:
                runMorningRush();
                break;
            
            case 3:
                runEveningRush();
                break;
            
            case 4:
                runMidday();
                break;
            
            case 5:
                runNight();
                break;
            
            case 6:
                runEmergencyScenario();
                break;
            
            case 7:
                runAccidentScenario();
                break;
            
            case 8:
                runComparison();
                break;
            
            case 9:
                runCustomSimulation();
                break;
            
            case 10:
                displayAlgorithmInfo();
                break;
            
            case 11:
                cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
                cout << "║   Thank you for using Traffic Signal System!     ║" << endl;
                cout << "║   DAA Project - Greedy Algorithm                  ║" << endl;
                cout << "╚═══════════════════════════════════════════════════╝\n" << endl;
                running = false;
                break;
            
            default:
                cout << "\n❌ Invalid choice! Please try again.\n" << endl;
        }
        
        if (running && choice >= 1 && choice <= 9) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }
    
    return 0;
}
