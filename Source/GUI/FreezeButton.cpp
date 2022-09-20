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
