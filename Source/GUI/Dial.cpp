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

#include "Dial.h"
#include "MyColours.h"

Dial::TextBox::TextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (false, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
}

void Dial::TextBox::resized()
{
    juce::Label::resized();
    setFont (getHeight() * 0.8f);
}

juce::TextEditor* Dial::TextBox::createEditorComponent()
{
    auto* ed = juce::Label::createEditorComponent();

    ed->setJustification (juce::Justification::centred);
    ed->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
    ed->setColour (juce::CaretComponent::caretColourId, MyColours::red);
    ed->setInputRestrictions (5, "-0123456789.");
    ed->setIndents (4, 0);
    ed->onTextChange = [&]()
    {
        auto mms = juce::Desktop::getInstance().getMainMouseSource();
        mms.hideCursor();
    };

    return ed;
}

void Dial::TextBox::editorShown (juce::TextEditor* ed)
{
    ed->clear();
    ed->setText (valueShownWithEditor);
}

void Dial::TextBox::editorAboutToBeHidden (juce::TextEditor* editor)
{
    juce::ignoreUnused (editor);
    getParentComponent()->grabKeyboardFocus();
}

Dial::Dial (juce::RangedAudioParameter& param, juce::UndoManager* um)
    : audioParam (param),
      paramAttachment (audioParam, [&](float v) { updateValue (v); }, um)
{
    paramAttachment.sendInitialUpdate();

    setWantsKeyboardFocus (true);
    setRepaintsOnMouseActivity (true);
    setColour (foregroundArcColourId, MyColours::blue);
    setColour (backgroundArcColourId, MyColours::blackGrey);
    setColour (needleColourId,        MyColours::midGrey);
    setColour (borderColourId,        MyColours::grey);

    auto range   = audioParam.getNormalisableRange();
    interval     = range.getRange().getLength() / 100.0f;
    fineInterval = interval * 0.1f;

    auto pi    = juce::MathConstants<float>::pi;
    startAngle = pi + pi / 6.0f; 
    endAngle   = 3.0f * pi - pi / 6.0f;

    auto paramId = audioParam.getName (8);
    setLabelColour (MyColours::grey);
    label.setText (paramId, juce::NotificationType::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setInterceptsMouseClicks (false, false);

    setTextBoxColour (MyColours::grey);
    textBox.onTextChange = [&]()
    {
        auto newNormValue   = audioParam.getValueForText (textBox.getText());
        auto newDenormValue = audioParam.convertFrom0to1 (newNormValue);
        paramAttachment.setValueAsCompleteGesture  (newDenormValue);
        textBox.setText (audioParam.getCurrentValueAsText(),
                         juce::NotificationType::dontSendNotification);
    };

    addAndMakeVisible (label);
    addAndMakeVisible (textBox);
}

void Dial::resized()
{
    auto bounds = getLocalBounds().toFloat();

    label.setBounds (bounds.removeFromTop (getHeight() / 4.0f).toNearestInt());
    label.setFont (label.getHeight() * 0.8f);

    textBox.setBounds (bounds.removeFromBottom (getHeight() / 4.0f).toNearestInt());

    dialBounds = bounds.expanded (1.0f).withY (bounds.getY() + 1);
}

void Dial::paint (juce::Graphics& g)
{
    drawDial (g);

    if (hasKeyboardFocus (true))
        drawBorder (g);
}

void Dial::mouseDown (const juce::MouseEvent& e)
{
    paramAttachment.beginGesture();

    e.source.enableUnboundedMouseMovement (true);

    mousePosWhenLastDragged = e.position;
}

void Dial::mouseDrag (const juce::MouseEvent& e)
{
    auto diffY = (mousePosWhenLastDragged.y - e.position.y) * sensitivity;

    if (e.mods.isShiftDown())
        diffY *= 0.1f;

    value = juce::jlimit (0.0f, 1.0f, value + diffY);
    auto newDenormValue = audioParam.convertFrom0to1 (value);
    paramAttachment.setValueAsPartOfGesture (newDenormValue);
    textBox.setText (audioParam.getCurrentValueAsText(),
                     juce::NotificationType::dontSendNotification);

    mousePosWhenLastDragged = e.position;
}

void Dial::mouseUp (const juce::MouseEvent& e)
{
    paramAttachment.endGesture();

    e.source.enableUnboundedMouseMovement (false);

    auto mms = juce::Desktop::getInstance().getMainMouseSource();
    mms.setScreenPosition (e.source.getLastMouseDownPosition());
}

void Dial::mouseDoubleClick (const juce::MouseEvent& e)
{
    juce::ignoreUnused(e);

    auto defaultValue = audioParam.getDefaultValue();
    value = defaultValue;

    auto newDenormValue = audioParam.convertFrom0to1 (defaultValue);
    paramAttachment.setValueAsCompleteGesture (newDenormValue);

    textBox.setText (audioParam.getCurrentValueAsText(),
                     juce::NotificationType::dontSendNotification);
}

bool Dial::keyPressed (const juce::KeyPress& k)
{
    if (('0' <= k.getKeyCode() && k.getKeyCode() <= '9')
        || k.getKeyCode() == '-' || k.getKeyCode() == '.')
    {
        textBox.valueShownWithEditor = juce::String::charToString (k.getTextCharacter());
        textBox.showEditor();

        return true;
    }

    if (k.getKeyCode() == juce::KeyPress::upKey)
    {
        auto newValue = getValue() + interval;

        if (k.getModifiers() == juce::ModifierKeys::shiftModifier)
            newValue = getValue() + fineInterval;

        paramAttachment.setValueAsCompleteGesture (newValue);

        return true;
    }

    if (k.getKeyCode() == juce::KeyPress::downKey)
    {
        auto newValue = getValue() - interval;

        if (k.getModifiers() == juce::ModifierKeys::shiftModifier)
            newValue = getValue() - fineInterval;

        paramAttachment.setValueAsCompleteGesture (newValue);

        return true;
    }

    return false;
}

void Dial::focusLost (FocusChangeType cause)
{
    juce::ignoreUnused (cause);
    repaint();
}

float Dial::getValue() const
{
    return audioParam.convertFrom0to1 (value);
}

void Dial::setInterval (float newInterval)
{
    interval = newInterval;
}

void Dial::setFineInterval (float newFineInterval)
{
    fineInterval = newFineInterval;
}

void Dial::setTextBoxColour (juce::Colour newColour)
{
    textBox.setColour (juce::Label::textColourId,            newColour);
    textBox.setColour (juce::Label::textWhenEditingColourId, newColour);
}

void Dial::setLabelColour (juce::Colour newColour)
{
    label.setColour (juce::Label::textColourId, newColour);
}

void Dial::setLabelText (const juce::String& newLabelText)
{
    label.setText (newLabelText, juce::NotificationType::dontSendNotification);
}

void Dial::setAngle (float startAngleRadians, float endAngleRadians)
{
    startAngle = startAngleRadians;
    endAngle   = endAngleRadians;
}

void Dial::updateValue (float newValue)
{
    value = audioParam.convertTo0to1 (newValue);
    textBox.setText (audioParam.getCurrentValueAsText(),
                     juce::NotificationType::dontSendNotification);

    if (onValueChange != nullptr)
        onValueChange();

    repaint();
}

void Dial::drawDial (juce::Graphics& g)
{
    const auto radius    = juce::jmin (dialBounds.getWidth(), dialBounds.getHeight()) / 2.0f;
    const auto toAngle   = startAngle + value * (endAngle - startAngle);
    const auto lineWidth = radius * 0.1f;
    const auto arcRadius = radius - lineWidth;
    const auto centre = dialBounds.getCentre();
    auto space = 0.2f;

    if (toAngle + space >= endAngle - space)
    {
        auto restAngle = endAngle - toAngle;
        space *= restAngle / (space * 2.0f);
    }

    juce::Path backgroundArc;
    backgroundArc.addCentredArc (centre.x,
                                 centre.y,
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 juce::jlimit (startAngle, endAngle, toAngle + space),
                                 endAngle,
                                 true);
    g.setColour (findColour (backgroundArcColourId));
    g.strokePath (backgroundArc, juce::PathStrokeType { lineWidth });

    juce::Path valueArc;
    valueArc.addCentredArc (centre.x,
                            centre.y,
                            arcRadius,
                            arcRadius,
                            0.0f,
                            startAngle,
                            toAngle,
                            true);
    g.setColour (findColour (foregroundArcColourId));
    g.strokePath (valueArc, juce::PathStrokeType { lineWidth });

    juce::Path needle;
    auto needleWidth = lineWidth * 1.5f;
    auto needleLen   = radius + lineWidth * 0.3f;
    needle.addRoundedRectangle (centre.x - needleWidth * 0.5f, 
                                centre.y + needleWidth * 0.5f - needleLen, 
                                needleWidth, needleLen, needleWidth * 0.5f);
    g.setColour (findColour (needleColourId));
    g.fillPath (needle, juce::AffineTransform::rotation (toAngle, centre.x, centre.y));
}

void Dial::drawBorder (juce::Graphics& g)
{
    constexpr auto length    = 4.0f;
    constexpr auto thickness = 1.5f;
    const auto bounds = getLocalBounds().toFloat().reduced (thickness * 1.9f);
    auto radian = 0.0f;

    // Draw in clockwise order, starting from top left.
    for (const auto corner : { bounds.getTopLeft(), bounds.getTopRight(), bounds.getBottomRight(), bounds.getBottomLeft() })
    {
        juce::Path path;

        path.startNewSubPath (corner.x, corner.y + length);
        path.lineTo          (corner);
        path.lineTo          (corner.x + length, corner.y);

        g.strokePath (path,
                      juce::PathStrokeType (thickness),
                      juce::AffineTransform::rotation (radian, corner.x, corner.y));

        radian += juce::MathConstants<float>::halfPi;
    };
}