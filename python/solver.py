import numpy as np
from gurobipy import GRB


class PowerGridSolver:
    """
    Executes a built Gurobi model and extracts results
    into structured numpy arrays.
    """

    def __init__(self, model, network, snapshot):
        self.model = model
        self.network = network
        self.snapshot = snapshot

        self.g = None   # (N, T)
        self.f = None   # (E, T)

        self.status = None

    # ============================================================
    # SOLVE
    # ============================================================

    def solve(self, verbose=False):
        """
        Runs Gurobi optimization.
        """

        if not verbose:
            self.model.Params.OutputFlag = 0

        self.model.optimize()

        self.status = self.model.status

        if self.status != GRB.OPTIMAL:
            raise RuntimeError(f"Optimization failed. Status: {self.status}")

        self._extract_solution()

        return self

    # ============================================================
    # SOLUTION EXTRACTION
    # ============================================================

    def _extract_solution(self):
        """
        Converts Gurobi decision variables into numpy arrays.
        """

        N = self.network.N
        E = self.network.E
        T = int(self.snapshot.meta["num_timesteps"])

        # -------------------------
        # Allocate arrays
        # -------------------------
        self.g = np.zeros((N, T), dtype=np.float64)
        self.f = np.zeros((E, T), dtype=np.float64)

        # -------------------------
        # Extract generation
        # -------------------------
        for i in range(N):
            for t in range(T):
                self.g[i, t] = self.model.getVarByName(f"gen[{i},{t}]").X

        # -------------------------
        # Extract flows
        # -------------------------
        for e in range(E):
            for t in range(T):
                self.f[e, t] = self.model.getVarByName(f"flow[{e},{t}]").X

    # ============================================================
    # SIMPLE ACCESSORS
    # ============================================================

    def generation(self):
        return self.g

    def flows(self):
        return self.f

    def node_balance(self):
        """
        Quick diagnostic: generation - demand imbalance per node/time
        """
        N = self.network.N
        T = int(self.snapshot.meta["num_timesteps"])

        demand = self.snapshot.nodes.demand

        return self.g - demand
