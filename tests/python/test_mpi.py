# tests/python/test_mpi.py
import subprocess
import os

# Root of repo (tests/python -> project root)
ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
BUILD_DIR = os.path.join(ROOT_DIR, "build")


def run_ctest(args):
    """
    Run ctest in the correct build directory and return:
    (exit_code, combined_output)
    """
    result = subprocess.run(
        ["ctest", "--test-dir", BUILD_DIR] + args,
        text=True,
        capture_output=True
    )
    return result.returncode, result.stdout + result.stderr


def test_mpi_group():
    """
    MPI integration test:
    - verifies MPI test runs via CTest
    - verifies successful execution (exit code only)
    """
    code, output = run_ctest([
        "-L", "mpi",
        "--output-on-failure"
    ])

    # optional debug (useful during development)
    print(output)

    # ONLY stable assertion in MPI+CTEST+SLURM systems
    assert code == 0
