
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel(){};
CustomLookAndFeel::~CustomLookAndFeel(){};

juce::Slider::SliderLayout CustomLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    auto localBounds = slider.getLocalBounds();
    
    juce::Slider::SliderLayout layout;

    layout.textBoxBounds = localBounds;
    layout.sliderBounds = localBounds;

    return layout;
}

void CustomLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.5f;
    
    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    
    g.setColour (blackGrey);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    juce::Path valueArc;
    valueArc.addCentredArc (bounds.getCentreX(),
                            bounds.getCentreY(),
                            arcRadius,
                            arcRadius,
                            0.0f,
                            rotaryStartAngle,
                            toAngle,
                            true);
        
    auto alpha = 0.1f + (float) slider.getValue() * 0.9f;
    auto brightness = 0.4f + (float) slider.getValue() * 0.6f;

    g.setColour (fill.withAlpha (alpha).brighter (brightness));
    g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto thumbWidth = lineW * 2.0f;
 
    juce::Path thumb;
    thumb.addRectangle (-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius + lineW);
    
    g.setColour (offWhite);
    g.fillPath (thumb, juce::AffineTransform::rotation (toAngle + 3.12f).translated (bounds.getCentre()));

    g.fillEllipse (bounds.reduced (8.0f));
}

juce::Label* CustomLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* l = new juce::Label();
    l->setJustificationType (juce::Justification::centred);
    l->setColour (juce::Label::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    l->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    l->setColour (juce::Label::outlineWhenEditingColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));
    l->setInterceptsMouseClicks (false, false);
    l->setFont (15);
    
    return l;
}

juce::Font CustomLookAndFeel::getTextButtonFont (juce::TextButton&, int buttonHeight)
{
    juce::Font font ("Avenir Next Medium", 90.f, 0);
    return { font };
}

void CustomLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
}
