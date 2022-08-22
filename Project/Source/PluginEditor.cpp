/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DeepcoreAudioProcessorEditor::DeepcoreAudioProcessorEditor (DeepcoreAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // output gain
    outputGainSlider.setSliderStyle(Slider::Rotary);
    outputGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    outputGainSlider.setRange(0.0, 10.0);
    outputGainSlider.setValue(1.0);
    outputGainSlider.addListener(this);
    addAndMakeVisible(outputGainSlider);

    addAndMakeVisible (outputGainLabel);
    outputGainLabel.setText ("Output Gain", dontSendNotification);
    outputGainLabel.setJustificationType (Justification::centred);
    
    // path to model label
    addAndMakeVisible (pathToModelLabel);
    pathToModelLabel.setText ("", dontSendNotification);
    pathToModelLabel.setJustificationType (Justification::centred);
    pathToModelLabel.setColour (Label::backgroundColourId, Colours::lightslategrey);

    // button 
    modelButton.setButtonText ("Choose Model");
    modelButton.onClick = [this]() {chooseModel();};
    addAndMakeVisible (modelButton);

    // final layout
    setSize (300, 300);
}

DeepcoreAudioProcessorEditor::~DeepcoreAudioProcessorEditor()
{
}

//==============================================================================
void DeepcoreAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void DeepcoreAudioProcessorEditor::resized()
{
    // int x, int y, int width, int height
    auto area = getLocalBounds();
    auto border = 4;

    auto buttonHeight = 30;
    auto LabelHeight = 80;
    modelButton.setBounds (area.removeFromTop (buttonHeight).reduced (border));

     // pathToModel Label
    pathToModelLabel.setBounds (area.removeFromTop (LabelHeight).reduced (border));

    // label
    outputGainLabel.setBounds (area.removeFromTop (30));

    // slider
    outputGainSlider.setBounds (area.removeFromTop (150));

}

void DeepcoreAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if(slider == &outputGainSlider)
    {
        processor.outputGain = outputGainSlider.getValue();
    }
}

void DeepcoreAudioProcessorEditor::chooseModel()
{
    FileChooser chooser ("Select a Wave file to play...",
                             {},
                             "*.pt");  
    if (chooser.browseForFileToOpen())                               
    {
        auto file = chooser.getResult();            
        const juce::String pathToModel = file.getFullPathName();

        pathToModelLabel.setText (pathToModel, dontSendNotification);
        processor.setModule(pathToModel.toStdString());
    }
}

