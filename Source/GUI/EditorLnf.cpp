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

#include "EditorLnf.h"
#include "../../Resources/FuturaMedium.h"
#include "MyColours.h"

EditorLnf::EditorLnf()
{
    const auto futuraMediumFont =
        juce::Typeface::createSystemTypefaceFor (FuturaMedium::FuturaMedium_ttf, FuturaMedium::FuturaMedium_ttfSize);
    setDefaultSansSerifTypeface (futuraMediumFont);
}

void EditorLnf::drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging)
{
    juce::ignoreUnused (isMouseDragging);

    auto lineThickness = juce::jmin ((float) w, (float) h) * 0.07f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        auto colour = isMouseOver ? MyColours::blue : MyColours::blackGrey;
        g.setColour (colour);

        g.drawLine ((float) w * i, (float) h + 1.0f, (float) w + 1.0f, (float) h * i, lineThickness);

        g.drawLine ((float) w * i + lineThickness,
                    (float) h + 1.0f,
                    (float) w + 1.0f,
                    (float) h * i + lineThickness,
                    lineThickness);
    }
}
