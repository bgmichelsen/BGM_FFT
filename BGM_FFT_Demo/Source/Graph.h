/*
  ==============================================================================

    Graph.h
    Created: 13 Nov 2024 7:31:16pm
    Author:  brand

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include <vector>

namespace BGM
{
    class Graph
    {
    public:
        Graph(juce::Colour colour, int x_range[2], int y_range[2], float width, float height);
        Graph();
        ~Graph();

        void setDomain(int x_min, int x_max);
        void setRange(int y_min, int y_max);
        void setBounds(float width, float height);
        void setColour(juce::Colour c);

        void drawFrame(float *const data, size_t size, juce::Graphics& g);
    private:
        int domain;
        int range;
        int min_x;
        int max_x;
        int min_y;
        int max_y;
        float screen_width;
        float screen_height;

        juce::Colour line_color;

        void drawAxes(juce::Graphics& g);
    };
}