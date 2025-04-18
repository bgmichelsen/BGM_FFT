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
    bot_panel(juce::Colours::black, juce::Colours::red)
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

    bot_panel.setFFT_Data(&(getProcessor().getFifo()));
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

BGM_FFT_DemoAudioProcessorEditor::BotPanel::BotPanel(juce::Colour bgc, juce::Colour lc) :
    juceFFT(FFT_ORDER),
    window(PERIODIC_WINDOW, juce::dsp::WindowingFunction<float>::WindowingMethod::hann, false)
{
    bgColor = bgc;
    scopeData.clear();
    plot.setDomain(0, FFT_SIZE);
    plot.setRange(0, 1);
    plot.setColour(lc);
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::paint(juce::Graphics& g)
{
    g.fillAll(bgColor);

    auto bounds = getLocalBounds();
    float* fft_ptr = fftData.data();

    // Get the windowed data
    window.multiplyWithWindowingTable(fft_ptr, FFT_SIZE);

    juceFFT.performRealOnlyForwardTransform(fft_ptr);

    // FFT results are complex numbers, need to get magnitude and phase separately
    for (int idx = 0; idx < FFT_SIZE; idx++)
    {
        std::complex<float> cmplx;
        cmplx.real(fft_ptr[idx * 2]);
        cmplx.imag(fft_ptr[idx * 2 + 1]);

        // Get the magnitude and pass it to the scope data
        scopeData.push_back(std::abs(cmplx));
    }

    plot.setBounds((float)bounds.getWidth(), (float)bounds.getHeight());
    plot.drawFrame(&scopeData, g);
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::setScopeData(std::vector<float> *const data)
{
    scopeData.clear();
    std::copy(data->begin(), data->end(), std::back_inserter(scopeData));
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::setFFT_Data(std::vector<float>* const data)
{
    if (data->end() - data->begin() == FFT_SIZE)
    {
        std::copy_n(data->begin(), FFT_SIZE, fftData.begin());
    }
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::resized()
{
    /*plot.setBounds((float)getWidth(), (float)getHeight());
    plot.drawFrame(data, g);*/
}
