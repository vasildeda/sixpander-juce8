#pragma once

#include "Sixpander.h"

using juce::AudioProcessorEditor;

//==============================================================================
class SixpanderEditor : public juce::AudioProcessorEditor
{
public:
    SixpanderEditor (Sixpander&);
    ~SixpanderEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Sixpander& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SixpanderEditor)
};