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
      sizeSliderAttachment  (audioProcessor.apvts, "size",   sizeSlider),
      dampSliderAttachment  (audioProcessor.apvts, "damp",   dampSlider),
      widthSliderAttachment (audioProcessor.apvts, "width",  widthSlider),
      dwSliderAttachment    (audioProcessor.apvts, "dw",     dwSlider),
      freezeAttachment      (audioProcessor.apvts, "freeze", freezeButton)
{
    juce::LookAndFeel::setDefaultLookAndFeel (&customLookAndFeel);
    setWantsKeyboardFocus (true);

    const auto ratio = 2.0f / 1.0f; // width / height
    setResizable (true, true);
    setResizeLimits (400,  juce::roundToInt (400 / ratio),
                     1200, juce::roundToInt (1200 / ratio));
    getConstrainer()->setFixedAspectRatio (ratio);
    setSize (560, juce::roundToInt (560 / ratio));

    sizeLabel.setText ("size", juce::NotificationType::dontSendNotification);
    sizeLabel.attachToComponent (&sizeSlider, false);

    dampLabel.setText ("damp", juce::NotificationType::dontSendNotification);
    dampLabel.attachToComponent (&dampSlider, false);

    widthLabel.setText ("width", juce::NotificationType::dontSendNotification);
    widthLabel.attachToComponent (&widthSlider, false);

    dwLabel.setText ("dw", juce::NotificationType::dontSendNotification);
    dwLabel.attachToComponent (&dwSlider, false);

    freezeButton.setButtonText (juce::String (juce::CharPointer_UTF8 ("∞")));
    freezeButton.setClickingTogglesState (true);
    freezeButton.setColour (juce::TextButton::buttonColourId,   juce::Colours::transparentWhite);
    freezeButton.setColour (juce::TextButton::buttonOnColourId, juce::Colours::transparentWhite);
    freezeButton.setColour (juce::TextButton::textColourOnId,   MyColours::blue);
    freezeButton.setColour (juce::TextButton::textColourOffId,  MyColours::grey);

    addAndMakeVisible (sizeSlider);
    addAndMakeVisible (dampSlider);
    addAndMakeVisible (widthSlider);
    addAndMakeVisible (dwSlider);
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

    sizeSlider.setBounds   (sizeBounds);
    dampSlider.setBounds   (dampBounds);
    freezeButton.setBounds (freezeBounds);
    widthSlider.setBounds  (widthBounds);
    dwSlider.setBounds     (dwBounds);
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
