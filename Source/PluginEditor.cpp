/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "GUI/LabeledRotary.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
QuickSandAudioProcessorEditor::QuickSandAudioProcessorEditor (QuickSandAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (880, 200);
    
    for (std::vector<LabeledRotary*> group : dial_groups) {
      for (LabeledRotary* dial : group) addAndMakeVisible(*dial);
    }
}

QuickSandAudioProcessorEditor::~QuickSandAudioProcessorEditor()
{
}

//==============================================================================
void QuickSandAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (COLOUR_BG_DARK);
    
    g.setColour(COLOUR_BG_LIGHT);
    for (int group = 1; group < dial_groups.size(); ++group) {
      int line_x = getGroupX(group);
      g.drawLine(line_x - 10, 0, line_x - 10, 200, 8);
    }
}

void QuickSandAudioProcessorEditor::resized()
{
  int dialCount = 0;
  for (int group = 0; group < dial_groups.size(); ++group) {
    for (LabeledRotary* dial : dial_groups[group]) {
      dial->setBounds(getDialX(dialCount), TOP_PAD, ROTARY_WIDTH, ROTARY_HEIGHT);
      ++dialCount;
    }
  }
}
