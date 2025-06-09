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

    addAndMakeVisible(attackKnob);
    if (auto* param = dynamic_cast<juce::AudioParameterFloat*>(p.state.getParameter("attack")))
    {
        attackKnob.setRange(
            param->getNormalisableRange().start, 
            param->getNormalisableRange().end, 
            param->getNormalisableRange().interval
        );
    }
    attackKnob.setNumDecimalPlacesToDisplay(0);
    attackKnob.setTextValueSuffix(" ms");

    addAndMakeVisible(decayKnob);
    if (auto* param = dynamic_cast<juce::AudioParameterFloat*>(p.state.getParameter("decay")))
    {
        decayKnob.setRange(
            param->getNormalisableRange().start, 
            param->getNormalisableRange().end, 
            param->getNormalisableRange().interval
        );
    }
    decayKnob.setNumDecimalPlacesToDisplay(0);
    decayKnob.setTextValueSuffix(" ms");

    addAndMakeVisible(modeComboBox);
    modeComboBox.addItem("Max", 1);
    modeComboBox.addItem("Target", 2);
    modeComboBox.setLookAndFeel(&customLookAndFeel);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.state, "attack", attackKnob);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.state, "decay", decayKnob);
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
}

void SixpanderEditor::resized()
{
    auto r = getLocalBounds();
    r.removeFromTop(172);

    auto sliderArea = r.removeFromTop(134);
    sliderArea.removeFromLeft(38);
    
    attackKnob.setBounds(sliderArea.removeFromLeft(124));
    decayKnob.setBounds(sliderArea.removeFromLeft(124));
    
    auto modeArea = sliderArea.removeFromLeft(248);
    modeArea.removeFromTop(46);
    modeArea.removeFromBottom(44);
    modeArea.removeFromLeft(40);
    modeArea.removeFromRight(40);
    modeComboBox.setBounds(modeArea);

    auto meterArea = r.removeFromTop(r.getHeight());
    meterArea.removeFromTop(91);
    meterArea.removeFromBottom(29);
    meterArea.removeFromLeft(57);
    meterArea.removeFromRight(280);

    audioInputMeter.setBounds(meterArea.removeFromLeft(85));
    meterArea.removeFromLeft(41);
    audioSidechainMeter.setBounds(meterArea.removeFromLeft(85));
    meterArea.removeFromLeft(41);
    audioGainMeter.setBounds(meterArea.removeFromLeft(85));
    meterArea.removeFromLeft(41);
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