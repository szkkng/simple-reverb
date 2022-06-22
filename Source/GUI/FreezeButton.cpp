#include "FreezeButton.h"
#include "../../Resources/FreezeIcon.h"

FreezeButton::FreezeButton()
{
    setOpaque (true);
    setClickingTogglesState (true);
    onClick = [&]()
    {
        freezeColour = getToggleState() ? MyColours::blue : MyColours::grey;
    };
    freezeIconPath.loadPathFromData (FreezeIcon::pathData, sizeof (FreezeIcon::pathData));
}

void FreezeButton::resized()
{
    freezeIconBounds = juce::Rectangle<float> (getWidth() * 0.215f, getHeight() * 0.06f, getWidth() * 0.56f, getHeight() * 0.9f);
    freezeIconPath.applyTransform (freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true));
}

void FreezeButton::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);

    g.setColour (freezeColour);
    g.fillPath (freezeIconPath);
}

void FreezeButton::mouseDown (const juce::MouseEvent& event)
{
    juce::ToggleButton::mouseDown (event);

    auto transform = freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true)
                                   .scale (0.95f, 0.95f, freezeIconBounds.getCentreX(), freezeIconBounds.getCentreY());
    freezeIconPath.applyTransform (transform);
}

void FreezeButton::mouseUp (const juce::MouseEvent& event)
{
    juce::ToggleButton::mouseUp (event);

    auto transform = freezeIconPath.getTransformToScaleToFit (freezeIconBounds, true);
    freezeIconPath.applyTransform (transform);
}
