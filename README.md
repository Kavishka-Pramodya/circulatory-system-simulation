# Circulatory System Simulation

A C++ graph-based architecture that models the human circulatory system. This project simulates the flow of different types of blood cells through the body using nodes (organs) and edges (blood vessels), tracking their lifespan, pathfinding probabilities, and biological behaviors over time.

## 🚀 Features

- **Graph Network Architecture**: Accurately models the human circulatory system using a directed graph. 
  - Nodes represent organs (Right Atrium, Left Ventricle, Brain, Kidneys, Lungs, etc.).
  - Edges represent blood vessels (Aorta, Pulmonary Artery, Jugular Vein) weighted with specific flow rates and distances.
- **Cellular Simulation & Tracking**: 
  - Simulates multiple blood cell types: `Red Blood Cells (RBC)`, `White Blood Cells (WBC)`, and `Platelets`.
  - Tracks individual properties such as oxygen levels, pathogen detection, clotting events, and max lifespans (e.g., 120 ticks for RBCs).
- **Real-Time Simulation Engine**: 
  - Executes tick-based simulation loops to age cells and move them through the network stochastically based on available outgoing blood vessels.
- **Data Profiling**: 
  - Analyzes simulation traffic, tracking the exact probability and hit count of cells visiting specific organs over time.

## 🧠 Core Data Structures

- **`BloodVessel` (Edge)**: Stores destination organ ID, flow rate, distance, and vessel name.
- **`Organ` (Node)**: Stores organ ID, name, and an adjacency list of outgoing vessels.
- **`BloodCell` (Base Class)**: Abstract structure handling ID, cell type, lifespan, current age, and location.
- **`SimulationEngine`**: The core controller handling initialization, tick advancement, and profiling outputs.

## 🛠️ How to Compile & Run

Make sure you have a standard C++ compiler installed (like `g++`).

1. Open your terminal or command prompt.
2. Navigate to the project directory.
3. Compile the simulation using `g++`:
   ```bash
   g++ circulatory-system-simulation.cpp -o circulatory-sim
   ```
4. Run the executable:
   ```bash
   ./circulatory-sim    # On Linux/Mac
   circulatory-sim.exe  # On Windows
   ```

## 📊 Output Example

When executed, the program will output:
1. The full **Graph Structure** of the circulatory system.
2. The **Initialization tests** for all blood cell types.
3. The **Simulation Results**, generating a profiling table of how many times each blood cell type visited each organ.

---
*Developed as an exploration of Data Structures, Graph Theory, and Object-Oriented Programming (OOP) in C++.*
