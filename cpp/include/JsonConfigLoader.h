#pragma once

#include "GridSpec.h"
#include "TimeSeriesAttributes.h"

#include <string>

/**
 * @file JsonConfigLoader.h
 * @brief Load grid and time series configurations from a JSON file.
 *
 * JsonConfigLoader provides a stateless interface to populate:
 * - GridSpec: declarative grid generation parameters
 * - TimeSeriesAttributes: per-node/per-edge time series metadata
 *
 * This loader does not retain any internal state.
 *
 * @section usage Example
 * @code
 * GridSpec spec;
 * TimeSeriesAttributes ts_attr;
 * JsonConfigLoader::load("config.json", spec, ts_attr);
 * @endcode
 */
class JsonConfigLoader {
public:

    // ============================================================
    // Loading API
    // ============================================================

    /**
     * @brief Load JSON configuration into GridSpec and TimeSeriesAttributes.
     *
     * @param filename Path to JSON configuration file
     * @param spec Output GridSpec structure (populated)
     * @param ts_attr Output TimeSeriesAttributes structure (populated)
     *
     * @throws std::runtime_error if file cannot be read or parsing fails
     */
    static void load(const std::string& filename,
                     GridSpec& spec,
                     TimeSeriesAttributes& ts_attr);
};