#pragma once

#include <JuceHeader.h>

class NameLabel  : public juce::Label
{
public:
    NameLabel();

    void resized() override;
};
