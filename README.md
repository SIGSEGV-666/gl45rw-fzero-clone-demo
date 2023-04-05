# gl45rw-fzero-clone-demo

This is a F-Zero X/GX style demo for my game engine, gl45rw (which is also included under the `gl45rw` subfolder)

# How to run:
Windows users (64-bit only): simply run the pre-built `main_sdl2.exe` or you can attempt to build a new exe by running `build_sdl2_win64.bat` (requires MinGW-w64, the SDL2 and OpenAL Soft headers to be installed somewhere.
Linux users (64-bit architectures only): you can either run `build_glfw.sh` to build the demo with GLFW 3 (requires libglfw3 and libopenal) then run `main_glfw` or execute `build_sdl2.sh` for building with SDL2 (requires libsdl2 and libopenal) then run the generated `main_sdl2` executable

# Command line params:
`-basedir=<path-to-folder>`: Specify an alternate `base` folder containing the game data. Can be a relative or absolute path. (defaults to `base`.)

`-width=<integer-value>`: The game window's width (default value is `640`)

`-height=<integer-value>`: The game window's height (default value is `480`)

`--fullscreen`: Run the game in fullscreen mode if passed (the game runs in windowed mode when omitted)

`-map=<map-name>`: Which track to load (refer to the `.map` files in `base/maps/` to find which ones you can load) e.g. if you want to load `base/maps/newtrack3.map`, then pass `-map=newtrack3` (defaults to `firefield`.)

`-machine=<machine-name>`: Which machine/car to play as (like `-map`, refer to the `.mac` files in `base/machines/` to find which machines you can play as) e.g. if you want to drive `base/machines/bluefalcon_gx.mac`, then pass `-machine=bluefalcon_gx` (defaults to `boatmobile1`.)

`--swmode`: Run the game using the built-in software rasterizer if passed otherwise runs in OpenGL if omitted (this flag is SDL2 only and may be slow even in 640x480, see `-renderscale` for details)

`-renderscale=<integer-percentage-value>`: Runs the game at a specified percentage of the window's resolution, then upscales the result to the main window (`-renderscale=50` is useful for the software rasterizer to get more performance in software mode, defaults to `100`)


# Controls:

-Up Arrow/Gamepad "A" button: Throttle

-Down Arrow/Gamepad "B" buttom: Brake

-W/Gamepad left-stick forward: Pitch down

-S/Gamepad left-stick backward: Pitch up

-F/Gamepad "Y" button: Boost

-Escape: Exit the demo

-Tilde key: Toggles the drop-down console

-Q/Gamepad right-stick forward: Move the camera forwards relative to the car

-Z/Gamepad right-stick backward: Move the camera backwards relative to the car




