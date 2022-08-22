/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DeepcoreAudioProcessorEditor  :
    public AudioProcessorEditor,
    public Slider::Listener
{
public:
    DeepcoreAudioProcessorEditor (DeepcoreAudioProcessor&);
    ~DeepcoreAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider *slider) override;

private:
    void chooseModel();
    DeepcoreAudioProcessor& processor;
    Slider outputGainSlider;
    Label outputGainLabel;
    TextButton modelButton;
    Label pathToModelLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeepcoreAudioProcessorEditor)
};
