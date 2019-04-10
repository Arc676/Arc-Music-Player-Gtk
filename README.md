# Arc Music Player (Gtkmm)
GTK version of Arc Music Player for Linux (you can check out the repository for the Mac version [here](https://github.com/Arc676/Arc-Music-Player)). The playlist saving format is described in the README in that repository.

Please check the README for the Mac version for more info regarding the application.

### Legal

Project available under GPLv3 (full license text in `LICENSE`). SDL and SDL Mixer libraries available under Zlib license (license text in `Zlib`). The Gtkmm and `libnotify` libraries are available under the Lesser GPL (the LGPL consists of the GPLv3 terms plus some additional terms which can be found in `LGPL`).

### Dependencies

Compiling this version of Arc Music Player requires the following packages to be installed.

```
libgtkmm-3.0-dev
libsdl2-dev
libsdl2-mixer-dev
libnotify-dev
```

**Note**: The default target of the project Makefile currently depends on `std::experimental::filesystem`.

### Compiling with `make`

The Makefile provides the following targets:
- `amp`: Compiles Arc Music Player for release
- `debug`: Sets up compilation for debugging by passing `-g -O0` and `-export-dynamic` before compiling the `amp` target

If `std::experimental::filesystem` is not available on your machine or you otherwise cannot compile Arc Music Player due to its use of experimental features, you can disable their use by defining the `NOEXP` macro when calling `make`.

Features disabled when experimental code is disabled:
- Adding directories of music files

By defining the `DEBUG` macro on compilation, the program is compiled in debug mode (no optimizations and with debug information for debuggers).
