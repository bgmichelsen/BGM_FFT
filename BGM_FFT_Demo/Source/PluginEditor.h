/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "Graph.h"

#include <vector>


//==============================================================================
/**
*/
class BGM_FFT_DemoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BGM_FFT_DemoAudioProcessorEditor (BGM_FFT_DemoAudioProcessor&);
    ~BGM_FFT_DemoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    BGM_FFT_DemoAudioProcessor& getProcessor() const;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BGM_FFT_DemoAudioProcessor& audioProcessor;


    struct TopPanel : public Component
    {
        TopPanel(juce::Colour c);
        void paint(juce::Graphics& g) override;
        void resized() override;

        juce::Colour bgColor;
        juce::ToggleButton toggle;
        juce::Label tog_label;
    };

    struct BotPanel : public Component
    {
        BotPanel(juce::Colour bgc, juce::Colour lc);
        void paint(juce::Graphics& g) override;
        void resized() override;
        void setScopeData(std::vector<float> *const data);
        void setFFT_Data(std::vector<float>* const data);

        juce::Colour bgColor;
        BGM::Graph plot;
        std::vector<float> scopeData;

        juce::dsp::FFT juceFFT;
        juce::dsp::WindowingFunction<float> window;

        std::array<float, FFT_SIZE * 2> fftData;
    };

    TopPanel top_panel;
    BotPanel bot_panel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BGM_FFT_DemoAudioProcessorEditor)
};
