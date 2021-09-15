#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LoadMonsterProcessor::LoadMonsterProcessor()
    : AudioProcessor (BusesProperties()
#if !JucePlugin_IsMidiEffect
    #if !JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
    #endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
    addParameter (multipliesPerSample = new juce::AudioParameterInt ("multipliesPerBlock",
                      "Multiplies Per Block",
                      0,
                      maxNumberOfMultiplies,
                      50));
}

LoadMonsterProcessor::~LoadMonsterProcessor()
{
}

//==============================================================================
const juce::String LoadMonsterProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LoadMonsterProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool LoadMonsterProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool LoadMonsterProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double LoadMonsterProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LoadMonsterProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int LoadMonsterProcessor::getCurrentProgram()
{
    return 0;
}

void LoadMonsterProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String LoadMonsterProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void LoadMonsterProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void LoadMonsterProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    measurer.reset (sampleRate, samplesPerBlock);
    maxBlockSize = (size_t) samplesPerBlock;
}

void LoadMonsterProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool LoadMonsterProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
    #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    #endif

    return true;
#endif
}

void LoadMonsterProcessor::processBlock (juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    auto multiplies = (size_t) multipliesPerSample->get();

    {
        // Only measure time inside this scope
        juce::AudioProcessLoadMeasurer::ScopedTimer s (measurer);

        volatile float sum = 0.0f;
        for (size_t i = 0; i < multiplies * (size_t) buffer.getNumSamples(); ++i)
        {
            sum = sum * dist (gen);
        }
    }

    cpuLoadProportion = (float) measurer.getLoadAsProportion();

    if (automate)
    {
        //DBG("multipliesPerSample: " << multiplies << " currentBuffer: " << currentBuffer << " cpuLoadProportion: " << cpuLoadProportion);
        currentBuffer++;
        lastMultiplies = multiplies;
        results.push_back (cpuLoadProportion);

        if (((size_t) multiplies + increment > maxNumberOfMultiplies) || (cpuLoadProportion >= 1.0))
        {
            automate = false;
            multipliesPerSample->setValueNotifyingHost (0);
        }
        else if (currentBuffer >= buffersPerIncrement)
        {
            multipliesPerSample->setValueNotifyingHost (multipliesPerSample->convertTo0to1 ((float) (multiplies + (int) increment)));
            currentBuffer = 0;
        }
    }
}

void LoadMonsterProcessor::startProfile()
{
    automate = true;
    multipliesPerSample->setValueNotifyingHost (0);
    lastMultiplies = 0;
    currentBuffer = 0;
    results.clear();
    results.reserve (maxNumberOfMultiplies / increment * buffersPerIncrement);
}
//==============================================================================
bool LoadMonsterProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LoadMonsterProcessor::createEditor()
{
    return new LoadMonsterEditor (*this);
}

//==============================================================================
void LoadMonsterProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void LoadMonsterProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LoadMonsterProcessor();
}
