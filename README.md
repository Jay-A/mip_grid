# MIP Grid: Power Grid Numerical Methods Research Platform

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
- CMake b	% 3.18  
- HDF5 library  
- Python b	% 3.9 (for solver API)  
- Optional: Graphviz for DOT visualization  

### Build

```bash
git clone <repo-url>
cd mip_grid
mkdir build && cd build
cmake ..
make
```

Binary outputs will be in `build/bin/mip_grid`.

---

## Quick Start

### Generate HDF5 Grid Snapshot

```bash
build/bin/mip_grid -i configs/small.yaml
```

Output:

```text
Successfully wrote: grid_timeseries.h5
```

---

### View Grid Graph

```bash
build/bin/mip_grid -i configs/small.yaml -v
dot -Tpng grid.dot -o grid.png
xdg-open grid.png
```

---

### Export Grid to CSV

```bash
build/bin/mip_grid -i configs/small.yaml --export-csv
```

---

## Python Integration

Once a grid HDF5 snapshot is generated, it can be loaded in Python for solver workflows:

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

---

## Configuration Files

The system is driven by YAML/JSON configuration files located in `configs/`.

Each configuration defines:

- Grid size and topology parameters  
- Random seed for reproducibility  
- Generator/load ratios  
- Edge capacity and reactance ranges  
- Time series generation settings  

Example files:

- `small.yaml` b
 minimal test grid for CI validation  
- `medium.yaml` b
 moderate-scale research grid  
- `large.yaml` b
 high-scale performance benchmarking grid  

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
build/bin/mip_grid -i configs/small.yaml
```


Optional modes:

- Visualization only:
  ```bash
  build/bin/mip_grid -i configs/small.yaml -v
  ```

- CSV export of static grid topology:

  ```bash
  build/bin/mip_grid -i configs/small.yaml --export-csv
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
