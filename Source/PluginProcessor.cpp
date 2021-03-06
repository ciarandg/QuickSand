/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Data/ParamData.h"
#include "PluginEditor.h"
#include "Test/TestSuite.h"

//==============================================================================
QuickSandAudioProcessor::QuickSandAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
  #ifdef DEBUG
  TestSuite ts{};
  juce::UnitTestRunner runner{};
  runner.runAllTests();
  #endif

  initIntParam(&cacheSizeParameter, CACHE_SIZE_PARAM_DATA);
  addParameter(cacheSizeParameter);
  initIntParam(&voicesParameter, VOICES_PARAM_DATA);
  addParameter(voicesParameter);
  initFloatParam(&grainSizeParameter, GRAIN_SIZE_PARAM_DATA);
  addParameter(grainSizeParameter);
  initIntParam(&grainShapeParameter, GRAIN_SHAPE_PARAM_DATA);
  addParameter(grainShapeParameter);
  initIntParam(&randomnessParameter, GRAIN_RAND_PARAM_DATA);
  addParameter(randomnessParameter);
  initFloatParam(&grainOverlapParameter, GRAIN_OVERLAP_PARAM_DATA);
  addParameter(grainOverlapParameter);
  initFloatParam(&wetDryParameter, WET_DRY_PARAM_DATA);
  addParameter(wetDryParameter);
}

QuickSandAudioProcessor::~QuickSandAudioProcessor() {}

//==============================================================================
const juce::String QuickSandAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool QuickSandAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool QuickSandAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool QuickSandAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double QuickSandAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int QuickSandAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int QuickSandAudioProcessor::getCurrentProgram() { return 0; }

void QuickSandAudioProcessor::setCurrentProgram(int index) {}

const juce::String QuickSandAudioProcessor::getProgramName(int index) {
  return {};
}

void QuickSandAudioProcessor::changeProgramName(int index,
                                                const juce::String &newName) {}

//==============================================================================
void QuickSandAudioProcessor::prepareToPlay(double sampleRate,
                                            int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  gran = {sampleRate, samplesPerBlock, &gran_settings, &cache};
  dryMonoBuf.resize(samplesPerBlock);
  outBuf.resize(samplesPerBlock);
  cache = {(uint) (CACHE_SIZE_PARAM_DATA.max / 1000.f * sampleRate)};
}

void QuickSandAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool QuickSandAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void QuickSandAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                           juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  auto numSamples = buffer.getNumSamples();

  // RESPONSIBILITIES:
  // dry signal passthrough
  // feed into cache
  // pull wet buf from Granulator
  // mix wet/dry

  if (nextBlockVoiceCount != gran.voiceCount) {
    gran.set_voice_count(nextBlockVoiceCount);
  }
  if (nextCacheSize != cache.get_capacity()) {
    cache.set_capacity(nextCacheSize);
  }

  // fill wet signal buf
  gran.fill(buffer);
  auto wetMonoBuf = gran.read();

  // fill dry signal buf
  for (int ch = 0; ch < totalNumInputChannels; ++ch) {
    auto readPointer = buffer.getReadPointer(ch);
    for (int samp = 0; samp < numSamples; ++samp) {
      if (ch == 0)
        dryMonoBuf[samp] = 0.f;
      dryMonoBuf[samp] += readPointer[samp];
    }
  }

  // output wet/dry mix
  mix(dryMonoBuf, wetMonoBuf);
  for (int ch = 0; ch < totalNumOutputChannels; ++ch) {
    auto *channelData = buffer.getWritePointer(ch);
    for (int sample = 0; sample < numSamples; ++sample) {
      channelData[sample] = outBuf[sample];
    }
  }
}

void QuickSandAudioProcessor::mix(std::vector<float> dry,
                                  std::vector<float> wet) {
  jassert(dry.size() == wet.size() && wet.size() == outBuf.size());
  auto size = dry.size();
  float pct_dry = 1.f - mix_pct_wet;

  outBuf.resize(size);
  for (int samp = 0; samp < size; ++samp) {
    outBuf[samp] = (wet[samp] * mix_pct_wet) + (dry[samp] * pct_dry);
  }
}

//==============================================================================
bool QuickSandAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *QuickSandAudioProcessor::createEditor() {
  return new QuickSandAudioProcessorEditor(*this);
}

//==============================================================================
void QuickSandAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void QuickSandAudioProcessor::setStateInformation(const void *data,
                                                  int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new QuickSandAudioProcessor();
}
