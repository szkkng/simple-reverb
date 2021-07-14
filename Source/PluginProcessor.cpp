/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestReverbAudioProcessor::TestReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TestReverbAudioProcessor::~TestReverbAudioProcessor()
{
}

//==============================================================================
const juce::String TestReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TestReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TestReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    
    leftReverb.prepare(spec);
    rightReverb.prepare(spec);
}

void TestReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.roomSize   = *apvts.getRawParameterValue ("Room Size");
    params.damping    = *apvts.getRawParameterValue ("Damping");
    params.width      = *apvts.getRawParameterValue ("Width");
    params.wetLevel   = *apvts.getRawParameterValue ("Dry/Wet");
    params.dryLevel   = 1.0f - *apvts.getRawParameterValue ("Dry/Wet");
    params.freezeMode = *apvts.getRawParameterValue ("Freeze");
    
    leftReverb.setParameters (params);
    rightReverb.setParameters (params);

    juce::dsp::AudioBlock<float> block (buffer);
        
    auto leftBlock = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
    
    leftReverb.process (leftContext);
    rightReverb.process (rightContext);
}

//==============================================================================
bool TestReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestReverbAudioProcessor::createEditor()
{
    return new TestReverbAudioProcessorEditor (*this);
}

//==============================================================================
void TestReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestReverbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TestReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("Room Size",
                                                             "Room Size",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                if (value * 100 < 10.0f)
                                                                    return juce::String (value * 100, 2);
                                                                else if (value * 100 < 100.0f)
                                                                    return juce::String (value * 100, 1);
                                                                else
                                                                    return juce::String (value * 100, 0); },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("Damping",
                                                             "Damping",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                if (value * 100 < 10.0f)
                                                                    return juce::String (value * 100, 2);
                                                                else if (value * 100 < 100.0f)
                                                                    return juce::String (value * 100, 1);
                                                                else
                                                                    return juce::String (value * 100, 0); },
                                                             nullptr));
    
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("Width",
                                                             "Width",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                if (value * 100 < 10.0f)
                                                                    return juce::String (value * 100, 2);
                                                                else if (value * 100 < 100.0f)
                                                                    return juce::String (value * 100, 1);
                                                                else
                                                                    return juce::String (value * 100, 0); },
                                                            nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("Dry/Wet",
                                                             "Dry/Wet",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                if (value * 100 < 10.0f)
                                                                    return juce::String (value * 100, 2);
                                                                else if (value * 100 < 100.0f)
                                                                    return juce::String (value * 100, 1);
                                                                else
                                                                    return juce::String (value * 100, 0); },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterBool> ("Freeze", "Freeze", false));
    
    return layout;
}
