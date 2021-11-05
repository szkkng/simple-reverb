/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleReverbAudioProcessor::SimpleReverbAudioProcessor()
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

SimpleReverbAudioProcessor::~SimpleReverbAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;

    leftReverb.prepare(spec);
    rightReverb.prepare(spec);
}

void SimpleReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.roomSize   = *apvts.getRawParameterValue ("size");
    params.damping    = *apvts.getRawParameterValue ("damp");
    params.width      = *apvts.getRawParameterValue ("width");
    params.wetLevel   = *apvts.getRawParameterValue ("dry/wet");
    params.dryLevel   = 1.0f - *apvts.getRawParameterValue ("dry/wet");
    params.freezeMode = *apvts.getRawParameterValue ("freeze");

    leftReverb.setParameters  (params);
    rightReverb.setParameters (params);

    juce::dsp::AudioBlock<float> block (buffer);

    auto leftBlock  = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);

    juce::dsp::ProcessContextReplacing<float> leftContext  (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);

    leftReverb.process  (leftContext);
    rightReverb.process (rightContext);
}

//==============================================================================
bool SimpleReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleReverbAudioProcessor::createEditor()
{
    return new SimpleReverbAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void SimpleReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, sizeInBytes);

    if (tree.isValid())
        apvts.replaceState (tree);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleReverbAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> ("size",
                                                             "size",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                value *= 100;
                                                                if (value < 10.0f)
                                                                    return juce::String (value, 2) + " %";
                                                                else if (value < 100.0f)
                                                                    return juce::String (value, 1) + " %";
                                                                else
                                                                    return juce::String (value, 0) + " %"; },
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("damp",
                                                             "damp",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                value *= 100;
                                                                if (value < 10.0f)
                                                                    return juce::String (value, 2) + " %";
                                                                else if (value < 100.0f)
                                                                    return juce::String (value, 1) + " %";
                                                                else
                                                                    return juce::String (value, 0) + " %"; },
                                                             nullptr));


    layout.add (std::make_unique<juce::AudioParameterFloat> ("width",
                                                             "width",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                value *= 100;
                                                                if (value < 10.0f)
                                                                    return juce::String (value, 2) + " %";
                                                                else if (value < 100.0f)
                                                                    return juce::String (value, 1) + " %";
                                                                else
                                                                    return juce::String (value, 0) + " %"; },
                                                            nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> ("dry/wet",
                                                             "dry/wet",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                value *= 100;
                                                                if (value < 10.0f)
                                                                    return juce::String (value, 2) + " %";
                                                                else if (value < 100.0f)
                                                                    return juce::String (value, 1) + " %";
                                                                else
                                                                    return juce::String (value, 0) + " %"; },
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterBool> ("freeze", "freeze", false));

    return layout;
}
