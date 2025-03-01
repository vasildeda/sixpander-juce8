#include "SixpanderEditor.h"

//==============================================================================
SixpanderEditor::SixpanderEditor(Sixpander& p) : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set the size of the plugin editor window
    setSize (400, 300);
}

SixpanderEditor::~SixpanderEditor()
{
}

//==============================================================================
void SixpanderEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid colour
    g.fillAll (juce::Colours::black);

    // Set the font size and draw some text
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello JUCE!", getLocalBounds(), juce::Justification::centred, 1);
}

void SixpanderEditor::resized()
{
    // This is where you would lay out the positions of any
    // subcomponents in your editor..
}