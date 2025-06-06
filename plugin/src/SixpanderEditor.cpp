#include "SixpanderEditor.h"

//==============================================================================
SixpanderEditor::SixpanderEditor(Sixpander& p):
    AudioProcessorEditor(&p),
    audioProcessor(p),
    audioInputMeter(TIMER_FREQ),
    audioSidechainMeter(TIMER_FREQ),
    audioGainMeter(TIMER_FREQ),
    audioOutputMeter(TIMER_FREQ)
{
    // Set the size of the plugin editor window
    setSize(800, 600);

    // Load the background SVG
    auto backgroundSvg = juce::File::getCurrentWorkingDirectory()
        .getChildFile("plugin")
        .getChildFile("assets")
        .getChildFile("background.svg");
    
    if (backgroundSvg.existsAsFile())
    {
        backgroundDrawable = juce::Drawable::createFromSVGFile(backgroundSvg);
    }

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::Rotary);

    addAndMakeVisible(modeComboBox);
    modeComboBox.addItem("Max", 1);
    modeComboBox.addItem("Target", 2);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.state, "gain", gainSlider);
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.state, "mode", modeComboBox);

    addAndMakeVisible(audioInputMeter);
    addAndMakeVisible(audioSidechainMeter);
    addAndMakeVisible(audioGainMeter);
    addAndMakeVisible(audioOutputMeter);

    startTimerHz(TIMER_FREQ);  // Start the timer to trigger every 30ms
}

SixpanderEditor::~SixpanderEditor()
{
}

//==============================================================================
void SixpanderEditor::paint(juce::Graphics& g)
{
    // Draw the background SVG if available
    if (backgroundDrawable != nullptr)
    {
        backgroundDrawable->drawWithin(g, getLocalBounds().toFloat(),
                                     juce::RectanglePlacement::stretchToFit, 1.0f);
    }
    else
    {
        // Fallback to solid color if SVG is not available
        g.fillAll(juce::Colours::black);
    }

    // Set the font size and draw some text
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello JUCE!", getLocalBounds(), juce::Justification::centred, 1);
}

void SixpanderEditor::resized()
{
    auto r = getLocalBounds().reduced(8);
    r.removeFromTop(200);

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
    auto meterWidth = meterArea.getWidth() / 4;

    audioInputMeter.setBounds(meterArea.removeFromLeft(meterWidth));
    audioSidechainMeter.setBounds(meterArea.removeFromLeft(meterWidth));
    audioGainMeter.setBounds(meterArea.removeFromLeft(meterWidth));
    audioOutputMeter.setBounds(meterArea);
}

void SixpanderEditor::timerCallback()
{
    audioInputMeter.pushLevel(audioProcessor.getAudioInputLevel());
    audioSidechainMeter.pushLevel(audioProcessor.getAudioSidechainLevel());
    audioGainMeter.pushLevel(audioProcessor.getAudioGainLevel());
    audioOutputMeter.pushLevel(audioProcessor.getAudioOutputLevel());
    
    repaint();
}