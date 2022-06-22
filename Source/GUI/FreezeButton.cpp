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
#include "FreezeIcon.h"

FreezeButton::FreezeButton()
{
    setOpaque (true);
    setClickingTogglesState (true);
    onClick = [&]()
    {
        freezeColour = getToggleState() ? MyColours::blue : MyColours::grey;
    };
    freezeIconPath.loadPathFromData (FreezeIcon::pathData, sizeof (FreezeIcon::pathData));
}

void FreezeButton::resized()
{
    freezeIconBounds = juce::Rectangle<float> (getWidth() * 0.215f, getHeight() * 0.06f, getWidth() * 0.56f, getHeight() * 0.9f);
    freezeIconPath.applyTransform (freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true));
}

void FreezeButton::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);

    g.setColour (freezeColour);
    g.fillPath (freezeIconPath);
}

void FreezeButton::mouseDown (const juce::MouseEvent& event)
{
    juce::ToggleButton::mouseDown (event);

    auto transform = freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true)
                                   .scale (0.95f, 0.95f, freezeIconBounds.getCentreX(), freezeIconBounds.getCentreY());
    freezeIconPath.applyTransform (transform);
}

void FreezeButton::mouseUp (const juce::MouseEvent& event)
{
    juce::ToggleButton::mouseUp (event);

    auto transform = freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true);
    freezeIconPath.applyTransform (transform);
}
