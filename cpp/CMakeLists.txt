# -----------------------------
# Core library (ALL logic here)
# -----------------------------
add_library(mip_core
    src/Grid.cpp
    src/GridGenerator.cpp
    src/GridValidator.cpp
    src/GridBuilder.cpp
    src/Node.cpp
    src/Edge.cpp
    src/GridInjector.cpp
    src/TimeSeries.cpp
    src/TimeSeriesConstructor.cpp
    src/JsonConfigLoader.cpp
)

target_include_directories(mip_core
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(mip_core
    PUBLIC MPI::MPI_CXX
)

# -----------------------------
# Utilities library (analysis / I/O / visualization)
# -----------------------------

add_library(mip_utils
    src/GridExport.cpp
    src/DotGraphExporter.cpp
    src/GridDatasetExporter.cpp
    src/PowerGridSnapshot.cpp
    src/PowerGridSnapshotHDF5Writer.cpp
)

target_link_libraries(mip_utils
    PUBLIC mip_core
    PUBLIC hdf5
    PUBLIC hdf5_cpp
)

target_link_libraries(mip_utils
    PUBLIC mip_core
)

# -----------------------------
# Main executable (thin wrapper)
# -----------------------------
add_executable(mip_grid
    src/main.cpp
)

target_link_libraries(mip_grid
    PRIVATE
        mip_core
        mip_utils
)

# -----------------------------
# CSV export executable (analysis tool)
# -----------------------------
#add_executable(gridexport
#    src/GridExport.cpp   # or wherever your CLI entry lives
#)
#
#target_include_directories(gridexport PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/include
#)
#
#target_link_libraries(gridexport PRIVATE
#    mip_core
#    mip_utils
#)    