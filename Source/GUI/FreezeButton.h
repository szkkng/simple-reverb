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

#pragma once

#include "JuceHeader.h"
#include "MyColours.h"

class FreezeButton  : public juce::Button
{
public:
    FreezeButton();

    void paint (juce::Graphics& g) override;
    void resized () override;

    void mouseDown (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    void paintButton (juce::Graphics& g, 
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown) override;

private:
    juce::Path freezeIconPath;
    juce::Rectangle<float> freezeIconBounds;
    juce::Colour freezeColour { MyColours::midGrey };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreezeButton)
};
