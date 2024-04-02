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

#include "SimpleReverbProcessor.h"
#include "ParamIDs.h"
#include "SimpleReverbEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto percentFormat = [] (float value, int /*maximumStringLength*/)
    {
        if (value < 10.0f)
            return juce::String { value, 2 } + " %";

        if (value < 100.0f)
            return juce::String { std::floor (value * 10.0f) / 10.0f, 1 } + " %";

        return juce::String { value, 0 } + " %";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::size, 1 },
                                                             ParamIDs::size,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String {},
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::damp, 1 },
                                                             ParamIDs::damp,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String {},
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::width, 1 },
                                                             ParamIDs::width,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String {},
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 },
                                                             ParamIDs::mix,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String {},
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::freeze, 1 }, ParamIDs::freeze, false));

    return layout;
}

SimpleReverbAudioProcessor::SimpleReverbAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , apvts (*this, &undoManager, "Parameters", createParameterLayout())
{
    auto castParameter = [&apvts = this->apvts]<typename T> (juce::StringRef paramID, T& destination)
    {
        destination = dynamic_cast<T> (apvts.getParameter (paramID));
        jassert (destination != nullptr);
    };

    castParameter (ParamIDs::size, size);
    castParameter (ParamIDs::damp, damp);
    castParameter (ParamIDs::width, width);
    castParameter (ParamIDs::mix, mix);
    castParameter (ParamIDs::freeze, freeze);
}

SimpleReverbAudioProcessor::~SimpleReverbAudioProcessor() {}

const juce::String SimpleReverbAudioProcessor::getName() const { return JucePlugin_Name; }

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

double SimpleReverbAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SimpleReverbAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleReverbAudioProcessor::getCurrentProgram() { return 0; }

void SimpleReverbAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }

const juce::String SimpleReverbAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void SimpleReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void SimpleReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    reverb.prepare (spec);
}

void SimpleReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimpleReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void SimpleReverbAudioProcessor::updateReverbParams()
{
    params.roomSize = size->get() * 0.01f;
    params.damping = damp->get() * 0.01f;
    params.width = width->get() * 0.01f;
    params.wetLevel = mix->get() * 0.01f;
    params.dryLevel = 1.0f - mix->get() * 0.01f;
    params.freezeMode = freeze->get();

    reverb.setParameters (params);
}

void SimpleReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    updateReverbParams();

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing ctx (block);
    reverb.process (ctx);
}

bool SimpleReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleReverbAudioProcessor::createEditor()
{
    return new SimpleReverbAudioProcessorEditor (*this, undoManager);
}

void SimpleReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void SimpleReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (const auto tree = juce::ValueTree::readFromData (data, static_cast<size_t> (sizeInBytes)); tree.isValid())
        apvts.replaceState (tree);
}

juce::AudioProcessorValueTreeState& SimpleReverbAudioProcessor::getPluginState() { return apvts; }

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SimpleReverbAudioProcessor(); }
