#pragma once

#include "GridSpec.h"
#include "TimeSeriesAttributes.h"
#include "Types.h"

#include <string>

/**
 * @file YamlConfigLoader.h
 * @brief YAML-based configuration loader for grid and time series setup.
 *
 * @defgroup config_loader YamlConfigLoader
 * @brief Loads simulation configuration from YAML files into core data structures.
 *
 * YamlConfigLoader is responsible for parsing external YAML configuration files
 * and populating the core simulation configuration structs:
 * - \ref GridSpec (static grid topology and physics parameters)
 * - \ref TimeSeriesAttributes (dynamic time series generation parameters)
 *
 * It acts as a boundary layer between external configuration files and
 * internal deterministic simulation structures.
 *
 * @section design Design Philosophy
 * - Pure configuration ingestion layer
 * - No simulation logic or transformation beyond parsing
 * - Stateless and deterministic (given same input file)
 * - Centralized entry point for experiment configuration
 *
 * @section scope Scope of Responsibility
 * This loader is responsible only for:
 * - Parsing YAML configuration files
 * - Populating GridSpec
 * - Populating TimeSeriesAttributes
 *
 * It does NOT:
 * - Validate simulation physics
 * - Modify runtime grid or time series objects
 * - Perform any stochastic generation
 *
 * @section usage Example
 * @code
 * GridSpec spec;
 * TimeSeriesAttributes attr;
 *
 * YamlConfigLoader::load("config.yaml", spec, attr);
 * @endcode
 *
 * @section type_summary Type dependencies
 * - std::string -> file path and labels
 * - GridSpec -> static grid configuration
 * - TimeSeriesAttributes -> dynamic time series configuration
 */
class YamlConfigLoader
{
public:

    /**
     * @brief Load configuration from YAML file.
     *
     * Populates both GridSpec and TimeSeriesAttributes from the given file.
     *
     * @param filename Path to YAML configuration file
     * @param spec Output GridSpec structure
     * @param ts_attr Output TimeSeriesAttributes structure
     */
    static void load(const std::string& filename,
                     GridSpec& spec,
                     TimeSeriesAttributes& ts_attr);
};