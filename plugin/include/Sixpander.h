#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using juce::AudioBuffer;
using juce::AudioParameterFloat;
using juce::AudioProcessor;
using juce::AudioProcessorEditor;
using juce::MemoryBlock;
using juce::MidiBuffer;
using juce::String;

class Sixpander final : public AudioProcessor
{
public:
    Sixpander();
    ~Sixpander() override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                          { return true; }
    const String getName() const override                    { return "Sixpander J8"; }
    bool acceptsMidi() const override                        { return false; }
    bool producesMidi() const override                       { return false; }
    double getTailLengthSeconds() const override             { return 0.0; }
    int getNumPrograms() override                            { return 1; }
    int getCurrentProgram() override                         { return 0; }
    void setCurrentProgram (int) override                    {}
    const String getProgramName (int) override               { return "None"; }
    void changeProgramName (int, const String&) override     {}
    bool isVST2() const noexcept                             { return (wrapperType == wrapperType_VST); }

    void prepareToPlay(double, int) override { lowPassCoeff = 0.0f; sampleCountDown = 0; }
    void releaseResources() override {}
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override {}

    void getStateInformation (MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}

private:    
    AudioParameterFloat* threshold;
    AudioParameterFloat* alpha;
    int sampleCountDown;

    float lowPassCoeff;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sixpander)
};