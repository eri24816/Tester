#pragma once
#include <JuceHeader.h>
#include "PoleZeroPlot.h"
#include "PluginProcessor.h"
#include "Spectrum.h"
using namespace juce;

class Visualizer :public Component, Timer {
private:
    float activeThres = 1e-2;
public:
    TesterAudioProcessor &processor;
	PoleZeroPlot<65> poleZeroPlot;
    Spectrum<200> spectrum;
    int signalTime = -1;
    juce::TextButton resetButton;
    SafeQueue<float> *inputSignal;

    Visualizer(TesterAudioProcessor &processor) :processor(processor),poleZeroPlot(4),resetButton("reset") {
        addAndMakeVisible(poleZeroPlot);
        addAndMakeVisible(resetButton);
        addAndMakeVisible(spectrum);
        resetButton.onClick = [this, &processor]() {this->poleZeroPlot.Zero();  this->signalTime = -1; processor.AddInpulse(); };
        startTimerHz(1);
        signalTime = -1;
    }

    int L = 1000;

    float Hann(float x) {
        //return 0.5 * (1 - cos(6.283 * x / (L-1)));
        return 1;
    }

    void timerCallback() {
        while (inputSignal->size() > 0) {
            
            float signal = inputSignal->dequeue();
            if (signalTime == -1 && abs(signal) >= activeThres) { signalTime = 0; poleZeroPlot.Zero(); spectrum.Zero();}
            if (signalTime >= L) {
                signalTime = -1; 
                poleZeroPlot.repaint();
                spectrum.repaint();
            }
            if (signalTime >= 0) {
                //poleZeroPlot.AddSample(sin(signalTime)*pow(0.5f,(float)signalTime));
                if(signalTime<100)
                    poleZeroPlot.AddSample(signal*Hann(signalTime));
                spectrum.AddSample(signal);
                signalTime++;
            }
            if(signalTime == 100)poleZeroPlot.repaint();
        }
    }
    void resized () override{
        poleZeroPlot.setBounds(getBounds().getProportion(juce::Rectangle<float>(0, 0, 0.3, 0.8)).reduced(10));
        spectrum.setBounds(getBounds().getProportion(juce::Rectangle<float>(0.3, 0, 0.7, 0.8)).reduced(10));
        resetButton.setBounds(getBounds().getProportion(juce::Rectangle<float>(0, 0.8, 1, 0.2)).reduced(10));
    }

};