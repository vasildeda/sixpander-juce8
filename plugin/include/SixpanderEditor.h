#pragma once

#include "Sixpander.h"
#include "MeterComponent.h"

using juce::AudioProcessorEditor;
using juce::Slider;

//==============================================================================
class SixpanderEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    SixpanderEditor (Sixpander&);
    ~SixpanderEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    Sixpander& audioProcessor;

    Slider gainSlider;
    AudioProcessorValueTreeState::SliderAttachment gainAttachment;

    MeterComponent audioInputMeter;
    MeterComponent audioSidechainMeter;
    MeterComponent midiInputMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SixpanderEditor)
};