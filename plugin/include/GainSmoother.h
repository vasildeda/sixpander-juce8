#pragma once

#include <cmath>

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

    void reset(float initialValue = 1.0f)
    {
        current = initialValue;
    }

    float process(float target)
    {
        const float coeff = (target > current) ? attackCoeff : releaseCoeff;
        current = coeff * current + (1.0f - coeff) * target;
        return current;
    }

    float getCurrent() const { return current; }

private:
    void updateCoefficients()
    {
        attackCoeff  = std::expf(-1.0f / (attackTime  * sr));
        releaseCoeff = std::expf(-1.0f / (releaseTime * sr));
    }

    float attackTime  = 0.01f;  // seconds
    float releaseTime = 0.1f;   // seconds
    float sr = 44100.0;

    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    float current = 1.0f;
};