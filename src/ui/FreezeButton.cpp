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
#include <BinaryData.h>

FreezeButton::FreezeButton (juce::RangedAudioParameter& param, juce::UndoManager* um)
    : audioParam (param)
    , paramAttachment (audioParam, [&] (float v) { updateState (static_cast<bool> (v)); }, um)
{
    setWantsKeyboardFocus (true);
    setRepaintsOnMouseActivity (true);
    setColour (onColourId, MyColours::blue);
    setColour (offColourId, MyColours::midGrey);
    setColour (focusColourId, MyColours::midGrey.brighter (0.25f));

    const auto svg = juce::Drawable::createFromImageData (BinaryData::FreezeIcon_svg, BinaryData::FreezeIcon_svgSize);
    jassert (svg != nullptr);

    if (svg != nullptr)
        iconPath = svg->getOutlineAsPath();

    paramAttachment.sendInitialUpdate();
}

void FreezeButton::resized()
{
    iconBounds = getLocalBounds().toFloat();
    iconPath.applyTransform (iconPath.getTransformToScaleToFit (iconBounds, true));
}

void FreezeButton::paint (juce::Graphics& g)
{
    g.setColour (findColour (state ? onColourId : hasKeyboardFocus (true) ? focusColourId : offColourId));
    g.fillPath (iconPath);
}

void FreezeButton::mouseDown (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);

    paramAttachment.setValueAsCompleteGesture (! state);

    const auto centre = iconBounds.getCentre();
    iconPath.applyTransform (juce::AffineTransform::scale (0.95f, 0.95f, centre.x, centre.y));
}

void FreezeButton::mouseUp (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);
    iconPath.applyTransform (iconPath.getTransformToScaleToFit (iconBounds, true));
}

void FreezeButton::focusGained (FocusChangeType cause)
{
    juce::ignoreUnused (cause);
    repaint();
}

void FreezeButton::focusLost (FocusChangeType cause)
{
    juce::ignoreUnused (cause);
    repaint();
}

bool FreezeButton::keyPressed (const juce::KeyPress& key)
{
    if (key == juce::KeyPress::returnKey)
    {
        paramAttachment.setValueAsCompleteGesture (! state);
        return true;
    }

    return false;
}

void FreezeButton::updateState (bool newState)
{
    state = newState;
    repaint();
}
