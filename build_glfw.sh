g++ ./gl45rw/stb_vorbis.c -c
g++ ./stb_vorbis.o ./main.cpp -o ./main_glfw -g -O2 -lm -lopenal -lglfw -ldl -DGL45RW_WITH_OPENGL=gles3 -DGL45RW_WINDOW_PROVIDER=GLFW3 -DGL45RW_NO_TOUCHSCREEN_CODE -DGL45RW_WITH_OPENAL -DGL45RW_WITH_STB_VORBIS
