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
#include "FuturaMedium.h"
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

    const auto width = static_cast<float> (w);
    const auto height = static_cast<float> (h);
    const auto lineThickness = juce::jmin (width, height) * 0.07f;

    g.setColour (isMouseOver ? MyColours::blue : MyColours::blackGrey);

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.drawLine (width * i, height + 1.0f, width + 1.0f, height * i, lineThickness);
        g.drawLine (width * i + lineThickness, height + 1.0f, width + 1.0f, height * i + lineThickness, lineThickness);
    }
}
