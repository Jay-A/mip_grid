#pragma once

#include <cstdint>
#include <string>

/**
 * @file TimeSeriesAttributes.h
 * @brief Configuration struct for deterministic time series generation.
 *
 * This struct encapsulates all user-controllable parameters for 
 * generating node-wise synthetic demand and generation profiles. 
 * It is intended to be passed to \ref TimeSeriesConstructor for 
 * deterministic construction of a \ref TimeSeries object.
 *
 * @section design Design Philosophy
 * - Simple, header-only configuration
 * - Serializable and copyable
 * - No internal logic, pure data
 * - Deterministic via RNG seed
 *
 * @section usage Example
 * @code
 * TimeSeriesAttributes attr;
 * attr.timesteps = 24;
 * attr.sample_frequency_minutes = 60;
 * attr.demand_modulation_amplitude = 0.2;
 * attr.generator_modulation_amplitude = 0.0;
 * attr.noise_amplitude = 0.01;
 * attr.seed = 42;
 * @endcode
 */
struct TimeSeriesAttributes {

    /// Number of time points in the series (e.g., 24 hours)
    std::size_t timesteps{24};

    /// Sampling frequency in minutes (e.g., 60 = hourly)
    std::size_t sample_frequency_minutes{60};

    /// Maximum relative modulation around node base load [0.0, 1.0]
    double demand_modulation_amplitude{0.1};

    /// Maximum relative modulation around generator capacity [0.0, 1.0]
    double generator_modulation_amplitude{0.0};

    /// Maximum amplitude of additive stochastic noise [0.0, 1.0]
    double noise_amplitude{0.01};

    /// Global RNG seed for deterministic generation
    uint64_t seed{0};

    /// Optional label for identifying the time series configuration
    std::string label{""};

    /**
     * @brief Default constructor.
     * 
     * Initializes attributes with typical defaults:
     * - 24 timesteps (hourly)
     * - 60 min sample frequency
     * - 10% demand modulation
     * - 0% generator modulation
     * - 1% noise amplitude
     * - Seed 0
     */
    TimeSeriesAttributes() = default;

    /**
     * @brief Construct with explicit parameters.
     *
     * @param ts Number of timesteps
     * @param freq Sampling frequency in minutes
     * @param demand_amp Relative amplitude for demand modulation
     * @param gen_amp Relative amplitude for generator modulation
     * @param noise_amp Maximum amplitude of stochastic noise
     * @param s RNG seed
     * @param lbl Optional label
     */
    TimeSeriesAttributes(
        std::size_t ts,
        std::size_t freq,
        double demand_amp,
        double gen_amp,
        double noise_amp,
        uint64_t s,
        const std::string& lbl = "")
        : timesteps(ts),
          sample_frequency_minutes(freq),
          demand_modulation_amplitude(demand_amp),
          generator_modulation_amplitude(gen_amp),
          noise_amplitude(noise_amp),
          seed(s),
          label(lbl)
    {}
};