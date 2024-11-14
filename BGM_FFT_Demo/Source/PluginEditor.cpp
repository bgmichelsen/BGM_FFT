/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BGM_FFT_DemoAudioProcessorEditor::BGM_FFT_DemoAudioProcessorEditor (BGM_FFT_DemoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(select);
}

BGM_FFT_DemoAudioProcessorEditor::~BGM_FFT_DemoAudioProcessorEditor()
{
}

//==============================================================================
void BGM_FFT_DemoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void BGM_FFT_DemoAudioProcessorEditor::resized()
{
    juce::FlexBox fb_all;
    juce::FlexBox fb_top;
    juce::FlexBox fb_bot;

    fb_top.flexWrap = juce::FlexBox::Wrap::wrap;
    fb_top.justifyContent = juce::FlexBox::JustifyContent::center;
    fb_top.alignContent = juce::FlexBox::AlignContent::center;
    fb_top.flexDirection = juce::FlexBox::Direction::row;
    fb_top.items.add(juce::FlexItem(select).withMinHeight(15.0f).withMinWidth(20.0f).withFlex(1));

    fb_top.flexWrap = juce::FlexBox::Wrap::wrap;
    fb_top.justifyContent = juce::FlexBox::JustifyContent::center;
    fb_top.flexDirection = juce::FlexBox::Direction::row;

    fb_all.flexDirection = juce::FlexBox::Direction::column;
    fb_all.items.add(juce::FlexItem(fb_top).withFlex(2.5));
    fb_all.items.add(juce::FlexItem(fb_bot).withFlex(2.5));
    fb_all.performLayout(getLocalBounds());
}
