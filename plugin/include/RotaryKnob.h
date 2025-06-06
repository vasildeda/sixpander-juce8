#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RotaryKnob : public juce::Slider
{
public:
    RotaryKnob();
    ~RotaryKnob() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<juce::Drawable> knobDrawable;
    float rotationAngle = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
}; 