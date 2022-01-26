#include "DialTextBox.h"

juce::String DialTextBox::valueShownWithEditor = "";

DialTextBox::DialTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (false, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
}

void DialTextBox::resized()
{
    juce::Label::resized();
    setFont (getWidth() * 0.2f);
}

juce::TextEditor* DialTextBox::createEditorComponent()
{
    auto* ed = juce::Label::createEditorComponent();

    ed->setJustification (juce::Justification::centred);
    ed->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);
    ed->setInputRestrictions (5, "0123456789.");
    ed->setIndents (4, 0);

    return ed;
}

void DialTextBox::editorShown (juce::TextEditor* ed)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NoCursor);
    ed->clear();
    ed->setText (valueShownWithEditor);
}

void DialTextBox::editorAboutToBeHidden (juce::TextEditor* /*ed*/)
{
    getParentComponent()->setMouseCursor (juce::MouseCursor::NormalCursor);
}
