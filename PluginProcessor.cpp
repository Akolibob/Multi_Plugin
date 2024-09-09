/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

    Pre-added classes and functions from JUCE are not explained with comments. Only the code written is explained.
    Comments explaining the functions are not repeated for similiar sections but a comment is added to know which part the function affects.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiPluginAudioProcessor::MultiPluginAudioProcessor()
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

MultiPluginAudioProcessor::~MultiPluginAudioProcessor()
{
}

//==============================================================================
const juce::String MultiPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultiPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultiPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultiPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultiPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultiPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultiPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultiPluginAudioProcessor::setCurrentProgram (int index)
{

}

const juce::String MultiPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultiPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultiPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
     //Setting expected parameters for the processor (https://docs.juce.com/master/structdsp_1_1ProcessSpec.html)
    //Also used this tuti
    multiPluginSampleRate = sampleRate; //Samplerate sent to a variable in order to get saved to spec

    juce::dsp::ProcessSpec spec; //Creates a struct were crucial information for dsp processing are saved
    spec.sampleRate = multiPluginSampleRate; //Sample Rate
    spec.maximumBlockSize = samplesPerBlock; //Bit Rate
    spec.numChannels = getNumOutputChannels(); //Number of output channels

    //Preparing the DSP processes
    filter.prepare(spec); //Filter
    compressor.prepare(spec); //Compressor
    gain.prepare(spec); //Gain
    reset(); //Calls the function reset created
}

void MultiPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MultiPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

  
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    //The following link was used to create the DSP chain and apply the processes to both channels
    //https://docs.juce.com/master/tutorial_dsp_introduction.html
    //The loop was not used as it caused distortion of the signal for uknown reasons

    auto audioBlock = juce::dsp::AudioBlock<float>(buffer); //Creates an audioblock that points to the buffer
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock); //Processes the audioblock and replaces it (https://docs.juce.com/master/structdsp_1_1ProcessContextReplacing.html)
 
    
    switch (pluginType)
    {
    case 1: //Filter
        MultiPluginAudioProcessor::filterSetType(); //Sets the type
        filter.setCutoffFrequency(filterFrequency); //Sets the value of the frequency
        filter.setResonance(filterResonance); //Sets the value of the resonance

        filter.process(context); //Initialazes the process of the filter
        break;
    case 2: //Compressor
        compressor.setAttack(compressorAttack); //Sets the value of the attack
        compressor.setRatio(compressorRatio); //Sets the value of the ratio
        compressor.setRelease(compressorRelease); //Sets the value of the release
        compressor.setThreshold(compressorThreshold); //Sets the value of the threshold
        gain.setGainDecibels(gainGain); //Sets the value of the gain

        compressor.process(context); //Initialazes the process of the compressor
        gain.process(context); //Initialazes the process of the gain
        break;
    default: // Default is the default state of the switch case. If none of the above apply this is the state that the switch case is going to be in. In that case its the same as case 1 which is the filter.
        MultiPluginAudioProcessor::filterSetType();
        filter.setCutoffFrequency(filterFrequency);
        filter.setResonance(filterResonance);

        filter.process(context); //Initialazes the process of the filter
        break;
    }
    
}

//==============================================================================
bool MultiPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiPluginAudioProcessor::createEditor()
{
    return new MultiPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MultiPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultiPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void MultiPluginAudioProcessor::reset() //Function to reset the properties of the plugin
{
    filter.reset(); //Filter
    compressor.reset(); //Compressor
    gain.reset(); //Gain
}

void MultiPluginAudioProcessor::filterSetType() //Switch case for selecting the filter type
{
    switch (filterType) //Switch was used instead of if as it looks nicer and it was autocompleted which helped eliminating misstyping in the process
    {
    case 1: //Low Pass
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); //The function sets the type of the filter
        break;
    case 2: //Band Pass
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass); 
        break;
    case 3: //High Pass
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    default: //Low Pass
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiPluginAudioProcessor();
}
