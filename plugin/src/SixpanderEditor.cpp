#include "SixpanderEditor.h"

//==============================================================================
SixpanderEditor::SixpanderEditor(Sixpander& p):
    AudioProcessorEditor(&p),
    audioProcessor(p),
    gainAttachment(p.state, "gain", gainSlider),
    modeAttachment(p.state, "mode", modeComboBox)
{
    // Set the size of the plugin editor window
    setSize(400, 300);

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::Rotary);

    addAndMakeVisible(modeComboBox);
    modeComboBox.addItem("Max", 1);
    modeComboBox.addItem("Target", 2);

    addAndMakeVisible(audioInputMeter);
    addAndMakeVisible(audioSidechainMeter);
    addAndMakeVisible(midiInputMeter);

    startTimerHz(30);  // Start the timer to trigger every 30ms
}

SixpanderEditor::~SixpanderEditor()
{
}

//==============================================================================
void SixpanderEditor::paint(juce::Graphics& g)
{
    // Fill the background with a solid colour
    g.fillAll(juce::Colours::black);

    // Set the font size and draw some text
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello JUCE!", getLocalBounds(), juce::Justification::centred, 1);
}

void SixpanderEditor::resized()
{
    auto r = getLocalBounds().reduced(8);

    auto sliderArea = r.removeFromTop(60);
    gainSlider.setBounds(
        sliderArea.removeFromLeft(
            juce::jmin(180, sliderArea.getWidth() / 2)
        )
    );
    modeComboBox.setBounds(sliderArea.removeFromLeft(
        juce::jmin(180, sliderArea.getWidth())
    ));

    auto meterArea = r.removeFromTop(200);
    auto meterWidth = meterArea.getWidth() / 3;

    audioInputMeter.setBounds(meterArea.removeFromLeft(meterWidth));
    audioSidechainMeter.setBounds(meterArea.removeFromLeft(meterWidth));
    midiInputMeter.setBounds(meterArea);
}

void SixpanderEditor::timerCallback()
{
    audioInputMeter.pushLevel(audioProcessor.getAudioInputLevel());
    audioSidechainMeter.pushLevel(audioProcessor.getAudioSidechainLevel());
    repaint();
}