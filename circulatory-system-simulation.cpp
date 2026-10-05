#include <cstdlib> // For rand()
#include <ctime>   // For time()
#include <iomanip> // For setw()
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
//   Member 1: Graph Architecture
// ==========================================

// Blood Vessel (Edge)
struct BloodVessel {
  int toOrganId;      // ID of the destination organ
  double flowRate;    // Blood flow rate
  double distance;    // Distance between the two organs
  string vesselName;  // Name of the blood vessel
};

// Organ (Node)
struct Organ {
  int id;
  string name;
  vector<BloodVessel> outgoingVessels; // List of blood vessels exiting this organ
};

class CirculatorySystem {
public:
  vector<Organ> organs; // List of all organs in the system

  // Add a new organ (Node)
  void addOrgan(int id, string name) {
    Organ newOrgan;
    newOrgan.id = id;
    newOrgan.name = name;
    organs.push_back(newOrgan);
  }

  // Connect two organs with a blood vessel (Edge)
  void addVessel(int fromId, int toId, double flowRate, double distance,
                 string vesselName) {
    BloodVessel vessel = {toId, flowRate, distance, vesselName};
    organs[fromId].outgoingVessels.push_back(vessel);
  }

  // Print the structure of the Graph
  void printSystem() {
    cout << "=====================================================" << endl;
    cout << "         CIRCULATORY SYSTEM GRAPH STRUCTURE          " << endl;
    cout << "=====================================================" << endl
         << endl;

    for (const auto &organ : organs) {
      cout << "[" << organ.name << "]" << endl;
      if (organ.outgoingVessels.empty()) {
        cout << "  (No outgoing vessels defined)" << endl;
      }
      for (size_t i = 0; i < organ.outgoingVessels.size(); ++i) {
        const auto &vessel = organ.outgoingVessels[i];
        string branch =
            (i == organ.outgoingVessels.size() - 1) ? "`-- " : "|-- ";

        cout << "  " << branch << vessel.vesselName << " --> "
             << organs[vessel.toOrganId].name << "  [Flow: " << vessel.flowRate
             << " mL/min"
             << ", Dist: " << vessel.distance << " cm]" << endl;
      }
      cout << endl;
    }
  }
};

// ==========================================
//   Member 2: Data Structures (Blood Cells)
// ==========================================

enum CellType { RBC, WBC, PLATELET };

// Base structure for a blood cell
struct BloodCell {
  int id;
  CellType type;
  int maxLifespan;    // Maximum lifespan of the cell
  int currentAge;     // Current age of the cell
  int currentOrganId; // ID of the organ where the cell is currently located

  // Check if the cell is still alive
  bool isAlive() const { return currentAge < maxLifespan; }

  // Increase age by one (during a simulation tick)
  void ageTick() {
    if (isAlive()) {
      currentAge++;
    }
  }
};

// Red Blood Cells
struct RedBloodCell : public BloodCell {
  double oxygenLevel;

  RedBloodCell(int cellId, int lifespan, int startOrgan) {
    id = cellId;
    type = RBC;
    maxLifespan = lifespan;
    currentAge = 0;
    currentOrganId = startOrgan;
    oxygenLevel = 100.0; // 100% oxygen at the beginning
  }
};

// White Blood Cells
struct WhiteBloodCell : public BloodCell {
  int pathogensDetected;

  WhiteBloodCell(int cellId, int lifespan, int startOrgan) {
    id = cellId;
    type = WBC;
    maxLifespan = lifespan;
    currentAge = 0;
    currentOrganId = startOrgan;
    pathogensDetected = 0; // No pathogens detected initially
  }
};

// Platelets
struct Platelet : public BloodCell {
  int clottingEvents;

  Platelet(int cellId, int lifespan, int startOrgan) {
    id = cellId;
    type = PLATELET;
    maxLifespan = lifespan;
    currentAge = 0;
    currentOrganId = startOrgan;
    clottingEvents = 0; // No clotting events initially
  }
};

// ==========================================
//   Member 3: Simulation Logic & Profiling
// ==========================================

class SimulationEngine {
private:
  CirculatorySystem *system;
  vector<BloodCell *> allCells;

  // For profiling: keep track of how many times cells visit each organ
  // rbcVisits[organId] = count
  vector<int> rbcVisits;
  vector<int> wbcVisits;
  vector<int> plateletVisits;
  int totalRbcMoves;
  int totalWbcMoves;
  int totalPlateletMoves;

public:
  SimulationEngine(CirculatorySystem *sys) {
    system = sys;
    int organCount = sys->organs.size();
    rbcVisits.resize(organCount, 0);
    wbcVisits.resize(organCount, 0);
    plateletVisits.resize(organCount, 0);
    totalRbcMoves = 0;
    totalWbcMoves = 0;
    totalPlateletMoves = 0;

    srand(time(0)); // Seed for random number generation
  }

  void addCell(BloodCell *cell) {
    allCells.push_back(cell);

    // Count the starting organ as the first visit
    recordVisit(cell->type, cell->currentOrganId);
  }

  void recordVisit(CellType type, int organId) {
    if (type == RBC) {
      rbcVisits[organId]++;
      totalRbcMoves++;
    } else if (type == WBC) {
      wbcVisits[organId]++;
      totalWbcMoves++;
    } else if (type == PLATELET) {
      plateletVisits[organId]++;
      totalPlateletMoves++;
    }
  }

  // The main Simulation Loop
  void runSimulation(int totalTicks) {
    cout << "=====================================================" << endl;
    cout << "            STARTING SIMULATION ENGINE               " << endl;
    cout << "=====================================================" << endl;
    cout << "Simulating for " << totalTicks << " ticks..." << endl << endl;

    for (int tick = 1; tick <= totalTicks; ++tick) {
      for (size_t i = 0; i < allCells.size(); ++i) {
        BloodCell *cell = allCells[i];

        if (cell->isAlive()) {
          int currentLoc = cell->currentOrganId;
          const vector<BloodVessel> &options =
              system->organs[currentLoc].outgoingVessels;

          if (!options.empty()) {
            // Randomly select the next organ to visit
            int choice = rand() % options.size();
            int nextOrgan = options[choice].toOrganId;

            // Move the cell
            cell->currentOrganId = nextOrgan;

            // Include the movement in profiling
            recordVisit(cell->type, nextOrgan);
          }

          // Increase the age of the cell
          cell->ageTick();
        }
      }
    }
    cout << "Simulation Completed!" << endl << endl;
  }

  // Output the profiling data
  void printProfiling() {
    cout << "=========================================================================================================" << endl;
    cout << "                                         PROFILING RESULTS                                               " << endl;
    cout << "=========================================================================================================" << endl;

    cout << left << setw(20) << "Organ Name" 
         << setw(15) << "RBC Visits" << setw(15) << "Prob (%)"
         << setw(15) << "WBC Visits" << setw(15) << "Prob (%)"
         << setw(15) << "PLT Visits" << "Prob (%)" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < system->organs.size(); ++i) {
      int rVisits = rbcVisits[i];
      int wVisits = wbcVisits[i];
      int pVisits = plateletVisits[i];

      double rProb = (totalRbcMoves > 0) ? ((double)rVisits / totalRbcMoves) * 100.0 : 0.0;
      double wProb = (totalWbcMoves > 0) ? ((double)wVisits / totalWbcMoves) * 100.0 : 0.0;
      double pProb = (totalPlateletMoves > 0) ? ((double)pVisits / totalPlateletMoves) * 100.0 : 0.0;

      // Format and print the results
      cout << left << setw(20) << system->organs[i].name 
           << setw(15) << rVisits << rProb << setw(15 - to_string(rProb).length() + 3) << "%"
           << setw(15) << wVisits << wProb << setw(15 - to_string(wProb).length() + 3) << "%"
           << setw(15) << pVisits << pProb << "%" << endl;
    }
    cout << endl;
  }

  ~SimulationEngine() {
    for (size_t i = 0; i < allCells.size(); ++i) {
      delete allCells[i];
    }
  }
};

// ==========================================
//   Main Function
// ==========================================

int main() {
  // 1. Graph Setup (Member 1)
  CirculatorySystem humanBody;

  humanBody.addOrgan(0, "Right Atrium");
  humanBody.addOrgan(1, "Right Ventricle");
  humanBody.addOrgan(2, "Lungs     "); // Spaces added for formatting
  humanBody.addOrgan(3, "Left Atrium");
  humanBody.addOrgan(4, "Left Ventricle");
  humanBody.addOrgan(5, "Brain     ");
  humanBody.addOrgan(6, "Kidneys   ");
  humanBody.addOrgan(7, "Liver     ");
  humanBody.addOrgan(8, "Intestines");
  humanBody.addOrgan(9, "Muscles   ");

  humanBody.addVessel(0, 1, 5000.0, 5.0, "Tricuspid Valve");
  humanBody.addVessel(1, 2, 5000.0, 15.0, "Pulmonary Artery");
  humanBody.addVessel(2, 3, 5000.0, 15.0, "Pulmonary Vein");
  humanBody.addVessel(3, 4, 5000.0, 5.0, "Mitral Valve");
  humanBody.addVessel(4, 5, 750.0, 40.0, "Carotid Artery");
  humanBody.addVessel(4, 6, 1100.0, 25.0, "Renal Artery");
  humanBody.addVessel(4, 7, 350.0, 30.0, "Hepatic Artery");
  humanBody.addVessel(4, 8, 1000.0, 35.0, "Mesenteric Artery");
  humanBody.addVessel(4, 9, 800.0, 50.0, "Peripheral Arteries");
  humanBody.addVessel(5, 0, 750.0, 40.0, "Jugular Vein");
  humanBody.addVessel(6, 0, 1100.0, 25.0, "Renal Vein");
  humanBody.addVessel(9, 0, 800.0, 50.0, "Systemic Veins");
  humanBody.addVessel(8, 7, 1000.0, 20.0, "Hepatic Portal Vein");
  humanBody.addVessel(7, 0, 1350.0, 15.0, "Hepatic Vein");

  // Print the created graph network
  humanBody.printSystem();

  // --- Executing Member 2's part (Testing) ---
  cout << "=====================================================" << endl;
  cout << "         BLOOD CELLS INITIALIZATION (TEST)           " << endl;
  cout << "=====================================================" << endl
       << endl;

  // Create and test a few cells
  RedBloodCell myRBC(1, 120, 4);
  WhiteBloodCell myWBC(2, 13, 4);
  Platelet myPlatelet(3, 10, 4);

  cout << "Created RBC      - ID: " << myRBC.id
       << " | Lifespan: " << myRBC.maxLifespan
       << " | Alive: " << (myRBC.isAlive() ? "Yes" : "No") << endl;
  cout << "Created WBC      - ID: " << myWBC.id
       << " | Lifespan: " << myWBC.maxLifespan
       << " | Alive: " << (myWBC.isAlive() ? "Yes" : "No") << endl;
  cout << "Created Platelet - ID: " << myPlatelet.id
       << " | Lifespan: " << myPlatelet.maxLifespan
       << " | Alive: " << (myPlatelet.isAlive() ? "Yes" : "No") << endl
       << endl;

  // 2. Start the Simulation Engine (Member 3)
  SimulationEngine sim(&humanBody);

  // 3. Add a number of cells to the system (Member 2)
  // Let's start with 50 cells from the Left Ventricle (ID = 4)
  for (int i = 0; i < 50; ++i) {
    sim.addCell(new RedBloodCell(i, 120, 4)); // Lifespan 120
  }
  for (int i = 50; i < 70; ++i) {
    sim.addCell(new WhiteBloodCell(i, 13, 4)); // Lifespan 13
  }
  for (int i = 70; i < 100; ++i) {
    sim.addCell(new Platelet(i, 10, 4)); // Lifespan 10
  }

  // 4. Run the Simulation Loop (Member 3)
  // Run for 150 ticks
  sim.runSimulation(150);

  // 5. Print the profiling results (Member 3)
  sim.printProfiling();

  return 0;
}