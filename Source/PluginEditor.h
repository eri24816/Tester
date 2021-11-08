/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Visualizer.h"

//==============================================================================
/**
*/
class TesterAudioProcessorEditor  : public juce::AudioProcessorEditor 
{
public:
    TesterAudioProcessorEditor (TesterAudioProcessor&);
    ~TesterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TesterAudioProcessor& audioProcessor;
    Visualizer visualizer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TesterAudioProcessorEditor)
};
