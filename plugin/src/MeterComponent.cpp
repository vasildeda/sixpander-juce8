#include "MeterComponent.h"

#include "DebugUtils.h"

#include <juce_core/juce_core.h>

MeterComponent::MeterComponent(int frequency):
    frequency(frequency)
{
    // Set the size of the component
    setSize(200, 200);

    // Set up the levels deque with a fixed size
    levels.resize(30);

    // Set up the smoother
    smoother.setSampleRate(frequency);
    smoother.setAttackTime(0.01f);
    smoother.setReleaseTime(0.1f);
    smoother.reset(0.0f);
}

void MeterComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    auto bounds = getLocalBounds().toFloat();
    auto meterHeight = bounds.getHeight();

    for (size_t i = 0; i < levels.size(); ++i)
    {
        float histLevel = levels[i];
        float height = meterHeight * histLevel;

        float alpha = juce::jmap((float)i, 0.0f, (float)(levels.size() - 1), 0.2f, 1.0f);
        g.setColour(juce::Colours::limegreen.withAlpha(alpha));

        juce::Rectangle<float> bar = bounds.withY(meterHeight - height).withHeight(height);
        g.fillRect(bar);
    }

    if (!levels.empty())
    {
        auto smoothed = smoother.process(levels.back());

        g.drawText(
            juce::String(smoothed, 2),
            bounds,
            juce::Justification::centred,
            true
        );

    }
//    printf("MeterComponent::paint: %d     \r", levels.size());
}
