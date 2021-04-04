/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "UI/LabeledRotary.h"

const static int WINDOW_WIDTH = 880;
const static int WINDOW_HEIGHT = 200;

//==============================================================================
QuickSandAudioProcessorEditor::QuickSandAudioProcessorEditor(
    QuickSandAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

  cacheSizeRotary.addListener(this);
  grainSizeRotary.addListener(this);
  grainShapeRotary.addListener(this);
  grainRandomnessRotary.addListener(this);
  grainOverlapRotary.addListener(this);
  wetDryRotary.addListener(this);

  for (std::vector<LabeledRotary *> group : dial_groups) {
    for (LabeledRotary *dial : group)
      addAndMakeVisible(*dial);
  }
}

QuickSandAudioProcessorEditor::~QuickSandAudioProcessorEditor() {}

//==============================================================================
void QuickSandAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(COLOUR_BG_DARK);
  g.setColour(COLOUR_BG_LIGHT);
  for (int group = 1; group < dial_groups.size(); ++group) {
    int line_x = getGroupX(group) - GROUP_PAD / 2;
    g.drawLine(line_x, 0, line_x, WINDOW_HEIGHT, GROUP_DIVIDER_WIDTH);
  }

  g.setColour(COLOUR_ACCENT_1);
  grain_shape_graphic.draw(grainShapeRotary.getValue() * 0.01, g);
}

void QuickSandAudioProcessorEditor::resized() {
  int dialCount = 0;
  for (int group = 0; group < dial_groups.size(); ++group) {
    for (LabeledRotary *dial : dial_groups[group]) {
      dial->setBounds(getDialX(dialCount), TOP_PAD, ROTARY_WIDTH,
                      ROTARY_HEIGHT);
      ++dialCount;
    }
  }
}

void QuickSandAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  if (slider == &cacheSizeRotary)
    audioProcessor.granulator.resize(slider->getValue() *
                                     audioProcessor.getSamplesPerMillisecond());
  else if (slider == &grainSizeRotary)
    audioProcessor.granulator.set_grain_size(
        slider->getValue() * audioProcessor.getSamplesPerMillisecond());
  else if (slider == &grainShapeRotary) {
    audioProcessor.granulator.set_window(slider->getValue() * 0.01f);
    repaint();
  } else if (slider == &grainRandomnessRotary)
    audioProcessor.granulator.set_randomness(slider->getValue() * 0.01f);
  else if (slider == &grainOverlapRotary)
    audioProcessor.granulator.set_overlap(slider->getValue());
  else if (slider == &wetDryRotary)
    audioProcessor.set_mix(slider->getValue() * 0.01f);
}