#include "MeterComponent.h"

#include "DebugUtils.h"

#include <juce_core/juce_core.h>

void MeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.fillAll(juce::Colours::black);

        // Draw historical bars with fading color
    auto meterWidth = bounds.getWidth();
    auto meterHeight = bounds.getHeight();

    for (size_t i = 0; i < levels.size(); ++i)
    {
        float histLevel = levels[i];
        float height = meterHeight * histLevel;

        float alpha = juce::jmap((float)i, 0.0f, (float)(levels.size() - 1), 0.2f, 1.0f);
        g.setColour(juce::Colours::limegreen.withAlpha(alpha));

        juce::Rectangle<float> bar = bounds.withY(meterHeight - height).withHeight(height);
        g.drawRect(bar);
    }

//    printf("MeterComponent::paint: %d     \r", levels.size());
}
