#pragma once

#include <JuceHeader.h>

class DialTextBox : public juce::Label
{
public:
    static juce::String valueShownWithEditor;

    DialTextBox();

    void resized() override;

    juce::TextEditor* createEditorComponent() override;

    void editorShown (juce::TextEditor* editor) override;

    void editorAboutToBeHidden (juce::TextEditor * ed) override;
};
