#include "Sixpander.h"
#include "SixpanderEditor.h"

using juce::AudioChannelSet;

Sixpander::Sixpander()
: AudioProcessor(BusesProperties().withInput("Input",     AudioChannelSet::stereo())
                                  .withOutput("Output",   AudioChannelSet::stereo())
                                  .withInput("Sidechain", AudioChannelSet::stereo()))                                          
{
    addParameter(threshold = new AudioParameterFloat ({ "threshold", 1 }, "Threshold", 0.0f, 1.0f, 0.5f));
    addParameter(alpha     = new AudioParameterFloat ({ "alpha",     1 }, "Alpha",     0.0f, 1.0f, 0.8f));
}

Sixpander::~Sixpander()
{}

AudioProcessorEditor* Sixpander::createEditor()
{
    return new SixpanderEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sixpander();
}
