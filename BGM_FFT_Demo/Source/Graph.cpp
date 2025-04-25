/*
  ==============================================================================

    Graph.cpp
    Created: 13 Nov 2024 7:30:53pm
    Author:  brand

  ==============================================================================
*/

#include "Graph.h"

#include <vector>

BGM::Graph::Graph(juce::Colour color, int x_range[2], int y_range[2], float width, float height)
{
    domain = abs(x_range[0]) + abs(x_range[1]);
    range = abs(y_range[0]) + abs(y_range[1]);

    min_x = x_range[0];
    max_x = x_range[1];

    min_y = y_range[0];
    max_y = y_range[1];

    screen_width = width;
    screen_height = height;

    line_color = color;
}

BGM::Graph::Graph()
{
    min_x = 0;
    max_x = 10;

    min_y = 0;
    max_y = 10;

    screen_width = 600;
    screen_height = 400;

    domain = 10;
    range = 10;

    line_color = juce::Colours::green;
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

void BGM::Graph::setColour(juce::Colour c)
{
    line_color = c;
}

void BGM::Graph::drawFrame(std::vector<float> *const data, juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    drawAxes(g);

    if (!data->empty())
    {
        g.setColour(line_color);
        int data_len = data->size();
        for (int i = 1; i < data_len - 1; i++)
        {
            float x1 = (float)juce::jmap(i - 1, min_x, max_x - 1, 0, (int)screen_width);
            float y1 = (float)juce::jmap((*data)[i - 1], (float)min_y, (float)max_y, screen_height, 0.0f);
            float x2 = (float)juce::jmap(i, min_x, max_x - 1, 0, (int)screen_width);
            float y2 = (float)juce::jmap((*data)[i], (float)min_y, (float)max_y, screen_height, 0.0f);
            g.drawLine(x1, y1, x2, y2);
        }
    }
}

void BGM::Graph::drawAxes(juce::Graphics& g)
{
    float x_start, y_start;

    x_start = screen_height - juce::jmap<float>(0.0f, min_y, max_y, 0, screen_height);
    y_start = juce::jmap<float>(0.0f, min_x, max_x, 0, screen_width);

    // Draw the axes
    juce::Line<float> x_axis(juce::Point<float>(0.0f, x_start), juce::Point<float>(screen_width, x_start));
    juce::Line<float> y_axis(juce::Point<float>(y_start, 0.0f), juce::Point<float>(y_start, screen_height));

    g.drawLine(x_axis);
    g.drawLine(y_axis);

    // Draw the x tally marks


    // Draw teh y tally marks

}
