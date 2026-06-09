# -------------------------------------------------------------
# Examples / visualization tools
# -------------------------------------------------------------
# These are NOT part of mip_core.
# They are standalone executables linked against the library.

# Existing visualization tool
add_executable(visualize_grid
    visualize_grid.cpp
)

target_include_directories(visualize_grid PRIVATE
    ${CMAKE_SOURCE_DIR}/cpp/include
)

target_link_libraries(visualize_grid PRIVATE
    mip_core
    mip_utils
)

# -------------------------------------------------------------
# Static grid snapshot exporter
# -------------------------------------------------------------
add_executable(grid_snapshot
    grid_csv_export_static.cpp
)

target_include_directories(grid_snapshot PRIVATE
    ${CMAKE_SOURCE_DIR}/cpp/include
)

target_link_libraries(grid_snapshot PRIVATE
    mip_core
    mip_utils
)
