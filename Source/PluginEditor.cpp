/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamNames.h"

//==============================================================================
SimpleReverbAudioProcessorEditor::SimpleReverbAudioProcessorEditor (SimpleReverbAudioProcessor& p, juce::UndoManager& um)
    : AudioProcessorEditor (&p), audioProcessor (p), undoManager (um),
      sizeDialAttachment     (audioProcessor.apvts, ParamNames::size,   sizeDial),
      dampDialAttachment     (audioProcessor.apvts, ParamNames::damp,   dampDial),
      widthDialAttachment    (audioProcessor.apvts, ParamNames::width,  widthDial),
      dwDialAttachment       (audioProcessor.apvts, ParamNames::dryWet, dwDial),
      freezeButtonAttachment (audioProcessor.apvts, ParamNames::freeze, freezeButton)
{
    setWantsKeyboardFocus (true);

    const double ratio = 560.0 / 280.0;
    setResizable (true, true);
    setResizeLimits (400,  juce::roundToInt (400.0 / ratio),
                     1200, juce::roundToInt (1200.0 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (560, 280);

    sizeLabel.setText (ParamNames::size, juce::NotificationType::dontSendNotification);
    sizeLabel.attachToComponent (&sizeDial, false);

    dampLabel.setText (ParamNames::damp, juce::NotificationType::dontSendNotification);
    dampLabel.attachToComponent (&dampDial, false);

    widthLabel.setText (ParamNames::width, juce::NotificationType::dontSendNotification);
    widthLabel.attachToComponent (&widthDial, false);

    dwLabel.setText (ParamNames::dryWet, juce::NotificationType::dontSendNotification);
    dwLabel.attachToComponent (&dwDial, false);

    addAndMakeVisible (sizeDial);
    addAndMakeVisible (dampDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (dwDial);
    addAndMakeVisible (freezeButton);
}

SimpleReverbAudioProcessorEditor::~SimpleReverbAudioProcessorEditor()
{
}

//==============================================================================
void SimpleReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);
}

void SimpleReverbAudioProcessorEditor::resized()
{
    auto bounds       = getLocalBounds().toFloat()
                                        .removeFromBottom (getHeight() * 0.9f)
                                        .reduced (getWidth() * 0.06f, getHeight() * 0.25f);
    auto dialWidth    = bounds.getWidth() * 0.15f;
    auto dialHeight   = dialWidth;
    auto buttonWidth  = bounds.getWidth() * 0.18f;
    auto buttonHeight = buttonWidth * 0.8f;
    auto compArea     = bounds.getWidth() / 5.0f;

    auto sizeDialArea     = bounds.removeFromLeft (compArea);
    auto dampDialArea     = bounds.removeFromLeft (compArea);
    auto freezeButtonArea = bounds.removeFromLeft (compArea);
    auto widthDialArea    = bounds.removeFromLeft (compArea);
    auto dwDialArea       = bounds.removeFromLeft (compArea);

    sizeDial.setBounds     (sizeDialArea.withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt());
    dampDial.setBounds     (dampDialArea.withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt());
    freezeButton.setBounds (freezeButtonArea.withSizeKeepingCentre (buttonWidth, buttonHeight)
                                            .withY (bounds.getY() * 1.21f).toNearestInt());
    widthDial.setBounds    (widthDialArea.withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt());
    dwDial.setBounds       (dwDialArea.withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt());
}

bool SimpleReverbAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    auto cmdZ      = juce::KeyPress ('z', juce::ModifierKeys::commandModifier, 0);
    auto cmdShiftZ = juce::KeyPress ('z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier, 0);

    if (key == cmdZ && undoManager.canUndo())
        undoManager.undo();

    if (key == cmdShiftZ && undoManager.canRedo())
        undoManager.redo();

    return true;
}
