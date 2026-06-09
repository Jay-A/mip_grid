# network_model.py

import numpy as np

class PowerNetwork:
    """
    Constructs a network from a loaded snapshot.
    Handles node/edge attributes and builds incidence/adjacency matrices.
    """

    def __init__(self, snapshot):
        # -------------------------
        # Basic dimensions
        # -------------------------
        self.N = snapshot.num_nodes
        self.E = snapshot.num_edges

        # -------------------------
        # Node data
        # -------------------------
        self.node_id = snapshot.nodes.id
        self.node_type = snapshot.nodes.type
        self.base_load = snapshot.nodes.base_load
        self.max_capacity = snapshot.nodes.max_capacity

        # -------------------------
        # Edge data
        # -------------------------
        self.edge_id = snapshot.edges.id
        self.edge_from = snapshot.edges.from_
        self.edge_to = snapshot.edges.to
        self.edge_capacity = snapshot.edges.max_capacity
        self.edge_reactance = snapshot.edges.reactance

        # -------------------------
        # Topology structures
        # -------------------------
        self.incidence = self._build_incidence()
        self.adjacency = self._build_adjacency()

        # -------------------------
        # Time expansion (lazy)
        # -------------------------
        self.T = None
        self.time_expanded = False

    # ============================================================
    # Graph construction
    # ============================================================
    def _build_incidence(self):
        A = np.zeros((self.N, self.E), dtype=np.float64)
        for e in range(self.E):
            i = int(self.edge_from[e])
            j = int(self.edge_to[e])
            A[i, e] = -1.0
            A[j, e] = 1.0
        return A

    def _build_adjacency(self):
        adj = {i: [] for i in range(self.N)}
        for e in range(self.E):
            i = int(self.edge_from[e])
            j = int(self.edge_to[e])
            adj[i].append(j)
            adj[j].append(i)
        return adj

    # ============================================================
    # Time expansion
    # ============================================================
    def expand_time_series(self, num_timesteps):
        self.T = num_timesteps
        self.node_time_index = {}
        idx = 0
        for t in range(self.T):
            for i in range(self.N):
                self.node_time_index[(i, t)] = idx
                idx += 1
        self.E_time = self.E * self.T
        self.time_expanded = True
        return self.node_time_index
