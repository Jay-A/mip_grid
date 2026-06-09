# MIP Grid: Power Grid Numerical Methods Research Platform

[![Documentation](https://img.shields.io/badge/docs-GitHub%20Pages-blue)](https://jay-a.github.io/mip_grid/)


**MIP Grid** is a research-oriented platform for numerical analysis of power grids, providing:  

- Synthetic grid generation with configurable topology and attributes  
- Time series construction for dynamic simulations  
- HDF5 snapshot storage for downstream solver workflows  
- DOT and CSV export for visualization and dataset inspection  
- Python API integration for MIP-based optimization workflows  

This platform is intended for **researchers and developers** exploring power grid optimization, feasibility studies, and solver integration.

---

## Table of Contents

1. Features
2. Installation
3. Quick Start
4. Usage
5. Python Integration
6. Configuration Files
7. Examples
8. License

---

## Features

- **Grid generation**: Generate synthetic power grids of arbitrary size and topology.  
- **Attribute injection**: Physical and operational attributes injected automatically.  
- **Time series construction**: Generate dynamic loads, injections, and line flows.  
- **HDF5 snapshots**: Store a unified grid + time series dataset for solver consumption.  
- **Visualization**: Export DOT graphs and render PNGs (`-v` / `--view-graph`).  
- **CSV export**: Export node, edge, or full grid topology for analysis (`--export-csv`).  
- **Python solver API**: Interact with generated datasets using Gurobi (or other solvers).

---

## Installation

### Prerequisites

- C++17 compiler (GCC/Clang)  
- CMake >= 3.18  
- HDF5 library  
- Python >= 3.9 (for solver API)  
- Optional: Graphviz for DOT visualization  

### Build

```bash
git clone https://github.com/Jay-A/mip_grid.git
```

The project has three external dependencies: **JSON**, **HDF5**, and **MPI**.

As of the initial commit, the build has only been tested with:

- GCC 12
- OpenMPI 4.1.2
- HDF5-serial 1.14.0

The `scripts/env.sh.localexample` file shows how to either load these as modules or point to local installations.

The JSON header can be downloaded directly from `https://github.com/nlohmann/json/` with 

```bash
mkdir -p external 
wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -O external/json.hpp
```

When all externals have been loaded or added to path build is simply done with

```bash
./scripts/build_cpp.sh
```

where
```bash
./scripts/build_cpp.sh --help
```

reveals usage guidelines:
```console
Usage: build_cpp.sh [MODE] [-b BUILD_TYPE]

Modes:
  build        Configure (if needed) + build (DEFAULT)
  configure    Only run CMake configure step
  clean        Remove build directory only
  rebuild      Clean + configure + build

Options:
  -b, --build-type <type>    Debug, Release, RelWithDebInfo, MinSizeRel
  -h, --help                 Show help

Examples:
  build_cpp.sh                     # build (default Release)
  build_cpp.sh build -b Debug
  build_cpp.sh rebuild -b Release
  build_cpp.sh configure -b Debug
  build_cpp.sh clean
```

Binary outputs will be in `build/bin/mip_grid`.

---

## Quick Start

### Generate HDF5 Grid Snapshot

```bash
build/bin/mip_grid -i ../../examples/example_json_input.json
```

Output:

```text
Successfully wrote: grid_timeseries.h5
```

The .h5 file can be explored with h5ls or in python.

---

### View Grid Graph

```bash
build/bin/mip_grid -i ../../examples/example_json_input.json -v
dot -Tpng grid.dot -o grid.png
xdg-open grid.png
```

---

### Export Grid to CSV

```bash
build/bin/mip_grid -i ../../examples/example_json_input.json --export-csv
```

---

## Python Integration

The long-term goal is a fully integrated workflow in which a parallel MIP solver
calls the C++ power-grid generation backend directly from Python, consumes the
resulting HDF5 snapshots, performs optimization, and returns solutions for
analysis and visualization in the Python frontend.

The optimization backend is still under active development. For now, generated
HDF5 snapshots can be loaded into Python and used with external solvers such as
Gurobi.

A grid HDF5 snapshot can be loaded in Python for solver workflows:

```python
from python.snapshot_loader import SnapshotLoader
from python.model import PowerGridModel
import gurobipy as gp

# Load snapshot
snapshot = SnapshotLoader.load("grid_timeseries.h5")

# Initialize optimization model
model = PowerGridModel(snapshot)
solver = gp.Model()
model.build_mip(solver)

# Solve
solver.optimize()
```

---

---

## Configuration Files

The system is driven by JSON GridSpec and TimeSeries configuration files, for example
`examples/example_json_input.json`.

Each configuration defines:

### Grid Generation

- Number of nodes in the synthetic network
- Generator and load node ratios
- Connectivity and feasibility requirements
- Branching and topology controls
  - ring backbone support
  - cross-link insertion
  - branch factor
- Random seeds for deterministic generation
- Generator capacity ranges
- Load demand ranges
- Transmission line capacity ranges
- Transmission line reactance ranges
- Node and edge failure probabilities

### Time-Series Generation

- Number of timesteps
- Sampling frequency
- Demand modulation amplitude
- Generator modulation amplitude
- Noise amplitude
- Independent random seed
- Dataset label metadata

Using fixed seeds produces reproducible grid topologies and time-series realizations, which is useful for testing, benchmarking, and optimization studies.

---

## Data Outputs

Running the C++ pipeline produces a unified HDF5 snapshot:

grid_timeseries.h5


This file contains:

- `nodes/` b
 node metadata and attributes  
- `edges/` b
 transmission line topology and parameters  
- `time_series/` b
 demand and generation profiles over time  
- `meta/` b
 simulation configuration and provenance  

The HDF5 structure is designed to be directly consumable by both:

- Python MIP solver frontend  
- External analysis tools (HDF5 readers, MATLAB, etc.)

---

## Examples

The repository includes example programs for common workflows:

- `examples/visualize_grid.cpp`  
  Generates a DOT graph representation of the synthetic grid  

- `examples/grid_csv_export_static.cpp`  
  Exports node/edge topology into CSV format  

- `examples/example_json_input.json`  
  Minimal configuration for quick testing and debugging  

---

## Build and Execution Summary

After building:

```bash
build/bin/mip_grid -i examples/example_json_input.json
```


Optional modes:

- Visualization only:
  ```bash
  build/bin/mip_grid -i examples/example_json_input.json -v
  ```

- CSV export of static grid topology:

  ```bash
  build/bin/mip_grid -i examples/example_json_input.json --export-csv
  ```

- Display help / usage information:

  ```bash
  build/bin/mip_grid -h
  ```

## Contributing

Contributions are welcome, but development bandwidth is currently limited.

To keep discussion organized, please open an issue first to describe your idea or proposed change before submitting a pull request.

This helps ensure contributions align with the current architecture and research direction.

---

## References

- HDF5: [https://www.hdfgroup.org/solutions/hdf5/](https://www.hdfgroup.org/solutions/hdf5/)  
- Graphviz DOT format: [https://graphviz.org/doc/info/lang.html](https://graphviz.org/doc/info/lang.html)  
- Gurobi Optimization: [https://www.gurobi.com/](https://www.gurobi.com/)  

---

## License

This project is licensed under the **Apache License 2.0** with the **Commons Clause Addendum**.  

You may:

- Use, modify, and distribute the software for research or personal purposes.

You may **not**:

- Sell the software or provide it as a paid service, either fully or substantially derived from its functionality.

Full license text can be found in the [LICENSE](LICENSE) file.
