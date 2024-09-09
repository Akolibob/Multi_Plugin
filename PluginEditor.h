/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Pre-added classes and functions from JUCE are not explained with comments. Only the code written is explained.
    Comments explaining the functions are not repeated for similiar sections but a comment is added to know which part the function affects.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MultiPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Slider::Listener, //Inherited class Slider::Listener
                                         public juce::ComboBox::Listener //Inherited class ComboBox::Listener
{
public:
    MultiPluginAudioProcessorEditor (MultiPluginAudioProcessor&);
    ~MultiPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override; //Overriding slider listener function from the class Slider::Listener
    void sliderDragEnded(juce::Slider* slider) override; //Overriding slider listener function from the class Slider::Listener
    void comboBoxChanged(juce::ComboBox* combobox) override; //Overriding combobox listener function from the class ComboBox::Listener
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MultiPluginAudioProcessor& audioProcessor;

    //Comboboxes
    juce::ComboBox filterTypeMenu; //Filter Menu
    juce::ComboBox pluginTypeMenu; //Plugin Menu
    //Filter
    juce::Slider filterFrequencySlider; //Frequency
    juce::Slider filterResonanceSlider; //Resonance
    //Compressor
    juce::Slider compressorAttackSlider; //Attack
    juce::Slider compressorRatioSlider; //Ratio
    juce::Slider compressorReleaseSlider; // Release
    juce::Slider compressorThresholdSlider; //Threshold
    //Gain
    juce::Slider gainGainSlider; //Gain
    //Labels
    juce::Label filterFrequencyLabel; //Filter Frequency
    juce::Label filterResonanceLabel; //Filter Resonance
    juce::Label compressorAttackLabel; //Compressor Attack
    juce::Label compressorRatioLabel; //Compressor Ratio
    juce::Label compressorReleaseLabel; //Compressor Release
    juce::Label compressorThresholdLabel; //Compressor Threshold
    juce::Label gainGainLabel; //Gain
    //Effects
    juce::GlowEffect glowEffect; //Glow

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiPluginAudioProcessorEditor)
};
