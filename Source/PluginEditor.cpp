#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
LoadMonsterEditor::LoadMonsterEditor (LoadMonsterProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    int size = 0;
    auto data = BinaryData::getNamedResource ("logo_png", size);
    jassert (size > 0);
    logo = juce::ImageCache::getFromMemory (data, size);

    addAndMakeVisible (automateButton);
    automateButton.setLookAndFeel (&bigButtonLNF);
    automateButton.setButtonText ("EAT BRAINS!");
    automateButton.setColour (automateButton.buttonColourId, Colors::brainsButton);
    automateButton.setColour (juce::ComboBox::outlineColourId, Colors::brainsButton); //yikes
    automateButton.setColour (automateButton.textColourOnId, Colors::buttonText);
    automateButton.onClick = [this]() { startProfile(); };

    addAndMakeVisible (numberOfMultiplies);
    addAndMakeVisible (numberOfMultipliesLabel);
    numberOfMultipliesLabel.setText ("Multiplies per sample", juce::dontSendNotification);

    addAndMakeVisible (meterLabel);
    meterLabel.setText ("% CPU LOAD", juce::dontSendNotification);
    addAndMakeVisible (meter);
    addAndMakeVisible (graph);

    setSize (450, 500);
}

//==============================================================================
void LoadMonsterEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colors::pluginBackground);

    // logo is 618x618 natively
    g.drawImageTransformed (logo, juce::AffineTransform::scale (0.25).translated ((float) getWidth() * 0.5f - (float) logo.getWidth() * .25f * 0.5f, 10));

    auto configLabelBounds = juce::Rectangle<int> (30, 30, 70, 25);
    auto blockSizeLabelBounds = juce::Rectangle<int> (getWidth() - 30 - 120, 30, 120, 25);

    g.setColour (Colors::configLabelBackground);
    g.fillRoundedRectangle (configLabelBounds.toFloat(), 4.0f);
    g.fillRoundedRectangle (blockSizeLabelBounds.toFloat(), 4.0f);

    g.setColour (Colors::buttonText);
    g.drawText (CMAKE_BUILD_TYPE, configLabelBounds, juce::Justification::centred);
    g.drawText (juce::String (processorRef.maxBlockSize) + " samples", blockSizeLabelBounds, juce::Justification::centred);
}

void LoadMonsterEditor::resized()
{
    auto area = getLocalBounds();
    area.reduce (40, 30);

    auto logoHeight = (int) ((float) logo.getHeight() * 0.25f);
    area.removeFromTop (logoHeight - 44);
    automateButton.setBounds (area.removeFromTop (35).reduced (100, 0));

    area.removeFromTop (30);
    numberOfMultipliesLabel.setBounds (area.removeFromTop (20));
    numberOfMultiplies.setBounds (area.removeFromTop (30));

    area.removeFromTop (20);
    meterLabel.setBounds (area.removeFromTop (30));
    meter.setBounds (area.removeFromTop (30));

    area.removeFromTop (20);
    graph.setBounds (area);
}

void LoadMonsterEditor::startProfile()
{
    processorRef.startProfile();
    startTimerHz (60);
}

void LoadMonsterEditor::timerCallback()
{
    graph.repaint();
    if (processorRef.automate == false)
    {
        automateButton.setEnabled (true);
        stopTimer();
    }
}