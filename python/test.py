import h5py

with h5py.File("grid_timeseries.h5", "r") as f:
    node_ids = f["nodes/id"][:]
    edge_from = f["edges/from"][:]
    edge_to = f["edges/to"][:]

    print("Node IDs:", node_ids)
    print("Edge from IDs:", edge_from)
    print("Edge to IDs:", edge_to)
