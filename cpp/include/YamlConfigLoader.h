#pragma once

#include "GridSpec.h"
#include "TimeSeriesAttributes.h"
#include "Types.h"

#include <string>

class YamlConfigLoader
{
public:
    // Load from file into GridSpec and TimeSeriesAttributes structs
    static void load(const std::string& filename, GridSpec& spec, TimeSeriesAttributes& ts_attr);
};

