/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

    Pre-added classes and functions from JUCE are not explained with comments. Only the code written is explained.
    Comments explaining the functions are not repeated for similiar sections but a comment is added to know which part the function affects.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MultiPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MultiPluginAudioProcessor();
    ~MultiPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //Plugin Type
    int pluginType = 1;
    //Filter
    float filterFrequency = 400.0; //Frequency
    float filterResonance = 1.0; //Resonance
    int filterType = 1; //Type
    //Compressor
    float compressorAttack = 0; //Attack
    float compressorRatio = 1; //Ratio
    float compressorRelease = 0; //Release
    float compressorThreshold = 0; //Threshold
    //Gain
    float gainGain = 0;

private:
    void reset() override; //Function for reseting the plugin processes
    void filterSetType(); //Function to reset the properties of the plugin

    juce::dsp::StateVariableTPTFilter<float> filter; //State Variable TPT Filter
    juce::dsp::Compressor<float> compressor; //Compressor
    juce::dsp::Gain<float> gain; //Gain

    float multiPluginSampleRate; //Creating a samplerate variable where the samplerate is going to be saved for the processing

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiPluginAudioProcessor)
};