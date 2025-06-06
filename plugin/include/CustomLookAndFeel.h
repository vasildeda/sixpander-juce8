#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // ComboBox colors
        setColour(juce::ComboBox::backgroundColourId, juce::Colours::transparentBlack);
        setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
        setColour(juce::ComboBox::buttonColourId, juce::Colours::transparentBlack);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::black);
        setColour(juce::ComboBox::textColourId, juce::Colours::black);

        // PopupMenu colors
        setColour(juce::PopupMenu::backgroundColourId, juce::Colours::transparentBlack);
        setColour(juce::PopupMenu::textColourId, juce::Colours::black);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::black);
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::white);
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return getPopupMenuFont();
    }
};
