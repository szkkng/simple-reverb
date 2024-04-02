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

#include "EditorContent.h"
#include "../ParamIDs.h"

EditorContent::EditorContent (SimpleReverbAudioProcessor& p, juce::UndoManager& um)
    : apvts (p.getPluginState())
    , undoManager (um)
    , sizeDial (*apvts.getParameter (ParamIDs::size), &um)
    , dampDial (*apvts.getParameter (ParamIDs::damp), &um)
    , widthDial (*apvts.getParameter (ParamIDs::width), &um)
    , mixDial (*apvts.getParameter (ParamIDs::mix), &um)
    , freezeButton (*apvts.getParameter (ParamIDs::freeze), &um)
{
    setWantsKeyboardFocus (true);
    setFocusContainerType (FocusContainerType::keyboardFocusContainer);

    sizeDial.setExplicitFocusOrder (1);
    dampDial.setExplicitFocusOrder (2);
    freezeButton.setExplicitFocusOrder (3);
    widthDial.setExplicitFocusOrder (4);
    mixDial.setExplicitFocusOrder (5);

    addAndMakeVisible (sizeDial);
    addAndMakeVisible (dampDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (mixDial);
    addAndMakeVisible (freezeButton);
}

void EditorContent::paint (juce::Graphics& g) { g.fillAll (MyColours::black); }

void EditorContent::resized()
{
    const juce::Rectangle baseDialBounds { 0, 73, 80, 96 };
    sizeDial.setBounds (baseDialBounds.withX (46));
    dampDial.setBounds (baseDialBounds.withX (144));
    widthDial.setBounds (baseDialBounds.withX (342));
    mixDial.setBounds (baseDialBounds.withX (440));

    freezeButton.setBounds (259, 110, 48, 32);
}

bool EditorContent::keyPressed (const juce::KeyPress& k)
{
    if (k.isKeyCode (juce::KeyPress::tabKey) && hasKeyboardFocus (false))
    {
        sizeDial.grabKeyboardFocus();
        return true;
    }

    if (const auto cmdZ = juce::KeyPress ('z', juce::ModifierKeys::commandModifier, 0);
        k == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }

    if (const auto cmdShiftZ =
            juce::KeyPress ('z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier, 0);
        k == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }

    return false;
}
