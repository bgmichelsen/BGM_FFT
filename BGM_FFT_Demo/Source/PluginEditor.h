/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "Graph.h"

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
        BotPanel(juce::Colour c);
        void paint(juce::Graphics& g) override;
        void resized() override;

        juce::Colour bgColor;
        BGM::Graph plot;
    };

    TopPanel top_panel;
    BotPanel bot_panel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BGM_FFT_DemoAudioProcessorEditor)
};
