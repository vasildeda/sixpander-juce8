#pragma once

#include "GainSmoother.h"
#include <atomic>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::Component
{
public:
    MeterComponent(int frequency);
    ~MeterComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override {}

    void pushLevel(float level)
    {
        levels_.push_back(juce::jlimit(0.0f, 1.0f, level));
        if (levels_.size() > 30)
        {
            levels_.pop_front();
        }
    }

private:
    int frequency_;
    
    std::deque<float> levels_;

    GainSmoother smoother_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};