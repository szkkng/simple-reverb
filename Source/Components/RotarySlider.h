#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/CustomLookAndFeel.h"
#include "../LookAndFeel/MyColours.h"

class RotarySlider  : public juce::Slider
{
public:
    RotarySlider();
    ~RotarySlider() override;

    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    bool keyPressed (const juce::KeyPress& key) override;

private:
    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotarySlider)
};
