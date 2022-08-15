/*
  ==============================================================================

   Copyright 2021, 2022 Suzuki Kengo

   Simple Reverb is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   Simple Reverb is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with Simple Reverb. If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "Dial.h"
#include "DialTextBox.h"

Dial::Dial()
{
    setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    setLookAndFeel (&lnf);
    setColour (juce::Slider::rotarySliderFillColourId, MyColours::blue);
    setColour (juce::Slider::textBoxTextColourId,      MyColours::black);
    setColour (juce::Slider::textBoxOutlineColourId,   MyColours::grey);
    setColour (juce::CaretComponent::caretColourId,    juce::Colours::red);
    setVelocityBasedMode (true);
    setVelocityModeParameters (1.0, 1, 0.1, false);
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

    const auto bounds = getLocalBounds().toFloat().reduced (3.0f);
    const auto length = juce::jmin (bounds.getHeight(), bounds.getWidth()) * 0.07f;
    const auto thick  = length * 0.38f;
    const std::array<juce::Point<float>, 4> corners { bounds.getTopLeft(), 
                                                      bounds.getTopRight(), 
                                                      bounds.getBottomRight(), 
                                                      bounds.getBottomLeft() };
    auto radian = 0.0f;

    // Draw in clockwise order, starting from top left.
    for (auto& corner : corners)
    {
        juce::Path path;

        path.startNewSubPath (corner.x + length, corner.y);
        path.lineTo          (corner.x,          corner.y);
        path.lineTo          (corner.x,          corner.y + length);

        g.strokePath (path,
                      juce::PathStrokeType (thick),
                      juce::AffineTransform::rotation (radian, corner.x, corner.y));

        radian += juce::MathConstants<float>::halfPi;
    };
}

void Dial::mouseEnter (const juce::MouseEvent& /*event*/)
{
    grabKeyboardFocus();
}

void Dial::mouseExit (const juce::MouseEvent& /*event*/)
{
    giveAwayKeyboardFocus();
}

void Dial::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);

    setMouseCursor (juce::MouseCursor::NoCursor);
}

void Dial::mouseDrag (const juce::MouseEvent& event)
{
    juce::Slider::mouseDrag (event);

    const auto fineMode = event.mods.isShiftDown();

    if (fineMode != lastFineMode)
        setVelocityModeParameters (fineMode ? 0.1 : 1.0, 1, 0.1, false);

    lastFineMode = fineMode;
}

void Dial::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);

    auto mms = juce::Desktop::getInstance().getMainMouseSource();
    mms.setScreenPosition (event.source.getLastMouseDownPosition());

    mouseUpTime = event.eventTime;

    setMouseCursor (juce::MouseCursor::NormalCursor);
}

bool Dial::keyPressed (const juce::KeyPress& k)
{
    if ('0' <= k.getKeyCode() && k.getKeyCode() <= '9')
    {
        DialTextBox::valueShownWithEditor = juce::String::charToString (k.getTextCharacter());
        showTextBox();

        return true;
    }

    return false;
}

void Dial::focusLost (FocusChangeType /*cause*/)
{
    // Avoid a bug that causes this component to lose focus when the mouse is released.
    const auto currentMillisec = juce::Time::getCurrentTime().toMilliseconds();
    const auto diffMillisec    = currentMillisec - mouseUpTime.toMilliseconds();
    
    if (diffMillisec < 50)
        grabKeyboardFocus();
}
