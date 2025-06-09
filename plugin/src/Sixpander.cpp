#include "Sixpander.h"
#include "SixpanderEditor.h"

Sixpander::Sixpander()
: AudioProcessor(BusesProperties().withInput("Input",     juce::AudioChannelSet::stereo())
                                  .withOutput("Output",   juce::AudioChannelSet::stereo())
                                  .withInput("Sidechain", juce::AudioChannelSet::mono())),                                          
state(*this, nullptr, "PARAMETERS", { 
    std::make_unique<juce::AudioParameterFloat>("attack", "Attack", 
        juce::NormalisableRange<float>(1.0f, 100.0f, 1.0f), 1.0f),
    std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 
        juce::NormalisableRange<float>(1.0f, 1000.0f, 1.0f), 1.0f),
    std::make_unique<juce::AudioParameterChoice>("mode", "Mode", 
        juce::StringArray { "max", "target" }, 0) 
})
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
    gainSmoother_.setSampleRate(sampleRate);
    gainSmoother_.setAttackTime(0.001f);
    gainSmoother_.setReleaseTime(0.001f);
    gainSmoother_.reset(0.0f);
}

void Sixpander::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // Get parameter values from state and convert from milliseconds to seconds
    float attackTime = *state.getRawParameterValue("attack") / 1000.0f;
    float decayTime = *state.getRawParameterValue("decay") / 1000.0f;
    
    gainSmoother_.setAttackTime(attackTime);
    gainSmoother_.setReleaseTime(decayTime);
    
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

    audioInputLevel_.store(inputSamples > 0 ? std::sqrt(inputSumSquares / inputSamples) : 0.0f);
    audioSidechainLevel_.store(sidechainSamples > 0 ? std::sqrt(sidechainSumSquares / sidechainSamples) : 0.0f);
    midiInputLevel_ = 0.0f;

    if (audioInputLevel_ > maxAudioInputLevel_) {
        maxAudioInputLevel_.store(audioInputLevel_);
    }
    if (audioSidechainLevel_ > maxAudioSidechainLevel_) {
        maxAudioSidechainLevel_.store(audioSidechainLevel_);
    }

    const int mode = static_cast<int>(*state.getRawParameterValue("mode"));
    const float gain = [&, mode]() -> float
    {
        switch (mode)
        {
            case 0: // "max"
                return (maxAudioSidechainLevel_ > 0.0f) ? audioSidechainLevel_ / maxAudioSidechainLevel_ : 0.0f;

            case 1: // "target"
                return (audioInputLevel_ > 0.0f) ? audioSidechainLevel_ / audioInputLevel_ : 0.0f;

            default:
                return 0.0f;
        }
    }();

    float smoothedGain = gainSmoother_.process(gain);
    audioGainLevel_.store(smoothedGain);

    float outputSumSquares = 0.0f;
    int outputSamples = 0;

    if (getBusCount(true) > 0 && getBusCount(false) > 0)
    {
        auto outputBuffer = getBusBuffer(buffer, false, 0);
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            float* data = outputBuffer.getWritePointer(channel);
            for (int sample = 0; sample < outputBuffer.getNumSamples(); ++sample)
            {
                data[sample] *= smoothedGain;
                outputSumSquares += data[sample] * data[sample];
            }
        }

        outputSamples = outputBuffer.getNumSamples() * outputBuffer.getNumChannels();
    }
    audioOutputLevel_.store(outputSamples > 0 ? std::sqrt(outputSumSquares / outputSamples) : 0.0f);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sixpander();
}
