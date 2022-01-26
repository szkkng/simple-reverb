#include "NameLabel.h"
#include "MyColours.h"

NameLabel::NameLabel()
{
    setColour (juce::Label::textColourId, MyColours::grey);
    setJustificationType (juce::Justification::centred);
}

void NameLabel::resized()
{
    if (getAttachedComponent() != nullptr)
        setFont (getAttachedComponent()->getHeight() * 0.28f);
}
