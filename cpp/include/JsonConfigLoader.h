#pragma once

#include "GridSpec.h"
#include "TimeSeriesAttributes.h"

#include <string>

class JsonConfigLoader
{
public:
    // Load JSON file into GridSpec and TimeSeriesAttributes structs
    static void load(const std::string& filename, GridSpec& spec, TimeSeriesAttributes& ts_attr);
};
