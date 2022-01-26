#pragma once

#include <JuceHeader.h>
#include "../../Resources/AvenirNextMedium.h"
#include "../../Resources/FuturaMedium.h"
#include "MyColours.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    struct CustomLabel : public juce::Label
    {
        static juce::String initValue;

        void resized() override
        {
            juce::Label::resized();
            setFont (getWidth() * 0.2f);
        }

        juce::TextEditor* createEditorComponent() override
        {
            auto* ed = juce::Label::createEditorComponent();

            ed->setJustification (juce::Justification::centred);
            ed->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
            ed->setInputRestrictions (5, "0123456789.");
            ed->setIndents (4, 0);

            return ed;
        }

        void editorShown (juce::TextEditor* editor) override
        {
            getParentComponent()->setMouseCursor (juce::MouseCursor::NoCursor);
            editor->clear();
            editor->setText (initValue);
        }

        void editorAboutToBeHidden (juce::TextEditor * ed) override
        {
            getParentComponent()->setMouseCursor (juce::MouseCursor::NormalCursor);
        }
    };

    CustomLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    CustomLabel* createSliderTextBox (juce::Slider& slider) override;

    juce::CaretComponent* createCaretComponent (juce::Component* keyFocusOwner) override;

    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;

    void drawButtonBackground (juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool /*isMouseOver*/, bool /*isMouseDragging*/) override;
};
