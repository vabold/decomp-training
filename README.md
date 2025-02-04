# Decomp Training

This repository builds a DOL with CodeWarrior for the purpose of loading it into analysis software. It is intended to be compiled with minimal runtime, and is not intended to play back on console.

## Dependencies

- ninja 1.3
- Python 3.6
- [dtk](https://github.com/encounter/decomp-toolkit)
- CodeWarrior v3.0a5.2

Extract dtk to `build`, and extract CodeWarrior to `build/compiler`.

## Building

```bash
python configure.py
ninja
```
