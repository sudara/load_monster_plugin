#pragma once

#include "Colors.h"
#include "juce_gui_basics/juce_gui_basics.h"

class MeterComponent : public juce::Component, public juce::Timer
{
public:
    explicit MeterComponent (float& c) : cpuLoadProportion (c)
    {
        startTimerHz (60);
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        g.setColour (Colors::meterTrack);
        g.fillRoundedRectangle (bounds, 4.0f);

        g.setColour (Colors::meterValue);
        float widthOfValue = bounds.getWidth() * currentProportion;
        g.fillRoundedRectangle (bounds.withWidth (widthOfValue), 4.0f);
    }

    // paint only when the value changes
    void timerCallback() override
    {
        if (currentProportion != cpuLoadProportion)
        {
            currentProportion = cpuLoadProportion;
            repaint();
        }
    }

private:
    float& cpuLoadProportion;
    float currentProportion;
};