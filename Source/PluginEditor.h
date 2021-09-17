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
    MelatoninInspector inspector { *this, false };
    BigButton bigButtonLNF;

    MeterComponent meter { processorRef.cpuLoadProportion };
    GraphComponent<float> cpuGraph { { "multiplies per sample", "% CPU", processorRef.lastMultiplies, 1.0f, processorRef.results, Colors::graphValue } };
    GraphComponent<size_t> blockSizeGraph { { "block sizes", "", processorRef.blockSizes.size(), processorRef.maxBlockSize, processorRef.blockSizes, Colors::blockGraphValue } };

    juce::Label multipliesPerBlockLabel;
    juce::Slider multipliesPerBlock;
    juce::SliderParameterAttachment multipliesPerBlockAttachment { *processorRef.multipliesPerBlock, multipliesPerBlock };

    juce::Label multipliesPerSampleLabel;
    juce::Slider multipliesPerSample;
    juce::SliderParameterAttachment multipliesPerSampleAttachment { *processorRef.multipliesPerSample, multipliesPerSample };

    juce::Image logo;
    juce::Label title;
    juce::TextButton automateButton;
    juce::Label meterLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadMonsterEditor)
};
