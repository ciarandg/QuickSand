# QuickSand

**Note: I'm archiving this repo because I haven't touched it since I was working on it
as a capstone project. I make no guarantees about how well it will run, but it's probably
still usable.**

QuickSand is a live-input granular synthesis VST, built in JUCE.

**Note:** QuickSand was my first C++ project, and a capstone project for
UBC's Applied Music Technology minor. As a result, there are a few rough
edges; most notably in terms of performance overhead due to excessive iteration.

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
