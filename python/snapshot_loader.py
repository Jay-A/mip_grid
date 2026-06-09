# snapshot_loader.py

import h5py
import numpy as np
from types import SimpleNamespace


class SnapshotLoader:
    """
    Loads HDF5 snapshot and returns a simple container object
    with nodes, edges, time series, and meta information.
    """

    def __init__(self, h5_file):
        self.h5_file = h5_file
        
    def load(self):
        with h5py.File(self.h5_file, "r") as f:

            # -------------------
            # Meta (flatten scalars)
            # -------------------
            meta = {k: f["meta"][k][()] for k in f["meta"].keys()}

            # -------------------
            # Nodes
            # -------------------
            nodes_group = f["nodes"]
            nodes = SimpleNamespace(
                id=np.array(nodes_group["id"]),
                type=np.array(nodes_group["type"]),
                base_load=np.array(nodes_group["base_load"]),
                max_capacity=np.array(nodes_group["max_capacity"]),
                demand=np.array(f["time_series"]["demand"])   # <--- added here
            )

            # -------------------
            # Edges
            # -------------------
            edges_group = f["edges"]
            edges = SimpleNamespace(
                id=np.array(edges_group["id"]),
                from_=np.array(edges_group["from"]),
                to=np.array(edges_group["to"]),
                max_capacity=np.array(edges_group["max_capacity"]),
                reactance=np.array(edges_group["reactance"])
            )

            # -------------------
            # Time series
            # -------------------
            ts_group = f["time_series"]
            time_series = SimpleNamespace(
                demand=np.array(ts_group["demand"]),
                generation=np.array(ts_group["generation"])
            )

            return SimpleNamespace(
                meta=meta,
                nodes=nodes,
                edges=edges,
                time_series=time_series,
                num_nodes=meta["num_nodes"],
                num_edges=meta["num_edges"],
                num_timesteps=meta["num_timesteps"]
            )