#pragma once

#include <atomic>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::Component
{
public:
    MeterComponent() = default;
    ~MeterComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override {}

    void pushLevel(float level)
    {
        levels.push_back(juce::jlimit(0.0f, 1.0f, level));
        if (levels.size() > 60)
        {
            levels.pop_front();
        }
    }

private:
    std::deque<float> levels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};