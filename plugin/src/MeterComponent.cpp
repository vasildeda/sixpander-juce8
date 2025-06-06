#include "MeterComponent.h"

#include "DebugUtils.h"

#include <juce_core/juce_core.h>

MeterComponent::MeterComponent(int frequency):
    frequency_(frequency)
{
    // Set the size of the component
    setSize(200, 200);

    // Set up the levels deque with a fixed size
    levels_.resize(30);

    // Set up the smoother
    smoother_.setSampleRate(frequency);
    smoother_.setAttackTime(0.01f);
    smoother_.setReleaseTime(0.1f);
    smoother_.reset(0.0f);
}

void MeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(10, 10);
    auto meterHeight = bounds.getHeight();
    
    const int numSegments = 16;  // Number of segments in the meter
    const float segmentHeight = std::floor(meterHeight / numSegments);  // Round down to ensure consistent segment heights
    const float segmentGap = 1.0f;  // Gap between segments in pixels

    // Calculate total height of all segments to center them in the available space
    const float totalSegmentsHeight = (segmentHeight * numSegments) + (segmentGap * (numSegments - 1));
    const float startY = bounds.getY() + (meterHeight - totalSegmentsHeight) / 2.0f;

    for (size_t i = 0; i < levels_.size(); ++i)
    {
        float histLevel = levels_[i];
        float height = meterHeight * histLevel;

        float alpha = juce::jmap((float)i, 0.0f, (float)(levels_.size() - 1), 0.2f, 1.0f);
        g.setColour(juce::Colours::black.withAlpha(alpha));

        // Draw segments
        for (int segment = 0; segment < numSegments; ++segment)
        {
            float segmentY = startY + (numSegments - segment - 1) * (segmentHeight + segmentGap);
            float segmentLevel = (segment + 1) * (1.0f / numSegments);
            
            if (histLevel >= segmentLevel)
            {
                juce::Rectangle<float> segmentRect = bounds
                    .withY(segmentY)
                    .withHeight(segmentHeight);
                g.fillRoundedRectangle(segmentRect, 3.0f);
            }
        }
    }

    if (!levels_.empty())
    {
        auto smoothed = smoother_.process(levels_.back());

        // Draw text background
        g.setColour(juce::Colour::fromRGB(0xD9, 0xD9, 0xD9));  // Use RGB values for the background color
        auto textBounds = bounds.withHeight(20)
                              .withY(bounds.getBottom() - 25)
                              .withWidth(50)
                              .withCentre(juce::Point<float>(bounds.getCentreX(), bounds.getBottom() - 15));
        g.fillRoundedRectangle(textBounds, 4.0f);

        // Draw text
        g.setColour(juce::Colours::black);  // Change text color to black for better contrast
        g.drawText(
            juce::String(smoothed, 2),
            textBounds,
            juce::Justification::centred,
            true
        );
    }
}
