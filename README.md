# microui
![screenshot](https://user-images.githubusercontent.com/3920290/53285737-f4cae500-375b-11e9-96bc-ba7c314dc161.png)

A tiny, portable, immediate-mode UI library written in ANSI C

## Features
* Tiny: around `1100 sloc` of ANSI C
* Works within a fixed-sized memory region: no additional memory is
  allocated
* Built-in controls: window, panel, button, slider, textbox, label,
  checkbox, wordwrapped text
* Easy to add custom controls
* Simple layout system

## Usage
* See [`doc/usage.md`](doc/usage.md) for usage instructions
* See the [`demo`](demo) directory for a usage example

## Notes
* The library expects the user to provide input and handle the resultant
  drawing commands, it does not do any drawing itself
* In contrast to other immediate mode ui libraries microui does not store
  window, panel, header or treenode state internally. It is up to the
  user to manage this data themselves

## Contributing
The library is designed to be lightweight, providing a foundation to which
you can easily add custom controls and UI elements; pull requests adding
additional features will likely not be merged. Bug reports are welcome.

## License
This library is free software; you can redistribute it and/or modify it
under the terms of the MIT license. See [LICENSE](LICENSE) for details.

