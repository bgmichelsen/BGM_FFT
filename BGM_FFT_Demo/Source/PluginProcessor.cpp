/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <vector>

//==============================================================================
BGM_FFT_DemoAudioProcessor::BGM_FFT_DemoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
}

BGM_FFT_DemoAudioProcessor::~BGM_FFT_DemoAudioProcessor()
{
}

//==============================================================================
const juce::String BGM_FFT_DemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BGM_FFT_DemoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BGM_FFT_DemoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BGM_FFT_DemoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BGM_FFT_DemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BGM_FFT_DemoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BGM_FFT_DemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BGM_FFT_DemoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BGM_FFT_DemoAudioProcessor::getProgramName (int index)
{
    return {};
}

void BGM_FFT_DemoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BGM_FFT_DemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fifo.clear();
}

void BGM_FFT_DemoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BGM_FFT_DemoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BGM_FFT_DemoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* channelData = buffer.getReadPointer(0);

    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        if (fifo.size() >= FFT_SIZE && !fifo.empty())
            fifo.clear();
        fifo.push_back(channelData[sample]);
    }
}

//==============================================================================
bool BGM_FFT_DemoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BGM_FFT_DemoAudioProcessor::createEditor()
{
    return new BGM_FFT_DemoAudioProcessorEditor (*this);
}

//==============================================================================
void BGM_FFT_DemoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BGM_FFT_DemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BGM_FFT_DemoAudioProcessor();
}

std::vector<float>& BGM_FFT_DemoAudioProcessor::getFifo()
{
    return fifo;
}
