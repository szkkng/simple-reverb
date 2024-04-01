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

#include "SimpleReverbEditor.h"
#include "SimpleReverbProcessor.h"

SimpleReverbAudioProcessorEditor::SimpleReverbAudioProcessorEditor (SimpleReverbAudioProcessor& p,
                                                                    juce::UndoManager& um)
    : AudioProcessorEditor (&p)
    , editorContent (p, um)
{
    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable (false, true);
    getConstrainer()->setFixedAspectRatio (ratio);
    getConstrainer()->setSizeLimits (defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth, defaultHeight);
    editorContent.setSize (defaultWidth, defaultHeight);

    addAndMakeVisible (editorContent);
}

SimpleReverbAudioProcessorEditor::~SimpleReverbAudioProcessorEditor() {}

void SimpleReverbAudioProcessorEditor::paint (juce::Graphics& g) { g.fillAll (MyColours::black); }

void SimpleReverbAudioProcessorEditor::resized()
{
    const auto factor = static_cast<float> (getWidth()) / defaultWidth;
    editorContent.setTransform (juce::AffineTransform::scale (factor));
}
