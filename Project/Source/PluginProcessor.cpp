/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <torch/script.h>
#include <torch/torch.h>

//==============================================================================
DeepcoreAudioProcessor::DeepcoreAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{   
    setModule("/Users/wayne391/Documents/Projects/audio_effect/deepcore/torch/bypass.pt");
}

DeepcoreAudioProcessor::~DeepcoreAudioProcessor()
{
}

//==============================================================================
const String DeepcoreAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DeepcoreAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DeepcoreAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DeepcoreAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DeepcoreAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DeepcoreAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DeepcoreAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DeepcoreAudioProcessor::setCurrentProgram (int index)
{
}

const String DeepcoreAudioProcessor::getProgramName (int index)
{
    return {};
}

void DeepcoreAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DeepcoreAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DeepcoreAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DeepcoreAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void DeepcoreAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    // count necessary info
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // clear redundant channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    // input: copy buffer to tensor
    torch::Tensor tensor_in = torch::zeros({totalNumInputChannels, numSamples});
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            tensor_in[channel][sample] = buffer.getSample(channel, sample);
        }
    }

    // run model
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(tensor_in);
    torch::Tensor tensor_out = module.forward(inputs).toTensor(); 

    // output: copy tensor to buffer 
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = tensor_out[channel][sample].item().to<float>() * outputGain;
        }
    }


}

//==============================================================================
bool DeepcoreAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DeepcoreAudioProcessor::createEditor()
{
    return new DeepcoreAudioProcessorEditor (*this);
}

//==============================================================================
void DeepcoreAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DeepcoreAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..

void DeepcoreAudioProcessor::setModule(std::string pathToModel)
{
    module = torch::jit::load(pathToModel);
}
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DeepcoreAudioProcessor();
}
