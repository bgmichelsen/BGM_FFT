/*
  ==============================================================================

    Graph.cpp
    Created: 13 Nov 2024 7:30:53pm
    Author:  brand

  ==============================================================================
*/

#include "Graph.h"

BGM::Graph::Graph(int x_range[2], int y_range[2], float width, float height)
{
    domain = abs(x_range[0]) + abs(x_range[1]);
    range = abs(y_range[0]) + abs(y_range[1]);

    min_x = x_range[0];
    max_x = x_range[1];

    min_y = y_range[0];
    max_y = y_range[1];

    screen_width = width;
    screen_height = height;
}

BGM::Graph::Graph()
{

}

BGM::Graph::~Graph()
{

}

void BGM::Graph::setDomain(int x_min, int x_max)
{
    min_x = x_min;
    max_x = x_max;
}

void BGM::Graph::setRange(int y_min, int y_max)
{
    min_y = y_min;
    max_y = y_max;
}

void BGM::Graph::setBounds(float width, float height)
{
    screen_width = width;
    screen_height = height;
}

void BGM::Graph::drawFrame(juce::Array<float>& data, juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    drawAxes(g);
}

void BGM::Graph::drawAxes(juce::Graphics& g)
{
    float x_start, y_start;

    x_start = juce::jmap<float>(0.0f, min_y, max_y, 0, screen_height);
    y_start = juce::jmap<float>(0.0f, min_x, max_x, 0, screen_width);


    juce::Line<float> x_axis(juce::Point<float>(0.0f, x_start), juce::Point<float>(screen_width, x_start));
    juce::Line<float> y_axis(juce::Point<float>(y_start, 0.0f), juce::Point<float>(y_start, screen_height));

    g.drawLine(x_axis);
    g.drawLine(y_axis);
}
