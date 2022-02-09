#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
{
    auto futuraMediumFont = juce::Typeface::createSystemTypefaceFor (FuturaMedium::FuturaMedium_otf, FuturaMedium::FuturaMedium_otfSize);
    setDefaultSansSerifTypeface (futuraMediumFont);
}

juce::Slider::SliderLayout MyLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    auto localBounds = slider.getLocalBounds();

    juce::Slider::SliderLayout layout;

    layout.textBoxBounds = localBounds;
    layout.sliderBounds = localBounds;

    return layout;
}

void MyLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                      const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto fill = slider.findColour (juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.075f;
    auto arcRadius = radius - lineW * 2.0f;

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

    g.setColour (fill);
    g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto thumbWidth = lineW * 2.2f;

    juce::Path thumb;
    thumb.addRectangle (-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius + lineW);

    g.setColour (MyColours::creamWhite);
    g.fillPath (thumb, juce::AffineTransform::rotation (toAngle + 3.12f).translated (bounds.getCentre()));

    g.fillEllipse (bounds.reduced (bounds.getWidth() * 0.125f));
}

DialTextBox* MyLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* textBox = new DialTextBox();

    textBox->setColour (juce::Label::textColourId,            slider.findColour (juce::Slider::textBoxTextColourId));
    textBox->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));

    return textBox;
}

juce::CaretComponent* MyLookAndFeel::createCaretComponent (juce::Component* keyFocusOwner)
{
    auto caret = new juce::CaretComponent (keyFocusOwner);

    caret->setColour (juce::CaretComponent::caretColourId, juce::Colours::red);

    return caret;
}

juce::Font MyLookAndFeel::getTextButtonFont (juce::TextButton&, int buttonHeight)
{
    auto avenirNextMediumFont = juce::Typeface::createSystemTypefaceFor (AvenirNextMedium::AvenirNextMedium_otf, AvenirNextMedium::AvenirNextMedium_otfSize);
    juce::Font font (avenirNextMediumFont);

    return font.withHeight (buttonHeight * 1.2f);
}

// Remove the button border
void MyLookAndFeel::drawButtonBackground (juce::Graphics& /*g*/, juce::Button& /*button*/, const juce::Colour& /*backgroundColour*/,
                                          bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
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
