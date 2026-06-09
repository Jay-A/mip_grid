# run_optimization.py

import argparse
import logging
from snapshot_loader import SnapshotLoader
from network_model import PowerNetwork
from model import PowerGridModel
from constraints import LinearConstraints
from solver import PowerGridSolver

def main(snapshot_file: str, time_limit: int, mip_gap: float):

    logging.basicConfig(level=logging.INFO, format="%(levelname)s: %(message)s")
    logging.info("Starting power grid optimization...")

    # -------------------------------------------------
    # 1. Load snapshot
    # -------------------------------------------------
    logging.info(f"Loading snapshot from {snapshot_file}...")
    loader = SnapshotLoader(snapshot_file)
    snapshot = loader.load()  # returns proper container object

    # -------------------------------------------------
    # 2. Build network
    # -------------------------------------------------
    logging.info("Constructing power network...")
    network = PowerNetwork(snapshot)
    network.expand_time_series(snapshot.num_timesteps)

    # -------------------------------------------------
    # 3. Build optimization model
    # -------------------------------------------------
    logging.info("Building optimization model...")
    model = PowerGridModel(snapshot, network)
    g = model.g
    f = model.f

    # -------------------------------------------------
    # 4. Add constraints
    # -------------------------------------------------
    logging.info("Adding constraints...")
    constraints = LinearConstraints(model=model.model, network=network, snapshot=snapshot, g=g, f=f)
    constraints.add_power_balance_constraints()
    constraints.add_capacity_constraints()
    constraints.add_generation_constraints()

    # -------------------------------------------------
    # 5. Solve
    # -------------------------------------------------
    logging.info("Solving with Gurobi...")
    solver = PowerGridSolver(model.model, network, snapshot)
    if time_limit:
        solver.model.Params.TimeLimit = time_limit
    if mip_gap:
        solver.model.Params.MIPGap = mip_gap
    solver.solve(verbose=True)

    # -------------------------------------------------
    # 6. Results
    # -------------------------------------------------
    g = solver.g
    f = solver.f
    logging.info("Done.")
    for i in range(min(3, network.N)):
        logging.info(f"Node {i} gen sample: {g[i, :5]}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run power grid optimization")
    parser.add_argument("snapshot_file", type=str)
    parser.add_argument("--time_limit", type=int, default=300)
    parser.add_argument("--mip_gap", type=float, default=0.01)
    args = parser.parse_args()
    main(args.snapshot_file, args.time_limit, args.mip_gap)
