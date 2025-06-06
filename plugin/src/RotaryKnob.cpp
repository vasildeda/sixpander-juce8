#include "../include/RotaryKnob.h"

RotaryKnob::RotaryKnob()
{
    // Set up the slider properties
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
        rotationAngle = getValue() / 100.0f * 326.0f;
        
        // Save the current graphics state
        g.saveState();

        // Draw debug crosshair at component center
        g.setColour(juce::Colours::red);
        float centerX = getWidth() / 2.0f;
        float centerY = getHeight() / 2.0f;
        g.drawLine(centerX - 10, centerY, centerX + 10, centerY, 1.0f);
        g.drawLine(centerX, centerY - 10, centerX, centerY + 10, 1.0f);

        // Calculate the actual path bounds from the SVG
        float pathWidth = 88.0f;   // 106 - 18
        float pathHeight = 97.5f;  // 115.5 - 18
        float svgSize = 124.0f;    // Original SVG size

        // Calculate the center of the circular path in SVG coordinates
        float svgCenterX = 62.0f;  // Center of the circle in SVG
        float svgCenterY = 62.0f;  // Center of the circle in SVG

        // Calculate the offset from the path's top-left to the center
        float centerOffsetX = svgCenterX - 18.0f;  // Distance from left edge to center
        float centerOffsetY = svgCenterY - 18.0f;  // Distance from top edge to center

        // Draw debug rectangle showing actual path bounds
        g.setColour(juce::Colours::yellow);
        g.drawRect(centerX - pathWidth/2, centerY - pathHeight/2, pathWidth, pathHeight, 1.0f);

        // Move to the center of the component
        g.addTransform(juce::AffineTransform::translation(centerX, centerY));

        // Apply rotation around the center of the circle
        g.addTransform(juce::AffineTransform::rotation(
            juce::degreesToRadians(rotationAngle),
            centerOffsetX - pathWidth/2,  // X offset from path center
            centerOffsetY - pathHeight/2  // Y offset from path center
        ));
        
        // Draw the knob maintaining original proportions
        knobDrawable->drawWithin(g, 
            juce::Rectangle<float>(-pathWidth/2, -pathHeight/2, pathWidth, pathHeight).toFloat(),
            juce::RectanglePlacement::centred, 1.0f);
        
        // Restore the graphics state
        g.restoreState();
    }
}

void RotaryKnob::resized()
{
    // The component will be sized by its parent
} 