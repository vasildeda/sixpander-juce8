#pragma once

#include "Sixpander.h"
#include "MeterComponent.h"
#include "RotaryKnob.h"
#include "CustomLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
class SixpanderEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    SixpanderEditor (Sixpander&);
    ~SixpanderEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    static constexpr int TIMER_FREQ = 30;

    void timerCallback() override;

    Sixpander& audioProcessor;
    CustomLookAndFeel customLookAndFeel;

    RotaryKnob attackKnob;
    RotaryKnob decayKnob;
    juce::ComboBox modeComboBox;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    MeterComponent audioInputMeter;
    MeterComponent audioSidechainMeter;
    MeterComponent audioGainMeter;
    MeterComponent audioOutputMeter;

    std::unique_ptr<juce::Drawable> backgroundDrawable;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SixpanderEditor)
};