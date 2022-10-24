# Simple Reverb

This is a simple reverb plugin made with the JUCE DSP module.

![simple-reverb](/Resources/SimpleReverb.png)

## UI Manual

- value changes: dragging or arrow keys
- fine mode: shift + dragging or shift + arrow keys
- edit mode: 0-9
- undo: cmd + z
- redo: cmd + shift + z
- reset: double click

## Building

```
$ git clone https://github.com/szkkng/simple-reverb.git --recursive
$ cd simple-reverb
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --config Release
```
