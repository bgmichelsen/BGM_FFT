/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Graph.h"

#include <vector>
#include <algorithm>

//==============================================================================
BGM_FFT_DemoAudioProcessorEditor::BGM_FFT_DemoAudioProcessorEditor(BGM_FFT_DemoAudioProcessor& p)
    : AudioProcessorEditor(&p),
    audioProcessor(p),
    top_panel(juce::Colours::darkslategrey),
    bot_panel(juce::Colours::black, juce::Colours::red)
{
    addAndMakeVisible(top_panel);
    addAndMakeVisible(bot_panel);

    startTimerHz(60);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
}

BGM_FFT_DemoAudioProcessorEditor::~BGM_FFT_DemoAudioProcessorEditor()
{
}

void BGM_FFT_DemoAudioProcessorEditor::timerCallback()
{
    if (getProcessor().isBlockReady())
    {
        bool use_juce = top_panel.getToggleState();
        bot_panel.useJuceFFT(use_juce);
        repaint();
        getProcessor().resetBlock();
    }
}

//==============================================================================
void BGM_FFT_DemoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
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

bool BGM_FFT_DemoAudioProcessorEditor::TopPanel::getToggleState()
{
    return toggle.getToggleState();
}

BGM_FFT_DemoAudioProcessorEditor::BotPanel::BotPanel(juce::Colour bgc, juce::Colour lc) :
    juceFFT(FFT_ORDER),
    window(PERIODIC_WINDOW, juce::dsp::WindowingFunction<float>::WindowingMethod::hann, false)
{
    bgColor = bgc;
    scopeData.clear();
    plot.setDomain(0, FFT_SIZE / 2);
    plot.setRange(0, 1);
    plot.setColour(lc);
    useJuce = false;
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::paint(juce::Graphics& g)
{
    if (!std::all_of(fftData.begin(), fftData.end(), [](float x) { return x == 0.0f; }))
    {
        g.fillAll(bgColor);

        auto bounds = getLocalBounds();
        float* fft_ptr = fftData.data();

        scopeData.clear();

        // Get the windowed data
        window.multiplyWithWindowingTable(fft_ptr, FFT_SIZE);

        // Perform the FFT
        if (useJuce)
        {
            juceFFT.performRealOnlyForwardTransform(fft_ptr, true);
        }
        else
        {
            bgmFFT.forwardTransform(fft_ptr, (FFT_SIZE * 2));
        }

        // FFT results are complex numbers, need to get magnitude and phase separately
        for (int idx = 0; idx < FFT_SIZE / 2; idx++)
        {
            std::complex<float> cmplx;
            cmplx.real(fft_ptr[idx * 2]);
            cmplx.imag(fft_ptr[idx * 2 + 1]);

            // Get the magnitude and pass it to the scope data
            float mag = std::abs(cmplx);
            scopeData.push_back(std::abs(mag));
        }

        plot.setBounds((float)bounds.getWidth(), (float)bounds.getHeight());
        plot.drawFrame(&scopeData, g);
    }
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::setScopeData(std::vector<float> *const data)
{
    scopeData.clear();
    std::copy(data->begin(), data->end(), std::back_inserter(scopeData));
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::setFFT_Data(float * const data, size_t size)
{
    if (data != nullptr)
    {
        fftData.fill(0.0f);
        std::copy_n(data, size, fftData.begin());
    }
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::resized()
{
    /*plot.setBounds((float)getWidth(), (float)getHeight());
    plot.drawFrame(data, g);*/
}

void BGM_FFT_DemoAudioProcessorEditor::BotPanel::useJuceFFT(bool flag)
{
    useJuce = flag;
}

BGM_FFT_DemoAudioProcessorEditor::TopPanel& BGM_FFT_DemoAudioProcessorEditor::getTopPanel()
{
    return top_panel;
}

BGM_FFT_DemoAudioProcessorEditor::BotPanel& BGM_FFT_DemoAudioProcessorEditor::getBotPanel()
{
    return bot_panel;
}

