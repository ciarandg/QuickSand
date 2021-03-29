/*
  ==============================================================================

    LabeledRotary.h
    Created: 28 Mar 2021 8:18:59pm
    Author:  ciaran

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColourScheme.h"

class LabeledRotary : public juce::Slider {
public:
  LabeledRotary(
    double min_val, double max_val, double step, double initial,
    std::string label_text, std::string suffix, juce::Colour highlight) {
    // slider setup
    setRange(min_val, max_val, step);
    setValue(initial);
    setTextValueSuffix(suffix);
    // label setup
    label.setText(label_text, juce::dontSendNotification);

    // slider visuals
    setSliderStyle(RotaryVerticalDrag);
    setTextBoxStyle(TextBoxBelow, false, TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);
    // label visuals
    label.setFont({20.f, juce::Font::bold});
    label.setJustificationType(juce::Justification::centred);
    label.attachToComponent(this, false);
    // colours
    setColour(rotarySliderFillColourId,     highlight);
    setColour(rotarySliderOutlineColourId,  COLOUR_BG_LIGHT);
    setColour(thumbColourId,                juce::Colours::transparentBlack);
    setColour(textBoxOutlineColourId,       COLOUR_TEXT);
    setColour(textBoxTextColourId,          COLOUR_TEXT);
    label.setColour(juce::Label::ColourIds::textColourId, COLOUR_TEXT);
  };

private:
  static const int TEXT_BOX_WIDTH = 100;
  static const int TEXT_BOX_HEIGHT = 25;
  juce::Label label;
};