#pragma once

#include <JuceHeader.h>
#include "MyLookAndFeel.h"
#include "MyColours.h"

class Dial  : public juce::Slider
{
public:
    Dial();
    ~Dial() override;

    void paint (juce::Graphics& g) override;

    void mouseEnter (const juce::MouseEvent& event) override;
    void mouseExit (const juce::MouseEvent& event) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    bool keyPressed (const juce::KeyPress& key) override;

    void focusLost (FocusChangeType cause) override;

private:
    void drawFocusMark (juce::Graphics& g, juce::Colour colour);

    juce::Time mouseUpTime;

    MyLookAndFeel lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
