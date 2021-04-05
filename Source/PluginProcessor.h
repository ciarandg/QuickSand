/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Data/ParamData.h"
#include "Granulator/Mixer.h"
#include <JuceHeader.h>


//==============================================================================
/**
 */
class QuickSandAudioProcessor : public juce::AudioProcessor {
public:
  //==============================================================================
  QuickSandAudioProcessor();
  ~QuickSandAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  //===CUSTOM
  // STUFF===============================================================
  juce::AudioParameterInt *cacheSizeParameter;
  juce::AudioParameterInt *voicesParameter;
  juce::AudioParameterFloat *grainSizeParameter;
  juce::AudioParameterInt *grainShapeParameter;
  juce::AudioParameterInt *randomnessParameter;
  juce::AudioParameterFloat *grainOverlapParameter;
  juce::AudioParameterFloat *wetDryParameter;

  int getSamplesPerMillisecond() { return (int)(getSampleRate() / 1000.); }

  // PRE: 0.f <= mix <= 1.f
  void set_mix(float pct) { mix = pct; }

private:
  void initIntParam(juce::AudioParameterInt **param, const struct param data) {
    *param = new juce::AudioParameterInt(data.id, data.name, data.min, data.max,
                                         data.initial);
  }
  void initFloatParam(juce::AudioParameterFloat **param,
                      const struct param data) {
    *param = new juce::AudioParameterFloat(data.id, data.name, data.min,
                                           data.max, data.initial);
  }

  Mixer mixer;
  float mix;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QuickSandAudioProcessor)
};
