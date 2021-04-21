# QuickSand
QuickSand is a live-input granular synthesis VST, built in JUCE.

<p align="center">
  <img src="https://i.imgur.com/ecO6GqO.png" alt="screenshot"/>
</p>

## Features
- Dynamically resizable cache
- 16-voice polyphony
- Adjustable grain envelopes
- Adjustable per-voice randomness
- Adjustable grain overlap

## Planned features
- BPM synchronization
- Playback while cache is still filling
- Per-voice pitch shifting
- Additional grain shapes
- Phase displacement, to avoid positive feedback
  - Currently solved by guarding the randomness parameter, but using phase would be a cleaner solution
- Multi-channel support