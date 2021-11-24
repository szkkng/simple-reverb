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
    setSize (560, 300);
    setWantsKeyboardFocus (true);

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
    sizeSlider.setBounds   (50,  130, 75, 75);
    dampSlider.setBounds   (145, 130, 75, 75);
    freezeButton.setBounds (240, 130, 80, 55);
    widthSlider.setBounds  (345, 130, 75, 75);
    dwSlider.setBounds     (440, 130, 75, 75);
}

bool SimpleReverbAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    if (key == cmdZ && undoManager.canUndo())
        undoManager.undo();
    else if (key == cmdShiftZ && undoManager.canRedo())
        undoManager.redo();

    return true;
}
