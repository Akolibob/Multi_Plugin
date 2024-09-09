/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Pre-added classes and functions from JUCE are not explained with comments. Only the code written is explained.
    Comments explaining the functions are not repeated for similiar sections but a comment is added to know which part the function affects

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiPluginAudioProcessorEditor::MultiPluginAudioProcessorEditor (MultiPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //===========================================================MENUS===============================================================\\

    //Plugin Type Menu
    pluginTypeMenu.setJustificationType(juce::Justification::centred); //Sets the position of the text
    pluginTypeMenu.addItem("Filter", 1); //Adds an option
    pluginTypeMenu.addItem("Compressor", 2); //Adds an option
    pluginTypeMenu.setSelectedId(1); //Sets the initial state of the menu
    //Plugin Type Menu Colours
    pluginTypeMenu.setColour(0x1000a00, juce::Colour(0xffff88ff)); //Text
    pluginTypeMenu.setColour(0x1000e00, juce::Colour(0xffff88ff)); //Arrow
    pluginTypeMenu.setColour(0x1000c00, juce::Colour(0xffff88ff)); //Outline
    
    //Filter Type Menu
    filterTypeMenu.setJustificationType(juce::Justification::centred);
    filterTypeMenu.addItem("Low Pass", 1);
    filterTypeMenu.addItem("Band Pass", 2);
    filterTypeMenu.addItem("High Pass", 3);
    filterTypeMenu.setSelectedId(1);
    //Filter Type Menu Colours (Set in an if statement inside the ComboboxChanged function)

    //==========================================================SLIDERS==============================================================\\

    //Frequency Slider
    filterFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary); //Sets the style of the slider to rotary
    filterFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20); //Sets the position and size of the textbox of the slider
    filterFrequencySlider.setRange(20.0f, 20000.0f, 1.0f); //Sets the range of values of the sliders as well as the increment which it changes
    filterFrequencySlider.setValue(audioProcessor.filterFrequency); //Sets the initial value to the value set from the audio processor
    filterFrequencySlider.setTextValueSuffix("Hz"); //Sets a suffix after the displayed value inside the textbox
    filterFrequencySlider.setSkewFactor(0.3, false); //Sets skew factor which makes the slider work in a logarithmic way (Applied only on this slider)
    //Frequency Slider Colours
    filterFrequencySlider.setColour(0x1001311, juce::Colour(0xff4B0082)); //Before Knob (rotarySliderFillColourId = 0x1001311)
    filterFrequencySlider.setColour(0x1001312, juce::Colour(0xdd111111)); //After Knob (rotarySliderOutlineColourId = 0x1001312)
    filterFrequencySlider.setColour(0x1001500, juce::Colour(0x23ffffff)); //Textbox Backround (textBoxBackgroundColourId = 0x1001500)
    filterFrequencySlider.setColour(0x1001300, juce::Colour(0xffdd00ff)); //Knob (thumbColourId = 0x1001300)
    filterFrequencySlider.setColour(0x1001400, juce::Colour(0xffff88ff)); //Text (textBoxTextColourId = 0x1001400)
    filterFrequencySlider.setColour(0x1001600, juce::Colour(0x9a8414ff)); //Text Highlight (textBoxHighlightColourId = 0x1001600)
    filterFrequencySlider.setColour(0x1001700, juce::Colour(0x9fdd00ff)); //Textbox Border (textBoxOutlineColourId = 0x1001700)

    //Resonance Slider
    filterResonanceSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    filterResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    filterResonanceSlider.setRange(1.0f, 10.0f, 0.1f);
    filterResonanceSlider.setValue(audioProcessor.filterResonance);
    //Resonance Slider Colours
    filterResonanceSlider.setColour(0x1001311, juce::Colour(0x8fff3252));
    filterResonanceSlider.setColour(0x1001312, juce::Colour(0xdd111111));
    filterResonanceSlider.setColour(0x1001500, juce::Colour(0x23ffffff));
    filterResonanceSlider.setColour(0x1001300, juce::Colour(0xffdd00ff));
    filterResonanceSlider.setColour(0x1001400, juce::Colour(0xffff88ff));
    filterResonanceSlider.setColour(0x1001600, juce::Colour(0x9a8414ff));
    filterResonanceSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff));

    //Compressor Attack Slider
    compressorAttackSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    compressorAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    compressorAttackSlider.setRange(0.01f, 300.0f, 0.0001f);
    compressorAttackSlider.setValue(audioProcessor.compressorAttack);
    compressorAttackSlider.setTextValueSuffix("ms");
    //Compressor Attack Slider Colours
    compressorAttackSlider.setColour(0x1001311, juce::Colour(0x8f87cefa));
    compressorAttackSlider.setColour(0x1001312, juce::Colour(0xdd111111));
    compressorAttackSlider.setColour(0x1001500, juce::Colour(0x23ffffff));
    compressorAttackSlider.setColour(0x1001300, juce::Colour(0xffdd00ff));
    compressorAttackSlider.setColour(0x1001400, juce::Colour(0xffff88ff));
    compressorAttackSlider.setColour(0x1001600, juce::Colour(0x9a8414ff));
    compressorAttackSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff));

    //Compressor Ratio Slider
    compressorRatioSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    compressorRatioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    compressorRatioSlider.setRange(1.0f, 10.0f, 1.0f);
    compressorRatioSlider.setValue(audioProcessor.compressorRatio);
    //Compressor Ratio Slider Colours
    compressorRatioSlider.setColour(0x1001311, juce::Colour(0x8f87cefa));
    compressorRatioSlider.setColour(0x1001312, juce::Colour(0xdd111111));
    compressorRatioSlider.setColour(0x1001500, juce::Colour(0x23ffffff));
    compressorRatioSlider.setColour(0x1001300, juce::Colour(0xffdd00ff));
    compressorRatioSlider.setColour(0x1001400, juce::Colour(0xffff88ff));
    compressorRatioSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff));

    //Compressor Release Slider
    compressorReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    compressorReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    compressorReleaseSlider.setRange(5.0f, 4000.0f, 0.1f);
    compressorReleaseSlider.setValue(audioProcessor.compressorRelease);
    compressorReleaseSlider.setTextValueSuffix("ms");
    //Compressor Release Slider Colours
    compressorReleaseSlider.setColour(0x1001311, juce::Colour(0x8f87cefa));
    compressorReleaseSlider.setColour(0x1001312, juce::Colour(0xdd111111));
    compressorReleaseSlider.setColour(0x1001500, juce::Colour(0x23ffffff));
    compressorReleaseSlider.setColour(0x1001300, juce::Colour(0xffdd00ff));
    compressorReleaseSlider.setColour(0x1001400, juce::Colour(0xffff88ff));
    compressorReleaseSlider.setColour(0x1001600, juce::Colour(0x9a8414ff));
    compressorReleaseSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff));

    //Compressor Threshold Slider
    compressorThresholdSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    compressorThresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    compressorThresholdSlider.setRange(-30.0f, 0.0f, 1.0f);
    compressorThresholdSlider.setValue(audioProcessor.compressorThreshold);
    compressorThresholdSlider.setTextValueSuffix("dB");
    //Compressor Threshold Slider Colours
    compressorThresholdSlider.setColour(0x1001311, juce::Colour(0x8f87cefa));
    compressorThresholdSlider.setColour(0x1001312, juce::Colour(0xdd111111));
    compressorThresholdSlider.setColour(0x1001500, juce::Colour(0x23ffffff));
    compressorThresholdSlider.setColour(0x1001300, juce::Colour(0xffdd00ff));
    compressorThresholdSlider.setColour(0x1001400, juce::Colour(0xffff88ff));
    compressorThresholdSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff));

    //Gain in dB Slider
    gainGainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical); //Sets the stule of the slider to a vertical
    gainGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    gainGainSlider.setRange(0.0f, 20.0f, 1.1f);
    gainGainSlider.setValue(audioProcessor.compressorRelease);
    gainGainSlider.setTextValueSuffix("dB");
    //Gain in dB Slider Colours
    gainGainSlider.setColour(0x1001200, juce::Colour(0xdd111111)); //Background (rotarySliderFillColourId = 0x1001200)
    gainGainSlider.setColour(0x1001310, juce::Colour(0x8f87cefa)); //Track (rotarySliderOutlineColourId = 0x1001312)
    gainGainSlider.setColour(0x1001500, juce::Colour(0x23ffffff)); //Textbox Backround (textBoxBackgroundColourId = 0x1001500)
    gainGainSlider.setColour(0x1001300, juce::Colour(0xffdd00ff)); //Knob (thumbColourId = 0x1001300)
    gainGainSlider.setColour(0x1001400, juce::Colour(0xffff88ff)); //Text (textBoxTextColourId = 0x1001400)
    gainGainSlider.setColour(0x1001600, juce::Colour(0x9a8414ff)); //Text Highlight (textBoxHighlightColourId = 0x1001600)
    gainGainSlider.setColour(0x1001700, juce::Colour(0x9fdd00ff)); //Textbox Border (textBoxOutlineColourId = 0x1001700)

    //==========================================================LABELS==============================================================\\

    //Filter Frequency Label
    filterFrequencyLabel.setText("Frequency", juce::dontSendNotification); //Sets the text of the label
    filterFrequencyLabel.setJustificationType(juce::Justification::centredBottom); //Positions the text at the centre bottom of the (transparent) box
    filterFrequencyLabel.attachToComponent(&filterFrequencySlider, false); //Attaches the label to the top of the slider
    filterFrequencyLabel.setColour(0x1000281, juce::Colour(0xffff88ff)); //Sets the colour of the text (0x1000281)
    //Filter Resonance Label
    filterResonanceLabel.setText("Resonance", juce::dontSendNotification);
    filterResonanceLabel.setJustificationType(juce::Justification::centredBottom);
    filterResonanceLabel.attachToComponent(&filterResonanceSlider, false);
    filterResonanceLabel.setColour(0x1000281, juce::Colour(0xffff88ff));
    //Compressor Attack Label
    compressorAttackLabel.setText("Attack", juce::dontSendNotification);
    compressorAttackLabel.setJustificationType(juce::Justification::centredBottom);
    compressorAttackLabel.attachToComponent(&compressorAttackSlider, false);
    compressorAttackLabel.setColour(0x1000281, juce::Colour(0xffff88ff));
    //Compressor Ratio Label
    compressorRatioLabel.setText("Ratio", juce::dontSendNotification);
    compressorRatioLabel.setJustificationType(juce::Justification::centredBottom);
    compressorRatioLabel.attachToComponent(&compressorRatioSlider, false);
    compressorRatioLabel.setColour(0x1000281, juce::Colour(0xffff88ff));
    //Compressor Release Label
    compressorReleaseLabel.setText("Release", juce::dontSendNotification);
    compressorReleaseLabel.setJustificationType(juce::Justification::centredBottom);
    compressorReleaseLabel.attachToComponent(&compressorReleaseSlider, false);
    compressorReleaseLabel.setColour(0x1000281, juce::Colour(0xffff88ff));
    //Compressor Threshhold Label
    compressorThresholdLabel.setText("Threshold", juce::dontSendNotification);
    compressorThresholdLabel.setJustificationType(juce::Justification::centredBottom);
    compressorThresholdLabel.attachToComponent(&compressorThresholdSlider, false);
    compressorThresholdLabel.setColour(0x1000281, juce::Colour(0xffff88ff));
    //Gain Label
    gainGainLabel.setText("Gain", juce::dontSendNotification);
    gainGainLabel.setJustificationType(juce::Justification::centredTop);
    gainGainLabel.attachToComponent(&gainGainSlider, false);
    gainGainLabel.setColour(0x1000281, juce::Colour(0xffff88ff));

    //==========================================================LISTENERS==============================================================\\
    //(This section is dedicated to connecting the UI elements to the variables for the processing

    //Comboboxes
    pluginTypeMenu.addListener(this); //Plugin Type Menu
    filterTypeMenu.addListener(this); //Filter Type Menu
    //Filter Sliders
    filterFrequencySlider.addListener(this); //Frequency Slider
    filterResonanceSlider.addListener(this); //Resonance Slider
    //Compressor Sliders
    compressorAttackSlider.addListener(this); //Attack Slider
    compressorRatioSlider.addListener(this); //Ratio Slider
    compressorReleaseSlider.addListener(this); //Release Slider
    compressorThresholdSlider.addListener(this); //Threshold Slider
    //Gain
    gainGainSlider.addListener(this);

    //Making elements visible
    addAndMakeVisible(&pluginTypeMenu);

    //Effects (Sets the parameters for the glow effect. This effect is used on components to highlight them when in use)
    glowEffect.setGlowProperties(5, juce::Colour(0x1fff88ff));
    
    /*This section does not contain the addAndMakeVisible function for the sliders and other certain elements as these elements
    are defined in the if loop of pluginType combobox to make different sliders visible for each option.Lables do not need to
    have this function used in order to get visible as we have attached them to their respective sliders.*/
   
    setSize (600, 400); //Sets initial size of the plugin window
}

MultiPluginAudioProcessorEditor::~MultiPluginAudioProcessorEditor()
{
}

//==============================================================================
void MultiPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MultiPluginAudioProcessorEditor::resized()
{
    setSize(400, 400); //Keeps window size unchangeable

   //Sets positions of the UI elements
    //Combobox
    pluginTypeMenu.setBounds(100, 10, 200, 25); //Plugin Type Menu
    filterTypeMenu.setBounds(100, 290, 200, 25); //Filter Type Menu
    //Filter
    filterFrequencySlider.setBounds(20, 80, 170, 170); //Frequency Slider
    filterResonanceSlider.setBounds(210, 80, 170, 170); //Resonance Slider
    //Compressor
    compressorAttackSlider.setBounds(80, 240, 140, 140); //Compressor Attack
    compressorRatioSlider.setBounds(80, 60, 140, 140); //Compressor Ratio
    compressorReleaseSlider.setBounds(240, 240, 140, 140); //Compressor Release
    compressorThresholdSlider.setBounds(240, 60, 140, 140); //Compressor Threshold
    //Gain
    gainGainSlider.setBounds(20, 80, 60, 280); //Gain Slider
}

void MultiPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &filterFrequencySlider) { //This if statement checks which slider changed and conencts it to the respective processor value
        audioProcessor.filterFrequency = slider->getValue(); //changes the value of the processor when the value is changed
        
        if (slider->getValue() <= 60) { //This if statement changes the colour of a part of a slider depending on the value selected
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0x2fff3252)); //20 - 60 Hz (Sub-Bass)
        }
        else if (slider->getValue() > 60 && slider->getValue() <= 250) {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0x4fff3252)); //60 - 250 Hz (Bass)
        }
        else if (slider->getValue() > 250 && slider->getValue() <= 500) {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0x6fff3252)); //250 - 500 Hz (Low-Midrange)
        }
        else if (slider->getValue() > 500 && slider->getValue() <= 2000) {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0x8fff3252)); //500 - 2000 Hz (Low-Midrange)
        }
        else if (slider->getValue() > 2000 && slider->getValue() <= 4000) {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0xafff3252)); //2000 - 4000 Hz (Low-Midrange)
        }
        else if (slider->getValue() > 4000 && slider->getValue() <= 6000) {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0xdfff3252)); //4000 - 6000 Hz (Presence)
        }
        else {
            filterFrequencySlider.setColour(0x1001311, juce::Colour(0xffff3252)); //6000 - 20000 Hz (Brilliance)
        }

        if (filterFrequencySlider.isMouseButtonDown(true) == true) { //Function that make the component glow when the mouse 
            filterFrequencySlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &filterResonanceSlider) {
        audioProcessor.filterResonance = slider->getValue(); //Resonance Slider

        if (filterResonanceSlider.isMouseButtonDown(true) == true) {
            filterResonanceSlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &compressorAttackSlider) {
        audioProcessor.compressorAttack = slider->getValue(); //Attack Slider

        if (compressorAttackSlider.isMouseButtonDown(true) == true) {
            compressorAttackSlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &compressorRatioSlider) {
        audioProcessor.compressorRatio = slider->getValue(); //Ratio Slider

        if (compressorRatioSlider.isMouseButtonDown(true) == true) {
            compressorRatioSlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &compressorReleaseSlider) {
        audioProcessor.compressorRelease = slider->getValue(); //Release Slider

        if (compressorReleaseSlider.isMouseButtonDown(true) == true) {
            compressorReleaseSlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &compressorThresholdSlider) {
        audioProcessor.compressorThreshold = slider->getValue(); //Threshold Slider

        if (compressorThresholdSlider.isMouseButtonDown(true) == true) {
            compressorThresholdSlider.setComponentEffect(&glowEffect);
        }
    }
    else if (slider == &gainGainSlider) {
        audioProcessor.gainGain = slider->getValue(); //Gain Slider

        if (gainGainSlider.isMouseButtonDown(true) == true) {
            gainGainSlider.setComponentEffect(&glowEffect);
        }
    }
}

void MultiPluginAudioProcessorEditor::sliderDragEnded(juce::Slider* slider) //Function that is initiated when the user stops draging the slider
{
    //In this function we make sure that every glow is deleted when the user does not react with any sliders
    filterFrequencySlider.setComponentEffect(NULL);
    filterResonanceSlider.setComponentEffect(NULL);
    compressorAttackSlider.setComponentEffect(NULL);
    compressorRatioSlider.setComponentEffect(NULL);
    compressorReleaseSlider.setComponentEffect(NULL);
    compressorThresholdSlider.setComponentEffect(NULL);
    gainGainSlider.setComponentEffect(NULL);
}

void MultiPluginAudioProcessorEditor::comboBoxChanged(juce::ComboBox* combobox)
{
    if (combobox == &pluginTypeMenu) { //Plugin Type Menu
        audioProcessor.pluginType = combobox->getSelectedId();
        if (combobox->getSelectedId() == 1) { //Filter
            //setVisible() sets the visibility of the item and setVisible(false) means that the elements are invisible so any unwanted elements are removed here
            compressorAttackSlider.setVisible(false); //Compressor Attack Slider
            compressorRatioSlider.setVisible(false); //Compressor Ratio Slider
            compressorReleaseSlider.setVisible(false); //Compressor Release Slider
            compressorThresholdSlider.setVisible(false); //Compressor Theshold Slider
            gainGainSlider.setVisible(false); //Gain Slider

            //With the addAndMakeVisible function we create and make visible the elements that are used in the current state of the plugin
            addAndMakeVisible(&filterFrequencySlider); //Frequency Slider
            addAndMakeVisible(&filterResonanceSlider); //Resonance Slider
            addAndMakeVisible(&filterTypeMenu); //Filter Type Menu
        }
        else if (combobox->getSelectedId() == 2) { //Compressor
            filterFrequencySlider.setVisible(false);
            filterResonanceSlider.setVisible(false);
            filterTypeMenu.setVisible(false);

            //Making elements visible
            addAndMakeVisible(&compressorAttackSlider); //Attack Slider
            addAndMakeVisible(&compressorRatioSlider); //Ratio Slider
            addAndMakeVisible(&compressorReleaseSlider); //Release Slider
            addAndMakeVisible(&compressorThresholdSlider); //Threshold Slider
            addAndMakeVisible(&gainGainSlider); //Gain Slider
        }
    }
    else if (combobox == &filterTypeMenu) { //Filter Type Menu
        audioProcessor.filterType = combobox->getSelectedId();

        if (combobox->getSelectedId() == 1) { // If statement that changes the colour depending on the filter type
            filterTypeMenu.setColour(0x1000a00, juce::Colour(0xffff3252)); //Text
            filterTypeMenu.setColour(0x1000e00, juce::Colour(0xffff3252)); //Arrow
            filterTypeMenu.setColour(0x1000c00, juce::Colour(0xffff3252)); //Outline
        }
        else if (combobox->getSelectedId() == 2) {
            filterTypeMenu.setColour(0x1000a00, juce::Colour(0xff7252ff)); //Text
            filterTypeMenu.setColour(0x1000e00, juce::Colour(0xff7252ff)); //Arrow
            filterTypeMenu.setColour(0x1000c00, juce::Colour(0xff7252ff)); //Outline
        }
        else if (combobox->getSelectedId() == 3) {
            filterTypeMenu.setColour(0x1000a00, juce::Colour(0xff32ff52)); //Text
            filterTypeMenu.setColour(0x1000e00, juce::Colour(0xff32ff52)); //Arrow
            filterTypeMenu.setColour(0x1000c00, juce::Colour(0xff32ff52)); //Outline
        }
    }
}