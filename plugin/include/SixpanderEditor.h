#pragma once

#include "Sixpander.h"
#include "MeterComponent.h"

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

    juce::Slider gainSlider;
    juce::ComboBox modeComboBox;

    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment modeAttachment;

    MeterComponent audioInputMeter;
    MeterComponent audioSidechainMeter;
    MeterComponent midiInputMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SixpanderEditor)
};