/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleReverbAudioProcessorEditor::SimpleReverbAudioProcessorEditor (SimpleReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      sizeSliderAttachment (audioProcessor.apvts, "Room Size", sizeSlider),
      dampSliderAttachment (audioProcessor.apvts, "Damping", dampSlider),
      widthSliderAttachment (audioProcessor.apvts, "Width", widthSlider),
      dwSliderAttachment (audioProcessor.apvts, "Dry/Wet", dwSlider),
      freezeAttachment (audioProcessor.apvts, "Freeze", freezeButton)
{
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Avenir Next Medium");

    setSize (500, 250);
    setWantsKeyboardFocus (true);
    
    sizeLabel.setText ("Size", juce::NotificationType::dontSendNotification);
    sizeLabel.attachToComponent (&sizeSlider, false);
    
    dampLabel.setText ("Damp", juce::NotificationType::dontSendNotification);
    dampLabel.attachToComponent (&dampSlider, false);
    
    widthLabel.setText ("Width", juce::NotificationType::dontSendNotification);
    widthLabel.attachToComponent (&widthSlider, false);

    dwLabel.setText ("Dry/Wet", juce::NotificationType::dontSendNotification);
    dwLabel.attachToComponent (&dwSlider, false);
    
    freezeButton.setButtonText (juce::String (juce::CharPointer_UTF8 ("∞")));
    freezeButton.setClickingTogglesState (true);
    freezeButton.setLookAndFeel (&customLookAndFeel);
    freezeButton.setColour (juce::TextButton::buttonColourId, blue);
    freezeButton.setColour (juce::TextButton::buttonOnColourId, juce::Colours::transparentWhite);
    freezeButton.setColour (juce::TextButton::textColourOnId, blue);
    freezeButton.setColour (juce::TextButton::textColourOffId, grey);

    addAndMakeVisible (sizeSlider);
    addAndMakeVisible (dampSlider);
    addAndMakeVisible (widthSlider);
    addAndMakeVisible (dwSlider);
   
    addAndMakeVisible (freezeButton);
}

SimpleReverbAudioProcessorEditor::~SimpleReverbAudioProcessorEditor()
{
}

//==============================================================================
void SimpleReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (black);
    
    g.setFont (30);
    g.setColour (offWhite);
    g.drawText ("Simple Reverb", 150, 0, 200, 75, juce::Justification::centred);
}

void SimpleReverbAudioProcessorEditor::resized()
{
    sizeSlider.setBounds (30, 120, 60, 60);
    dampSlider.setBounds (125, 120, 60, 60);
    widthSlider.setBounds (315, 120, 60, 60);
    dwSlider.setBounds (410, 120, 60, 60);
    freezeButton.setBounds (210, 120, 80, 40);
}


