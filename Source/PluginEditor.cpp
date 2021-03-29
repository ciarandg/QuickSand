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
    
    for (LabeledRotary* dial : dials) addAndMakeVisible(*dial);
}

QuickSandAudioProcessorEditor::~QuickSandAudioProcessorEditor()
{
}

//==============================================================================
void QuickSandAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (COLOUR_BG_DARK);
}

void QuickSandAudioProcessorEditor::resized()
{
  for (int i = 0; i < dials.size(); ++i) {
    dials[i]->setBounds(ROTARY_WIDTH*i, TOP_PAD, ROTARY_WIDTH, ROTARY_HEIGHT);
  }
}
