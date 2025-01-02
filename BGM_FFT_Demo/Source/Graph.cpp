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

BGM::Graph::~Graph()
{

}

void BGM::Graph::drawFrame(juce::Array<float>& data, juce::Graphics& g)
{
    drawAxes(g);
}

void BGM::Graph::drawAxes(juce::Graphics& g)
{
    float x_start, y_start;
    
    if (min_y < 0.0f)
        x_start = abs(min_y);
    else
        x_start = 0.0;

    if (min_x < 0.0f)
        y_start = abs(min_x);
    else
        y_start = screen_height;

    juce::Line<float> x_axis(juce::Point<float>(0.0f, x_start), juce::Point<float>(screen_width, x_start));
    juce::Line<float> y_axis(juce::Point<float>(y_start, 0.0f), juce::Point<float>(y_start, screen_height));

    g.drawLine(x_axis);
    g.drawLine(y_axis);
}
