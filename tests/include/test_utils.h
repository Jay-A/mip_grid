#pragma once

#include <cstddef>

#include "Types.h"
#include "Grid.h"
#include "InjectedGrid.h"
#include "TimeSeries.h"
#include "GridAttributes.h"
#include "GridBuilder.h"

/**
 * @file test_utils.h
 * @brief Deterministic test fixture builders for grid simulation pipeline.
 *
 * These helpers construct fully valid, aligned datasets for unit and
 * integration testing. They mirror the production pipeline using
 * simplified deterministic logic.
 */

// ============================================================
// Topology
// ============================================================

/**
 * @brief Build a deterministic test grid using GridBuilder.
 *
 * @note This is the ONLY valid way to construct Grid in tests.
 */
Grid make_test_grid(std::size_t nodes = 10, std::size_t edges = 12);

// ============================================================
// InjectedGrid
// ============================================================

/**
 * @brief Build a fully attributed InjectedGrid from a test topology.
 *
 * @note Node/edge attributes are deterministic and simplified.
 */
InjectedGrid make_test_injected_grid(std::size_t nodes = 10,
                                      std::size_t edges = 12);

// ============================================================
// TimeSeries
// ============================================================

/**
 * @brief Build a deterministic TimeSeries aligned to an InjectedGrid.
 *
 * @param grid InjectedGrid providing node topology reference
 * @param timesteps Number of time steps per node
 */
TimeSeries make_test_timeseries(const InjectedGrid& grid,
                                std::size_t timesteps = 24);

// ============================================================
// Full dataset helper (optional but strongly useful)
// ============================================================

/**
 * @brief Build a fully consistent test dataset (grid + attributes + time series).
 *
 * @note Useful for exporter and bundle tests.
 */
struct TestDataset {
    Grid grid;
    InjectedGrid injected;
    TimeSeries timeseries;
};

/**
 * @brief Construct a fully aligned dataset in one call.
 */
TestDataset make_test_dataset(std::size_t nodes = 10,
                              std::size_t edges = 12,
                              std::size_t timesteps = 24);
