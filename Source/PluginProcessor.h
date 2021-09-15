#pragma once

#include <random>
#include <juce_audio_processors/juce_audio_processors.h>

class LoadMonsterProcessor : public juce::AudioProcessor
{
public:
    LoadMonsterProcessor();
    ~LoadMonsterProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void startProfile();
    float cpuLoadProportion;
    juce::AudioParameterInt* multipliesPerSample;
    size_t lastMultiplies = 0;
    std::atomic<bool> automate = false;
    std::vector<float> results;
    size_t maxBlockSize;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadMonsterProcessor)
#if DEBUG
    static const size_t maxNumberOfMultiplies = 1000;
    static const size_t increment = 50;
#else
    static const size_t maxNumberOfMultiplies = 100000;
    static const size_t increment = 200;
#endif
    static const size_t buffersPerIncrement = 10;
    size_t currentBuffer = 0;
    juce::AudioProcessLoadMeasurer measurer;
    std::minstd_rand gen { std::random_device{}() };
    std::uniform_real_distribution<float> dist { 0, 1 };

};
