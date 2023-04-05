g++ ./gl45rw/stb_vorbis.c -c
g++ ./stb_vorbis.o ./main.cpp -o ./main_sdl2 -g -Ofast -lm -lopenal -lSDL2 -ldl -DGL45RW_WITH_OPENGL=gles3 -DGL45RW_WITH_SW3D3 -DGL45RW_WINDOW_PROVIDER=SDL2 -DGL45RW_NO_TOUCHSCREEN_CODE -DGL45RW_WITH_OPENAL -DGL45RW_WITH_STB_VORBIS
