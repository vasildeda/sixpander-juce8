#pragma once

#include <cmath>
#include <stdexcept>

// Calculate the smoothing factor alpha
inline double computeAlpha(double targetPercentage, double timeMs, double samplingRateHz) {
    // Ensure percentage is in (0,1)
    if (targetPercentage <= 0.0 || targetPercentage >= 1.0) {
        throw std::invalid_argument("targetPercentage must be between 0 and 1 (exclusive)");
    }
    if (timeMs <= 0.0 || samplingRateHz <= 0.0) {
        throw std::invalid_argument("timeMs and samplingRateHz must be positive");
    }

    // Convert time from milliseconds to seconds
    double timeSec = timeMs / 1000.0;

    // Calculate time constant tau
    double tau = -timeSec / std::log(1.0 - targetPercentage);

    // Calculate alpha
    double alpha = 1.0 - std::exp(-1.0 / (tau * samplingRateHz));
    return alpha;
}