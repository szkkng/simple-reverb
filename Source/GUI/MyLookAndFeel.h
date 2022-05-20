#pragma once

#include <JuceHeader.h>
#include "MyColours.h"
#include "DialTextBox.h"

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MyLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    DialTextBox* createSliderTextBox (juce::Slider& slider) override;

    juce::CaretComponent* createCaretComponent (juce::Component* keyFocusOwner) override;

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;
};
