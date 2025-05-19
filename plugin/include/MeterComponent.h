#pragma once

#include "GainSmoother.h"
#include <atomic>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::Component
{
public:
    MeterComponent(int frequnecy);
    ~MeterComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override {}

    void pushLevel(float level)
    {
        levels.push_back(juce::jlimit(0.0f, 1.0f, level));
        if (levels.size() > 30)
        {
            levels.pop_front();
        }
    }

private:
    int frequency;
    
    std::deque<float> levels;

    GainSmoother smoother;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};