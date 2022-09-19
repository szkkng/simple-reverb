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

#include "FreezeButton.h"

FreezeButton::FreezeButton() : juce::Button (juce::String{})
{
    setOpaque (true);
    setWantsKeyboardFocus (true);
    setClickingTogglesState (true);
    onClick = [&]()
    {
        freezeColour = getToggleState() ? MyColours::blue : MyColours::midGrey;
    };

    auto svg = juce::Drawable::createFromImageData (BinaryData::FreezeIcon_svg, 
                                                    BinaryData::FreezeIcon_svgSize);
    jassert (svg != nullptr);

    if (svg != nullptr)
        freezeIconPath = svg->getOutlineAsPath();
}

void FreezeButton::resized()
{
    freezeIconBounds = getLocalBounds().toFloat().reduced (6.0f);
    freezeIconPath.applyTransform (freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true));
}

void FreezeButton::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);

    g.setColour (freezeColour);
    g.fillPath (freezeIconPath);

    if (hasKeyboardFocus (true))
        drawFocusMark (g, freezeColour); 
}

void FreezeButton::mouseEnter (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);

    grabKeyboardFocus();
}

void FreezeButton::mouseExit (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);

    giveAwayKeyboardFocus();
}

void FreezeButton::mouseDown (const juce::MouseEvent& e)
{
    juce::Button::mouseDown (e);

    const auto centre = freezeIconBounds.getCentre();
    const auto trans  = juce::AffineTransform::scale (0.95f, 0.95f, centre.x, centre.y);
    freezeIconPath.applyTransform (trans);
}

void FreezeButton::mouseUp (const juce::MouseEvent& e)
{
    juce::Button::mouseUp (e);

    const auto trans = freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true);
    freezeIconPath.applyTransform (trans);
}

void FreezeButton::paintButton (juce::Graphics& g, 
                                bool shouldDrawButtonAsHighlighted,
                                bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused (g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown); 
}

void FreezeButton::drawFocusMark (juce::Graphics& g, juce::Colour colour)
{
    g.setColour (colour);

    const auto bounds = getLocalBounds().toFloat().reduced (3.0f);
    const auto length = juce::jmin (bounds.getHeight(), bounds.getWidth()) * 0.18f;
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
