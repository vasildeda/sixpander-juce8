#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>

using juce::AudioBuffer;
using juce::AudioParameterFloat;
using juce::AudioProcessor;
using juce::AudioProcessorEditor;
using juce::AudioProcessorValueTreeState;
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
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState state;

    const std::atomic<float>& getAudioInputLevel() const { return maxAudioInputLevel; }
    const std::atomic<float>& getAudioSidechainLevel() const { return maxAudioSidechainLevel; }
    const std::atomic<float>& getMidiInputLevel() const { return midiInputLevel; }

private:    
    AudioParameterFloat* threshold;
    AudioParameterFloat* alpha;
    int sampleCountDown;

    float lowPassCoeff;

    std::atomic<float> audioInputLevel {0.0f};
    std::atomic<float> audioSidechainLevel {0.0f};
    std::atomic<float> midiInputLevel {0.0f};

    std::atomic<float> maxAudioInputLevel { 0.0f };
    std::atomic<float> maxAudioSidechainLevel { 0.0f };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sixpander)
};