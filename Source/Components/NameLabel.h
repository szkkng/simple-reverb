#pragma once

#include <JuceHeader.h>

class NameLabel  : public juce::Label
{
public:
	NameLabel()
	{
		setFont (20.0f);
		setColour (juce::Label::textColourId, MyColours::grey);
		setJustificationType (juce::Justification::centred);
	}
};
