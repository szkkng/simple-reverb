#include <JuceHeader.h>
#include "RotarySlider.h"

RotarySlider::RotarySlider()
{
    setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    setLookAndFeel (&customLookAndFeel);
    setColour (juce::Slider::rotarySliderFillColourId, MyColours::blue);
    setColour (juce::Slider::textBoxTextColourId,      MyColours::blackGrey);
    setColour (juce::Slider::textBoxOutlineColourId,   MyColours::blackGrey.brighter (0.2f));
    setVelocityBasedMode (true);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f,
                         true);
    setWantsKeyboardFocus (true);
}

RotarySlider::~RotarySlider()
{
    setLookAndFeel (nullptr);
}

void RotarySlider::paint (juce::Graphics& g)
{
    juce::Slider::paint (g);

    if (hasKeyboardFocus (true))
    {
        auto length = getHeight() > 15 ? 5.0f : 4.0f;
        auto thick  = getHeight() > 15 ? 3.0f : 2.5f;

        g.setColour (findColour (juce::Slider::textBoxOutlineColourId));

        //          fromX       fromY        toX                  toY
        g.drawLine (0,          0,           0,                   length,               thick);
        g.drawLine (0,          0,           length,              0,                    thick);
        g.drawLine (0,          getHeight(), 0,                   getHeight() - length, thick);
        g.drawLine (0,          getHeight(), length,              getHeight(),          thick);
        g.drawLine (getWidth(), getHeight(), getWidth() - length, getHeight(),          thick);
        g.drawLine (getWidth(), getHeight(), getWidth(),          getHeight() - length, thick);
        g.drawLine (getWidth(), 0,           getWidth() - length, 0,                    thick);
        g.drawLine (getWidth(), 0,           getWidth(),          length,               thick);
    }
}

void RotarySlider::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);

    setMouseCursor (juce::MouseCursor::NoCursor);
}

void RotarySlider::mouseDrag (const juce::MouseEvent& event)
{
    juce::Slider::mouseDrag (event);

    if (event.mods.isShiftDown())
        setVelocityModeParameters (0.1, 1, 0.1, false);
    else
        setVelocityModeParameters (1.0, 1, 0.1, false);
}

void RotarySlider::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);

    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition (event.source.getLastMouseDownPosition());
    setMouseCursor (juce::MouseCursor::NormalCursor);
}

bool RotarySlider::keyPressed (const juce::KeyPress& k)
{
    char numChars[] = "0123456789";

    for (auto numChar : numChars)
    {
        if (k.isKeyCode (numChar))
        {
            CustomLookAndFeel::CustomLabel::initialPressedKey = juce::String::charToString (numChar);
            showTextBox();

            return true;
        }
    }

    return false;
}
