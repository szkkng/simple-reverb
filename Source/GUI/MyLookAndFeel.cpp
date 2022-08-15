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

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
{
    const auto futuraMediumFont = juce::Typeface::createSystemTypefaceFor (FuturaMedium::FuturaMedium_ttf, FuturaMedium::FuturaMedium_ttfSize);
    setDefaultSansSerifTypeface (futuraMediumFont);
}

juce::Slider::SliderLayout MyLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    juce::Slider::SliderLayout layout;

    const auto bounds    = slider.getLocalBounds().withY (3);
    layout.textBoxBounds = bounds;
    layout.sliderBounds  = bounds;

    return layout;
}

void MyLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                      const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    const auto bounds    = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (1.0f);
    const auto toAngle   = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const auto radius    = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const auto lineW     = radius * 0.075f;
    const auto arcRadius = radius - lineW * 2.0f;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    g.setColour (MyColours::blackGrey);
    juce::PathStrokeType strokeType { lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded };
    g.strokePath (backgroundArc, strokeType);

    juce::Path valueArc;
    valueArc.addCentredArc (bounds.getCentreX(),
                            bounds.getCentreY(),
                            arcRadius,
                            arcRadius,
                            0.0f,
                            rotaryStartAngle,
                            toAngle,
                            true);
    g.setColour (slider.findColour (juce::Slider::rotarySliderFillColourId));
    g.strokePath (valueArc, strokeType);

    juce::Path thumb;
    const auto thumbW = lineW * 2.5f;
    const auto thumbH = radius + lineW;
    const auto centre = bounds.getCentre();
    thumb.addRectangle (centre.x - thumbW * 0.5f, (centre.y + thumbW * 0.5f) - thumbH, thumbW, thumbH);
    g.setColour (MyColours::cream);
    g.fillPath (thumb, juce::AffineTransform::rotation (toAngle, bounds.getCentreX(), bounds.getCentreY()));

    auto innerRadius = arcRadius - lineW * 1.8f;
    g.fillEllipse (bounds.withSizeKeepingCentre (innerRadius * 2.0f, innerRadius * 2.0f));
}

DialTextBox* MyLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* textBox = new DialTextBox();

    textBox->setColour (juce::Label::textColourId,            slider.findColour (juce::Slider::textBoxTextColourId));
    textBox->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));

    return textBox;
}

void MyLookAndFeel::drawCornerResizer (juce::Graphics& g, int w, int h, bool /*isMouseOver*/, bool /*isMouseDragging*/)
{
    auto lineThickness = juce::jmin ((float) w, (float) h) * 0.07f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.setColour (MyColours::blackGrey);

        g.drawLine ((float) w * i,
                    (float) h + 1.0f,
                    (float) w + 1.0f,
                    (float) h * i,
                    lineThickness);

        g.setColour (MyColours::blackGrey);

        g.drawLine ((float) w * i + lineThickness,
                    (float) h + 1.0f,
                    (float) w + 1.0f,
                    (float) h * i + lineThickness,
                    lineThickness);
    }
}
