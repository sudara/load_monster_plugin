#pragma once

#include "BinaryData.h"
#include "Colors.h"
#include "GraphComponent.h"
#include "MeterComponent.h"
#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

class BigButton : public juce::LookAndFeel_V4
{
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight)
    {
        return { 20.0f, juce::Font::bold };
    }
};

class LoadMonsterEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit LoadMonsterEditor (LoadMonsterProcessor&);
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void startProfile();

private:
    LoadMonsterProcessor& processorRef;

    BigButton bigButtonLNF;

    MeterComponent meter { processorRef.cpuLoadProportion };
    GraphComponent graph { processorRef.results, processorRef.lastMultiplies };

    MelatoninInspector inspector { *this, false };

    juce::Slider numberOfMultiplies;
    juce::SliderParameterAttachment multipliesAttachment { *processorRef.multipliesPerSample, numberOfMultiplies };
    juce::Image logo;
    juce::Label title;
    juce::TextButton automateButton;
    juce::Label numberOfMultipliesLabel;
    juce::Label meterLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadMonsterEditor)
};
