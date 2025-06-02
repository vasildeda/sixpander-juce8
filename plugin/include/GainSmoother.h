#pragma once

#include <cmath>
#include <iostream>

class GainSmoother
{
public:
    GainSmoother() = default;

    void setSampleRate(double sampleRate)
    {
        sr = static_cast<float>(sampleRate);
        updateCoefficients();
    }

    void setAttackTime(float timeInSeconds)
    {
        attackTime = timeInSeconds;
        updateCoefficients();
    }

    void setReleaseTime(float timeInSeconds)
    {
        releaseTime = timeInSeconds;
        updateCoefficients();
    }

    void reset(float initialValue = 0.0f)
    {
        current = initialValue;
    }

    float process(float target)
    {
        const float coeff = (target > current) ? attackCoeff : releaseCoeff;
        current = coeff * current + (1.0f - coeff) * target;
        if (debug)
        {
            std::cout << "Target: " << target << ", Current: " << current << "                                              \r";
        }
        return current;
    }

    float getCurrent() const { return current; }

    void setDebug(bool enable) { debug = enable; }

private:
    void updateCoefficients()
    {
        constexpr float minTime = 1e-6f;
        float safeAttackTime  = std::max(attackTime, minTime);
        float safeReleaseTime = std::max(releaseTime, minTime);
        float safeSR = std::max(sr, 1.0f);

        // Reach ~99% of target in given time
        attackCoeff  = std::expf(-std::logf(100.0f) / (safeAttackTime  * safeSR));
        releaseCoeff = std::expf(-std::logf(100.0f) / (safeReleaseTime * safeSR));

        if (debug)
        {
            std::cout << "Updated coefficients: "
                    << "Attack Coeff: " << attackCoeff
                    << ", Release Coeff: " << releaseCoeff
                    << ", Sample Rate: " << sr
                    << ", Attack Time: " << attackTime
                    << ", Release Time: " << releaseTime
                    << std::endl;
        }
    }

    float attackTime  = 0.01f;  // seconds
    float releaseTime = 0.1f;   // seconds
    float sr = 44100.0;

    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    float current = 0.0f;

    bool debug = false;
};