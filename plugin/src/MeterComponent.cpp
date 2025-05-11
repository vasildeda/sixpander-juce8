#include "MeterComponent.h"

MeterComponent::MeterComponent(const std::atomic<float>& rmsRef)
    : rmsLevel(rmsRef)
{
}

void MeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.fillAll(juce::Colours::black);

    float level = juce::jlimit(0.0f, 1.0f, rmsLevel.load());
    float meterHeight = bounds.getHeight() * level;

    g.setColour(juce::Colours::limegreen);
    g.fillRect(bounds.removeFromBottom(meterHeight));
}
