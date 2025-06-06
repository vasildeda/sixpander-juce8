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
    auto bounds = getLocalBounds().toFloat().reduced(10, 5);
    auto meterHeight = bounds.getHeight();
    
    for (size_t i = 0; i < levels_.size(); ++i)
    {
        float histLevel = levels_[i];
        float height = meterHeight * histLevel;

        float alpha = juce::jmap((float)i, 0.0f, (float)(levels_.size() - 1), 0.2f, 1.0f);
        g.setColour(juce::Colours::black.withAlpha(alpha));

        juce::Rectangle<float> bar = bounds.withY(meterHeight - height).withHeight(height);
        g.fillRect(bar);
    }

    if (!levels_.empty())
    {
        auto smoothed = smoother_.process(levels_.back());

        g.setColour(juce::Colours::black);
        g.drawText(
            juce::String(smoothed, 2),
            bounds,
            juce::Justification::centred,
            true
        );

    }
}
