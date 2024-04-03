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

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class FreezeButton final : public juce::Component
{
public:
    enum ColourIds
    {
        onColourId,
        offColourId,
        focusColourId
    };

    explicit FreezeButton (juce::RangedAudioParameter& param, juce::UndoManager* um = nullptr);

    void paint (juce::Graphics& g) override;
    void resized() override;

    void mouseDown (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    void focusGained (FocusChangeType cause) override;
    void focusLost (FocusChangeType cause) override;

    bool keyPressed (const juce::KeyPress& key) override;

private:
    void updateState (bool newState);

    bool state { false };

    juce::Path iconPath;
    juce::Rectangle<float> iconBounds;

    juce::RangedAudioParameter& audioParam;
    juce::ParameterAttachment paramAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreezeButton)
};
