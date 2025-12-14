#pragma once

#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include <juce_core/juce_core.h>

#define DEBUG_MODE 0

inline double linearToExponential(double linearValue, double minValue, double maxValue) {
    linearValue = std::clamp(linearValue, minValue, maxValue);
    double normalized = (linearValue - minValue) / (maxValue - minValue);
    double exponentialValue = std::pow(normalized, 2.0);
    double result = minValue + exponentialValue * (maxValue - minValue);
    return result;
}

inline double gainToDecibels(double gain) {
    if (gain <= 0.0)
        return -1000;
    
    if (gain > 1000) gain = 1000;

    return 20.0 * std::log10(gain);
}

inline double decibelsToGain(double decibels) {
    if (decibels <= -1000)
        return 0.0;
    
    if (decibels > 1000) decibels = 1000;
    
    return std::pow(10.0, decibels / 20.0);
}

enum class ButtonName {
    saturate,
    reverse,
    none
};

enum class ParameterNames {
    inGain,
    outGain,
    reverse,
    END
};

struct ParameterQuery {
    std::string id;
    std::string label;
    ParameterNames parameterEnum;
};

inline ParameterQuery queryParameter(ParameterNames paramName) {

    static const std::unordered_map<ParameterNames, ParameterQuery> paramNameMap = {
        {ParameterNames::inGain,       { "inGain",       "Input Gain",  ParameterNames::inGain  }},
        {ParameterNames::outGain,      { "outGain",      "Output Gain", ParameterNames::outGain }},
        {ParameterNames::reverse,      { "reverse",     "Reverse",      ParameterNames::reverse }},
    };
    
    if (paramName != ParameterNames::END) {
        auto it = paramNameMap.find(paramName);
        if (it != paramNameMap.end()) return it->second;
    }

    static const std::unordered_map<std::string, ParameterNames> nameToEnumMap = {
        {"inGain",        ParameterNames::inGain},
        {"outGain",       ParameterNames::outGain},
        {"reverse",       ParameterNames::reverse},
    };
    
    throw std::invalid_argument("Both enum and string queries failed for parameter for queryParameter");
}
