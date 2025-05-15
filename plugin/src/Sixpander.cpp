#include "Sixpander.h"
#include "SixpanderEditor.h"

Sixpander::Sixpander()
: AudioProcessor(BusesProperties().withInput("Input",     juce::AudioChannelSet::stereo())
                                  .withOutput("Output",   juce::AudioChannelSet::stereo())
                                  .withInput("Sidechain", juce::AudioChannelSet::stereo())),                                          
state(*this, nullptr, "PARAMETERS", { std::make_unique<juce::AudioParameterFloat> ("a", "Parameter A", juce::NormalisableRange<float> (-100.0f, 100.0f), 0),
                                      std::make_unique<juce::AudioParameterInt>   ("gain", "Parameter B", 0, 5, 2),
                                      std::make_unique<juce::AudioParameterChoice>("mode", "Mode", juce::StringArray { "max", "target" }, 0) })
{}

Sixpander::~Sixpander()
{}

juce::AudioProcessorEditor* Sixpander::createEditor()
{
    return new SixpanderEditor(*this);
}

void Sixpander::getStateInformation(juce::MemoryBlock& destData)
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
    state.replaceState(juce::ValueTree::fromXml (*xmlState));

    std::cout << xmlState->toString() << std::endl;
}

void Sixpander::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    std::cout << "prepareToPlay" << std::endl;
    gainSmoother.setSampleRate(sampleRate);
    gainSmoother.setAttackTime(0.01f);
    gainSmoother.setReleaseTime(0.2f);
    gainSmoother.reset();
}

void Sixpander::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    float inputSumSquares = 0.0f;
    int inputSamples = 0;

    if (getBusCount(true) > 0)
    {
        auto inputBuffer = getBusBuffer(buffer, true, 0); // Main input
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
        auto sidechainBuffer = getBusBuffer(buffer, true, 1); // Sidechain input
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

    const int mode = static_cast<int>(*state.getRawParameterValue("mode"));
    const float gain = [&, mode]() -> float
    {
        switch (mode)
        {
            case 0: // "max"
                std::cout << "MAX sidechain: " << audioSidechainLevel << " maxSidechain: " << maxAudioSidechainLevel << "\r";
                return audioSidechainLevel / maxAudioSidechainLevel;

            case 1: // "target"
                std::cout << "TARGET sidechain: " << audioSidechainLevel << " input: " << audioInputLevel << "\r";
                return audioSidechainLevel / audioInputLevel;

            default:
                std::cout << "DEFAULT            " << "\r";
                return 1.0f;
        }
    }();

    float smoothedGain = gainSmoother.process(gain);

    if (getBusCount(true) > 0 && getBusCount(false) > 0)
    {
        auto outputBuffer = getBusBuffer(buffer, false, 0);
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            float* data = outputBuffer.getWritePointer(channel);
            for (int sample = 0; sample < outputBuffer.getNumSamples(); ++sample)
            {
                data[sample] *= smoothedGain;
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sixpander();
}
