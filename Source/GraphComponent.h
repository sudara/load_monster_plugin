#pragma once

#include "Colors.h"
#include "juce_gui_basics/juce_gui_basics.h"

class GraphComponent : public juce::Component
{
public:
    GraphComponent (std::vector<float>& r, size_t& m) : results (r), lastMultiplies (m)
    {
        xLabel.setText ("multiplies", juce::dontSendNotification);
        yLabel.setText ("% of callback time spent in CPU", juce::dontSendNotification);

        addAndMakeVisible (yValue);
        yValue.setColour (yValue.textColourId, Colors::graphValue);
        yValue.setJustificationType (juce::Justification::right);

        addAndMakeVisible (yLabel);
        yLabel.setColour (yLabel.textColourId, Colors::buttonText);
        yLabel.setJustificationType (juce::Justification::centred);
        yLabel.setText ("multiplies", juce::dontSendNotification);

        addAndMakeVisible (originValue);
        originValue.setColour (originValue.textColourId, Colors::graphValue);
        originValue.setJustificationType (juce::Justification::left);
        originValue.setText ("0", juce::dontSendNotification);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (Colors::graphBackground);
        g.fillRoundedRectangle (graphBounds.toFloat(), 4.0f);

        if (!results.empty())
        {
            float widthOfOneResult = (float) getWidth() / (float) results.size();
            g.setColour (Colors::graphValue);
            for (size_t i = 0; i < results.size(); ++i)
            {
                g.fillRect ((float) i * widthOfOneResult, (float) graphBounds.getHeight() * (1.0f - results[i]), widthOfOneResult, (float) graphBounds.getHeight() * results[i]);
            }
            yValue.setText (juce::String (lastMultiplies), juce::dontSendNotification);
        }
        else
        {
            yValue.setText ("", juce::dontSendNotification);
        }
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto footer = area.removeFromBottom (20);
        yValue.setBounds (footer.removeFromRight (50));
        originValue.setBounds (footer.removeFromLeft (50));
        yLabel.setBounds (footer);

        graphBounds = area;
    }

private:
    std::vector<float>& results;
    size_t& lastMultiplies;

    juce::Rectangle<int> graphBounds;

    juce::Label xLabel;
    juce::Label yLabel;
    juce::Label originValue;
    juce::Label yValue;
};