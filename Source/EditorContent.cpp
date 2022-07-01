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
#include "ParamNames.h"
#include "GUI/MyColours.h"

EditorContent::EditorContent (SimpleReverbAudioProcessor& p)
    : processor (p), apvts (p.getPluginState()),
      sizeDialAttachment     (apvts, ParamNames::size,   sizeDial),
      dampDialAttachment     (apvts, ParamNames::damp,   dampDial),
      widthDialAttachment    (apvts, ParamNames::width,  widthDial),
      dwDialAttachment       (apvts, ParamNames::dryWet, dwDial),
      freezeButtonAttachment (apvts, ParamNames::freeze, freezeButton)
{
    setWantsKeyboardFocus (true);

    auto setDial = [&](Dial& dial, juce::Label& label, juce::String labelName) 
    {
        addAndMakeVisible (dial);
        addAndMakeVisible (label);
        label.setText (labelName, juce::NotificationType::dontSendNotification);
    };

    setDial (sizeDial,  sizeLabel,  ParamNames::size);
    setDial (dampDial,  dampLabel,  ParamNames::damp);
    setDial (widthDial, widthLabel, ParamNames::width);
    setDial (dwDial,    dwLabel,    ParamNames::dryWet);

    addAndMakeVisible (freezeButton);
}

void EditorContent::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);
}

void EditorContent::resized()
{
    auto dialSize = 74;
    auto dialY    = 117;
    sizeDial.setBounds  (46,  dialY, dialSize, dialSize);
    dampDial.setBounds  (144, dialY, dialSize, dialSize);
    widthDial.setBounds (342, dialY, dialSize, dialSize);
    dwDial.setBounds    (440, dialY, dialSize, dialSize);

    auto labelHeight = 22;
    auto labelY      = dialY - juce::roundToInt ((float) labelHeight * 1.1f);
    auto labelWidth  = dialSize;
    sizeLabel.setBounds  (sizeDial.getX(),  labelY, labelWidth, labelHeight);
    dampLabel.setBounds  (dampDial.getX(),  labelY, labelWidth, labelHeight);
    widthLabel.setBounds (widthDial.getX(), labelY, labelWidth, labelHeight);
    dwLabel.setBounds    (dwDial.getX(),    labelY, labelWidth, labelHeight);

    freezeButton.setBounds (243, 126, 74, 60);
}
