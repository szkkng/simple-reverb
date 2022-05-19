#pragma once

#include "JuceHeader.h"
#include "MyColours.h"

class FreezeButton  : public juce::ToggleButton
{
public:
    FreezeButton();

    void paint (juce::Graphics& g) override;

    void resized () override;

    void mouseDown (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

private:
    juce::Path freezeIconPath;
    juce::Rectangle<float> freezeIconBounds;
    juce::Colour freezeColour { MyColours::grey };
};
