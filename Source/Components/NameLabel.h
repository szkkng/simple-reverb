#pragma once

#include <JuceHeader.h>

class NameLabel  : public juce::Label
{
public:
	NameLabel()
	{
		setFont (21.0f);
		setColour (juce::Label::textColourId, MyColours::grey);
		setJustificationType (juce::Justification::centred);
	}
};
