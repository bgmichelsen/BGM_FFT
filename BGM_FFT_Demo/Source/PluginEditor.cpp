/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Graph.h"

#include <vector>

//==============================================================================
BGM_FFT_DemoAudioProcessorEditor::BGM_FFT_DemoAudioProcessorEditor(BGM_FFT_DemoAudioProcessor& p)
    : AudioProcessorEditor(&p),
    audioProcessor(p),
    top_panel(juce::Colours::darkslategrey),
    bot_panel(juce::Colours::black, juce::Colours::red),
    juceFFT(FFT_ORDER)
{
    addAndMakeVisible(top_panel);
    addAndMakeVisible(bot_panel);   

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
}

BGM_FFT_DemoAudioProcessorEditor::~BGM_FFT_DemoAudioProcessorEditor()
{
}

//==============================================================================
void BGM_FFT_DemoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    bot_panel.setScopeData(&(getProcessor().getFifo()));
}

void BGM_FFT_DemoAudioProcessorEditor::resized()
{
    juce::FlexBox   fb;

    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;

    float height = (float)getHeight();
    const float offset = height / 6.0f;

    juce::FlexItem top((float)getWidth(), offset, top_panel);
    juce::FlexItem bot((float)getWidth(), height - offset, bot_panel);

    fb.items.addArray({ top, bot });
    fb.performLayout(getLocalBounds());
}

BGM_FFT_DemoAudioProcessor& BGM_FFT_DemoAudioProcessorEditor::getProcessor() const
{
    return static_cast<BGM_FFT_DemoAudioProcessor&>(processor);
}

BGM_FFT_DemoAudioProcessorEditor::TopPanel::TopPanel(juce::Colour c)
{
    bgColor = c;
    tog_label.setText("Use Juce FFT?:", juce::dontSendNotification);
    tog_label.attachToComponent(&toggle, true);
    addAndMakeVisible(toggle);
    addAndMakeVisible(tog_label);
}

void BGM_FFT_DemoAudioProcessorEditor::TopPanel::paint(juce::Graphics& g)
{
    g.fillAll(bgColor);
}

void BGM_FFT_DemoAudioProcessorEditor::TopPanel::resized()
{
    toggle.setBounds((float)getWidth() / 2.0f, (float)getHeight() / 2.0f, 50, 50);
}

BGM_FFT_DemoAudioProcessorEditor::BotPanel::BotPanel(juce::Colour bgc, juce::Colour lc)
{
    bgColor = bgc;
    scopeData = nullptr;
    plot.setDomain(0, 10);
    plot.setRange(0, 20);
    plot.setColour(lc);
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::paint(juce::Graphics& g)
{
    g.fillAll(bgColor);

    auto bounds = getLocalBounds();

    plot.setBounds((float)bounds.getWidth(), (float)bounds.getHeight());
    if (nullptr != scopeData)
    {
        plot.drawFrame(scopeData, g);
    }
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::setScopeData(std::vector<float> *const data)
{
    scopeData = data;
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::resized()
{
    /*plot.setBounds((float)getWidth(), (float)getHeight());
    plot.drawFrame(data, g);*/
}
