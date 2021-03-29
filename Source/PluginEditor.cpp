/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
QuickSandAudioProcessorEditor::QuickSandAudioProcessorEditor (QuickSandAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (880, 200);
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
}
