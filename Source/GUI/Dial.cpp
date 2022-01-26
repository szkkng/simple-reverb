#include <JuceHeader.h>
#include "Dial.h"

Dial::Dial()
{
    setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    setLookAndFeel (&lnf);
    setColour (juce::Slider::rotarySliderFillColourId, MyColours::blue);
    setColour (juce::Slider::textBoxTextColourId,      MyColours::blackGrey);
    setColour (juce::Slider::textBoxOutlineColourId,   MyColours::grey);
    setVelocityBasedMode (true);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f,
                         true);
    setWantsKeyboardFocus (true);
}

Dial::~Dial()
{
    setLookAndFeel (nullptr);
}

void Dial::paint (juce::Graphics& g)
{
    juce::Slider::paint (g);

    if (hasKeyboardFocus (true))
        drawFocusMark (g, findColour (juce::Slider::textBoxOutlineColourId));
}

void Dial::drawFocusMark (juce::Graphics& g, juce::Colour colour)
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

void Dial::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);

    setMouseCursor (juce::MouseCursor::NoCursor);
}

void Dial::mouseDrag (const juce::MouseEvent& event)
{
    juce::Slider::mouseDrag (event);

    if (event.mods.isShiftDown())
        setVelocityModeParameters (0.1, 1, 0.1, false);
    else
        setVelocityModeParameters (1.0, 1, 0.1, false);
}

void Dial::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);

    auto mms = juce::Desktop::getInstance().getMainMouseSource();
    mms.setScreenPosition (event.source.getLastMouseDownPosition());

    setMouseCursor (juce::MouseCursor::NormalCursor);
}

bool Dial::keyPressed (const juce::KeyPress& k)
{
    if ('0' <= k.getKeyCode() && k.getKeyCode() <= '9')
    {
        MyLookAndFeel::CustomLabel::initValue = juce::String::charToString (k.getTextCharacter());
        showTextBox();

        return true;
    }

    /** If we implement it to return true, the undo/redo shortcuts implemented in the keyPressed function
        of the parent component will not work properly when Dial component has KeyboardFocus.
        This is because the keypress event will be consumed only by this component.
        By returning false, the event will be passed to the parent component,
        and the undo/redo shortcuts will work properly.
    */
    return false;
}
