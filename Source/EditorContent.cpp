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
#include "ParamIDs.h"
#include "GUI/MyColours.h"

EditorContent::EditorContent (SimpleReverbAudioProcessor& p)
    : apvts (p.getPluginState()),
      sizeDialAttachment     (apvts, ParamIDs::size,   sizeDial),
      dampDialAttachment     (apvts, ParamIDs::damp,   dampDial),
      widthDialAttachment    (apvts, ParamIDs::width,  widthDial),
      dwDialAttachment       (apvts, ParamIDs::dryWet, dwDial),
      freezeButtonAttachment (apvts, ParamIDs::freeze, freezeButton)
{
    setWantsKeyboardFocus (true);

    auto setDial = [&](Dial& dial, juce::Label& label, const juce::String& labelName) 
    {
        label.setFont (20.0f);
        label.setText (labelName, juce::NotificationType::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setColour (juce::Label::textColourId, MyColours::grey);
        addAndMakeVisible (dial);
        addAndMakeVisible (label);
    };

    setDial (sizeDial,  sizeLabel,  ParamIDs::size);
    setDial (dampDial,  dampLabel,  ParamIDs::damp);
    setDial (widthDial, widthLabel, ParamIDs::width);
    setDial (dwDial,    dwLabel,    ParamIDs::dryWet);

    addAndMakeVisible (freezeButton);
}

void EditorContent::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);
}

void EditorContent::resized()
{
    const juce::Rectangle<int> baseDialBounds { 0, 117, 74, 74 };
    sizeDial.setBounds  (baseDialBounds.withX (46));
    dampDial.setBounds  (baseDialBounds.withX (144));
    widthDial.setBounds (baseDialBounds.withX (342));
    dwDial.setBounds    (baseDialBounds.withX (440));

    const juce::Rectangle<int> baseLabelBounds { 0, 94, 74, 22};
    sizeLabel.setBounds  (baseLabelBounds.withX (sizeDial.getX()));
    dampLabel.setBounds  (baseLabelBounds.withX (dampDial.getX()));
    widthLabel.setBounds (baseLabelBounds.withX (widthDial.getX()));
    dwLabel.setBounds    (baseLabelBounds.withX (dwDial.getX()));

    freezeButton.setBounds (243, 126, 74, 60);
}
