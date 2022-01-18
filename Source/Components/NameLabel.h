#pragma once

#include <JuceHeader.h>

class NameLabel  : public juce::Label
{
public:
    NameLabel()
    {
        setColour (juce::Label::textColourId, MyColours::grey);
        setJustificationType (juce::Justification::centred);
    }

    void resized()
    {
        setFont (getAttachedComponent()->getWidth() / 3.5f);
    }
};
