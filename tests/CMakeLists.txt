set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/tests)

# =======================================================
# TEST UTILITIES (shared test-only library)
# =======================================================

add_library(test_utils_lib
    unit/test_utils.cpp
)

target_link_libraries(test_utils_lib PRIVATE mip_core)

target_include_directories(test_utils_lib PUBLIC
    include/
    mip_core
    mip_utils
)

# =======================================================
# SERIAL TESTS (safe default suite)
# =======================================================

add_executable(test_node_creation
    unit/test_node_creation.cpp
)

target_link_libraries(test_node_creation PRIVATE mip_core)

set_target_properties(test_node_creation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.NodeCreation
    COMMAND ${TEST_OUTPUT_DIR}/test_node_creation
)

set_tests_properties(unit.NodeCreation PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------

add_executable(test_edge_creation
    unit/test_edge_creation.cpp
)

target_link_libraries(test_edge_creation PRIVATE mip_core)

set_target_properties(test_edge_creation PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.EdgeCreation
    COMMAND ${TEST_OUTPUT_DIR}/test_edge_creation
)

set_tests_properties(unit.EdgeCreation PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------

add_executable(test_edge_connectivity
    unit/test_edge_connectivity.cpp
)

target_link_libraries(test_edge_connectivity PRIVATE mip_core)

set_target_properties(test_edge_connectivity PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.EdgeConnectivity
    COMMAND ${TEST_OUTPUT_DIR}/test_edge_connectivity
)

set_tests_properties(unit.EdgeConnectivity PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------

add_executable(test_grid_feasibility
    unit/test_grid_feasibility.cpp
)

target_link_libraries(test_grid_feasibility PRIVATE mip_core)

set_target_properties(test_grid_feasibility PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.GridFeasibility
    COMMAND ${TEST_OUTPUT_DIR}/test_grid_feasibility
)

set_tests_properties(unit.GridFeasibility PROPERTIES
    LABELS "serial"
)

# =======================================================
# MPI TESTS (cluster-only suite)
# =======================================================

add_executable(test_mpi_startup
    mpi/test_mpi_startup.cpp
)

target_link_libraries(test_mpi_startup
    PRIVATE MPI::MPI_CXX
)

target_include_directories(test_mpi_startup PRIVATE
    ${CMAKE_SOURCE_DIR}/cpp/include
)

set_target_properties(test_mpi_startup PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME mpi.Startup
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/mpi_test_helper.sh
            ${TEST_OUTPUT_DIR}/test_mpi_startup
)

set_tests_properties(mpi.Startup PROPERTIES
    LABELS "mpi;startup"
)

# =======================================================
# GRID + INFRASTRUCTURE TESTS
# =======================================================

add_executable(test_grid_injected
    unit/test_grid_injected.cpp
)

target_link_libraries(test_grid_injected PRIVATE
    mip_core
    mip_utils
)

set_target_properties(test_grid_injected PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.GridInjected
    COMMAND ${TEST_OUTPUT_DIR}/test_grid_injected
)

set_tests_properties(unit.GridInjected PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------
# TIME SERIES TESTS
# -------------------------------------------------------

add_executable(test_time_series_shape
    unit/test_time_series_shape.cpp
)

add_executable(test_time_series_determinism
    unit/test_time_series_determinism.cpp
)

target_link_libraries(test_time_series_shape PRIVATE mip_core)
target_link_libraries(test_time_series_determinism PRIVATE mip_core)

set_target_properties(test_time_series_shape PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)
set_target_properties(test_time_series_determinism PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.TimeSeriesShape
    COMMAND ${TEST_OUTPUT_DIR}/test_time_series_shape
)

add_test(NAME unit.TimeSeriesDeterminism
    COMMAND ${TEST_OUTPUT_DIR}/test_time_series_determinism
)

set_tests_properties(unit.TimeSeriesShape PROPERTIES LABELS "serial")
set_tests_properties(unit.TimeSeriesDeterminism PROPERTIES LABELS "serial")

# -------------------------------------------------------

add_executable(test_time_series_node_alignment
    unit/test_time_series_node_alignment.cpp
)

target_link_libraries(test_time_series_node_alignment PRIVATE mip_core)

set_target_properties(test_time_series_node_alignment PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.TimeSeriesNodeAlignment
    COMMAND ${TEST_OUTPUT_DIR}/test_time_series_node_alignment
)

set_tests_properties(unit.TimeSeriesNodeAlignment PROPERTIES
    LABELS "serial"
)

# =======================================================
# DATASET EXPORTER TESTS
# =======================================================

add_executable(test_dataset_exporter
    unit/test_grid_dataset_exporter.cpp
)

target_link_libraries(test_dataset_exporter PRIVATE
    mip_core
    mip_utils
    hdf5
)

set_target_properties(test_dataset_exporter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.DatasetExporterHDF5
    COMMAND ${TEST_OUTPUT_DIR}/test_dataset_exporter
)

set_tests_properties(unit.DatasetExporterHDF5 PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------

add_executable(test_grid_dataset_export_roundtrip
    unit/test_grid_dataset_export_roundtrip.cpp
)

target_link_libraries(test_grid_dataset_export_roundtrip PRIVATE
    mip_core
    mip_utils
    hdf5
)

set_target_properties(test_grid_dataset_export_roundtrip PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.DatasetExporterHDF5Roundtrip
    COMMAND ${TEST_OUTPUT_DIR}/test_grid_dataset_export_roundtrip
)

set_tests_properties(unit.DatasetExporterHDF5Roundtrip PROPERTIES
    LABELS "serial"
)

# =======================================================
# POWER GRID SNAPSHOT TESTS
# =======================================================

add_executable(test_power_grid_snapshot_construction
    unit/test_power_grid_snapshot_construction.cpp
)

target_link_libraries(test_power_grid_snapshot_construction PRIVATE
    mip_core
    mip_utils
    test_utils_lib
)

set_target_properties(test_power_grid_snapshot_construction PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.PowerGridSnapshotConstruction
    COMMAND ${TEST_OUTPUT_DIR}/test_power_grid_snapshot_construction
)

set_tests_properties(unit.PowerGridSnapshotConstruction PROPERTIES
    LABELS "serial"
)

add_executable(test_power_grid_snapshot_hdf5_write
    unit/test_snapshot_hdf5_writer.cpp
)

target_link_libraries(test_power_grid_snapshot_hdf5_write PRIVATE
    mip_core
    mip_utils
    test_utils_lib
    hdf5
    hdf5_cpp
)

set_target_properties(test_power_grid_snapshot_hdf5_write PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.PowerGridSnapshotHDF5Write
    COMMAND ${TEST_OUTPUT_DIR}/test_power_grid_snapshot_hdf5_write
)

set_tests_properties(unit.PowerGridSnapshotHDF5Write PROPERTIES
    LABELS "serial"
)

# -------------------------------------------------------------
# JSON Config Loader Unit Test
# -------------------------------------------------------------
add_executable(test_json_config_loader
    unit/test_json_config_loader.cpp
)

target_link_libraries(test_json_config_loader PRIVATE
    mip_core       # contains JsonConfigLoader, GridSpec, TimeSeriesAttributes
    test_utils_lib # optional, for common test utilities if any
)

set_target_properties(test_json_config_loader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_test(NAME unit.JsonConfigLoader
    COMMAND ${TEST_OUTPUT_DIR}/test_json_config_loader
)

set_tests_properties(unit.JsonConfigLoader PROPERTIES
    LABELS "serial"
)
