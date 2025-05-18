#pragma once

#include <juce_core/juce_core.h>
#include <type_traits> // for std::is_arithmetic
#include <iterator>    // for std::begin, std::end

namespace DebugUtils
{
    // Generic stringify for numeric containers
    template <typename Container,
              typename T = typename Container::value_type,
              std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    juce::String stringify(const Container& container, int decimalPlaces = 2)
    {
        juce::String result;
        for (const auto& value : container)
            result << juce::String(value, decimalPlaces) << " ";
        return result.trimEnd();
    }

    // Overload: stringify a range using iterators (more generic)
    template <typename Iterator>
    juce::String stringify(Iterator begin, Iterator end, int decimalPlaces = 2)
    {
        juce::String result;
        for (auto it = begin; it != end; ++it)
            result << juce::String(*it, decimalPlaces) << " ";
        return result.trimEnd();
    }

    // Print to stdout (for convenience)
    template <typename Container>
    void print(const Container& container, const juce::String& label = "Values:")
    {
        std::cout << label << " " << stringify(container).toStdString() << std::endl;
    }
}