# Simple Reverb

This is a simple reverb plugin made with the JUCE DSP module.

<img alt="Screenshot 2024-04-02 at 17 33 04" src="https://github.com/szkkng/simple-reverb/assets/61953352/d00f4fbd-04db-4e4c-9eb9-2e3c77c37d0b">

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
