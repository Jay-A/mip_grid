import gurobipy as gp
from gurobipy import GRB


class PowerGridModel:
    """
    Builds a linear MIP power dispatch model.

    Assumes:
    - snapshot: contains network + time series demand
    - network: incidence + topology
    """

    def __init__(self, snapshot, network):
        self.snapshot = snapshot
        self.network = network

        self.model = gp.Model("power_grid_dispatch")

        # Decision variables
        self.g = None   # generation
        self.f = None   # line flows (optional but included)

        self._build_variables()
        self._add_constraints()
        self._set_objective()

    # ============================================================
    # VARIABLES
    # ============================================================

    def _build_variables(self):
        # -------------------------
        # Dimensions
        # -------------------------
        N = int(self.network.N)  # number of nodes
        E = int(self.network.E)  # number of edges
        T = int(self.network.T)  # number of timesteps

        # -------------------------
        # Generation variables (nodes × time)
        # -------------------------
        self.g = self.model.addVars(
            range(N),
            range(T),
            lb=0.0,
            name="gen"
        )

        # -------------------------
        # Flow variables (edges × time)
        # can be positive or negative
        # -------------------------
        self.f = self.model.addVars(
            range(E),
            range(T),
            lb=-GRB.INFINITY,
            name="flow"
        )

    # ============================================================
    # CONSTRAINTS
    # ============================================================

    def _add_constraints(self):
        self._power_balance()
        self._line_limits()

    def _power_balance(self):
        """
        Kirchhoff-style node balance:
        generation + inflow - outflow = demand
        """

        N = self.network.N
        E = self.network.E
        T = int(self.snapshot.meta["num_timesteps"])

        A = self.network.incidence  # N x E

        for t in range(T):
            for i in range(N):

                demand_it = self.snapshot.nodes.demand[i][t]

                expr = self.g[i, t]

                # add flows via incidence matrix
                for e in range(E):
                    expr += A[i, e] * self.f[e, t]

                self.model.addConstr(
                    expr == demand_it,
                    name=f"balance_{i}_{t}"
                )

    def _line_limits(self):
        """
        Flow capacity constraints on edges
        """

        E = self.network.E
        T = int(self.snapshot.meta["num_timesteps"])

        cap = self.network.edge_capacity

        for e in range(E):
            for t in range(T):
                self.model.addConstr(
                    self.f[e, t] <= cap[e],
                    name=f"flow_pos_{e}_{t}"
                )

                self.model.addConstr(
                    self.f[e, t] >= -cap[e],
                    name=f"flow_neg_{e}_{t}"
                )

    # ============================================================
    # OBJECTIVE
    # ============================================================

    def _set_objective(self):
        """
        Minimal objective:
        - minimize total generation (placeholder cost model)
        """

        N = self.network.N
        T = int(self.snapshot.meta["num_timesteps"])

        obj = gp.quicksum(self.g[i, t] for i in range(N) for t in range(T))

        self.model.setObjective(obj, GRB.MINIMIZE)

    # ============================================================
    # SOLVE
    # ============================================================

    def solve(self):
        self.model.optimize()

        if self.model.status != GRB.OPTIMAL:
            raise RuntimeError("No optimal solution found")

        return self.model
