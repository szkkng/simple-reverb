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
      sizeSliderAttachment  (audioProcessor.apvts, "size",   sizeDial),
      dampSliderAttachment  (audioProcessor.apvts, "damp",   dampDial),
      widthSliderAttachment (audioProcessor.apvts, "width",  widthDial),
      dwSliderAttachment    (audioProcessor.apvts, "dw",     dwDial),
      freezeAttachment      (audioProcessor.apvts, "freeze", freezeButton)
{
    juce::LookAndFeel::setDefaultLookAndFeel (&lnf);
    setWantsKeyboardFocus (true);

    const auto ratio = 2.0f / 1.0f; // width / height
    setResizable (true, true);
    setResizeLimits (400,  juce::roundToInt (400 / ratio),
                     1200, juce::roundToInt (1200 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (560, juce::roundToInt (560 / ratio));

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
    auto width        = bounds.getWidth();
    auto dialWidth    = getWidth() * 0.13f;
    auto dialHeight   = dialWidth;
    auto freezeWidth  = bounds.getWidth() * 0.2f;
    auto freezeHeight = bounds.getHeight() * 0.6f;

    auto sizeBounds   = bounds.removeFromLeft (width / 5.0f).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto dampBounds   = bounds.removeFromLeft (width / 5.0f).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto freezeBounds = bounds.removeFromLeft (width / 5.0f).withSizeKeepingCentre (freezeWidth, freezeHeight).toNearestInt();
    auto widthBounds  = bounds.removeFromLeft (width / 5.0f).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();
    auto dwBounds     = bounds.removeFromLeft (width / 5.0f).withSizeKeepingCentre (dialWidth, dialHeight).toNearestInt();

    sizeDial.setBounds     (sizeBounds);
    dampDial.setBounds     (dampBounds);
    freezeButton.setBounds (freezeBounds);
    widthDial.setBounds    (widthBounds);
    dwDial.setBounds       (dwBounds);
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
