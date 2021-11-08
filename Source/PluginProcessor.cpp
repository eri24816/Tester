/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TesterAudioProcessor::TesterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TesterAudioProcessor::~TesterAudioProcessor()
{
}

//==============================================================================
const juce::String TesterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TesterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TesterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TesterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TesterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TesterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TesterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TesterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TesterAudioProcessor::getProgramName (int index)
{
    return {};
}

void TesterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TesterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TesterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TesterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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



void TesterAudioProcessor::AddInpulse() {
    addInpulse = true;
}

void TesterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //editor = dynamic_cast<NewProjectAudioProcessorEditor*>( getActiveEditor());
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = 0; i < buffer.getNumSamples(); ++i)
        bufferForEditor.enqueue((*(buffer.getReadPointer(0, i)) + *(buffer.getReadPointer(1, i))) / 2);
/*
    for (auto i = 0; i < totalNumOutputChannels; ++i)
        for (auto j = 0; j < buffer.getNumSamples(); ++j)
            (*buffer.getWritePointer(i,j)) = 0;
    
    if (addInpulse) {
        (*buffer.getWritePointer(0, 0))= 1;
        (*buffer.getWritePointer(1, 0)) = 1;
        addInpulse = false;
    }*/


    

}

//==============================================================================
bool TesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TesterAudioProcessor::createEditor()
{
    return new TesterAudioProcessorEditor(*this);;
}

//==============================================================================
void TesterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TesterAudioProcessor();
}
