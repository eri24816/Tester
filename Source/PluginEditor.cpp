/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
TesterAudioProcessorEditor::TesterAudioProcessorEditor (TesterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), visualizer(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    addAndMakeVisible(visualizer);
    visualizer.inputSignal = &p.bufferForEditor;
}

TesterAudioProcessorEditor::~TesterAudioProcessorEditor()
{

}

//==============================================================================


void TesterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
}

void TesterAudioProcessorEditor::resized()
{
    visualizer.setBounds(getBounds());
}



