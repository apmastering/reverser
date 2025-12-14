#pragma once

#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include <juce_core/juce_core.h>

#define DEBUG_MODE 0

inline double linearToExponential(double linearValue, const double minValue, const double maxValue) {
    linearValue = std::clamp(linearValue, minValue, maxValue);
    const double normalized = (linearValue - minValue) / (maxValue - minValue);
    const double exponentialValue = std::pow(normalized, 2.0);
    const double result = minValue + exponentialValue * (maxValue - minValue);
    return result;
}

inline double gainToDecibels(double gain) {
    if (gain <= 0.0)
        return -1000;
    
    if (gain > 1000) gain = 1000;

    return 20.0 * std::log10(gain);
}

inline float decibelsToGain(float decibels) {
    if (decibels <= -1000)
        return 0.0;
    
    if (decibels > 1000) decibels = 1000;
    
    return std::pow(10.0f, decibels / 20.0f);
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

inline ParameterQuery queryParameter(const ParameterNames paramName) {

    static const std::unordered_map<ParameterNames, ParameterQuery> paramNameMap = {
        {ParameterNames::inGain,       { "inGain",       "Input Gain",  ParameterNames::inGain  }},
        {ParameterNames::outGain,      { "outGain",      "Output Gain", ParameterNames::outGain }},
        {ParameterNames::reverse,      { "reverse",     "Reverse",      ParameterNames::reverse }},
    };
    
    if (paramName != ParameterNames::END) {
        if (const auto it = paramNameMap.find(paramName); it != paramNameMap.end()) return it->second;
    }

    throw std::invalid_argument("Enum query failed for parameter for queryParameter");
}
