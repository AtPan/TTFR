# TTFR

## What is TTFR?

    `TTFR` stands for TrueType Font Renderer, and is exactly as its name
    implies. This project is centered primarily around modularity, capable of
    being plugged into any other program, library, or project.

    This library is primarily made to be a simple replacement of the SDL_ttf 
    library to reduce the overhead of loading and rendering text in graphical
    applications. Applications employing SDL2 have to manage many aspects of
    the program's state manually; this library exists to handle some of that
    state regarding text rendering for the user in a safe and efficient way.

## The State of TTFR

    The state of TTFR is currently incomplete and not in a usable condition.
    There are many missing basic components that any renderer would need.
    [Here] (#feature-list) is a list of planned features, as well as which
    version that feature was added (if applicable).

### Required Dependencies

    - [SDL2] (https://www.libsdl.org) is required.

### Supported Files

    Only TrueType Font files with Unicode 2.0 (and onward) BMP character are
    supported. There are no plans currently to expand support beyond these
    file formats.

### Feature List

    - Doxygen-compatible Documentation          [ ]
    - Full Character Rendering                  [ ]
    - Unicode Character Support                 [ ]
    - Basic Character Glyf Rendering            [*] (v. 0.1)
    - Complex Character Glyf Rendering          [ ]
    - Multi-character Rendering                 [ ]
    - Graceful Forced Exits (e.g. Alt-F4)       [ ]
    - C++ Compatibility                         [ ]
    - Zero-cost Debugging                       [*] (v. 0.1)

## License

    `TTFR` is licensed under the open source MIT license.
    The full text of the license can be found in LICENSE at the root of
    this project.
