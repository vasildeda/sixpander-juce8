#pragma once

#include "GainSmoother.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>

class Sixpander final : public juce::AudioProcessor
{
public:
    Sixpander();
    ~Sixpander() override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                            { return true; }
    const juce::String getName() const override                { return "Sixpander J8"; }
    bool acceptsMidi() const override                          { return false; }
    bool producesMidi() const override                         { return false; }
    double getTailLengthSeconds() const override               { return 0.0; }
    int getNumPrograms() override                              { return 1; }
    int getCurrentProgram() override                           { return 0; }
    void setCurrentProgram (int) override                      {}
    const juce::String getProgramName (int) override           { return "None"; }
    void changeProgramName (int, const juce::String&) override {}
    bool isVST2() const noexcept                               { return (wrapperType == wrapperType_VST); }

    void prepareToPlay(double, int) override;
    void releaseResources() override {}
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState state;

    const std::atomic<float>& getAudioInputLevel() const { return maxAudioInputLevel; }
    const std::atomic<float>& getAudioSidechainLevel() const { return maxAudioSidechainLevel; }
    const std::atomic<float>& getMidiInputLevel() const { return midiInputLevel; }

private:
    std::atomic<float> audioInputLevel {0.0f};
    std::atomic<float> audioSidechainLevel {0.0f};
    std::atomic<float> midiInputLevel {0.0f};

    std::atomic<float> maxAudioInputLevel { 0.0f };
    std::atomic<float> maxAudioSidechainLevel { 0.0f };

    GainSmoother gainSmoother;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sixpander)
};