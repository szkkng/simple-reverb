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

#include "DialTextBox.h"

juce::String DialTextBox::valueShownWithEditor = "";

DialTextBox::DialTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (false, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
}

void DialTextBox::resized()
{
    juce::Label::resized();
    setFont (getWidth() * 0.2f);
}

juce::TextEditor* DialTextBox::createEditorComponent()
{
    auto* ed = juce::Label::createEditorComponent();

    ed->setJustification (juce::Justification::centred);
    ed->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
    ed->setInputRestrictions (5, "0123456789.");
    ed->setIndents (4, 0);

    return ed;
}

void DialTextBox::editorShown (juce::TextEditor* ed)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NoCursor);
    ed->clear();
    ed->setText (valueShownWithEditor);
}

void DialTextBox::editorAboutToBeHidden (juce::TextEditor* /*ed*/)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NormalCursor);
}
