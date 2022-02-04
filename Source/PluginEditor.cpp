/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleReverbAudioProcessorEditor::SimpleReverbAudioProcessorEditor (SimpleReverbAudioProcessor& p, juce::UndoManager& um)
    : AudioProcessorEditor (&p), audioProcessor (p), undoManager (um),
      sizeDialAttachment     (audioProcessor.apvts, "size",   sizeDial),
      dampDialAttachment     (audioProcessor.apvts, "damp",   dampDial),
      widthDialAttachment    (audioProcessor.apvts, "width",  widthDial),
      dwDialAttachment       (audioProcessor.apvts, "dw",     dwDial),
      freezeButtonAttachment (audioProcessor.apvts, "freeze", freezeButton)
{
    juce::LookAndFeel::setDefaultLookAndFeel (&lnf);
    setWantsKeyboardFocus (true);

    const double ratio = 560.0 / 280.0;
    setResizable (true, true);
    setResizeLimits (400,  juce::roundToInt (400.0 / ratio),
                     1200, juce::roundToInt (1200.0 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (560, 280);

    sizeLabel.setText ("size", juce::NotificationType::dontSendNotification);
    sizeLabel.attachToComponent (&sizeDial, false);

    dampLabel.setText ("damp", juce::NotificationType::dontSendNotification);
    dampLabel.attachToComponent (&dampDial, false);

    widthLabel.setText ("width", juce::NotificationType::dontSendNotification);
    widthLabel.attachToComponent (&widthDial, false);

    dwLabel.setText ("dw", juce::NotificationType::dontSendNotification);
    dwLabel.attachToComponent (&dwDial, false);

    freezeButton.setButtonText (juce::String (juce::CharPointer_UTF8 ("∞")));
    freezeButton.setClickingTogglesState (true);
    freezeButton.setColour (juce::TextButton::buttonColourId,   juce::Colours::transparentWhite);
    freezeButton.setColour (juce::TextButton::buttonOnColourId, juce::Colours::transparentWhite);
    freezeButton.setColour (juce::TextButton::textColourOnId,   MyColours::blue);
    freezeButton.setColour (juce::TextButton::textColourOffId,  MyColours::grey);

    addAndMakeVisible (sizeDial);
    addAndMakeVisible (dampDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (dwDial);
    addAndMakeVisible (freezeButton);
}

SimpleReverbAudioProcessorEditor::~SimpleReverbAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel (nullptr);
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

    auto sizeDialArea     = bounds.removeFromLeft (compArea).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto dampDialArea     = bounds.removeFromLeft (compArea).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto freezeButtonArea = bounds.removeFromLeft (compArea).withSizeKeepingCentre (buttonWidth, buttonHeight).toNearestInt();
    auto widthDialArea    = bounds.removeFromLeft (compArea).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto dwDialArea       = bounds.removeFromLeft (compArea).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();

    sizeDial.setBounds     (sizeDialArea);
    dampDial.setBounds     (dampDialArea);
    freezeButton.setBounds (freezeButtonArea);
    widthDial.setBounds    (widthDialArea);
    dwDial.setBounds       (dwDialArea);
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
