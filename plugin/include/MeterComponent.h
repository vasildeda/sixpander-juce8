#pragma once

#include <atomic>
#include <juce_gui_basics/juce_gui_basics.h>

class MeterComponent : public juce::Component
{
public:
    MeterComponent(const std::atomic<float>& rmsRef);
    ~MeterComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override {}

private:
    const std::atomic<float>& rmsLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};