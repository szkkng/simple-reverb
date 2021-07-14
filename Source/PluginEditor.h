/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "RotarySlider.h"
#include "NameLabel.h"

//==============================================================================
/**
*/
class TestReverbAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TestReverbAudioProcessorEditor (TestReverbAudioProcessor&);
    ~TestReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    TestReverbAudioProcessor& audioProcessor;

    NameLabel sizeLabel,
              dampLabel, 
              widthLabel,
              dwLabel;       

    RotarySlider sizeSlider, 
                 dampSlider, 
                 widthSlider,
                 dwSlider; 
    
    juce::TextButton freezeButton;
    
    juce::AudioProcessorValueTreeState::SliderAttachment sizeSliderAttachment,
                                                         dampSliderAttachment,
                                                         widthSliderAttachment,
                                                         dwSliderAttachment;
    
    juce::AudioProcessorValueTreeState::ButtonAttachment freezeAttachment;
    
    CustomLookAndFeel customLookAndFeel;
    
    juce::Colour blue      = juce::Colour::fromFloatRGBA (0.43f, 0.83f, 1.0f, 1.0f);
    juce::Colour offWhite  = juce::Colour::fromFloatRGBA (0.83f, 0.84f, 0.9f, 1.0f);
    juce::Colour grey      = juce::Colour::fromFloatRGBA (0.42f, 0.42f, 0.42f, 1.0f);
    juce::Colour black     = juce::Colour::fromFloatRGBA (0.08f, 0.08f, 0.08f, 1.0f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestReverbAudioProcessorEditor)
};
