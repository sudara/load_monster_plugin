#pragma once

#include "Colors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

template <class T>
class GraphComponent : public juce::Component
{
public:
    struct Data
    {
        juce::String xLabel;
        juce::String yLabel;
        size_t maxXValue;
        T maxYValue;
        std::vector<T>& results;
        juce::Colour valueColor;
    };

    GraphComponent (Data d) : data (d)
    {
        addAndMakeVisible (xLabel);
        xLabel.setColour (xLabel.textColourId, Colors::buttonText);
        xLabel.setJustificationType (juce::Justification::centred);
        xLabel.setText (data.xLabel, juce::dontSendNotification);

        addAndMakeVisible (xValue);
        xValue.setColour (xValue.textColourId, data.valueColor);
        xValue.setJustificationType (juce::Justification::right);

        addAndMakeVisible (originValue);
        originValue.setColour (originValue.textColourId, data.valueColor);
        originValue.setJustificationType (juce::Justification::left);
        originValue.setText ("0", juce::dontSendNotification);

        addAndMakeVisible (yLabel);
        yLabel.setColour (yLabel.textColourId, Colors::buttonText);
        yLabel.setJustificationType (juce::Justification::centred);
        yLabel.setText (data.yLabel, juce::dontSendNotification);

        addAndMakeVisible (yValue);
        yValue.setColour (yValue.textColourId, data.valueColor);
        yValue.setJustificationType (juce::Justification::left);
    }

    void setMaxXValue (size_t newValue)
    {
        xValue.setText (juce::String (newValue), juce::dontSendNotification);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (Colors::graphBackground);
        g.fillRoundedRectangle (graphBounds.toFloat(), 4.0f);

        if (!data.results.empty())
        {
            float widthOfOneResult = (float) graphBounds.getWidth() / (float) data.results.size();
            g.setColour (data.valueColor);
            for (size_t i = 0; i < data.results.size(); ++i)
            {
                auto xPosition = (float) graphBounds.getX() + (float) i * widthOfOneResult;
                auto yPosition = (float) graphBounds.getHeight() * (data.maxYValue - data.results[i]);
                auto height = (float) graphBounds.getHeight() - yPosition;
                g.fillRect (xPosition, yPosition, widthOfOneResult, height);
            }
        }
        else
        {
            xValue.setText ("", juce::dontSendNotification);
        }
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto footer = area.removeFromBottom (20);
        xValue.setBounds (footer.removeFromRight (50));
        originValue.setBounds (footer.removeFromLeft (50));
        xLabel.setBounds (footer);

        auto yAxis = area.removeFromLeft (20);
        yValue.setBounds (yAxis.removeFromTop (30));

        yLabel.setBounds (0, area.getHeight() - 20, 50, 20);
        yLabel.setTransform (juce::AffineTransform::rotation (-juce::MathConstants<float>::halfPi, 0, (float) area.getHeight() - 20));

        graphBounds = area;
    }

private:
    Data data;

    juce::Rectangle<int> graphBounds;

    juce::Label xLabel;
    juce::Label yLabel;
    juce::Label originValue;
    juce::Label xValue;
    juce::Label yValue;
};