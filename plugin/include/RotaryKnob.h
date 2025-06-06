#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RotaryKnob : public juce::Slider
{
public:
    RotaryKnob();
    ~RotaryKnob() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setDebug(bool debug) { debug_ = debug; }
    bool isDebug() const { return debug_; }

private:
    std::unique_ptr<juce::Drawable> knobDrawable_;
    float rotationAngle_ = 0.0f;
    bool debug_ = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
}; 