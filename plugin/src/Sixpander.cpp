#include "Sixpander.h"
#include "SixpanderEditor.h"

using juce::AudioChannelSet;
using juce::AudioParameterInt;
using juce::NormalisableRange;
using juce::ValueTree;

Sixpander::Sixpander()
: AudioProcessor(BusesProperties().withInput("Input",     AudioChannelSet::stereo())
                                  .withOutput("Output",   AudioChannelSet::stereo())
                                  .withInput("Sidechain", AudioChannelSet::stereo())),                                          
state(*this, nullptr, "PARAMETERS", { std::make_unique<AudioParameterFloat> ("a", "Parameter A", NormalisableRange<float> (-100.0f, 100.0f), 0),
                                      std::make_unique<AudioParameterInt>   ("gain", "Parameter B", 0, 5, 2) })
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

void Sixpander::getStateInformation(MemoryBlock& destData)
{
    std::cout << "getStateInformation" << std::endl;
    // Store an xml representation of our state.
    auto xmlState = state.copyState().createXml();
    copyXmlToBinary(*xmlState, destData);

    std::cout << xmlState->toString() << std::endl;
}

void Sixpander::setStateInformation (const void* data, int sizeInBytes)
{
    std::cout << "setStateInformation" << std::endl;
    // Restore our plug-in's state from the xml representation stored in the above
    // method.
    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    state.replaceState(ValueTree::fromXml (*xmlState));

    std::cout << xmlState->toString() << std::endl;
}

void Sixpander::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    float inputSumSquares = 0.0f;
    int inputSamples = 0;

    if (getBusCount(true) > 0)
    {
        const auto& inputBuffer = getBusBuffer(buffer, true, 0); // Main input
        for (int channel = 0; channel < inputBuffer.getNumChannels(); ++channel)
        {
            const float* data = inputBuffer.getReadPointer(channel);
            for (int sample = 0; sample < inputBuffer.getNumSamples(); ++sample)
            {
                inputSumSquares += data[sample] * data[sample];
            }
        }
        inputSamples = inputBuffer.getNumSamples() * inputBuffer.getNumChannels();
    }

    float sidechainSumSquares = 0.0f;
    int sidechainSamples = 0;

    if (getBusCount(true) > 1)
    {
        const auto& sidechainBuffer = getBusBuffer(buffer, true, 1); // Sidechain input
        for (int channel = 0; channel < sidechainBuffer.getNumChannels(); ++channel)
        {
            const float* data = sidechainBuffer.getReadPointer(channel);
            for (int sample = 0; sample < sidechainBuffer.getNumSamples(); ++sample)
            {
                sidechainSumSquares += data[sample] * data[sample];
            }
        }
        sidechainSamples = sidechainBuffer.getNumSamples() * sidechainBuffer.getNumChannels();
    }

    audioInputLevel = inputSamples > 0 ? std::sqrt(inputSumSquares / inputSamples) : 1.0f;
    audioSidechainLevel = sidechainSamples > 0 ? std::sqrt(sidechainSumSquares / sidechainSamples) : 0.0f;
    midiInputLevel = 0.0f;

    if (audioInputLevel > maxAudioInputLevel) {
        maxAudioInputLevel.store(audioInputLevel);
    }
    if (audioSidechainLevel > maxAudioSidechainLevel) {
        maxAudioSidechainLevel.store(audioSidechainLevel);
    }   
    printf("Max RMS: %.2f %.2f\r", maxAudioInputLevel.load(), maxAudioSidechainLevel.load());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sixpander();
}
