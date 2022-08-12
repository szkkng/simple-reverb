# Simple Reverb

This is a simple reverb plugin made with the JUCE DSP module.

![SimpleReverb](/Resources/simple-reverb.png)

## UI Manual

- undo: cmd + z
- redo: cmd + shift + z
- fine mode: shift + drag
- edit mode: 0-9 keypress
- reset: double click

## Building

```
$ git clone https://github.com/szkkng/SimpleReverb.git --recursive
$ cd SimpleReverb
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
```

```
# Here's Plugin.
$ ls -l build/SimpleReverb_artefacts/VST3
```

## Other

- YouTube: [Simple Reverb - JUCE DSP module](http://www.youtube.com/watch?v=jjTF3EwplWA)
