from gurobipy import GRB


class LinearConstraints:
    """
    Wrapper around constraint functions to integrate with run_optimization.py
    """

    def __init__(self, model, network, snapshot, g, f):
        self.model = model
        self.network = network
        self.snapshot = snapshot
        self.g = g
        self.f = f

    # ============================================================
    # POWER BALANCE
    # ============================================================

    def add_power_balance_constraints(self):
        N = self.network.N
        E = self.network.E
        T = self.snapshot.num_timesteps

        A = self.network.incidence

        for t in range(T):
            for i in range(N):

                demand_it = self.snapshot.nodes.demand[i][t]

                expr = self.g[i, t]

                for e in range(E):
                    expr += A[i, e] * self.f[e, t]

                self.model.addConstr(
                    expr == demand_it,
                    name=f"power_balance_{i}_{t}"
                )

    # ============================================================
    # LINE LIMITS
    # ============================================================

    def add_capacity_constraints(self):

        E = self.network.E
        T = self.snapshot.num_timesteps
        cap = self.network.edge_capacity

        for e in range(E):
            for t in range(T):

                self.model.addConstr(
                    self.f[e, t] <= cap[e],
                    name=f"flow_upper_{e}_{t}"
                )

                self.model.addConstr(
                    self.f[e, t] >= -cap[e],
                    name=f"flow_lower_{e}_{t}"
                )

    # ============================================================
    # GENERATION LIMITS
    # ============================================================

    def add_generation_constraints(self):

        N = self.network.N
        T = self.snapshot.num_timesteps
        cap = self.network.max_capacity

        for i in range(N):
            for t in range(T):

                self.model.addConstr(
                    self.g[i, t] <= cap[i],
                    name=f"gen_upper_{i}_{t}"
                )

                self.model.addConstr(
                    self.g[i, t] >= 0.0,
                    name=f"gen_lower_{i}_{t}"
                )