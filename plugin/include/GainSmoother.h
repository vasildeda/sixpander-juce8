#pragma once

#include "MathUtil.h"

#include <cmath>
#include <iostream>

class GainSmoother
{
public:
    GainSmoother() = default;

    void setSampleRate(double sampleRate)
    {
        if (sr_ != static_cast<float>(sampleRate))
        {
            sr_ = static_cast<float>(sampleRate);
            updateCoefficients();
        }
    }

    void setAttackTime(float timeInSeconds)
    {
        if (attackTime_ != timeInSeconds)
        {
            attackTime_ = timeInSeconds;
            updateCoefficients();
        }
    }

    void setReleaseTime(float timeInSeconds)
    {
        if (releaseTime_ != timeInSeconds)
        {
            releaseTime_ = timeInSeconds;
            updateCoefficients();
        }
    }

    void reset(float initialValue = 0.0f)
    {
        current_ = initialValue;
    }

    float process(float target)
    {
        const float coeff = (target > current_) ? attackCoeff_ : releaseCoeff_;
        current_ = (1.0f - coeff) * current_ + coeff * target;
        if (debug_)
        {
            std::cout << "Target: " << target << ", Current: " << current_ << "                                              \r";
        }
        return current_;
    }

    float getCurrent() const { return current_; }

    void setDebug(bool debug) { debug_ = debug; }

private:
    void updateCoefficients()
    {
        constexpr float minTime = 1e-6f;
        float safeAttackTime  = std::max(attackTime_, minTime);
        float safeReleaseTime = std::max(releaseTime_, minTime);
        float safeSR = std::max(sr_, 1.0f);

        attackCoeff_ = computeAlpha(0.75, attackTime_, sr_);
        releaseCoeff_ = computeAlpha(0.75, releaseTime_, sr_);

        if (debug_)
        {
            std::cout << "Updated coefficients: "
                    << "Attack Coeff: " << attackCoeff_
                    << ", Release Coeff: " << releaseCoeff_
                    << ", Sample Rate: " << sr_
                    << ", Attack Time: " << attackTime_
                    << ", Release Time: " << releaseTime_
                    << std::endl;
        }
    }

    float attackTime_  = 0.01f;  // seconds
    float releaseTime_ = 0.1f;
    float sr_ = 44100.0;

    float attackCoeff_ = 0.0f;
    float releaseCoeff_ = 0.0f;
    float current_ = 0.0f;

    bool debug_ = false;
};