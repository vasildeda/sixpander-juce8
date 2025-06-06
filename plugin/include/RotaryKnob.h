#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RotaryKnob : public juce::Slider
{
public:
    RotaryKnob();
    ~RotaryKnob() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setDebug(bool enable) { debug = enable; }
    bool isDebug() const { return debug; }

private:
    std::unique_ptr<juce::Drawable> knobDrawable;
    float rotationAngle = 0.0f;
    bool debug = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
}; 