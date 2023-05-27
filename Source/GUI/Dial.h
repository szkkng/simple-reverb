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

#include <JuceHeader.h>

class Dial  : public juce::Component
{
public:
    enum ColourIds
    {
        foregroundArcColourId,
        backgroundArcColourId,
        needleColourId,
        borderColourId,
    };

    explicit Dial (juce::RangedAudioParameter& param, juce::UndoManager* um = nullptr);

    void paint (juce::Graphics& g) override;
    void resized() override;

    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseDoubleClick (const juce::MouseEvent& e) override;

    bool keyPressed (const juce::KeyPress& k) override;

    void focusLost (FocusChangeType cause) override;

    float getValue() const;

    void setInterval (float newInterval);
    void setFineInterval (float newFineInterval);

    void setTextBoxColour (juce::Colour newColour);
    void setLabelColour (juce::Colour newColour);

    void setLabelText (const juce::String& newLabelText);

    void setAngle (float startAngleRadians, float endAngleRadians);

    std::function<void()> onValueChange;

private:
    void updateValue (float newValue);

    void drawDial (juce::Graphics& g);
    void drawBorder (juce::Graphics& g);

    juce::Label label;

    struct TextBox  : public juce::Label
    {
        juce::String valueShownWithEditor;

        TextBox();

        void resized() override;

        juce::TextEditor* createEditorComponent() override;

        void editorShown (juce::TextEditor* editor) override;
        void editorAboutToBeHidden (juce::TextEditor* editor) override;
    };

    TextBox textBox;

    float startAngle{};
    float endAngle{};

    float value{};

    juce::RangedAudioParameter& audioParam;
    juce::ParameterAttachment paramAttachment;

    float sensitivity { 0.01f };

    float interval { 1.0f };
    float fineInterval { 0.1f };

    juce::Point<float> mousePosWhenLastDragged;

    juce::Rectangle<float> dialBounds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
