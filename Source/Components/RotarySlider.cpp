#include <JuceHeader.h>
#include "RotarySlider.h"

RotarySlider::RotarySlider()
{
    setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    setLookAndFeel (&customLookAndFeel);
    setColour (juce::Slider::rotarySliderFillColourId, MyColours::blue);
    setColour (juce::Slider::textBoxTextColourId,      MyColours::blackGrey);
    setColour (juce::Slider::textBoxOutlineColourId,   MyColours::grey);
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
        drawFocusMark (g, findColour (juce::Slider::textBoxOutlineColourId));
}

void RotarySlider::drawFocusMark (juce::Graphics& g, juce::Colour colour)
{
    g.setColour (colour);

    auto bounds = getLocalBounds().toFloat();
    auto length = juce::jmin (bounds.getHeight(), bounds.getWidth()) * 0.07f;
    auto thick  = length * 0.5f;
    auto radian = 0.0f;

    auto topL    = bounds.getTopLeft();
    auto topR    = bounds.getTopRight();
    auto bottomR = bounds.getBottomRight();
    auto bottomL = bounds.getBottomLeft();

    std::array<juce::Point<float>, 4> corners { topL, topR, bottomR, bottomL };

    // Draw in clockwise order, starting from top left.
    for (auto corner : corners)
    {
        juce::Path path;

        // vertical path
        path.startNewSubPath (corner);
        path.lineTo          (corner.x, corner.y + length);

        // horizontal path
        path.startNewSubPath (corner);
        path.lineTo          (corner.x + length, corner.y);

        g.strokePath (path,
                      juce::PathStrokeType (thick),
                      juce::AffineTransform::rotation (radian, corner.x, corner.y));

        radian += juce::MathConstants<float>::halfPi;
    };
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
