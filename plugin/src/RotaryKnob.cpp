#include "../include/RotaryKnob.h"

RotaryKnob::RotaryKnob()
{
    // Set up the slider properties
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setRange(0.0, 1.0, 0.01);
    setValue(0.0);
    
    // Load the knob SVG
    auto knobSvg = juce::File::getCurrentWorkingDirectory()
        .getChildFile("plugin")
        .getChildFile("assets")
        .getChildFile("simple_knob.svg");
    
    if (knobSvg.existsAsFile())
    {
        knobDrawable = juce::Drawable::createFromSVGFile(knobSvg);
    }
}

RotaryKnob::~RotaryKnob()
{
}

void RotaryKnob::paint(juce::Graphics& g)
{
    if (knobDrawable != nullptr)
    {
        // Calculate rotation angle based on slider value (0-300 degrees)
        rotationAngle = getValue() * 300.0f;
        
        // Save the current graphics state
        g.saveState();
        
        // Move to the center of the component
        g.addTransform(juce::AffineTransform::translation(
            getWidth() / 2.0f,
            getHeight() / 2.0f
        ));
        
        // Apply rotation
        g.addTransform(juce::AffineTransform::rotation(
            juce::degreesToRadians(rotationAngle)
        ));
        
        // Draw the knob centered
        knobDrawable->drawWithin(g, 
            juce::Rectangle<float>(-getWidth() / 2.0f, -getHeight() / 2.0f, 
                                 getWidth(), getHeight()).toFloat(),
            juce::RectanglePlacement::centred, 1.0f);
        
        // Restore the graphics state
        g.restoreState();
    }
}

void RotaryKnob::resized()
{
    // The component will be sized by its parent
} 