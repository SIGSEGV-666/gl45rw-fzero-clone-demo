#ifndef __GL45RW_HPP_INCLUDED
#define __GL45RW_HPP_INCLUDED

#include <random>

#if (defined(_WIN32) || defined(_WIN64))
 #pragma message("[GL45RW] The target OS is Microsoft Windows (win32)")
 #define GL45RW_HOST_WIN32
 #include <windows.h>
#elif (defined(__linux__))
 #pragma message("[GL45RW] The target OS is either a Linux distro, WSL/WSL2, or is using the Linux kernel (e.g. Android)")
 #define GL45RW_HOST_LINUX
 #include <sys/types.h>
 #include <sys/time.h>
#else
 #error "couldn't determine the host OS at compile-time."
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image.h"
#include "./stb_image_write.h"

#ifdef GL45RW_WITH_STB_VORBIS
 #define STB_VORBIS_HEADER_ONLY
 #include "./stb_vorbis.c"
#endif

#define headless 0
#define gl2 1
#define gles3 2

#ifdef GL45RW_WITH_OPENAL
    #pragma message("[GL45RW] Building with OpenAL sound support enabled")
    #include "AL/al.h"
    #include "AL/alc.h"
    #include "AL/alext.h"
#endif

/*
#ifndef GL45RW_RENDERER
 #define GL45RW_RENDERER gl2
#endif

#if (GL45RW_RENDERER == headless)
 #define _GL45RW_HEADLESS
 #pragma message("[GL45RW] Building in headless-only mode (everything except for gfxapi::none is disabled)")
#elif (GL45RW_RENDERER == gl2)
 #define _GL45RW_GL2
#elif (GL45RW_RENDERER == gles3)
 #define _GL45RW_GLES3
#else
 #error "Bad value for GL45RW_RENDERER!"
#endif
*/

#ifdef GL45RW_WITH_OPENGL
    #if (GL45RW_WITH_OPENGL == gl2)
     #define _GL45RW_GL2
    #elif (GL45RW_WITH_OPENGL == gles3)
     #define _GL45RW_GLES3
    #else
     #error "Bad value for GL45RW_WITH_OPENGL!"
    #endif

    #ifndef GL45RW_GL2_MAXLIGHTS
      #define GL45RW_GL2_MAXLIGHTS 16
    #endif

    #ifndef GL45RW_GLES3_MAXFWDLIGHTS
     #define GL45RW_GLES3_MAXFWDLIGHTS 10
    #endif
#endif

#undef headless
#undef gl2
#undef gles3

#define GL45RW_MAX_ENTITIES 8192

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtx/intersect.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/component_wise.hpp"
#include "glm/gtx/extended_min_max.hpp"
#include "glm/gtx/closest_point.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/io.hpp"

#ifdef GL45RW_WITH_SW3D2
 #pragma message("[GL45RW] The gfxapi::sw3d2 rendering backend is enabled and will try to use dj_sw3d20.hpp")
 #include "./dj_sw3d20.hpp"
#endif

#ifdef GL45RW_WITH_SW3D3
 #pragma message("[GL45RW] The gfxapi::sw3d3 rendering backend is enabled and will try to use dj_sw3d3.hpp")
 #include "./dj_sw3d3.hpp"
#endif

#if (!defined(GL45RW_WITH_OPENGL) && !defined(GL45RW_WITH_SW3D2) && !defined(GL45RW_WITH_SW3D3))
 #define _GL45RW_HEADLESS
 #pragma message("[GL45RW] Building in headless-only mode.")
#endif

#define SDL2 1
#define GLFW3 2

#ifndef GL45RW_WINDOW_PROVIDER
    #if (!defined(_GL45RW_HEADLESS))
        #define GL45RW_WINDOW_PROVIDER SDL2
    #else
        #define GL45RW_WINDOW_PROVIDER 0
    #endif
#endif

#if (GL45RW_WINDOW_PROVIDER == SDL2)
    #define _GL45RW_WINDOW_SDL2
    #pragma message("[GL45RW] Building with SDL2 as the window provider.")
#elif (GL45RW_WINDOW_PROVIDER == GLFW3)
    #define _GL45RW_WINDOW_GLFW3
    #pragma message("[GL45RW] Building with GLFW3 as the window provider.")
#else
    #pragma message("[GL45RW] Building without a window provider.")
#endif

#undef SDL2
#undef GLFW3


#if (defined(_GL45RW_GL2))
 #pragma message("[GL45RW] The gfxapi::opengl rendering backend is enabled and will try to use OpenGL 2.x")
 #define GLAD_GL_IMPLEMENTATION
 #include "./glad.h"
 #define __GL45RW_HAS_OPENGL
#elif (defined(_GL45RW_GLES3))
 #pragma message("[GL45RW] The gfxapi::opengl rendering backend is enabled and will try to use OpenGL ES 3.x")
 #define GLAD_GL_IMPLEMENTATION
 #include "./glad.h"
 #define __GL45RW_HAS_OPENGL
#endif

#ifndef _GL45RW_HEADLESS
 #if (defined(_GL45RW_WINDOW_SDL2))
  #define SDL_MAIN_HANDLED
  #include "SDL2/SDL.h"
  #include "SDL2/SDL_hints.h"
  #if (defined(_GL45RW_GL2))
   #include "SDL2/SDL_opengl.h"
  #elif (defined(_GL45RW_GLES3))
   #include "SDL2/SDL_opengles2.h"
  #endif
 #elif (defined(_GL45RW_WINDOW_GLFW3))
  //#define GLFW_INCLUDE_NONE
  //#define GLFW_INCLUDE_GLEXT
  #define GLFW_JOYSTICK_HAT_BUTTONS GLFW_TRUE
  #include <GLFW/glfw3.h>
 #endif
#endif


#define DJUTIL_NEEDS_containers
#define DJUTIL_NEEDS_fixedpoint
#define DJUTIL_NEEDS_ezstr
#define DJUTIL_NEEDS_dmd
#define DJUTIL_NEEDS_binio
#define DJUTIL_NEEDS_imaging
#define DJUTIL_IMAGING_USE_STBI_LOAD
#define DJUTIL_IMAGING_USE_STBI_WRITE
//#define DJUTIL_NEEDS_uvm
#define DJUTIL_NEEDS_virfs
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <exception>
#include <array>

#include <deque>

#include "djutil.hpp"


#ifdef GL45RW_WITH_LUAJIT
 #pragma message("[GL45RW] Building with LuaJIT support enabled")
 //#include "lua.hpp"
 #include <luajit-2.1/lua.hpp>
#endif

namespace gl45rw {

    namespace _default_bitmap_font {
        static const int dims[2] = {160,160};
        static const unsigned char packed_bits[] = ""
            "\x0\x0\x0\x0\x0\x20\x0\x0\x2\x0\xff\x3\xf0\x3f\x0\x0\x80\x0\x0\x0\x0\xf0\xc3\xf\x77\x70\xc0\x1\x7\x0\xff\x3\xf0\x3f\x78\x78\x80\x81\x1f\xc\x0\x8\xe4\x9f\xff\xf8\xe0\x83\xf\x0\xff\xe3\x71\x38\x70\xcc\x80\x83\x9f\x6d\x0\x28\x65\x9b"
            "\xff\xfc\xc1\xc1\x1f\xc\xcf\x33\x33\x33\x78\xcc\x80\x82\x10\x1e\x0\x8\xe4\x9f\xff\xfe\xf3\xe7\x3f\x1e\x87\x13\xb2\x37\x5f\xcc\x80\x82\x90\x73\x0\xe8\x65\x18\x7f\xfc\xf9\xef\x3f\x1e\x87\x13\xb2\xb7\x19\x78\x80\x80\x98\x73\x0\xc8\xe4\x1c\x3e\xf8\xb0\xc6"
            "\x1a\xc\xcf\x33\x33\xb3\x19\x30\xe0\xc0\x1c\x1e\x0\x10\xc2\xf\x1c\x70\x80\x0\x2\x0\xff\xe3\x71\xb8\x19\xfc\xf0\xe0\x88\x6d\x0\xe0\x81\x7\x8\x20\xc0\x1\x7\x0\xff\x3\xf0\x3f\xf\x30\x60\x40\x0\xc\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xff\x3"
            "\xf0\x3f\x0\x0\x0\x0\x0\x0\x8\x0\x2\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x18\x0\x3\x3\x33\xfc\xe1\x3\x0\xc\x30\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x38\x80\x83\x7\x33\xb6\x31\x0\x0\x1e\x78\xc0\x0\x6\x6\x0"
            "\x20\x2\x82\xff\x78\xc0\xc3\xf\x33\xb6\xe1\x3\x0\x3f\xfc\xc0\x0\xc\x3\x0\x30\x6\x7\x7f\xf8\xe0\x3\x3\x33\xbc\x31\x6\x0\xc\x30\xc0\xe0\x9f\x7f\x6\xf8\x8f\xf\x3e\x78\xc0\x3\x3\x33\xb0\xe1\xe3\x1f\xc\x30\xc0\x0\xc\x3\x6\x30\xc6\x1f\x1c"
            "\x38\x80\xc3\xf\x0\xb0\x1\xe6\x1f\x3f\x30\xf0\x3\x6\x6\x6\x20\xe2\x3f\x8\x18\x0\x83\x7\x33\xb0\xe1\xe3\x1f\x1e\x30\xe0\x1\x0\x0\xfe\x1\x0\x0\x0\x8\x0\x2\x3\x0\x0\x0\x0\x0\xc\x30\xc0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x0\xc0\xff\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x20\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x80\xd\x36\xf8\x30\x86\x3\x1c\x60\x60\x0\x0\x0\x0\x0\x0\x0\x60\x0\xe0\x81\xd\x36\xac\x31\xc3"
            "\xc\xc\x30\xc0\xc0\xc\xc\x0\x0\x0\x0\x30\x0\xe0\x81\x8d\xff\x2c\x80\xc1\xc\x6\x18\x80\x81\x7\xc\x0\x0\x0\x0\x18\x0\xc0\x0\x0\x36\xf8\xc0\x80\x1f\x0\x18\x80\xe1\x1f\x3f\x0\xf0\x3\x0\xc\x0\xc0\x0\x80\xff\xa0\x61\xc6\xc\x0\x18\x80"
            "\x81\x7\xc\x0\x0\x0\x0\x6\x0\x0\x0\x0\x36\xac\x31\xc6\xc\x0\x30\xc0\xc0\xc\xc\x30\x0\x0\x3\x3\x0\xc0\x0\x0\x36\xf8\x0\x80\x1b\x0\x60\x60\x0\x0\x0\x30\x0\x0\x83\x1\x0\x0\x0\x0\x0\x20\x0\x0\x0\x0\x0\x0\x0\x0\x0\x18"
            "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xf8\xc0\x80\xf\x3e\xe0\xf0\x7\xf\x7f\xf8\xe0\x3\x0\x0\xc0\x0\x80\x1\x1e"
            "\x8c\xe1\xc0\x18\x63\xf0\x30\x80\x1\x63\x8c\x31\x6\x3\xc\x60\x0\x0\x3\x33\xcc\xc1\x0\x18\x60\xd8\x30\xc0\x0\x60\x8c\x31\x6\x3\xc\x30\xf0\x3\x6\x30\xfc\xc1\x0\xf\x38\xcc\xf0\xc3\xf\x30\xf8\xe0\x7\x0\x0\x18\x0\x0\xc\x30\x9c\xc1\x80\x1"
            "\x60\xfc\x1\xc6\x18\x18\x8c\x1\x6\x0\x0\x30\xf0\x3\x6\x18\x8c\xc1\xc0\x18\x63\xc0\x30\xc6\x18\xc\x8c\x1\x3\x3\xc\x60\x0\x0\x3\xc\xf8\xf0\xc3\x1f\x3e\xe0\xe1\x83\xf\xc\xf8\xe0\x1\x3\xc\xc0\x0\x80\x1\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x0\x0\x0\x6\x0\x0\x0\x0\xc\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xfc\xe0\xc3\xf\x3e\x7c\xf0\xc7\x1f\x3e\x8c\xe1"
            "\x1\xe\x63\xc\x18\xc6\x18\x3e\x86\x31\xc6\x18\x63\xcc\x30\xc0\x0\x63\x8c\xc1\x0\xc\x63\xc\x38\xc7\x19\x63\xe6\x31\xc6\x18\x3\x8c\x31\xc0\x0\x3\x8c\xc1\x0\xc\x33\xc\xf8\xc7\x1b\x63\xb6\xf1\xc7\xf\x3\x8c\xf1\xc3\xf\x73\xfc\xc1\x0\xc\x1f\xc"
            "\xd8\xc6\x1e\x63\xe6\x31\xc6\x18\x3\x8c\x31\xc0\x0\x63\x8c\xc1\xc0\xc\x33\xc\x18\xc6\x1c\x63\x6\x30\xc6\x18\x63\xcc\x30\xc0\x0\x63\x8c\xc1\xc0\xc\x63\xc\x18\xc6\x18\x63\xfc\x30\xc6\xf\x3e\x7c\xf0\xc7\x0\x3e\x8c\xe1\x81\x7\x63\xfc\x19\xc6\x18\x3e"
            "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xfc\xe0\xc3\xf"
            "\x3e\xfe\x31\xc6\x98\x61\x86\x19\xc6\x1f\x1e\x6\xe0\x1\x2\x0\x8c\x31\xc6\x18\x63\x30\x30\xc6\x98\x61\xcc\x18\x6\xc\x6\xc\x80\x1\x7\x0\x8c\x31\xc6\x18\x3\x30\x30\xc6\x98\x61\x78\x30\x3\x6\x6\x18\x80\x81\xd\x0\xfc\x30\xc6\xf\x3e\x30\x30\xc6"
            "\x98\x6d\x30\xe0\x1\x3\x6\x30\x80\xc1\x18\x0\xc\x30\xc6\xc\x60\x30\x30\x86\x8d\x7f\x78\xc0\x80\x1\x6\x60\x80\x1\x0\x0\xc\x30\xc7\x18\x63\x30\x30\x6\x87\x73\xcc\xc0\xc0\x0\x6\xc0\x80\x1\x0\x0\xc\xe0\xc3\x18\x3e\x30\xe0\x3\x82\x61\x86\xc1"
            "\xc0\x1f\x1e\x0\xe0\x1\x0\x0\x0\x0\x7\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\xff\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x0\x38\x0\xc0\x0\x0\x80\x1\x0\xf\x0\xc\xc0\x0\x6\x3\x38\x0\x0\x0\x0\x30\x0\xc0\x0\x0\x80\x1\x80\x1\x0\xc\x0\x0\x0\x3\x30\x0\x0\x0\x0\x60\xe0\xc3\xf\x3e\xf8\xe1\x83\x1\x7e\xfc\xe0\x0\x7\x63\x30\x30\xc3\xf\x3e"
            "\x0\x0\xc6\x18\x63\x8c\x31\xc6\x7\x63\x8c\xc1\x0\x6\x33\x30\xf8\xc7\x18\x63\x0\xe0\xc7\x18\x3\x8c\xf1\x87\x1\x63\x8c\xc1\x0\x6\x1f\x30\xd8\xc6\x18\x63\x0\x30\xc6\x18\x63\x8c\x31\x80\x1\x63\x8c\xc1\x0\x6\x33\x30\xd8\xc6\x18\x63\x0\xe0\xc7\xf"
            "\x3e\xf8\xe1\x83\x1\x7e\x8c\xe1\x1\x6\x63\xe0\x18\xc6\x18\x3e\x0\x0\x0\x0\x0\x0\x0\x0\x0\x60\x0\x0\x0\x6\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x3e\x0\x0\x80\x3\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x0\x0\x0\x0\x30\x0\x0\x0\x0\x0\x0\x0\x0\x0\x18\x0\x0\x0\x0\x0\x0\x0\x0\x1c\x30\xe0\x0\x0\x8\x0\x0\x0\x0\x0\x18\x0\x0\x0\x0\x0\x0\x0\x0\x6\x30\x80\x1\x0\x1c\xfc\xe0\xc7\xf\x7e\x7c\x30\xc6\x98\x61\x8c\x31"
            "\xc6\xf\x6\x30\x80\xc1\x1\x36\x8c\x31\xc6\x18\x3\x18\x30\xc6\x98\x6d\xd8\x30\x6\x6\x3\x0\x0\x63\x1b\x63\x8c\x31\xc6\x0\x3e\x18\x30\x86\x8d\x6d\x70\x30\x6\x3\x6\x30\x80\x1\xe\x63\xfc\xe0\xc7\x0\x60\x18\x30\x6\x87\x7f\xd8\xe0\x87\x1\x6\x30"
            "\x80\x1\x0\x63\xc\x0\xc6\x0\x3f\xf0\xe0\x7\x2\x33\x8c\x1\xc6\xf\x1c\x30\xe0\x0\x0\x7f\xc\x0\xe\x0\x0\x0\x0\x0\x0\x0\x0\xe0\x3\x0\x0\x30\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x6\x1c\x0\xc0\x0\x7\x0\x70\x0\x0\x3\x0\x38\x60\x0\x0\x1c\xf8\x60\x3\x3\x36\xd8\x80\x81\xd\x0\xd8\x60\x3\x6\x1b\x6c\xc0\x80\xd\x36\x8c\x1\x0\x0\x0\x0\x0\x0\x7\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x1c\xc\x30\x86\xf"
            "\x3e\xf8\xe0\x83\xf\x3e\xf8\xe0\x83\xf\xe\x38\xe0\x80\xf\x3e\xc\x30\xc6\x18\x60\x80\x1\x6\x18\x63\x8c\x31\xc6\x18\xc\x30\xc0\xc0\x18\x63\xc\x30\xc6\x1f\x7e\xf8\xe1\x87\x1f\x3\xfc\xf1\xc7\x1f\xc\x30\xc0\xc0\x1f\x7f\x8c\x31\xc6\x0\x63\x8c\x31\xc6"
            "\x18\x63\xc\x30\xc0\x0\xc\x30\xc0\xc0\x18\x63\xf8\xe0\x87\xf\x7e\xf8\xe1\x87\x1f\x3e\xf8\xe0\x83\xf\x1e\x78\xe0\xc1\x18\x63\x30\x0\x0\x0\x0\x0\x0\x0\x0\xc\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x18\x0\x0\x0\x0\x0\x0\x0\x0\x6\x0\x0"
            "\x0\x0\x0\x0\x0\x0\x0\x0\x60\x0\x0\x0\x1c\x0\xc0\x0\x7\xc\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x30\x0\x80\x1f\x36\xd8\x80\x81\xd\x18\xd8\x60\x83\xd\x0\xf0\x18\xe6\xf\x38\x0\x0\xc0\x6\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x8\x98"
            "\x19\x66\x18\xc\xfc\x71\x67\x6\x3e\xf8\xe0\xc3\x18\x63\x8c\xe1\xc3\x18\x3e\x18\x30\x63\x1b\xc\xc\xc0\xed\x1f\x63\x8c\x31\xc6\x18\x63\x8c\x31\xc6\x18\x6b\x7c\xe0\x61\x1b\x1e\xfc\xf0\x6f\x6\x63\x8c\x31\xc6\x18\x63\x8c\x31\xc6\x18\xb\x18\xf0\xe3\xf\xc"
            "\xc\xd8\x61\x6\x63\x8c\x31\xc6\x18\x63\xf8\x31\xc6\x18\x6b\x18\xc0\x60\x3\xc\xfc\x71\x67\x1e\x3e\xf8\xe0\x83\x1f\x7e\x80\xe1\x83\x1f\x3e\xc\xf0\x63\x3\xc\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xf8\x0\x0\x0\x8\xfc\xc1\x60\xe\x7\x0\x0\x0\x0"
            "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x60\xc0\x0\x6\x18\xb8\xe1\x6\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x30\x60\x0\x3\xc\xec\xb0\x83\x7\x1e\x30\x0\x0\x0\x41\x4\xc1\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\xc\x33\x0\x0\x0\x0\x21\x84\x0\x80\x99\x19\xf8\xe0\x80\xf\x63\xfc\x70\x86\xf\x33\x30\x0\x0\x0\x11\x44\xc0\xc0\xc\x33\x80\xc1\xc0\x18\x63\x8c\xf1\xc6\xc\x33\x18\xf0\xc7\x1f\x9\x24\xc0\x60\x6\x66\xf8\xc1\xc0\x18\x63\x8c\xb1\x87\xf\x1e\xc\x30"
            "\x0\x18\x34\x50\xe1\xc1\xc\x33\x8c\xc1\xc0\x18\x63\x8c\x31\x7\x0\x0\xc\x30\x0\x18\x62\x48\xe1\x81\x99\x19\xf8\xe1\x81\xf\x7e\x8c\x31\x6\x0\x0\xcc\x30\x0\x18\x31\xc4\xc1\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x78\x0\x0\x80\x70\x2"
            "\x1\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x44\xa8\xda\x1d\xc\x30\xc0\xc0\xc\x0\x0\x30\xc3\xc\x0\xcc\x30\x83\x1\x0\x11\x55\x75\x37\xc\x30\xc0\xc0\xc\x0\x0\x30\xc3\xc\x0\xcc\x30\x83\x1\x0"
            "\x44\xa8\xda\x1d\xc\x30\xfc\xc0\xc\x0\x3f\x3c\xc3\xcc\x3f\xcf\x30\xf3\x1\x0\x11\x55\x75\x37\xc\x30\xfc\xc0\xc\x0\x3f\x3c\xc3\xcc\x3f\xcf\x30\xf3\x1\x0\x44\xa8\xda\x1d\xc\x3f\xc0\xf0\xcc\x3f\x30\x0\xc3\xc\x30\xc0\xfc\x83\xc1\xf\x11\x55\x75\x37"
            "\xc\x3f\xc0\xf0\xcc\x3f\x30\x0\xc3\xc\x30\xc0\xfc\x83\xc1\xf\x44\xa8\xda\x1d\xc\x30\xfc\xc0\xc\x33\x3f\x3c\xc3\xcc\x33\xff\x0\xf0\x1\xc\x11\x55\x75\x37\xc\x30\xfc\xc0\xc\x33\x3f\x3c\xc3\xcc\x33\xff\x0\xf0\x1\xc\x44\xa8\xda\x1d\xc\x30\xc0\xc0"
            "\xc\x33\x30\x30\xc3\xc\x33\x0\x0\x0\x0\xc\x11\x55\x75\x37\xc\x30\xc0\xc0\xc\x33\x30\x30\xc3\xc\x33\x0\x0\x0\x0\xc\x30\xc0\x0\x0\xc\x0\xc0\x0\x3\x33\xcc\x0\xc0\xc\x0\xcc\x0\xc0\xc\xc\x30\xc0\x0\x0\xc\x0\xc0\x0\x3\x33\xcc\x0"
            "\xc0\xc\x0\xcc\x0\xc0\xc\xc\x30\xc0\x0\x0\xc\x0\xc0\x0\x3f\x33\xcc\xf3\xff\xfc\xff\xcc\xff\xff\xfc\xff\x30\xc0\x0\x0\xc\x0\xc0\x0\x3f\x33\xcc\xf3\xff\xfc\xff\xcc\xff\xff\xfc\xff\xf0\xff\xff\x3f\xfc\xff\xff\xf\x3\xf3\xc\x30\x0\x0\x0\xc"
            "\x0\x0\x0\x0\xf0\xff\xff\x3f\xfc\xff\xff\xf\x3\xf3\xc\x30\x0\x0\x0\xc\x0\x0\x0\x0\x0\x0\x0\x3\xc\x0\xc0\x0\x3f\x33\xfc\x33\xff\xff\xf3\xcc\xff\xff\xfc\xff\x0\x0\x0\x3\xc\x0\xc0\x0\x3f\x33\xfc\x33\xff\xff\xf3\xcc\xff\xff\xfc\xff"
            "\x0\x0\x0\x3\xc\x0\xc0\x0\x3\x33\x0\x30\x3\x0\x33\xcc\x0\xc0\xc\x0\x0\x0\x0\x3\xc\x0\xc0\x0\x3\x33\x0\x30\x3\x0\x33\xcc\x0\xc0\xc\x0\xcc\x0\x0\x0\x33\x30\x0\x0\x0\x33\x30\xc0\x0\xc0\xff\x0\x7c\x0\xfe\xff\xcc\x0\x0\x0"
            "\x33\x30\x0\x0\x0\x33\x30\xc0\x0\xc0\xff\x0\x7c\x0\xfe\xff\xcc\xfc\xf\x0\x33\xf0\xc3\xf\x0\x33\xff\xc3\x0\xc0\xff\x0\x7c\x0\xfe\xff\xcc\xfc\xf\x0\x33\xf0\xc3\xf\x0\x33\xff\xc3\x0\xc0\xff\x0\x7c\x0\xfe\xff\xff\x3\xf0\x3f\xff\x30\xc0\xc0"
            "\xff\xf3\x0\xfc\x0\xff\xff\x0\x7c\x0\xfe\xff\xff\x3\xf0\x3f\xff\x30\xc0\xc0\xff\xf3\x0\xfc\x0\xff\xff\xff\x7f\x0\x3e\x0\x0\xfc\xcf\xc\x0\xf0\xc3\xcf\xc\x33\xff\x3\x0\xc3\xff\xff\x7f\x0\x3e\x0\x0\xfc\xcf\xc\x0\xf0\xc3\xcf\xc\x33\xff\x3"
            "\x0\xc3\xff\xff\x7f\x0\x3e\x0\x0\xc0\xc0\xc\x0\x0\xc0\xc0\xc\x33\x30\x0\x0\xc3\xff\xff\x7f\x0\x3e\x0\x0\xc0\xc0\xc\x0\x0\xc0\xc0\xc\x33\x30\x0\x0\xc3\xff\xff\x7f\x0\x3e\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0"
            "\x0\x0\x0\x0\x0\xe0\xc1\x1f\x0\xfc\x1\x0\x0\x0\x78\xe0\x81\xf\x3e\x0\x0\x0\x0\x3e\x0\x30\xc3\x98\x7f\x18\x0\x0\x0\x0\x30\x30\xc3\x18\xc\x0\x0\x6\x0\x63\xb8\x31\xc3\x0\x33\x30\xe0\x67\xc\x3f\xfc\x30\xc3\x18\x18\xdc\xf1\x83\xf\x63"
            "\xec\xb0\xc1\x0\x33\x60\xb0\x61\xc\xc\xb6\xf1\xc3\x18\x3e\x66\x9b\xc7\x0\x63\x4c\x30\xc3\x0\x33\x30\x30\x63\xe\xc\xb6\x31\xc3\x18\x63\x26\xdb\x86\x7\x63\xec\x30\xc6\x0\x33\x18\x30\xe3\x1b\xc\xfc\x30\x83\xd\x63\x36\x7b\xc6\x0\x63\xb8\xb1\xc3\x0"
            "\x33\xfc\xe1\x61\x0\x18\x30\xe0\xc1\x1d\x3e\xdc\xf1\x83\xf\x63\x0\x30\x0\x0\x0\x0\x0\x60\x0\x0\x78\x0\x0\x0\x0\x0\x18\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0"
            "\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\xc0\x0\x6\x6\xe0\xc0\x0\x0\x7\x70\x0\x0\x0\xf8\x7c\xe0\x1\x0\x0\xfc\xc0\x0\x3\xc\xb0\xc1\x0\x83\x6d\xd8\x0\x0\x0\x18\xcc\x0\x3\x0\x0\x0\xf0\x83\x1\x18\xb0\xc1\x0\x0\x38\xd8\xc0"
            "\x0\x0\x18\xcc\xc0\x81\x7\x0\xfc\xc0\x0\x3\xc\x30\xc0\xc0\xf\x0\x70\xc0\x0\x83\x19\xcc\x60\x80\x7\x0\x0\xc0\x0\x6\x6\x30\xc0\x0\x0\x7\x0\x0\x0\x0\x1b\xcc\xe0\x83\x7\x0\xfc\x0\x0\x0\x0\x30\xd8\x0\x83\x6d\x0\x0\x0\x0\x1e\x0"
            "\x0\x80\x7\x0\x0\xf0\xc3\xf\x3f\x30\xd8\x0\x0\x38\x0\x0\x0\x0\x1c\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x30\x70\x0\x0\x0\x0\x0\x0\x0\x18\x0\x0\x0\x0\x0\x0\x0\x0\x0\x0\x30\x0\x0\x0\x0\x0\x0\x0\x0\x10\x0\x0\x0\x0\x0"
        "";
    };

    namespace dmd = djutil::dmd;
    namespace virfs = djutil::virfs;
    namespace ezstr = djutil::ezstr;
    namespace imaging = djutil::imaging;
    namespace ufpio = djutil::ufpio;
    namespace binio = djutil::binio;

    using virfs::fspath_t;

    using clockticks_t = size_t;

    #if (defined(GL45RW_HOST_WIN32))
        const clockticks_t _clock_ticks_per_second = 1e3;
        clockticks_t GetClockTicks() {return timeGetTime();}
    #elif (defined(GL45RW_HOST_LINUX))
        const clockticks_t _clock_ticks_per_second = 1e6;
        clockticks_t GetClockTicks() {
            struct timeval tv; gettimeofday(&tv, nullptr);
            return (clockticks_t(tv.tv_sec) * _clock_ticks_per_second) + clockticks_t(tv.tv_usec);
        }
    #endif

    bool _globally_initialized = false;

    using fscalar_t = float;
    using fclock_t = fscalar_t;
    const fclock_t _clock_resolution = fclock_t(1)/_clock_ticks_per_second;

    inline fclock_t GetTimeForTicks(const clockticks_t t){return t * _clock_resolution;}

    using fvec2_t = glm::vec<2,fscalar_t,glm::defaultp>;
    using fvec3_t = glm::vec<3,fscalar_t,glm::defaultp>;
    using fvec4_t = glm::vec<4,fscalar_t,glm::defaultp>;

    template <glm::length_t L> using fvecL_t = glm::vec<L,fscalar_t,glm::defaultp>;

    using fmat22_t = glm::mat<2,2,fscalar_t,glm::defaultp>;
    using fmat33_t = glm::mat<3,3,fscalar_t,glm::defaultp>;
    using fmat44_t = glm::mat<4,4,fscalar_t,glm::defaultp>;

    using ivec2_t = glm::ivec2;
    using ivec3_t = glm::ivec3;
    using ivec4_t = glm::ivec4;

    using rgb24_t = glm::vec<3, uint8_t>;
    using rgba32_t = glm::vec<4, uint8_t>;

    using uid_t = uint32_t;

    #define GL45RW_MAX_GAMEPADS 8
    #define GL45RW_GAMEPAD_MAXBUTTONS 21
    #define GL45RW_GAMEPAD_NUMAXES 6

    enum struct sgphysshape : int {
        none = 0,
        trimesh = 1
    };

    enum struct sgphystype : int {
        none = 0,
        static_collision = 1
    };

    std::string _fmt_with_linenums(const std::string& s) {
        std::stringstream oss;
        unsigned int cline = 1;
        std::vector<std::string> lines = {}; djutil::ezstr::splitlines<char>(lines, s);
        std::string nls = std::to_string(lines.size());
        std::string lnt;
        for (auto& ln : lines){
            lnt.resize(nls.size(), ' ');
            sprintf(&lnt.front(), "%u", cline);
            oss << lnt << " | " << ln << "\n";
            cline++;
        }
        return oss.str();
    }

    namespace _parsestuff {
        using ezstr::u8skipws;
        using ezstr::u8reads;
        using ezstr::parse_strlit;

    };

    namespace _mathstuff {
        //template <glm::length L, glm::qualifier Q> using _fvecLQ = glm::vec<L,fscalar_t,Q>;

        inline fscalar_t gmi(const fscalar_t c, const fscalar_t t, const fscalar_t a, const fscalar_t ft) {
            return c+((t-c)*(1.0f - glm::pow(glm::clamp(a, 0.0f, 1.0f), ft)));
        }

        template <glm::length_t L, glm::qualifier Q=glm::defaultp>
        #define _gmi_fvec glm::vec<L, fscalar_t, Q>
        inline _gmi_fvec gmi(const _gmi_fvec& c, const _gmi_fvec& t, const _gmi_fvec& a, const fscalar_t ft) {
            return c+((t-c)*(_gmi_fvec(1.0f) - glm::pow(glm::clamp(a, _gmi_fvec(0.0f), _gmi_fvec(1.0f)), _gmi_fvec(ft))));
        }
        #undef _gmi_fvec

        inline fscalar_t sdist_fromplane(const fvec3_t& co, const fvec3_t& po, const fvec3_t& pn){
            return glm::dot(glm::normalize(pn), co-po);
        }

        void axis_angle_between(fvec3_t& out_axis, fscalar_t& out_angle, const fvec3_t& v1, const fvec3_t& v2, const bool angle_in_radians){
            const fvec3_t
                a = glm::normalize(v1),
                b = glm::normalize(v2)
            ;
            out_axis = glm::cross(a, b);
            out_angle = acos(glm::dot(a, b));
            out_angle = (angle_in_radians ? out_angle : glm::degrees(out_angle));
        }

        fvec3_t cart2bary_tri3D(const fvec3_t& p, const fvec3_t& a, const fvec3_t& b, const fvec3_t& c){
            const fscalar_t invarea2 = 1.0f/glm::length(glm::cross(b-a, c-a));
            fvec3_t bary = {
                glm::length(glm::cross(p-b, p-c))*invarea2,
                glm::length(glm::cross(p-c, p-a))*invarea2,
                0.0f
            };
            bary.z = 1.0f - bary.x - bary.y;
            return bary;
        }

        template <class VT>
        VT wsum3(const fvec3_t& w, const VT& a, const VT& b, const VT& c){
            return (a*(w.x))+(b*(w.y))+(c*(w.z))/(w.x+w.y+w.z);
        }
        /*
        https://blackpawn.com/texts/pointinpoly/

        function SameSide(p1,p2, a,b)
            cp1 = CrossProduct(b-a, p1-a)
            cp2 = CrossProduct(b-a, p2-a)
            if DotProduct(cp1, cp2) >= 0 then return true
            else return false

        function PointInTriangle(p, a,b,c)
            if SameSide(p,a, b,c) and SameSide(p,b, a,c)
                and SameSide(p,c, a,b) then return true
            else return false
        */
        inline bool same_side(const fvec3_t& p1, const fvec3_t& p2, const fvec3_t& a, const fvec3_t& b){
            return (glm::dot(glm::cross(b-a, p1-a), glm::cross(b-a, p2-a)) >= 0.0f);
        }
        bool point_in_tri3d(const fvec3_t& p, const fvec3_t& a, const fvec3_t& b, const fvec3_t& c){
            return (same_side(p,a,b,c) && same_side(p,b,a,c) && same_side(p,c,a,b));
        }
        fvec3_t closest_point_on_line(const fvec3_t& p, const fvec3_t& a, const fvec3_t& b){
            fvec3_t pn = glm::normalize(b-a);
            fscalar_t ll = glm::distance(b, a), sd = sdist_fromplane(p, a, pn);
            return a + (pn * glm::clamp(sd, 0.0f, ll));
        }

        struct aabb3d_t {
            fvec3_t
                pmin = fvec3_t(INFINITY),
                pmax = fvec3_t(-INFINITY)
            ;

            const fvec3_t& operator[](const int i) const {return (&(this->pmin))[i];}
            fvec3_t& operator[](const int i) {return (&(this->pmin))[i];}

            inline fvec3_t centroid() const {return (this->pmin + this->pmax) * 0.5f;}
            inline fvec3_t dims() const {return this->pmax - this->pmin;}
            inline fscalar_t volume() const {return glm::compMul(this->dims());} //computes the volume of the AABB (length*width*height)
            fscalar_t area() const {
                //computes the sum of all the six faces' surface areas
                //a.k.a. the AABB's total surface area
                const fvec3_t d = this->dims();
                return (2*d.y*d.z)+(2*d.x*d.z)+(2*d.x*d.y);
            }

            aabb3d_t& add_point_ip(const fvec3_t& p) {
                this->pmin = glm::min(this->pmin, p);
                this->pmax = glm::max(this->pmax, p);
                return *this;
            }

            inline aabb3d_t add_point(const fvec3_t& p) const {
                return aabb3d_t(*this).add_point_ip(p);
            }

            aabb3d_t& add_tri_ip(const fvec3_t& pa, const fvec3_t& pb, const fvec3_t& pc){
                this->pmin = glm::min(this->pmin, pa, pb, pc);
                this->pmax = glm::max(this->pmax, pa, pb, pc);
                return *this;
            }

            inline aabb3d_t add_tri(const fvec3_t& pa, const fvec3_t& pb, const fvec3_t& pc) const {
                return aabb3d_t(*this).add_tri_ip(pa,pb,pc);
            }

            aabb3d_t& rebox_ip() { //ensures pmin and pmax are in the right order.
                const fvec3_t
                    a = this->pmin,
                    b = this->pmax
                ;
                this->pmin = glm::min(a, b);
                this->pmax = glm::max(a, b);
                return *this;
            }

            inline aabb3d_t rebox() const {
                return aabb3d_t(*this).rebox_ip();
            }

            inline bool contains_point(const fvec3_t& p) const {
                return glm::all(glm::greaterThanEqual(p, this->pmin)) &&
                       glm::all(glm::lessThanEqual(p, this->pmax))
                ;
            }

            inline bool is_overlapping(const aabb3d_t& other) const {
                return glm::all(glm::lessThanEqual(this->pmin, other.pmax)) &&
                       glm::all(glm::greaterThanEqual(this->pmax, other.pmin))
                ;
            }

            aabb3d_t& operator=(const aabb3d_t& other) {
                this->pmin = other.pmin;
                this->pmax = other.pmax;
                return *this;
            }
            fvec3_t constrain(const fvec3_t& p) const {
                return glm::clamp(p, this->pmin, this->pmax);
            }
            bool operator==(const aabb3d_t& other) const {
                return glm::all(glm::equal(this->pmin, other.pmin)) && glm::all(glm::equal(this->pmax, other.pmax));
            }

            friend std::ostream& operator<<(std::ostream& ostr, const aabb3d_t& self) {
                ostr << "aabb3d_t{" << glm::to_string(self.pmin) << ", " << glm::to_string(self.pmax) << '}';
                return ostr;
            }

            aabb3d_t() {}
            aabb3d_t(const fvec3_t& a, const fvec3_t& b) : pmin(a), pmax(b) {}
            aabb3d_t(const aabb3d_t& other) : pmin(other.pmin), pmax(other.pmax) {}

        };

        struct plane3d_t {
            fvec3_t
                point = {0,0,0},
                normal = {0,0,1}
            ;

            inline fvec3_t getnormal() const {return glm::normalize(this->normal);}

            plane3d_t& operator=(const plane3d_t& other) {
                this->point = other.point;
                this->normal = other.normal;
                return *this;
            }

            bool operator==(const plane3d_t& other) const {
                return glm::all(glm::equal(this->point, other.point)) && glm::all(glm::equal(this->normal, other.normal));
            }

            friend std::ostream& operator<<(std::ostream& ostr, const plane3d_t& self){
                ostr << "plane3d_t{" << glm::to_string(self.point) << ", " << glm::to_string(self.normal) << "}";
                return ostr;
            }

            plane3d_t() {}
            plane3d_t(const fvec3_t& p, const fvec3_t& n) : point(p), normal(n) {}
            plane3d_t(const plane3d_t& o) : point(o.point), normal(o.normal) {}
        };

        struct tri3d_t;
        struct raytrires3d_t {
            bool frontal;
            fvec3_t
                wuv,
                pos,
                normal,
                ognormal
            ;
            fscalar_t t;
        };

        struct rayboxres3d_t {
            int nhits = 0;
            struct {
                fscalar_t distance;
                fvec3_t pos;
            } hits[2] = {};
        };

        struct ray3d_t {
            fvec3_t
                origin = {0,0,0},
                dir = {0,0,-1}
            ;

            inline fvec3_t getdir() const {
                return glm::normalize(this->dir);
            };

            ray3d_t& operator=(const ray3d_t& other) {
                this->origin = other.origin;
                this->dir = other.dir;
                return *this;
            }

            bool operator==(const ray3d_t& other) const {
                return glm::all(glm::equal(this->origin, other.origin)) && glm::all(glm::equal(this->dir, other.dir));
            }

            friend std::ostream& operator<<(std::ostream& ostr, const ray3d_t& self){
                ostr << "ray3d_t{" << glm::to_string(self.origin) << ", " << glm::to_string(self.dir) << "}";
                return ostr;
            }

            fvec3_t perpvec_to(const fvec3_t& pt) const {
                fvec3_t
                    rdir = this->getdir(),
                    pdir = glm::normalize(pt - this->origin),
                    ndir = glm::cross(rdir, pdir)
                ;
                return glm::normalize(glm::cross(ndir, rdir));
            }

            fscalar_t signed_distance_to(const fvec3_t& pt) const {
                const fvec3_t pv = this->perpvec_to(pt);
                return glm::dot(pv, pt - this->origin);
            }
            bool intersect(raytrires3d_t& res, const tri3d_t& tri, const fscalar_t maxdist=INFINITY) const;

            bool intersect(rayboxres3d_t& res, const aabb3d_t& box, const fscalar_t maxdist=INFINITY) const {
                fscalar_t tmin, tmax, tymin, tymax, tzmin, tzmax;
                const fvec3_t
                    orig = this->origin,
                    dir = this->getdir(),
                    invdir = fvec3_t(1)/dir,
                    *bounds = &box.pmin
                ;
                const ivec3_t
                    rsign = ivec3_t(glm::lessThan(invdir, fvec3_t(0)))
                ;
                tmin = (bounds[rsign[0]].x - orig.x) * invdir.x;
                tmax = (bounds[1-rsign[0]].x - orig.x) * invdir.x;
                tymin = (bounds[rsign[1]].y - orig.y) * invdir.y;
                tymax = (bounds[1-rsign[1]].y - orig.y) * invdir.y;
                if ((tmin > tymax) || (tymin > tmax)){return false;}

                if (tymin > tmin){tmin = tymin;}
                if (tymax < tmax){tmax = tymax;}

                tzmin = (bounds[rsign[2]].z - orig.z) * invdir.z;
                tzmax = (bounds[1-rsign[2]].z - orig.z) * invdir.z;

                if ((tmin > tzmax) || (tzmin > tmax)){return false;}

                if (tzmin > tmin){tmin = tzmin;}
                if (tzmax < tmax){tmax = tzmax;}

                //if ((res.distance = tmin) < 0 && (res.distance = tmax) < 0){
                if (tmin < 0 && tmax < 0){
                    return false;
                }
                /*
                if (res.distance > maxdist){return false;}
                res.tmin = tmin;
                res.tmax = tmax;
                res.pos = orig + (dir * res.distance);
                res.normal = -dir;
                */
                const fscalar_t tees[2] = {tmin, tmax};
                for (int ti = 0; ti < 2; ti++){
                    if (tees[ti] < 0){continue;}
                    auto& hitres = res.hits[res.nhits];
                    hitres.distance = tees[ti];
                    hitres.pos = orig+(dir*tees[ti]);
                    res.nhits += 1;
                }
                return true;
            }

            ray3d_t() {}
            ray3d_t(const fvec3_t& ro, const fvec3_t& rd) : origin(ro), dir(rd) {}
            ray3d_t(const ray3d_t& other) : origin(other.origin), dir(other.dir) {}

        };

        static const std::array<int, 2> _TRI_EDGEVERT_INDICES[3] = {
            {0, 1},
            {1, 2},
            {2, 0}
        };

        struct tri3d_t {
            std::array<fvec3_t, 3> vertices = {};

            inline bool get_edgeverts(const fvec3_t*& a, const fvec3_t*& b, const int idx) const {
                if (idx < 0 || idx > 2){a = nullptr; b = nullptr; return false;}
                const auto& evi = _TRI_EDGEVERT_INDICES[idx];
                a = &(this->vertices[evi[0]]);
                b = &(this->vertices[evi[1]]);
                return true;
            }

            inline bool get_edgeverts(fvec3_t*& a, fvec3_t*& b, const int idx) {
                if (idx < 0 || idx > 2){a = nullptr; b = nullptr; return false;}
                const auto& evi = _TRI_EDGEVERT_INDICES[idx];
                a = &(this->vertices[evi[0]]);
                b = &(this->vertices[evi[1]]);
                return true;
            }

            inline bool get_edgeverts(fvec3_t& a, fvec3_t& b, const int idx) const {
                if (idx < 0 || idx > 2){return false;}
                const auto& evi = _TRI_EDGEVERT_INDICES[idx];
                a = (this->vertices[evi[0]]);
                b = (this->vertices[evi[1]]);
                return true;
            }

            inline fvec3_t facenormal() const {return glm::normalize(glm::cross(this->vertices[1] - this->vertices[0], this->vertices[2] - this->vertices[0]));}
            inline fvec3_t centroid() const {return (this->vertices[0] + this->vertices[1] + this->vertices[2]) / fvec3_t(3);}
            void get_edge_planes(std::array<plane3d_t, 3>& planes) const {
                const fvec3_t n = this->facenormal();
                for (int i = 0; i < 3; i++){
                    const fvec3_t *ev0, *ev1; this->get_edgeverts(ev0, ev1, i);
                    planes[i] = plane3d_t(ev0[0], glm::normalize(glm::cross(n, ev1[0] - ev0[0])));
                }
            }

            inline bool contains_point(const fvec3_t& p) const {
                return point_in_tri3d(p, this->vertices[0],this->vertices[1],this->vertices[2]);
            }

            fvec3_t closest_point_on_tri(const fvec3_t& p) const {
                const fvec3_t n = this->facenormal();
                const fscalar_t sd = sdist_fromplane(p, this->vertices[0], n);
                const fvec3_t cp1 = p + (n * -sd);
                if (!this->contains_point(cp1)){
                    const aabb3d_t box = this->bbox();
                    fvec3_t ipt; fscalar_t idist = INFINITY, cdist;
                    for (int i = 0; i < 3; i++){
                        fvec3_t ea, eb, ec; this->get_edgeverts(ea, eb, i);
                        ec = closest_point_on_line(cp1, ea, eb);
                        if ((cdist = glm::distance(cp1, ec)) <= idist){ipt = ec; idist = cdist;}
                    }
                    return ipt;
                }
                else {return cp1;}
            }


            inline aabb3d_t bbox() const {
                return aabb3d_t().add_tri_ip(this->vertices[0], this->vertices[1], this->vertices[2]);
            }
            tri3d_t() = default;
            tri3d_t(const fvec3_t& a, const fvec3_t& b, const fvec3_t& c) :
                vertices({a,b,c})
            {}

            tri3d_t(const tri3d_t& o) : vertices(o.vertices) {}

            ~tri3d_t() = default;

        };

        bool ray3d_t::intersect(raytrires3d_t& res, const tri3d_t& tri, const fscalar_t distance) const {
            //https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
            const fvec3_t
                dir = this->getdir(),
                &v0 = tri.vertices[0],
                &v1 = tri.vertices[1],
                &v2 = tri.vertices[2],
                v0v1 = v1 - v0,
                v0v2 = v2 - v0,
                pvec = glm::cross(dir, v0v2)
            ;
            const fscalar_t
                epsilon = FLT_EPSILON
            ;
            fscalar_t
                det = glm::dot(v0v1, pvec),
                invdet = 1 / det,
                &t = res.t,
                u, v
            ;
            if (fabs(det) < epsilon){return false;}
            res.frontal = (det >= 0);
            fvec3_t tvec = this->origin - v0;
            u = glm::dot(tvec, pvec) * invdet;
            if (u < 0 || u > 1){return false;}
            fvec3_t qvec = glm::cross(tvec, v0v1);
            v = glm::dot(dir, qvec) * invdet;
            if (v < 0 || (u+v) > 1){return false;}
            t = glm::dot(v0v2, qvec) * invdet;
            if (t < 0 || t > distance){return false;}
            res.wuv = fvec3_t{1-u-v, u, v};
            //res.pos = wsum3(res.wuv, v0, v1, v2);
            res.pos = this->origin + (dir * t);
            res.ognormal = glm::normalize(glm::cross(v0v1, v0v2));
            res.normal = glm::normalize((res.frontal ? glm::cross(v0v1, v0v2) : glm::cross(v0v2, v0v1)));
            return true;
        }

        struct MollerTrumbore {
            public:
                fvec3_t _in_tri[3] = {};
                fvec3_t _tri_edges[2] = {};
                struct {
                    //fscalar_t a, f, u, v, t;
                    fscalar_t hitdistance;
                    fvec3_t hitpoint, hitnormal, hit_wuv;
                } out = {};
                void set_triangle(const fvec3_t& v0, const fvec3_t& v1, const fvec3_t& v2) {
                    this->_in_tri[0] = v0;
                    this->_in_tri[1] = v1;
                    this->_in_tri[2] = v2;
                    this->_tri_edges[0] = v1-v0;
                    this->_tri_edges[1] = v2-v0;
                }
                bool operator()(const fvec3_t& ro, const fvec3_t& rd, const fscalar_t maxdist=INFINITY) {
                    static const fscalar_t epsilon = FLT_EPSILON;
                    fscalar_t a, f, u, v, t;
                    fvec3_t h, s, q, &edge1 = this->_tri_edges[0], &edge2 = this->_tri_edges[1];
                    a = glm::dot(edge1, (h = glm::cross(rd, edge2)));
                    if (a > -epsilon && a < epsilon){return false;}
                    f = 1/a;
                    s = ro - this->_in_tri[0];
                    u = f * glm::dot(s, h);
                    if (u <= 0 || u >= 1){return false;}
                    q = glm::cross(s, edge1);
                    v = f * glm::dot(rd, q);
                    if (v <= 0 || u+v >= 1){return false;}
                    t = f * glm::dot(edge2, q);
                    if (t > 0.0f && t <= maxdist){
                        this->out.hit_wuv = fvec3_t{1-u-v, u, v};
                        this->out.hitdistance = t;
                        this->out.hitpoint = ro + (rd * t);
                        //this->out.hitnormal = glm::normalize(glm::cross(edge1, edge2)*fscalar_t(a < 0.0f ? -1 : 1));
                        this->out.hitnormal = glm::normalize(glm::cross(edge1, edge2));
                        //if (glm::dot(ro - this->_in_tri[0], this->out.hitnormal) < 0.0f){
                        //    this->out.hitnormal = -this->out.hitnormal;
                        //}
                        return true;
                    }
                    return false;
                }
                bool operator()(const ray3d_t& ray, const fscalar_t maxdist=INFINITY) {
                    return this->operator()(ray.origin, ray.getdir(), maxdist);
                }
        };

    };

    struct _CellMap3D_DefaultUserdata {int _dummy=0;};

    template <class ET, class IFUNC=ET, class UDATA=_CellMap3D_DefaultUserdata>
    class CellMap3D {
        using elem_type = ET;
        using iscalar_type = int;
        using index_type = glm::vec<3, iscalar_type>;
        struct ibox_t {
            index_type a, b;
        };
        //using hash_index_type = std::array<iscalar_type, 3>;
        struct index_hash {
            size_t operator()(index_type const& s) const {
                const std::hash<iscalar_type> ihash;
                return ihash(s.x) * ihash(s.y) % ihash(s.z);
            }
        };
        private:
            struct _elemdata_t {
                ET element;
                std::unordered_set<index_type, index_hash> current_indices;
            };
            index_type _map_celldims = {1,1,1};
            ibox_t _map_totalbounds = {};
            std::vector<_elemdata_t> all_elems;

            std::unordered_map<index_type, std::vector<ET>, index_hash> _mapped_elems;
            index_type _cellidx4coord(const index_type& co) {
                return co/this->_map_celldims;
            }
            void _clear_mapelems() {
                this->_mapped_elems.clear();
                for (_elemdata_t& e : this->all_elems) {
                    e.current_indices.clear();
                }
                this->_recalc_totalbounds();
            }
            void _recalc_totalbounds() {
                this->_map_totalbounds = ibox_t{};
                if (this->all_elems.size() == 0){return;}
                this->_map_totalbounds.a = index_type(INT_MAX);
                this->_map_totalbounds.b = index_type(INT_MIN);

                for (auto& ed : this->all_elems) {
                    for (auto idx : ed.current_indices) {
                        const ibox_t eib = this->aabb4index(idx);
                        this->_map_totalbounds.a = glm::min(this->_map_totalbounds.a, eib.a);
                        this->_map_totalbounds.b = glm::max(this->_map_totalbounds.b, eib.b);
                    }
                }

            }
            int _map_element(const index_type& idx, _elemdata_t& e, const bool _do_totalbounds_recalc=true) {
                auto hidx = idx;
                auto iaabb = this->aabb4index(idx);
                if (!IFUNC::cellmap3d_intersects_aabb(iaabb.a, iaabb.b, e.element, this->userdata)){return -1;}
                try {
                    auto& l = this->_mapped_elems.at(hidx);
                    if (e.current_indices.count(idx) == 0){
                        e.current_indices.insert(idx);
                        l.push_back(e.element);
                    }
                    return 2;
                } catch (const std::exception& _exc) {
                    auto& l = this->_mapped_elems[hidx] = std::vector<ET>();
                    l.push_back(e.element);
                    e.current_indices.insert(idx);
                    if (_do_totalbounds_recalc){
                        this->_recalc_totalbounds();
                    }
                    return 0;
                }
            }
            void _unmap_element(const index_type& idx, _elemdata_t& e, const bool _do_totalbounds_recalc=true) {
                auto hidx = idx;
                try {
                    auto& l = this->_mapped_elems.at(hidx);
                    if (e.current_indices.count(idx) == 1){
                        l.erase(std::find(l.begin(), l.end(), e.element));
                        e.current_indices.erase(idx);
                    }
                    if (l.size() == 0){
                        this->_mapped_elems.erase(hidx);
                        if (_do_totalbounds_recalc){
                            this->_recalc_totalbounds();
                        }
                    }
                } catch (const std::exception& _exc) {
                    ;
                }
                if (e.current_indices.count(idx)){
                    e.current_indices.erase(idx);
                }
            }
            void _remap_elements() {
                this->_clear_mapelems();
                for (auto e : this->all_elems){
                    ibox_t eib = this->_calc_ibox(e.element);
                    index_type idx;
                    for (idx.x=eib.a.x; idx.x <= eib.b.x; idx.x++){
                     for (idx.y=eib.a.y; idx.y <= eib.b.y; idx.y++){
                      for (idx.z=eib.a.z; idx.z <= eib.b.z; idx.z++){
                          this->_map_element(idx, e, false);
                    }}}
                }
                this->_recalc_totalbounds();
            }

            void _add_element(const ET& e) {
                this->all_elems.push_back(_elemdata_t{.element=e});
                auto& ed = this->all_elems.back();
                ibox_t eib = this->_calc_ibox(e);
                index_type idx;
                for (idx.x=eib.a.x; idx.x <= eib.b.x; idx.x++){
                 for (idx.y=eib.a.y; idx.y <= eib.b.y; idx.y++){
                  for (idx.z=eib.a.z; idx.z <= eib.b.z; idx.z++){
                      this->_map_element(idx, ed);
                }}}
            }
            bool _del_element(const ET& e) {
                _elemdata_t* edp = nullptr;
                for (auto& ed : this->all_elems) {
                    if (ed.element == e){edp = &ed; break;}
                }
                if (edp == nullptr){return false;}
                while (edp->current_indices.size() > 0) {
                    this->_unmap_element(edp->current_indices.begin()[0], *edp);
                }
                return true;
            }

        public:
            UDATA userdata;
            ibox_t _calc_ibox(const ET& e) {
                ibox_t ret = {};
                IFUNC::cellmap3d_calc_aabb(ret.a, ret.b, e, this->userdata);
                ret.a /= this->_map_celldims;
                ret.b /= this->_map_celldims;
                return ret;
            }

            ibox_t aabb4index(const index_type& idx) {
                ibox_t aabb;
                aabb.a = this->_map_celldims * idx;
                aabb.b = aabb.a + this->_map_celldims;
                return aabb;
            }
            const ibox_t& totalbounds = _map_totalbounds;
            const index_type& celldims = _map_celldims;
            std::vector<_elemdata_t> const& all_elements = all_elems;
            std::unordered_map<index_type, std::vector<ET>, index_hash> const& mapped_elements = _mapped_elems;
            _elemdata_t& add(const ET& e) {
                this->_add_element(e);
                return this->all_elems.back();
            }
            void remove(const ET& e) {
                this->_del_element(e);
            }

            void remap() {this->_remap_elements();}

            void clear() {
                this->_clear_mapelems();
                this->all_elems.clear();
            }

            index_type index_for_coord(const index_type& coord) {
                return this->_cellidx4coord(coord);
            }

            std::vector<ET>* operator[](const index_type& coord){
                auto idx = this->_cellidx4coord(coord);
                try {
                    return &(this->_mapped_elems.at(idx));
                } catch (const std::exception& e){
                    return nullptr;
                }
            }

            CellMap3D(const index_type& _cell_dims, const UDATA& _userdata) :
                _map_celldims(_cell_dims),
                userdata(_userdata)
            {}
            CellMap3D(const index_type& _cell_dims) : _map_celldims(_cell_dims) {}
            CellMap3D() {}
            ~CellMap3D() {this->clear();}
    };
    /*
    enum struct vartypes : char {
        character = 'c', // char and unsigned char (1 byte)
        integer = 'i', // int32_t, uint32_t and uid_t (4 bytes)
        floating = 'f' // fscalar_t a.k.a. float (4 bytes)
    };
    #define _GL45RW_TRACKVAR_DATASIZE 128
    #define _GL45RW_TRACKVAR_CELLSIZE 4
    #define _GL45RW_TRACKVAR_NCELLS (_GL45RW_TRACKVAR_DATASIZE/_GL45RW_TRACKVAR_CELLSIZE)

    using _trackvarcell_t = std::array<char, _GL45RW_TRACKVAR_CELLSIZE>;
    union _trackvardata_t {
        char flat[_GL45RW_TRACKVAR_DATASIZE];
        _trackvarcell_t cells[_GL45RW_TRACKVAR_NCELLS];
        union {
            char chr[_GL45RW_TRACKVAR_DATASIZE];
            unsigned char uchr[_GL45RW_TRACKVAR_DATASIZE];
        } character;
        union {
            int32_t  sint[_GL45RW_TRACKVAR_DATASIZE / 4];
            uint32_t uint[_GL45RW_TRACKVAR_DATASIZE / 4];
            uid_t uid[_GL45RW_TRACKVAR_DATASIZE / 4];
        } integer;
        union {
            fscalar_t f[_GL45RW_TRACKVAR_DATASIZE / sizeof(fscalar_t)];
        } floating;
    };

    class TrackingVariable {
        public:
            _trackvardata_t _curdata = {}, _prevdata = {};
            bool _markedcells[_GL45RW_TRACKVAR_NCELLS];
        public:
            void unmark_all_cells() {
                std::fill_n(this->_markedcells, _GL45RW_TRACKVAR_NCELLS, false);
                this->_prevdata = this->_curdata;
            }
            void mark_changed_cells() {
                for (size_t i = 0; i < _GL45RW_TRACKVAR_NCELLS; i++) {
                    //this->_markedcells[i] = (memcmp(this->_prevdata.cells[i].data(), this->_curdata.cells[i].data(), _GL45RW_TRACKVAR_CELLSIZE) == 0);
                    this->_markedcells[i] = this->_curdata.cells[i] != this->_prevdata.cells[i];
                }
            }


    };
    */
    template <class E> using vectorlist = djutil::containers::vectorlist<E>;
    template <class K, class V> using dictionary = djutil::containers::dictionary<K,V>;

    //using bytevector = std::vector<uint8_t>;
    class bytevector : public std::vector<uint8_t> {
        using std::vector<uint8_t>::vector;
        public:
            template <class IT>
            size_t add(const IT istart, const IT iend){
                size_t oldsize = this->size();
                for (IT i = istart; i < iend; i++){
                    const auto& v = *i;
                    const auto* vp = reinterpret_cast<const uint8_t*>(&v);
                    this->insert(this->end(), vp, vp+sizeof(v));
                }
                return this->size() - oldsize;
            }
            template <class T>
            size_t add(const T& v){
                return this->add((&v), (&v)+1);
            }
    };
    template <class T>
    size_t bytevector_add(bytevector& bv, const T& v) {
        /*
    	size_t oldsize = bv.size();
    	//bv.insert(bv.end(), (const uint8_t*)&v, (const uint8_t*)((&v)+1));
    	const uint8_t* vp = reinterpret_cast<const uint8_t*>(&v);
    	for (size_t i = 0; i < sizeof(v); i++){bv.push_back(vp[i]);}
    	return bv.size() - oldsize;
    	*/
    	return bv.add(v);
    }
    template <class IT>
    size_t bytevector_add(bytevector& bv, const IT start, const IT end) {
    	return bv.add(start, end);
    }
    enum class inputclass : int {
        unknown = 0,
        key = 1,
        mousebutton = 2,
        mouseaxis = 3,
        joybutton = 4,
        joyaxis = 5
    };
    #ifndef _GL45RW_HEADLESS
     bool _gl45rw_pending_gamepad_device_reenum = true;
     bool _gl45rw_UpdateGamepadStates_firstrun = true;
     #if (defined(_GL45RW_WINDOW_SDL2))
         const uint8_t* _sdl2_keystates = nullptr;
         SDL_GameController* _sdl2_gamepads[GL45RW_MAX_GAMEPADS] = {};
         struct _sdl2_gpstate_t {
            bool buttons[GL45RW_GAMEPAD_MAXBUTTONS];
            fscalar_t axes[GL45RW_GAMEPAD_NUMAXES];
         };
         _sdl2_gpstate_t _sdl2_gamepad_states[GL45RW_MAX_GAMEPADS] = {};
     #elif (defined(_GL45RW_WINDOW_GLFW3))
         struct _glfw3_gpstate_t {
             int joyidx = -1;
             bool buttons[GL45RW_GAMEPAD_MAXBUTTONS];
             fscalar_t axes[GL45RW_GAMEPAD_NUMAXES];
         };
         _glfw3_gpstate_t _glfw3_gamepad_states[GL45RW_MAX_GAMEPADS] = {};
         void _gl45rw_glfw3_joystick_callback(int jid, int event) {
             if (event == GLFW_CONNECTED || event == GLFW_DISCONNECTED){
                 _gl45rw_pending_gamepad_device_reenum = true;
             }
         }

     #endif
    #endif
    struct _gl45rw_common_gpstate_t {
        bool connected = false;
        std::string devname = "";
        int numbuttons = 0, numaxes = 0;
    };

    _gl45rw_common_gpstate_t _gl45rw_common_gamepad_states[GL45RW_MAX_GAMEPADS] = {};

    void _gl45rw_UpdateGamepadStates() {
        #ifndef _GL45RW_HEADLESS
         if (_gl45rw_UpdateGamepadStates_firstrun){
             std::fill_n(_gl45rw_common_gamepad_states, GL45RW_MAX_GAMEPADS, _gl45rw_common_gpstate_t{});
             #if (defined(_GL45RW_WINDOW_SDL2))
             std::fill_n(_sdl2_gamepads, GL45RW_MAX_GAMEPADS, (SDL_GameController*)nullptr);
             std::fill_n(_sdl2_gamepad_states, GL45RW_MAX_GAMEPADS, _sdl2_gpstate_t{});
             #elif (defined(_GL45RW_WINDOW_GLFW3))
             std::fill_n(_glfw3_gamepad_states, GL45RW_MAX_GAMEPADS, _glfw3_gpstate_t{});
             #endif
         }
         if (_gl45rw_pending_gamepad_device_reenum) {
             _gl45rw_pending_gamepad_device_reenum = false;
             for (int i = 0; i < GL45RW_MAX_GAMEPADS; i++){
                 #if (defined(_GL45RW_WINDOW_SDL2))
                 auto& s = _sdl2_gamepad_states[i];
                 auto*& g = _sdl2_gamepads[i];
                 if (g != nullptr){
                     SDL_GameControllerClose(g);
                     g = nullptr;
                 }
                 #endif
             }

             int num = 0, numjs =
             #if (defined(_GL45RW_WINDOW_SDL2))
             SDL_NumJoysticks()
             #elif (defined(_GL45RW_WINDOW_GLFW3))
             GLFW_JOYSTICK_LAST - GLFW_JOYSTICK_1
             #else
             0
             #endif
             ;
             for (int i = 0; i < numjs; i++){
                 if (num >= GL45RW_MAX_GAMEPADS){break;}
                 #if (defined(_GL45RW_WINDOW_SDL2))
                 else if (SDL_IsGameController(i)){
                     _sdl2_gamepads[num] = SDL_GameControllerOpen(i);
                     auto& cgs = _gl45rw_common_gamepad_states[num];
                     cgs.connected = true;
                     const char* _devname = SDL_GameControllerName(_sdl2_gamepads[num]);
                     cgs.devname = (_devname != nullptr ? std::string(_devname) : "NULL");
                     cgs.numbuttons = GL45RW_GAMEPAD_MAXBUTTONS;
                     cgs.numaxes = GL45RW_GAMEPAD_NUMAXES;
                     std::cout << "[INFO] New game controller: " << num << '\n';
                     num++;
                 }
                 #elif (defined(_GL45RW_WINDOW_GLFW3))
                 else if (i > GLFW_JOYSTICK_LAST){break;}
                 else if (glfwJoystickIsGamepad(i)){
                     _glfw3_gamepad_states[num] = _glfw3_gpstate_t{.joyidx=i};
                     auto& cgs = _gl45rw_common_gamepad_states[num];
                     cgs.connected = true;
                     const char* _devname = glfwGetJoystickName(i);
                     cgs.devname = (_devname != nullptr ? std::string(_devname) : "NULL");
                     glfwGetJoystickButtons(i, &cgs.numbuttons); cgs.numbuttons = std::min(cgs.numbuttons, GL45RW_GAMEPAD_MAXBUTTONS);
                     glfwGetJoystickAxes(i, &cgs.numaxes); cgs.numaxes = std::min(cgs.numaxes, GL45RW_GAMEPAD_NUMAXES);
                     std::cout << "[INFO] New game controller: " << num << '\n';
                     num++;
                 }
                 #endif
             }
         }
         for (int i = 0; i < GL45RW_MAX_GAMEPADS; i++){
             #if (defined(_GL45RW_WINDOW_SDL2))
             auto& s = _sdl2_gamepad_states[i];
             std::fill_n(s.buttons, GL45RW_GAMEPAD_MAXBUTTONS, false);
             std::fill_n(s.axes, GL45RW_GAMEPAD_NUMAXES, fscalar_t(0));
             auto*& g = _sdl2_gamepads[i];
             if (g != nullptr && !SDL_GameControllerGetAttached(g)){
                 _gl45rw_pending_gamepad_device_reenum = true;
             }
             else if (g != nullptr){
                 for (int b = 0; b < GL45RW_GAMEPAD_MAXBUTTONS; b++){
                     s.buttons[b] = SDL_GameControllerGetButton(g, (SDL_GameControllerButton)b);
                 }
                 for (int a = 0; a < GL45RW_GAMEPAD_NUMAXES; a++){
                     s.axes[a] = glm::clamp(SDL_GameControllerGetAxis(g, (SDL_GameControllerAxis)a)/fscalar_t(32767), fscalar_t(-1), fscalar_t(1));
                 }
             }
             #elif (defined(_GL45RW_WINDOW_GLFW3))
             auto& s = _glfw3_gamepad_states[i];
             std::fill_n(s.buttons, GL45RW_GAMEPAD_MAXBUTTONS, false);
             std::fill_n(s.axes, GL45RW_GAMEPAD_NUMAXES, fscalar_t(0));
             int idx = s.joyidx;
             if (idx == -1){continue;}
             else {
                 if (!glfwJoystickIsGamepad(idx)){_gl45rw_pending_gamepad_device_reenum = true; continue;}
                 int btncount = 0, axiscount = 0;
                 const unsigned char* btns = glfwGetJoystickButtons(idx, &btncount);
                 for (int b = 0; b < btncount && b < GL45RW_GAMEPAD_MAXBUTTONS; b++){
                     s.buttons[b] = (btns[b] == GLFW_PRESS);
                 }
                 const float* axes = glfwGetJoystickAxes(idx, &axiscount);
                 for (int a = 0; a < axiscount && a < GL45RW_GAMEPAD_NUMAXES; a++){
                     s.axes[a] = fscalar_t(axes[a]);
                 }
             }
             #endif
         }
         _gl45rw_UpdateGamepadStates_firstrun = false;
        #endif
    }

    struct inputeventinfo_t {
        std::string strname = "";
        inputclass iclass = inputclass::unknown;
        #ifndef _GL45RW_HEADLESS
         #if (defined(_GL45RW_WINDOW_SDL2))
         SDL_Scancode keycode = SDL_SCANCODE_UNKNOWN;
         //const uint8_t* _sdl2keystateptr = nullptr;
         SDL_GameControllerAxis joyaxis = SDL_CONTROLLER_AXIS_INVALID;
         SDL_GameControllerButton joybtn = SDL_CONTROLLER_BUTTON_INVALID;
         #elif (defined(_GL45RW_WINDOW_GLFW3))
         int keycode = GLFW_KEY_UNKNOWN;
         int joybtn = 0, joyaxis = 0;
         #endif
         int joyindex = 0;
         bool joyaxisfwd = true;
        #endif
        int mouseaxisno = 0; //0 = mouse motion x, 1 = mouse motion y
        bool mouseaxisfwd = true;
        int mousebtn = 0;

        operator fscalar_t() const {
            #ifndef _GL45RW_HEADLESS
            switch (this->iclass)
            {
                case inputclass::key: {
                    #if (defined(_GL45RW_WINDOW_SDL2))
                    return fscalar_t(bool(_sdl2_keystates[this->keycode]));
                    #else
                    return fscalar_t(0);
                    #endif
                }
                case inputclass::joybutton: {
                    if (this->joyindex < 0 || this->joyindex >= 8){return fscalar_t(0);}
                    #if (defined(_GL45RW_WINDOW_SDL2))
                    return fscalar_t(int(_sdl2_gamepad_states[this->joyindex].buttons[int(this->joybtn)]));
                    #elif (defined(_GL45RW_WINDOW_GLFW3))
                    return fscalar_t(int(_glfw3_gamepad_states[this->joyindex].buttons[this->joybtn]));
                    #else
                    return fscalar_t(0);
                    #endif
                }
                case inputclass::joyaxis: {
                    if (this->joyindex < 0 || this->joyindex >= 8){return fscalar_t(0);}
                    #if (defined(_GL45RW_WINDOW_SDL2))
                    fscalar_t oval = _sdl2_gamepad_states[this->joyindex].axes[int(this->joyaxis)];
                    #elif (defined(_GL45RW_WINDOW_GLFW3))
                    fscalar_t oval = _glfw3_gamepad_states[this->joyindex].axes[this->joyaxis];
                    #else
                    fscalar_t oval = 0;
                    #endif
                    return glm::clamp((this->joyaxisfwd ? oval : -oval), fscalar_t(0), fscalar_t(1));
                }
                default: break;
            }
            #endif
            return fscalar_t(0);
        }
        bool operator==(const inputeventinfo_t& other) const {
            return (this->iclass == other.iclass) && (this->strname == other.strname);
        }
    };
    const inputeventinfo_t _null_input_event_info;
    std::unordered_map<std::string, inputeventinfo_t>* _input_event_map = nullptr;

    const inputeventinfo_t& E_GetInputInfo(const std::string& name)
    {
        if (_input_event_map == nullptr || _input_event_map->count(name) == 0)
        {
            return _null_input_event_info;
        }
        else
        {
            return _input_event_map[0][name];
        }
    }

    template <class CHR>
    int split_argstr(vectorlist<std::basic_string<CHR>>& out, const std::basic_string<CHR>& in) {
        typedef std::basic_string<CHR> str_t;

        if (in.size() == 0){return 0;}
        //int tokcount = 1;
        int oldsize = out.size();
        str_t curtok; curtok.resize(0);
        bool inquote = false;
        CHR quotechr;
        int advby = 1;
        #define _pusharg() if (curtok.size() > 0){out.push_back(curtok); curtok.resize(0);}
        for (auto it = in.cbegin(); it != in.cend(); it+=advby, advby=1) {
            const CHR curchr = *it;
            if (curchr == (CHR)'\\'){
                const CHR escaped = it[1];
                advby = 2;
                if (
                    escaped == (CHR)'\'' ||
                    escaped == (CHR)'"'  ||
                    escaped == (CHR)'\\' ||
                    escaped == (CHR)' '
                ){curtok.push_back(escaped); advby = 2;}
                //else if (escaped == (CHR)'r'){curtok.push_back((CHR)'\r'); advby = 2;}
                //else if (escaped == (CHR)'n'){curtok.push_back((CHR)'\n'); advby = 2;}
                continue;
            }
            else if (!inquote && (curchr == (CHR)' ' || curchr == (CHR)'\n' || curchr == (CHR)'\r' || curchr == (CHR)'\t')){
                _pusharg();
                continue;
            }
            else if ((curchr == (CHR)'\'' || curchr == (CHR)'"')) {
                if (!inquote) {
                    quotechr = curchr;
                    inquote = true;
                }
                else if (inquote && curchr != quotechr) {
                    curtok.push_back(curchr);
                }
                else if (inquote && curchr == quotechr) {
                    inquote = false;
                }
                continue;
            }
            curtok.push_back(curchr);
        }
        _pusharg();
        #undef _pusharg
        return int(out.size())-oldsize;
    }

    enum struct argprefix : int {
		dash = 1, // -<name>
		plus = 2, // +<name>
		double_dash = 3, // --<name>
		none = 0
	};
	enum struct argsyntax : int {
		no_value, // [argprefix]<name>
		name_ws_value = 1, // [argprefix]<name> <value>
		name_equal_value = 2 // [argprefix]<name>=<value>
	};
	template <class CT>
	struct parsedarg_t {
		int tokenidx = 0;
		argprefix prefix;
		std::basic_string<CT>
			name,
			value
		;
	};

	template <class CT>
	class ArgumentParser {
		using char_type = CT;
		using string_type = std::basic_string<CT>;
		private:
			vectorlist<string_type> _curtokens;
			int _curargnum;
			argsyntax _syntax_for_prefixes[4] = {
				argsyntax::no_value,
				argsyntax::name_ws_value,
				argsyntax::name_ws_value,
				argsyntax::no_value
			};
		public:
			ArgumentParser& set_argsyntax(const argprefix p, const argsyntax s) {
				this->_syntax_for_prefixes[(int)p] = s;
				return *this;
			}
			ArgumentParser& load_argstr(const string_type& argstr) {
				this->_curtokens.clear();
				gl45rw::split_argstr<CT>(this->_curtokens, argstr);
				this->_curargnum = this->_curtokens.size();
				return *this;
			}
			template <typename IT>
			ArgumentParser& load_tokens(IT start, IT end){
				this->_curtokens.clear();
				std::copy(start, end, std::back_inserter(this->_curtokens));
				this->_curargnum = this->_curtokens.size();
				return *this;
			}

			bool adv(parsedarg_t<CT>& hnd) const {
				if (hnd.tokenidx >= this->_curargnum){return false;}
				hnd.prefix = argprefix::none;
				hnd.name = string_type();
				hnd.value = string_type();
				const CT eqsign_cs[2] = {CT('='), CT(0)};
				const string_type equalsign = eqsign_cs;
				auto carg = this->_curtokens[hnd.tokenidx]; hnd.tokenidx++;
				if (carg.size() >= 2 && carg[0] == CT('-') && carg[1] == CT('-')){
					hnd.prefix = argprefix::double_dash;
					carg.erase(carg.begin());
					carg.erase(carg.begin());
				}
				else if (carg.size() >= 1){
					if (carg[0] == CT('-')){hnd.prefix = argprefix::dash; carg.erase(carg.begin());}
					else if (carg[0] == CT('+')){hnd.prefix = argprefix::plus; carg.erase(carg.begin());}
				}

				switch (this->_syntax_for_prefixes[(int)hnd.prefix]) {
					case argsyntax::name_ws_value: {
						if (hnd.tokenidx >= this->_curargnum){return false;}
						hnd.name = carg;
						hnd.value = this->_curtokens[hnd.tokenidx]; hnd.tokenidx++;
						break;
					}
					case argsyntax::name_equal_value: {
						djutil::ezstr::partition<CT>(hnd.name, hnd.value, carg, equalsign);
						break;
					}
					case argsyntax::no_value: {
						hnd.name = carg;
						hnd.value = carg;
						break;
					}
					default: ;
				}
				return true;
			}

			ArgumentParser() {}
			ArgumentParser(const string_type& argstr) {this->load_argstr(argstr);}
	};

    enum class gfxapi : int {
        none = 0,
        opengl = 1,
        sw3d2 = 2,
        sw3d3 = 3
    };

    enum class sndapi : int {
        none = 0,
        openal = 1
    };

    static const gfxapi _default_gfxapi = gfxapi::none;
    static const sndapi _default_sndapi = sndapi::none;

    enum class rendercap : int {
        unknown = 0,
        depth_test = 1,
        texture_2d = 2,
        texture_cube_map = 3,
        blending = 4
    };

    enum class cullface : int {
        disabled = 0,
        back = 1,
        front = 2
    };

    enum class cubeface : int {
        px = 0,
        nx = 1,
        py = 2,
        ny = 3,
        pz = 4,
        nz = 5
    };

    enum class texfilter : int {
        nearest = 0,
        linear = 1,
        nearest_mipmap_nearest = 2,
        nearest_mipmap_linear = 3,
        linear_mipmap_nearest = 4,
        linear_mipmap_linear = 5
    };

    enum class texwrap : int {
        repeat = 0,
        clamp_to_edge = 1
    };

    #ifdef GL45RW_WITH_SW3D2
     static const dj_sw3d20::enum_t _RENDERCAP_TO_SW3D2[] = {
         dj_sw3d20::enum_t::NONE,
         dj_sw3d20::enum_t::CAP_DEPTH_TEST,
         dj_sw3d20::enum_t::NONE,
         dj_sw3d20::enum_t::NONE,
         dj_sw3d20::enum_t::CAP_BLENDING
     };
     static const dj_sw3d20::enum_t _TEXFILTER_TO_SW3D2[] = {
         dj_sw3d20::enum_t::NEAREST,
         dj_sw3d20::enum_t::LINEAR,

         dj_sw3d20::enum_t::LINEAR,
         dj_sw3d20::enum_t::LINEAR,
         dj_sw3d20::enum_t::LINEAR,
         dj_sw3d20::enum_t::LINEAR
     };
     static const dj_sw3d20::enum_t _TEXWRAP_TO_SW3D2[] = {
         dj_sw3d20::enum_t::REPEAT,
         dj_sw3d20::enum_t::CLAMP_TO_EDGE
     };
     static const dj_sw3d20::enum_t _TEXBLEND_TO_SW3D2[] = {
         dj_sw3d20::enum_t::BLEND_MIX,
         dj_sw3d20::enum_t::BLEND_ADD,
         dj_sw3d20::enum_t::BLEND_MUL
     };
    #endif

    #ifdef GL45RW_WITH_SW3D3
     static const sw3d3::texwrap_e _TEXWRAP_TO_SW3D3[] = {
         sw3d3::texwrap_e::repeat,
         sw3d3::texwrap_e::clamp_to_edge
     };
    #endif

    #ifdef __GL45RW_HAS_OPENGL
     static const GLenum _RENDERCAP_TO_GL[] = {
         GL_NONE,
         GL_DEPTH_TEST,
         GL_TEXTURE_2D,
         GL_TEXTURE_CUBE_MAP,
         GL_BLEND
     };

     static const GLenum _CUBEFACE_TO_GL[] = {
         GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
         GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
         GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
     };

     static const GLenum _TEXFILTER_TO_GL[] = {
         GL_NEAREST, GL_LINEAR,
         GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
         GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
     };

     static const GLenum _TEXWRAP_TO_GL[] = {
         GL_REPEAT,
         GL_CLAMP_TO_EDGE
     };

     inline GLenum _rendercap2gl(const rendercap rc)
     {
         return _RENDERCAP_TO_GL[(int)rc];
     }
     inline GLenum _cubeface2gl(const cubeface cf) {
         return _CUBEFACE_TO_GL[(int)cf];
     }
     inline GLenum _texfilter2gl(const texfilter tf) {
         return _TEXFILTER_TO_GL[(int)tf];
     }
    #endif
    const int _GLVERSION[2] = {
        #if (defined(_GL45RW_GL2))
            2, 1
        #elif (defined(_GL45RW_GLES3))
            3, 1
        #else
            0, 0
        #endif
    };

    #ifndef GL45RW_WITHOUT_SW_RENDERER
    namespace sw_renderer {
        enum struct ctxobjtype : int {
            none = 0,
            texture = 1,
            material = 2,
            framebuffer = 3
        };
        class SWContext;
        class SWContextObjectBase {
            private:
                SWContext* _ctx = nullptr;
                uid_t _id = 0;
            protected:
            public:
                virtual ctxobjtype objtype() const {return ctxobjtype::none;}
                SWContextObjectBase(SWContext* _context, const uid_t _newuid);
                ~SWContextObjectBase();
        };
        class SWContext {
            private:
                dictionary<uid_t, SWContextObjectBase*> _objsbyid = {};
                uid_t _cur_objuid = 1;
                uid_t _current_fbo = 0;
            public:
                fmat44_t
                    model_matrix = fmat44_t(1),
                    view_matrix = fmat44_t(1),
                    projection_matrix = fmat44_t(1)
                ;
        };
        SWContextObjectBase::SWContextObjectBase(SWContext* _context, const uid_t _newid){}
        SWContextObjectBase::~SWContextObjectBase(){}
    };
    #endif

    struct boolstate {
        bool current = false, last = false;
        int state() const {
            return (this->last ? (this->current ? 1 : 3) : (this->current ? 2 : 0));
        }

        bool is_false() const {return this->state() == 0;}
        bool is_true() const {return this->state() == 1;}
        bool just_true() const {return this->state() == 2;}
        bool just_false() const {return this->state() == 3;}

        int operator=(const bool value)
        {
            this->last = this->current;
            this->current = value;
            return this->state();
        }

    };

    struct videomode_t {
        gfxapi renderer = _default_gfxapi;
        int width = 640, height = 480;
        std::u32string title = U"untitled window";
        int sw3d2_resolution_percent = 50;
        bool fullscreen = false;
        int opengl_resolution_percent = 100;
        fscalar_t gl_gamma = 1;
        //std::string opengl_libname = "";
        bool
            opengl_linear_upscale = false,
            opengl_linear_downscale = false
        ;
        int sw3d3_resolution_percent = 50;
    };

    struct soundmode_t {
        sndapi renderer = _default_sndapi;
        int output_device_idx = -1;
    };

    template <class ARGS>
	class ActionStrip;

	template <class ARGS>
	class ActionStripItem {
		private:
			ActionStrip<ARGS>* _strip = nullptr;
		public:
			ActionStrip<ARGS>* const& actstrip = _strip;
			virtual bool step(const ARGS& args) {
				return false;
			}
			ActionStripItem(ActionStrip<ARGS>* _a) : _strip(_a) {}
			virtual ~ActionStripItem() {std::cout << "~ActionStripItem()\n";}
	};
	template <class ARGS>
	class ActionStrip {
		private:
			ActionStripItem<ARGS>* _cur = nullptr;
			std::deque<ActionStripItem<ARGS>*> _queue;
			template <class C, typename... Args>
			C* _new_item(Args... args) {
				return new C(this, args...);
			}
			bool _getnext() {
				delete this->_cur; this->_cur = nullptr;
				if (this->_queue.size() == 0){return false;}
				this->_cur = this->_queue.front(); this->_queue.pop_front();
				return true;
			}
		public:
			template <class C, typename... Args>
			C* additem_beg(Args... args) {
				C* e = this->_new_item<C>(args...);
				this->_queue.push_front(e);
				return e;
			}

			template <class C, typename... Args>
			C* additem_ate(Args... args) {
				C* e = this->_new_item<C>(args...);
				this->_queue.push_back(e);
				return e;
			}

			bool update(const ARGS& args) {
				if (this->_cur == nullptr){
					if (!this->_getnext()){
						return false;
					}
				}
				else if (!this->_cur->step(args)){
					//delete this->_cur; this->_cur = nullptr;
				    if (!this->_getnext()){return false;}
					//this->_getnext();
				}
				return true;
			}
			ActionStrip() {}
			~ActionStrip() {
				while (this->_getnext()){;}
			}
    };




    class EngineInstance;
    void _register_engine_instance(EngineInstance* ei);
    void _remove_engine_instance(EngineInstance* ei);

    void _register_instance_window(const uint32_t wid, EngineInstance* ei);
    void _unregister_instance_window(const uint32_t wid);

    #ifdef _GL45RW_WINDOW_GLFW3
        void _register_instance_glfw_window(GLFWwindow* gwin, EngineInstance* ei);
        void _unregister_instance_glfw_window(GLFWwindow* gwin);
        void _glfw_window_close_callback(GLFWwindow* gwin);
    #endif

    struct MainloopData {
        fscalar_t ticdelta = 0, uptime = 0;
    };

    template <glm::length_t L, class T, glm::qualifier Q=glm::defaultp>
    struct _matstackLTQ {
        using matrix_type = glm::mat<L,L,T,Q>;
        public:
            std::vector<matrix_type> _stack;
            inline size_t size() const {return this->_stack.size();}

            inline const matrix_type& operator[](const int idx) const {
                return *((idx >= 0 ? this->_stack.cbegin() : this->_stack.cend())+idx);
            }
            inline matrix_type& operator[](const int idx) {
                return *((idx >= 0 ? this->_stack.begin() : this->_stack.end())+idx);
            }

            inline operator matrix_type() const {return matrix_type(this->_stack.back());}

            inline void loadIdentity() {
                this->_stack.back() = matrix_type(T(1));
            }

            bool popMatrix() {
                if (this->size() > 1){this->_stack.pop_back(); return true;}
                return false;
            }

            inline void multMatrix(const matrix_type& m) {
                matrix_type& top = this->_stack.back();
                top = m * top;
            }

            inline void pushMatrix() {
                this->_stack.push_back(matrix_type(this->_stack.back()));
            }
            inline void resetStack() {
                this->_stack.resize(1);
                this->_stack.back() = matrix_type(T(1));
            }
            _matstackLTQ(const size_t _num_reserve=32) {
                this->_stack.reserve(_num_reserve);
                this->resetStack();
            }

            _matstackLTQ(const _matstackLTQ<L,T,Q>& o) : _stack(o._stack) {}

    };

    inline fvec2_t _dmdvec2glm(const dmd::dvector2_t& dv) {
        return fvec2_t{
            fscalar_t(dv[0]),
            fscalar_t(dv[1])
        };
    }
    inline fvec3_t _dmdvec2glm(const dmd::dvector3_t& dv) {
        return fvec3_t{
            fscalar_t(dv[0]),
            fscalar_t(dv[1]),
            fscalar_t(dv[2])
        };
    }
    inline fvec4_t _dmdvec2glm(const dmd::dvector4_t& dv) {
        return fvec4_t{
            fscalar_t(dv[0]),
            fscalar_t(dv[1]),
            fscalar_t(dv[2]),
            fscalar_t(dv[3])
        };
    }

    const fscalar_t _FST_ONE = 1;
    struct trs_t {
        fvec3_t pos = {0,0,0};
        fmat33_t orn = fmat33_t(_FST_ONE);
        fvec3_t scale = {1,1,1};
        inline operator fmat44_t() const {
            /*
            return fmat44_t{
                fvec4_t(this->orn[0] * this->scale.x, 0),
                fvec4_t(this->orn[1] * this->scale.y, 0),
                fvec4_t(this->orn[2] * this->scale.z, 0),
                fvec4_t(this->pos, 1)
            };
            */
            const fmat44_t
                t = glm::translate(this->pos),
                r = fmat44_t(this->orn),
                s = glm::scale(this->scale)
            ;
            return t*r*s;
            /*
            fmat44_t m(1);
            m = glm::translate(m, this->pos);
            m = glm::scale(m, this->scale);
            m *= fmat44_t(this->orn);
            return m;
            */
        }
        inline trs_t& identity() {
            this->pos = fvec3_t(0);
            this->scale = fvec3_t(1);
            this->orn = fmat33_t(1);
            return *this;
        }
        inline trs_t& rotate(const fvec3_t& axis, const fscalar_t angle, const bool localaxis=false)
        {
            this->orn = fmat33_t(glm::rotate(
                glm::radians(angle),
                glm::normalize((localaxis ? (this->orn * axis) : axis))
            )) * this->orn;
            return *this;
        }
        inline trs_t& translate(const fvec3_t& dv, const bool local=false)
        {
            this->pos += (local ? (this->orn * dv) : dv);
            return *this;
        }
        inline fvec3_t getaxisvect(const fvec3_t& v) const
        {
            return this->orn * v;
        }
        inline trs_t& align_axis_to_vec(const fvec3_t& axis, const fvec3_t& vec, const fscalar_t damp=1, const fscalar_t t=1) {
            const fvec3_t waxis = glm::normalize(this->orn * axis);
            const fvec3_t nvec = glm::normalize(vec);
            fscalar_t angle;
            fvec3_t raxis;
            _mathstuff::axis_angle_between(raxis, angle, waxis, nvec, false);
            if (glm::any(glm::isnan(raxis)) || glm::any(glm::isinf(raxis)) || glm::isNull(raxis, FLT_EPSILON)){return *this;}
            if (!isfinite(angle) || (angle == 0.0f)){return *this;}
            return this->rotate(raxis, (damp >= 1.0f ? angle : _mathstuff::gmi(0.0f, angle, 1.0f - damp, t)));
        }
        inline trs_t& fps_look(const fscalar_t turnamount, const fscalar_t lookamount, const fscalar_t dt=fscalar_t(1))
        {
            this->rotate({0,0,1}, turnamount*dt);
            this->rotate({1,0,0}, lookamount*dt, true);
            return *this;
        }

        trs_t transform(const trs_t& local) const {
            const fmat44_t m = *this;
            return trs_t(
                fvec3_t(m * fvec4_t(local.pos, 1)),
                fmat33_t(this->orn*(local.orn)),
                this->scale * local.scale
            );
        }

        trs_t() {}
        trs_t(const fvec3_t& position, const fmat33_t& orientation, const fvec3_t& scaling) :
            pos(position),
            scale(scaling),
            orn(orientation)
        {}
    };



    struct fmat44stack : public _matstackLTQ<4,fscalar_t> {
        using parent_type = _matstackLTQ<4,fscalar_t>;
        using _matstackLTQ<4,fscalar_t>::_matstackLTQ;
        using translation_vector = fvec3_t;

        fmat44stack& translate(const translation_vector& t)
        {
            this[0][-1] = glm::translate(this[0][-1], t);
            return *this;
        }
        fmat44stack& rotate(const translation_vector& _axis, const fscalar_t _angle, const bool _angle_in_radians=false)
        {
            this[0][-1] = glm::rotate(this[0][-1], (_angle_in_radians ? _angle : glm::radians(_angle)), glm::normalize(_axis));
            return *this;
        }
        fmat44stack& scale(const translation_vector& _scale)
        {
            //this->multMatrix(glm::scale(_scale));
            this[0][-1] = glm::scale(this[0][-1], _scale);
            return *this;
        }

        fmat44stack& perspective(const fscalar_t fov, const fscalar_t aspect, const fscalar_t znear=0.1, const fscalar_t zfar=100.0)
        {
            this[0][-1] = glm::perspectiveRH_NO(glm::radians(fov), aspect, znear, zfar);
            return *this;
        }
        fmat44stack& perspectiveX(const fscalar_t fov_x, const fscalar_t aspect, const fscalar_t znear=0.1, const fscalar_t zfar=100.0)
        {
            return this->perspective(fabs(fov_x / aspect), aspect, znear, zfar);
        }

        class automatrix_t {
            private:
                fmat44stack* _stack = nullptr;
                size_t _eloc = 0;
            public:
                fmat44stack* const& matstack = _stack;
                const size_t& element_index = _eloc;

                operator fmat44_t() const {return this->_stack[0][this->_eloc];}
                fmat44_t& operator=(const fmat44_t& m) {
                	return (this->_stack[0][this->_eloc] = m);
                }

                automatrix_t(fmat44stack& s) : _stack(&s) {
                	this->_eloc = s.size();
                	s.pushMatrix();
                }
                ~automatrix_t() {
                	this->_stack->popMatrix();
                }
        };
    };
    struct matstacks_t {
        fmat44stack
            view,
            model,
            projection
        ;
        matstacks_t() {}
        ~matstacks_t() {}
    };
    struct StopWatch {
        fclock_t interval = 0, cur = 0, lastcur = 0;
        bool operator+=(const fclock_t t){
            this->cur += t;
            if (this->cur >= this->interval){
                this->lastcur = this->cur;
                this->cur = 0;
                return true;
            }
            return false;
        }
    };
    namespace _sw3d2stuff {
        #ifdef GL45RW_WITH_SW3D2
        #endif
    };
    namespace _glstuff {
        template <typename DT, typename ST>
        inline void _glcall_ric(DT& dst, ST& src) {dst = (DT)src;}
    #ifdef __GL45RW_HAS_OPENGL
        #define _GL45RW_OPENGL_CALLS_M(_glc, ...) \
            _glc(GLboolean, IsShader, (GLuint), __VA_ARGS__) \
            _glc(GLboolean, IsProgram, (GLuint), __VA_ARGS__) \
            _glc(GLuint, CreateShader, (GLenum), __VA_ARGS__) \
            _glc(void, DeleteShader, (GLuint), __VA_ARGS__) \
            _glc(void, GetShaderInfoLog, (GLuint, GLsizei, GLsizei*, GLchar*), __VA_ARGS__) \
            _glc(void, GetProgramInfoLog, (GLuint, GLsizei, GLsizei*, GLchar*), __VA_ARGS__) \
            _glc(void, GetShaderiv, (GLuint, GLenum, GLint*), __VA_ARGS__) \
            _glc(void, GetProgramiv, (GLuint, GLenum, GLint*), __VA_ARGS__) \
            _glc(void, GetIntegerv, (GLenum, GLint*), __VA_ARGS__) \
            _glc(GLuint, CreateProgram, (void), __VA_ARGS__) \
            _glc(void, AttachShader, (GLuint, GLuint), __VA_ARGS__) \
            _glc(void, DetachShader, (GLuint, GLuint), __VA_ARGS__) \
            _glc(void, UseProgram, (GLuint), __VA_ARGS__) \
            _glc(void, LinkProgram, (GLuint), __VA_ARGS__) \
            _glc(void, DeleteProgram, (GLuint), __VA_ARGS__) \
            _glc(GLint, GetUniformLocation, (GLuint, const GLchar*), __VA_ARGS__) \
            _glc(GLint, GetAttribLocation, (GLuint, const GLchar*), __VA_ARGS__) \
            _glc(void, CompileShader, (GLuint), __VA_ARGS__) \
            _glc(void, ShaderSource, (GLuint, GLsizei, const GLchar**, const GLint*), __VA_ARGS__) \
            _glc(void, Finish, (void), __VA_ARGS__) \
            _glc(void, Flush, (void), __VA_ARGS__) \
            \
            _glc(void, MatrixMode, (GLenum), __VA_ARGS__) \
            _glc(void, LoadIdentity, (void), __VA_ARGS__) \
            _glc(void, PushMatrix, (void), __VA_ARGS__) \
            _glc(void, PopMatrix, (void), __VA_ARGS__) \
            _glc(void, LoadMatrixf, (const GLfloat*), __VA_ARGS__) \
            _glc(void, Ortho, (GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble), __VA_ARGS__) \
            _glc(void, Translatef, (GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Scalef, (GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            \
            _glc(void, UniformMatrix3fv, (GLint, GLsizei, GLboolean, const GLfloat*), __VA_ARGS__) \
            _glc(void, UniformMatrix4fv, (GLint, GLsizei, GLboolean, const GLfloat*), __VA_ARGS__) \
            \
            _glc(void, Uniform1fv, (GLint, GLsizei, const GLfloat*), __VA_ARGS__) \
            _glc(void, Uniform2fv, (GLint, GLsizei, const GLfloat*), __VA_ARGS__) \
            _glc(void, Uniform3fv, (GLint, GLsizei, const GLfloat*), __VA_ARGS__) \
            _glc(void, Uniform4fv, (GLint, GLsizei, const GLfloat*), __VA_ARGS__) \
            \
            _glc(void, Uniform1iv, (GLint, GLsizei, const GLint*), __VA_ARGS__) \
            _glc(void, Uniform2iv, (GLint, GLsizei, const GLint*), __VA_ARGS__) \
            _glc(void, Uniform3iv, (GLint, GLsizei, const GLint*), __VA_ARGS__) \
            _glc(void, Uniform4iv, (GLint, GLsizei, const GLint*), __VA_ARGS__) \
            \
            _glc(void, Vertex3fv, (const GLfloat*), __VA_ARGS__) \
            _glc(void, Vertex3f, (GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Normal3fv, (const GLfloat*), __VA_ARGS__) \
            _glc(void, Normal3f, (GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Color4fv, (const GLfloat*), __VA_ARGS__) \
            _glc(void, Color4f, (GLfloat, GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, MultiTexCoord2fv, (GLenum, const GLfloat*), __VA_ARGS__) \
            _glc(void, TexCoord2f, (GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Vertex2f, (GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Begin, (GLenum), __VA_ARGS__) \
            _glc(void, End, (void), __VA_ARGS__) \
            \
            _glc(GLenum, GetError, (void), __VA_ARGS__) \
            _glc(void, Disable, (GLenum), __VA_ARGS__) \
            _glc(void, Enable, (GLenum), __VA_ARGS__) \
            _glc(GLboolean, IsEnabled, (GLenum), __VA_ARGS__) \
            _glc(void, CullFace, (GLenum), __VA_ARGS__) \
            _glc(void, ActiveTexture, (GLenum), __VA_ARGS__) \
            _glc(void, BlendEquation, (GLenum), __VA_ARGS__) \
            _glc(void, BlendFunc, (GLenum, GLenum), __VA_ARGS__) \
            _glc(void, BindTexture, (GLenum, GLuint), __VA_ARGS__) \
            _glc(void, DepthMask, (GLboolean), __VA_ARGS__) \
            _glc(void, ColorMask, (GLboolean, GLboolean, GLboolean, GLboolean), __VA_ARGS__) \
            _glc(void, ShadeModel, (GLenum), __VA_ARGS__) \
            \
            _glc(void, ClearColor, (GLfloat, GLfloat, GLfloat, GLfloat), __VA_ARGS__) \
            _glc(void, Viewport, (GLint, GLint, GLsizei, GLsizei), __VA_ARGS__) \
            _glc(void, Clear, (GLbitfield), __VA_ARGS__) \
            \
            _glc(void, TexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*), __VA_ARGS__) \
            _glc(void, TexParameteri, (GLenum, GLenum, GLint), __VA_ARGS__) \
            _glc(void, GenTextures, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteTextures, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, TexStorage2D, (GLenum, GLsizei, GLenum, GLsizei, GLsizei), __VA_ARGS__) \
            \
            _glc(const GLubyte*, GetString, (GLenum), __VA_ARGS__) \
            \
            _glc(void, GenBuffers, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteBuffers, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, BindBuffer, (GLenum, GLuint), __VA_ARGS__) \
            _glc(void, BufferData, (GLenum, GLsizeiptr, const GLvoid*, GLenum), __VA_ARGS__) \
            _glc(void, BufferSubData, (GLenum, GLintptr, GLsizeiptr, const GLvoid*), __VA_ARGS__) \
            _glc(void, GetBufferParameteriv, (GLenum, GLenum, GLint*), __VA_ARGS__) \
            \
            _glc(void, GenVertexArrays, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteVertexArrays, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, BindVertexArray, (GLuint), __VA_ARGS__) \
            \
            _glc(void, EnableClientState, (GLenum), __VA_ARGS__) \
            _glc(void, DisableClientState, (GLenum), __VA_ARGS__) \
            \
            _glc(void, VertexPointer, (GLint, GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            _glc(void, ColorPointer, (GLint, GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            _glc(void, NormalPointer, (GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            _glc(void, IndexPointer, (GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            _glc(void, TexCoordPointer, (GLint, GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            \
            _glc(void, DrawArrays, (GLenum, GLint, GLsizei), __VA_ARGS__) \
            \
            _glc(void*, MapBuffer, (GLenum, GLenum), __VA_ARGS__) \
            _glc(void, UnmapBuffer, (GLenum), __VA_ARGS__) \
            \
            _glc(void, ClientActiveTexture, (GLenum), __VA_ARGS__) \
            _glc(void, InterleavedArrays, (GLenum, GLsizei, const GLvoid*), __VA_ARGS__) \
            \
            _glc(void, EnableVertexAttribArray, (GLuint), __VA_ARGS__) \
            _glc(void, DisableVertexAttribArray, (GLuint), __VA_ARGS__) \
            \
            _glc(void, VertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*), __VA_ARGS__) \
            _glc(void, VertexAttrib1fv, (GLuint, const GLfloat*), __VA_ARGS__) \
            _glc(void, VertexAttrib2fv, (GLuint, const GLfloat*), __VA_ARGS__) \
            _glc(void, VertexAttrib3fv, (GLuint, const GLfloat*), __VA_ARGS__) \
            _glc(void, VertexAttrib4fv, (GLuint, const GLfloat*), __VA_ARGS__) \
            \
            _glc(void, GenTransformFeedbacks, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteTransformFeedbacks, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, BindTransformFeedback, (GLenum, GLuint), __VA_ARGS__) \
            _glc(void, PauseTransformFeedback, (void), __VA_ARGS__) \
            _glc(void, ResumeTransformFeedback, (void), __VA_ARGS__) \
            _glc(void, BeginTransformFeedback, (GLenum), __VA_ARGS__) \
            _glc(void, EndTransformFeedback, (void), __VA_ARGS__) \
            _glc(void, TransformFeedbackVaryings, (GLuint, GLsizei, const char**, GLenum), __VA_ARGS__) \
            \
            _glc(void, GenFramebuffers, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, GenFramebuffersEXT, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteFramebuffers, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, DeleteFramebuffersEXT, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, BindFramebuffer, (GLenum, GLuint), __VA_ARGS__) \
            _glc(void, BindFramebufferEXT, (GLenum, GLuint), __VA_ARGS__) \
            _glc(GLsync, FramebufferRenderbuffer, (GLenum, GLenum, GLenum, GLuint), __VA_ARGS__) \
            _glc(GLsync, FramebufferRenderbufferEXT, (GLenum, GLenum, GLenum, GLuint), __VA_ARGS__) \
            _glc(GLsync, FramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint), __VA_ARGS__) \
            _glc(GLsync, FramebufferTexture2DEXT, (GLenum, GLenum, GLenum, GLuint, GLint), __VA_ARGS__) \
            _glc(void, FramebufferParameteri, (GLenum, GLenum, GLint), __VA_ARGS__) \
            _glc(void, BlitFramebuffer, (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum), __VA_ARGS__) \
            _glc(void, BlitFramebufferEXT, (GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum), __VA_ARGS__) \
            _glc(GLenum, CheckFramebufferStatus, (GLenum), __VA_ARGS__) \
            _glc(GLenum, CheckFramebufferStatusEXT, (GLenum), __VA_ARGS__) \
            _glc(void, IsFramebuffer, (GLuint), __VA_ARGS__) \
            _glc(void, IsFramebufferEXT, (GLuint), __VA_ARGS__) \
            _glc(void, GetFramebufferAttachmentParameteriv, (GLenum, GLenum, GLenum, GLint*), __VA_ARGS__) \
            _glc(void, GetFramebufferAttachmentParameterivEXT, (GLenum, GLenum, GLenum, GLint*), __VA_ARGS__) \
            \
            _glc(void, GenRenderbuffers, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, GenRenderbuffersEXT, (GLsizei, GLuint*), __VA_ARGS__) \
            _glc(void, DeleteRenderbuffers, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, DeleteRenderbuffersEXT, (GLsizei, const GLuint*), __VA_ARGS__) \
            _glc(void, RenderbufferStorage, (GLenum, GLenum, GLsizei, GLsizei), __VA_ARGS__) \
            _glc(void, RenderbufferStorageEXT, (GLenum, GLenum, GLsizei, GLsizei), __VA_ARGS__) \
            _glc(void, BindRenderbuffer, (GLenum, GLuint), __VA_ARGS__) \
            _glc(void, BindRenderbufferEXT, (GLenum, GLuint), __VA_ARGS__) \
            \
        \

        #define _gl45rw_glc1(_rtype, _name, _params, ...) _rtype (*gl ## _name) _params;

        struct GLcalls_t {
            GladGLContext _gladctx;
            _GL45RW_OPENGL_CALLS_M(_gl45rw_glc1);
            void (*glFramebufferParameteriEXT)(GLenum, GLenum, GLint);
        };
        #undef _gl45rw_glc1
        GLcalls_t* curgl = nullptr;
        /*
        template <typename FPT>
        void _loadglproc(FPT& fptr, const char* procname) {
            #if (defined(_GL45RW_WINDOW_SDL2))
            fptr = reinterpret_cast<FPT>(SDL_GL_GetProcAddress(procname));
            #elif (defined(_GL45RW_WINDOW_GLFW3))
            fptr = reinterpret_cast<FPT>(glfwGetProcAddress(procname));
            #endif
            std::cout << "[GL Info]: ";
            if (fptr != nullptr) {
                std::cout << "successfully loaded " << procname << "\n";
            }
            else {
                std::cout << "couldn't load " << procname << "\n";
            }
        }
        */
        void gl_clear_err()
        {
            if (curgl == nullptr){return;}
            while (curgl->glGetError() != GL_NO_ERROR){;}
        }
        std::string _getNameForShaderType(const GLenum shtype)
        {
            switch (shtype)
            {
                case GL_VERTEX_SHADER: return "vertex";
                case GL_FRAGMENT_SHADER: return "fragment";
                default: return "<unknown-shader-type>";
            }
        }

        class GLShaderError : public std::runtime_error {using std::runtime_error::runtime_error;};

        struct GLShaderObject {
            GLuint id = 0;
            GLenum shadertype = GL_VERTEX_SHADER;

            std::string info_log() const {
                GLint loglen = 1;
                curgl->glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &loglen);
                std::string log; log.resize(loglen);
                curgl->glGetShaderInfoLog(this->id, loglen, &loglen, &(log.front()));
                return log;
            }
            bool compile_status() const {
                int status = GL_FALSE;
                curgl->glGetShaderiv(this->id, GL_COMPILE_STATUS, &status);
                return (status == GL_TRUE);
            }
            GLShaderObject& throw_if_error()
            {
                if (!*this){
                    throw GLShaderError("invalid shader!");
                }
                else if (!(this->compile_status())){
                    std::stringstream errss;
                    errss << "Failed to build the " << _getNameForShaderType(this->shadertype) << " shader: \n\n"
                          << this->info_log() << '\n'
                    ;
                    throw GLShaderError(errss.str());
                }
                return *this;
            }
            operator GLuint() const {
                return this->id;
            }
            operator bool() const {
                return curgl->glIsShader(this->id);
            }

            GLShaderObject& operator()(const std::string& fullsrc)
            {
                //const std::string typestr = _getNameForShaderType(this->shadertype);
                if (!*this){return *this;}
                const char* fullsrc_c = fullsrc.c_str();
                curgl->glShaderSource(this->id, 1, &fullsrc_c, nullptr);
                curgl->glCompileShader(this->id);
                return *this;
            }
            GLShaderObject() {}
            GLShaderObject(const GLenum _type) : shadertype(_type) {
                this->id = curgl->glCreateShader(this->shadertype);
            }

            ~GLShaderObject() {
                if (this->id != 0){curgl->glDeleteShader(this->id);}
            }
        };

        class GLProgramError : public std::runtime_error {using std::runtime_error::runtime_error;};
        struct GLProgramBinding {
            private:
                GLuint cur, prev;
                bool same = false;
            public:

                void setUniform(const GLint& idx, const size_t count, const float* v) {curgl->glUniform1fv(idx, count, v); gl_clear_err();}
                void setUniform(const GLint& idx, const size_t count, const int* v) {curgl->glUniform1iv(idx, count, v); gl_clear_err();}

                void setUniform(const GLint& idx, const size_t count, const fvec2_t* v) {curgl->glUniform2fv(idx, count, (const float*)v); gl_clear_err();}
                void setUniform(const GLint& idx, const size_t count, const ivec2_t* v) {curgl->glUniform2iv(idx, count, (const int*)v); gl_clear_err();}

                void setUniform(const GLint& idx, const size_t count, const fvec3_t* v) {curgl->glUniform3fv(idx, count, (const float*)v); gl_clear_err();}
                void setUniform(const GLint& idx, const size_t count, const ivec3_t* v) {curgl->glUniform3iv(idx, count, (const int*)v); gl_clear_err();}

                void setUniform(const GLint& idx, const size_t count, const fvec4_t* v) {curgl->glUniform4fv(idx, count, (const float*)v); gl_clear_err();}
                void setUniform(const GLint& idx, const size_t count, const ivec4_t* v) {curgl->glUniform4iv(idx, count, (const int*)v); gl_clear_err();}

                void setUniform(const GLint& idx, const size_t num, const fmat33_t* m33s, const bool transpose=false) {
                    curgl->glUniformMatrix3fv(idx, num, transpose, reinterpret_cast<const float*>(m33s));
                    gl_clear_err();
                }
                void setUniform(const GLint& idx, const size_t num, const fmat44_t* m44s, const bool transpose=false) {
                    curgl->glUniformMatrix4fv(idx, num, transpose, reinterpret_cast<const float*>(m44s));
                    gl_clear_err();
                }

                template <typename T>
                inline void setNamedUniform(const std::string& name, const size_t num, const T* values) {
                    this->setUniform(curgl->glGetUniformLocation(this->cur, name.c_str()), num, values);
                }

                template <typename T>
                inline void setNamedUniform(const std::string& name, const size_t num, const T* values, const bool transpose) {
                    this->setUniform(curgl->glGetUniformLocation(this->cur, name.c_str()), num, values, transpose);
                }

                GLint attrib_idx(const std::string& name) {
                    return curgl->glGetAttribLocation(this->cur, name.c_str());
                }

                GLProgramBinding(const GLuint _id=0) : cur(_id), prev(0) {
                    curgl->glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&(this->prev));
                    this->same = (this->cur == this->prev);
                    if (!this->same){curgl->glUseProgram(this->cur);}
                }
                ~GLProgramBinding(){if (!this->same){curgl->glUseProgram(this->prev);}}
        };
        struct GLShaderProgram {
            private:
                std::vector<GLuint> attachments = {};
                GLuint id = 0;
                bool owns_program = false;
                //GLuint (*glcreateprogram)() = nullptr;
                //void (*gldeleteprogram)(GLuint) = nullptr;
            public:
                GLShaderProgram& detach_all() {
                    if (!*this){return *this;}
                    for (auto s : this->attachments){curgl->glDetachShader(this->id, s);}
                    this->attachments.clear();
                    return *this;
                }
                void attach(const GLuint shader) {
                    gl_clear_err();
                    if (!*this){throw GLProgramError("invalid program object.");}
                    else if (!curgl->glIsShader(shader)){throw GLProgramError("invalid shader id.");}
                    else if (std::count(this->attachments.begin(), this->attachments.end(), shader) > 0){return;}
                    curgl->glAttachShader(this->id, shader);
                    this->attachments.push_back(shader);
                }
                std::string info_log() const {
                    GLint loglen = 1;
                    curgl->glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &loglen);
                    std::string log; log.resize(loglen);
                    curgl->glGetProgramInfoLog(this->id, loglen, &loglen, &(log.front()));
                    return log;
                }
                bool link_status() const {
                    int status = GL_FALSE;
                    curgl->glGetProgramiv(this->id, GL_LINK_STATUS, &status);
                    return (status == GL_TRUE);
                }

                inline GLint uniform_idx(const std::string& name) const {
                    return curgl->glGetUniformLocation(this->id, name.c_str());
                }
                inline GLint attrib_idx(const std::string& name) const {
                    return curgl->glGetAttribLocation(this->id, name.c_str());
                }


                GLShaderProgram& throw_if_error() {
                    if (!*this){throw GLProgramError("invalid program object.");}
                    else if (!this->link_status()){
                        std::stringstream errss;
                        errss << "program link failed: \n\n" << this->info_log() << "\n";
                        throw GLProgramError(errss.str());
                    }
                    return *this;
                }
                operator GLuint() const {return this->id;}
                operator bool() const {return curgl->glIsProgram(this->id);}
                GLShaderProgram& link() {
                    if (!*this){return *this;}
                    curgl->glLinkProgram(this->id);
                    return *this;
                }

                GLShaderProgram(const GLuint program=0, const bool assume_ownership=false) : id(program), owns_program(assume_ownership) {}
                /*GLShaderProgram(decltype(glcreateprogram) _gen, decltype(gldeleteprogram) _del) : owns_program(true), glcreateprogram(_gen), gldeleteprogram(_del) {
                    this->id = this->glcreateprogram();
                }*/
                ~GLShaderProgram() {
                    this->detach_all();
                    if (this->owns_program && this->id != 0){curgl->glDeleteProgram(this->id);}
                }
        };
        class GLSLCodeStream {
            public:
                std::stringstream sstr;
                std::string str() {return this->sstr.str();}
                int indentlevel = 0, indentspaces = 4;

                template <glm::length_t L, class T, glm::qualifier Q>
                GLSLCodeStream& operator<<(const glm::vec<L,T,Q>& v) {
                    this->sstr << glm::to_string(v);
                    return *this;
                }
                template <class T>
                GLSLCodeStream& operator<<(const T& v){
                    this->sstr << v;
                    return *this;
                }
                GLSLCodeStream& startline() {
                    std::string indent; indent.resize(this->indentlevel * this->indentspaces, ' ');
                    (*this) << '\n' << indent;
                    return *this;
                }
                GLSLCodeStream& lbrace() {
                    (*this) << "{"; this->indentlevel++;
                    return this->startline();
                }
                GLSLCodeStream& rbrace() {
                    if (this->indentlevel > 0){this->indentlevel--;}
                    this->startline() << "}";
                    return this->startline();
                }
                GLSLCodeStream& funcdef(const std::string& rtype, const std::string& fname, const std::vector<std::string>& fargs){
                    if (this->indentlevel > 0){throw std::runtime_error("indentlevel must be 0 to do a funcdef!");}
                    (*this) << rtype << " " << fname << "(" << djutil::ezstr::itrjoin<char>(", ", fargs.begin(), fargs.end()) << ")";
                    //return this->lbrace();
                    return *this;
                }
                GLSLCodeStream& localvardef(const std::string& vtype, const std::string& vname, const std::string vinit=""){
                    if (this->indentlevel == 0){throw std::runtime_error("indentlevel must be > 0 to do a localvardef!");}
                    (*this) << vtype << ' ' << vname;
                    if (vinit.size() > 0){(*this) << " = " << vinit;}
                    (*this) << ';';
                    return this->startline();
                }

                GLSLCodeStream& header() {
                    #if (defined(_GL45RW_GL2))
                     (*this) << "#version 120\n";
                    #elif (defined(_GL45RW_GLES3))
                     (*this) << "#version 310 es\n"
                             << "precision mediump float;\n"
                             << "precision mediump int;\n"
                     ;
                    #endif
                    return *this;
                }

        };
        class GLSLAutoBracer {
            private:
                GLSLCodeStream* _codestream = nullptr;
            public:
                GLSLCodeStream* operator->() {return this->_codestream;}
                template <class T>
                GLSLCodeStream& operator<<(const T& v) {
                    return ((*this->_codestream) << v);
                }
                GLSLAutoBracer(GLSLCodeStream& c) : _codestream(&c) {
                    this->_codestream->lbrace();
                }
                ~GLSLAutoBracer() {this->_codestream->rbrace();}
        };
        class GLSLAutoFuncDef {
            private:
                GLSLCodeStream* _codestream = nullptr;
            public:
                GLSLCodeStream* operator->() {return this->_codestream;}
                template <class T>
                GLSLCodeStream& operator<<(const T& v) {
                    return ((*this->_codestream) << v);
                }
                GLSLAutoFuncDef(GLSLCodeStream& c, const std::string& rtype, const std::string& fname, const std::initializer_list<std::string>& fargs) :
                    _codestream(&c)
                {
                    this->_codestream->funcdef(rtype, fname, fargs);
                    this->_codestream->lbrace();
                }
                ~GLSLAutoFuncDef() {this->_codestream->rbrace();}
        };
        class GLSLCodeGenerator {
            private:
                int _cur_uniform_location = 0, _cur_attrib_location = 0;
                std::unordered_map<int, std::string> frag_outmap = {};
            public:
                GLSLCodeStream
                    commonsrc,
                    vertexsrc,
                    fragsrc
                ;
                int uniform_def(const std::string& utype, const std::string& uname, const size_t nfields=1){
                    int uloc = this->_cur_uniform_location;
                    #ifdef _GL45RW_GLES3
                     this->commonsrc << "layout (location = " << uloc << ") ";
                    #endif
                    this->commonsrc << "uniform " << utype << " " << uname << ";\n";
                    this->_cur_uniform_location += nfields;
                    return uloc;
                }
                int uniform_def(const std::string& utype, const std::string& uname, const size_t narray, const size_t nfields=1){
                    int uloc = this->_cur_uniform_location;
                    #ifdef _GL45RW_GLES3
                     this->commonsrc << "layout (location = " << uloc << ") ";
                    #endif
                    this->commonsrc << "uniform " << utype << " " << uname << "[" << narray << "];\n";
                    this->_cur_uniform_location += narray*nfields;
                    return uloc;
                }
                int attrib_def(const std::string& atype, const std::string& aname){
                    int aloc = this->_cur_attrib_location++;
                    #if (defined(_GL45RW_GL2))
                     const std::string _attrns = "attribute";
                    #elif (defined(_GL45RW_GLES3))
                     const std::string _attrns = "in";
                     this->vertexsrc << "layout (location = " << aloc << ") ";
                    #endif
                    this->vertexsrc << _attrns << " " << atype << " " << aname << ";\n";
                    return aloc;
                }

                void varying_def(const std::string& otype, const std::string& oname){
                    #if (defined(_GL45RW_GL2))
                     std::string varying_str = "varying ";
                     varying_str += otype;
                     varying_str += " ";
                     varying_str += oname;
                     varying_str += ";\n";
                     this->vertexsrc << varying_str;
                     this->fragsrc << varying_str;
                    #elif (defined(_GL45RW_GLES3))
                     this->vertexsrc << "out " << otype << ' ' << oname << ";\n";
                     this->fragsrc << "in " << otype << ' ' << oname << ";\n";
                    #endif
                }

                struct fragoutdef_t {
                    std::string type, name;
                };
                using frag_outputs_list = std::vector<fragoutdef_t>;
                GLSLCodeGenerator& frag_header(const frag_outputs_list ol=frag_outputs_list{fragoutdef_t{"vec4", "OUT_glFragColor"}}) {
                    #if (defined(_GL45RW_GL2))
                     this->frag_outmap[0] = "gl_FragColor";
                    #elif (defined(_GL45RW_GLES3))
                     int curloc = 0;
                     for (auto loc : ol){
                         this->frag_outmap[curloc] = loc.name;
                         this->fragsrc << "layout (location = " << curloc++ << ") out " << loc.type << " " << loc.name << ";\n";
                     }
                    #endif
                    return *this;
                }

                GLSLCodeGenerator& frag_out(const int location, const std::string& expr){
                    std::string outname = this->frag_outmap.at(location);
                    this->fragsrc << outname << " = " << expr << ";"; this->fragsrc.startline();
                    return *this;
                }

                std::string vertexstr() {return this->commonsrc.str() + "\n" + this->vertexsrc.str() + "\n";}
                std::string fragstr() {return this->commonsrc.str() + "\n" + this->fragsrc.str() + "\n";}
        };

        #if (defined(_GL45RW_GL2))
         #define _gl45rw_glext(_n) curgl->gl ## _n ## EXT
        #else
         #define _gl45rw_glext(_n) curgl->gl ## _n
        #endif

        class GLFramebufferObject {
            private:
                GLuint _fbo = 0;
            public:
                const GLuint& fbo_idx = _fbo;
                struct _attachment_entry {
                    GLuint glidx = 0;
                    GLenum gltype, location;
                };
                dictionary<GLuint, _attachment_entry> attachments = {};
                dictionary<GLenum, GLuint> attachments_byloc = {};

                _attachment_entry& getAttachmentForLocation(const GLenum& loc){
                    return this->attachments.at(this->attachments_byloc.at(loc));
                }

                void delete_attachment(const GLuint idx) {
                    if (this->attachments.count(idx) == 0){return;}
                    auto& a = this->attachments.at(idx);
                    this->bind();
                    switch (a.gltype) {
                        case GL_RENDERBUFFER: {
                            _gl45rw_glext(FramebufferRenderbuffer)(GL_FRAMEBUFFER, a.location, GL_RENDERBUFFER, 0);
                            _gl45rw_glext(DeleteRenderbuffers)(1, &a.glidx);
                            a.glidx = 0;
                            break;
                        }
                        case GL_TEXTURE_2D: {
                            _gl45rw_glext(FramebufferTexture2D)(GL_FRAMEBUFFER, a.location, GL_TEXTURE_2D, 0, 0);
                            curgl->glDeleteTextures(1, &a.glidx);
                            a.glidx = 0;
                            break;
                        }
                        default: ;
                    }
                    this->attachments_byloc.erase(a.location);
                    this->attachments.erase(idx);
                    _gl45rw_glext(BindFramebuffer)(GL_FRAMEBUFFER, 0);
                }
                void clear_all_attachments() {
                    while (this->attachments.size() > 0){
                        this->delete_attachment(this->attachments.begin()->first);
                    }
                    this->attachments_byloc.clear();
                }
                void bind(const GLenum target=GL_FRAMEBUFFER){
                    _gl45rw_glext(BindFramebuffer)(target, this->_fbo);
                }
                static void unbind(const GLenum target=GL_FRAMEBUFFER){
                    _gl45rw_glext(BindFramebuffer)(target, 0);
                }
                GLuint attach_rbuf(const GLenum location, const GLenum internalformat, const size_t width, const size_t height){
                    _attachment_entry na = {.gltype=GL_RENDERBUFFER, .location=location};
                    if (this->attachments_byloc.count(location) > 0){
                        throw std::runtime_error("location " + std::to_string(int(location)) + " already in use!");
                    }
                    _gl45rw_glext(GenRenderbuffers)(1, &na.glidx);
                    _gl45rw_glext(BindRenderbuffer)(GL_RENDERBUFFER, na.glidx);
                    _gl45rw_glext(RenderbufferStorage)(GL_RENDERBUFFER, internalformat, width, height);
                    _gl45rw_glext(BindRenderbuffer)(GL_RENDERBUFFER, 0);
                    this->bind();
                    _gl45rw_glext(FramebufferRenderbuffer)(GL_FRAMEBUFFER, location, GL_RENDERBUFFER, na.glidx);
                    GLFramebufferObject::unbind();
                    this->attachments_byloc[location] = na.glidx;
                    this->attachments[na.glidx] = na;
                    return na.glidx;
                }
                GLuint attach_tex2D(const GLenum location, const GLenum internalformat, const size_t width, const size_t height){
                    _attachment_entry na = {.gltype=GL_TEXTURE_2D, .location=location};
                    if (this->attachments_byloc.count(location) > 0){
                        throw std::runtime_error("location " + std::to_string(int(location)) + " already in use!");
                    }
                    curgl->glGenTextures(1, &na.glidx);
                    curgl->glBindTexture(GL_TEXTURE_2D, na.glidx);
                    curgl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    curgl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    curgl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    curgl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    curgl->glTexStorage2D(GL_TEXTURE_2D, 1, internalformat, width, height);
                    curgl->glBindTexture(GL_TEXTURE_2D, 0);
                    this->bind();
                    _gl45rw_glext(FramebufferTexture2D)(GL_FRAMEBUFFER, location, GL_TEXTURE_2D, na.glidx, 0);
                    GLFramebufferObject::unbind();
                    this->attachments_byloc[location] = na.glidx;
                    this->attachments[na.glidx] = na;
                    return na.glidx;
                }
                void throw_if_incomplete() {
                    this->bind();
                    std::string errmsg = "";
                    switch (_gl45rw_glext(CheckFramebufferStatus)(GL_FRAMEBUFFER)){
                        case GL_FRAMEBUFFER_UNDEFINED: errmsg = "GL_FRAMEBUFFER_UNDEFINED"; break;
                        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errmsg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
                        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errmsg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
                        case GL_FRAMEBUFFER_UNSUPPORTED: errmsg = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
                        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errmsg = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
                        default: return;
                    }
                    throw std::runtime_error("throw_if_incomplete() -> " + errmsg);
                }
                GLFramebufferObject() {
                    _gl45rw_glext(GenFramebuffers)(1, &(this->_fbo));
                }

                ~GLFramebufferObject() {
                    this->clear_all_attachments();
                    _gl45rw_glext(DeleteFramebuffers)(1, &(this->_fbo));
                    this->_fbo = 0;
                }
        };
        #undef _gl45rw_glext
    #endif
    };
    template <class T>
    class AutoPointer {
        private:
            T* _ptr = nullptr;
        public:
            operator T*() {return this->_ptr;}
            T* operator->() {return this->_ptr;}

            template <typename... Args>
            AutoPointer<T>& operator()(Args... args) {
                delete this->_ptr;
                this->_ptr = new T(args...);
                return *this;
            }
            AutoPointer<T>& operator~() {
                delete this->_ptr; this->_ptr = nullptr;
                return *this;
            }

            operator bool() {return this->_ptr != nullptr;}
            bool operator!() {return this->_ptr == nullptr;}

            bool operator==(const T* other){return this->_ptr == other;}
            bool operator!=(const T* other){return this->_ptr != other;}

            AutoPointer() {}
            ~AutoPointer() {delete this->_ptr; this->_ptr = nullptr;}
    };



    enum struct assettype : int {
        none = 0,
        texture = 1,
        cubemap = 2,
        material = 3,
        trimesh = 4,
        sgmodel = 5,
        bitmapfont = 6,
        luaengine = 7,
        touchctrlsys = 8,
        lightsource = 9,
        sndfactory = 10,
        sndhandle = 11
    };

    static const char* _GL45RW_ASSET_TYPE_NAMES[] = {
        "none",
        "texture",
        "cubemap",
        "material",
        "trimesh",
        "sgmodel",
        "bitmapfont",
        "luaengine",
        "touchctrlsys",
        "sndfactory",
        "sndhandle"
    };
    class AssetRef;
    class AssetBase {
        private:
            EngineInstance* _instance = nullptr;
            uid_t _assetid = 0;
            std::u32string _name = U"";
            //bool is_refcounting = false;

            //vectorlist<uid_t> refd_by = {}, refs_of = {};
            std::unordered_set<AssetRef*> _references = {};
            bool _AssetBase_marked_as_garbage = false;
            friend class AssetRef;
            void _AssetBase_incrref(AssetRef* ref);
            void _AssetBase_decrref(AssetRef* ref);
        protected:
            //bool delref(const uid_t id);
            //bool addref(const uid_t id);

        public:
            const bool& _AssetBase_pending_garbage_collection = _AssetBase_marked_as_garbage;

            virtual void onCreate() {}
            virtual void onDestroy() {std::cout << "default AssetBase::onDestroy() called.\n";}
            bool _is_valid() const;
            void assertvalid() const;
            inline operator uid_t() const {return this->_assetid;}
            inline virtual assettype asset_type() const {return assettype::none;}

            EngineInstance* const& instance = _instance;
            const uid_t& id = _assetid;

            virtual void onRenderContextDelete() {}
            virtual void onRenderContextCreate() {}

            virtual void onSoundContextCreate() {}
            virtual void onSoundContextDelete() {}

            //virtual void onContextCreate() {}

            AssetBase(EngineInstance* _inst=nullptr, const uid_t _mynewid=0) : _instance(_inst), _assetid(_mynewid) {}
            virtual ~AssetBase();

    };
    class AssetRef {
		private:
		    AssetBase* _asset = nullptr;
		    void _unref();
			void _enref(AssetBase* a);
			friend class AssetBase;
		public:
		    uid_t user_uid = 0;
		    void* user_ptr = nullptr;

		    AssetBase* operator=(AssetBase* a){
			    this->_unref();
				this->_enref(a);
				return a;
			}
		    operator AssetBase*() {return this->_asset;}

		    AssetRef() {}
		    AssetRef(const AssetRef& other) {
				this->_asset = other._asset;
			}
		    virtual ~AssetRef() {
				std::cout << "~AssetRef()\n";
				this->_unref();
				//this->_asset = nullptr;
			}
	};

    void AssetBase::_AssetBase_decrref(AssetRef* ref){
		this->_references.erase(ref);
		if (this->_references.size() == 0){this->_AssetBase_marked_as_garbage = true;}
	}
	void AssetBase::_AssetBase_incrref(AssetRef* ref){
		this->_references.insert(ref);
		this->_AssetBase_marked_as_garbage = false;
	}

    class dynassetref_t {
		private:
		    uid_t _aid = 0;
		    EngineInstance* _ei = nullptr;
		public:
		    const uid_t& asset_id = _aid;
		    EngineInstance* const& instance = _ei;
		    bool valid() const;
		    assettype type() const;

		    template <class A>
		    operator A*() const;

		    dynassetref_t& operator=(const AssetBase* a);

		    dynassetref_t() {}
		    dynassetref_t(EngineInstance* i, const uid_t& a) :
		        _ei(i),
		        _aid(a)
		    {}
		    dynassetref_t(const dynassetref_t& r) :
		        _ei(r._ei),
		        _aid(r._aid)
		    {}
	};
    enum class eiclockfunc : int {
        uptime = 0,
        simtime = 1
    };
    enum class entity_realm : int {
        world = 0,
        gui = 1
    };
    enum class entity_form : int {
        phantom = 0,
        point = 1
    };
    /*
    struct entity;

    struct ethinkargs_t {
        EngineInstance* instance;
    };

    struct efield_t {
        private:
            char _data_bytes[128] = {};
        public:
            template <class T> inline const T& operator[](const int i) const {return ((const T*)(this->_data_bytes))[i];}
            template <class T> inline T& operator[](const int i) {return ((T*)(this->_data_bytes))[i];}

            efield_t() {}

            template <class T>
            efield_t(const T& v) {this[0] = v;}
    };

    //using ethinkfunc_t = int(*)(entity&, ethinkargs_t&);

    struct entityclass {
        std::string classname = "";

    };
    struct entity {
        private:
            uid_t _entity_id = 0, _network_id = 0;
            EngineInstance* _engine_instance = nullptr;
            std::string _eclassname = "";
            entityclass* _eclass = nullptr;
        public:
            const uid_t& eid = _entity_id;
            const uid_t& netid = _network_id;
            const std::string& classname = _eclassname;
        entity_realm realm = entity_realm::world;
        entity_form form = entity_form::phantom;
        fclock_t nextthink, spawntime;
        ethinkfunc_t think = nullptr;
        trs_t loc;
        dictionary<std::string, efield_t> fields;
        bool _pending_deletion = false;
    };
    */

    class Texture2D;
    class Material;
    class TriangleMesh;
    class BitmapFont;

    std::u32string escape_raw_text(const std::u32string& txt) {
        std::u32string escaped; escaped.reserve(txt.size());
        for (const auto& c : txt) {
            switch (c) {
                case U'%': escaped.push_back(c); break;
                default: escaped.push_back(c);
            }
        }
        return escaped;
    }

    std::string escape_raw_text(const std::string& txt) {
        return djutil::ezstr::utf32_to_utf8(escape_raw_text(djutil::ezstr::utf8_to_utf32(txt)));
    }

    struct DrawTextParser {
        public:
            enum struct action_type : int {
                none = 0,
                drawchar = 1,
                setfgcolor = 2
            };
        private:
            const char32_t* chr = nullptr;
            size_t pos = 0, num = 0;
            size_t _line = 0, _col = 0;
            char32_t _char2draw, _fgcolorcode;
            action_type _cur_action = action_type::none;

            inline bool check_newpos(const size_t newpos) const {
                return newpos < this->num;
            }

            inline void _newline() {this->_col = 0; this->_line++;}

            inline bool _adv(const size_t n=1){this->pos += n; this->chr += n; return this->check_newpos(this->pos);}
        public:
            const char32_t& char2draw = _char2draw;
            const char32_t& fgcolorcode = _fgcolorcode;
            const action_type& action = _cur_action;

            const size_t& line = _line;
            const size_t& column = _col;

            operator bool() {
                if (this->chr == nullptr || this->pos >= this->num){return false;}

                this->_char2draw = this->_fgcolorcode = 0;
                this->_cur_action = action_type::none;

                switch (this->chr[0]) {
                    case U'%': {
                        if (!this->_adv()){return false;}
                        switch (this->chr[0]) {
                            case U'%': {
                                this->_char2draw = U'%';
                                this->_cur_action = action_type::drawchar;
                                this->_col++;
                                this->_adv();
                                return true;
                            }
                            case U'c': {
                                if (!this->_adv()){return false;}
                                this->_fgcolorcode = this->chr[0];
                                this->_cur_action = action_type::setfgcolor;
                                this->_adv();
                                return true;
                            }
                            default: ;
                        }
                        return this->_adv();
                    }
                    case U'\r': {
                        if (this->check_newpos(this->pos + 1) && this->chr[1] == U'\n') {
                            // handle CRLF
                            this->_adv();
                        }
                        this->_newline();
                        this->_adv();
                        break;
                    }
                    case U'\n': {
                        this->_newline();
                        this->_adv();
                        break;
                    }
                    default: {
                        this->_char2draw = this->chr[0];
                        this->_cur_action = action_type::drawchar;
                        this->_col++;
                        this->_adv();
                        break;
                    }
                }

                return true;
            }

            DrawTextParser(const char32_t* str, const size_t len) : chr(str), num(len), pos(0) {}
            DrawTextParser(const std::u32string& ustr) : chr(&(ustr[0])), num(ustr.size()), pos(0) {}
    };

    #ifdef __GL45RW_HAS_OPENGL
     #if (defined(_GL45RW_GL2))
         const std::string _BITMAP_TEXT_GLSL_COMMON = R"""(#version 120
uniform vec2 tluv;
uniform vec2 bruv;
uniform int font_width;
uniform int font_height;
uniform sampler2D font;
uniform vec4 basecolor;
uniform float alpha_discard_threshold;
uniform vec3 pos_offset;
uniform mat4 mvp;

varying vec2 frag_uv;
)""";
         const std::string _BITMAP_TEXT_GLSL_VERTEX = R"""(
void main() {
    frag_uv = mix(tluv, bruv, gl_MultiTexCoord0.st);
    gl_Position = mvp * vec4(gl_Vertex.xyz + pos_offset, 1.0);
}
)""";
         const std::string _BITMAP_TEXT_GLSL_FRAGMENT = R"""(
vec2 uv_to_xy(vec2 uv) {
    //ivec2 ftsz = textureSize(font, 0);
    ivec2 ftsz = ivec2(font_width, font_height);
    return clamp(vec2(uv * (ftsz-1)), vec2(0), vec2(ftsz-1));
}
ivec2 uv_to_xyi(vec2 uv) {
    return ivec2((uv_to_xy(uv)));
}
vec4 get_font_texel(ivec2 pixelcoord) {
    //return texelFetch(font, pixelcoord, 0);
    return texture2D(font, pixelcoord/vec2(font_width-1, font_height-1), 0);
}
ivec2 iv2_clamp(in ivec2 v, in ivec2 l, in ivec2 h){
    return ivec2(
        (v.x < l.x ? l.x : (v.x > h.x ? h.x : v.x)),
        (v.y < l.y ? l.y : (v.y > h.y ? h.y : v.y))
    );
}
void main() {
    ivec2 minxy = uv_to_xyi(vec2(tluv.x, bruv.y));
    ivec2 maxxy = uv_to_xyi(vec2(bruv.x, tluv.y));
    vec2 pxcoordf = uv_to_xy(frag_uv);
    ivec2 pixelcoord = ivec2(pxcoordf);
    vec2 lerpfactors = clamp(pxcoordf - vec2(pixelcoord), vec2(0), vec2(1));
    vec4 ftexel = mix(
        mix(get_font_texel(iv2_clamp(pixelcoord+ivec2(0,0), minxy, maxxy)), get_font_texel(iv2_clamp(pixelcoord+ivec2(1,0), minxy, maxxy)), vec4(lerpfactors.x)),
        mix(get_font_texel(iv2_clamp(pixelcoord+ivec2(0,1), minxy, maxxy)), get_font_texel(iv2_clamp(pixelcoord+ivec2(1,1), minxy, maxxy)), vec4(lerpfactors.x)),
        vec4(lerpfactors.y)
    );

    vec4 fragcolor = basecolor * ftexel;
    if (fragcolor.a < alpha_discard_threshold){discard;}
    gl_FragColor = fragcolor;
}
)""";
     #elif (defined(_GL45RW_GLES3))
          const std::string _BITMAP_TEXT_GLSL_COMMON = R"(#version 310 es
precision mediump int;
precision mediump float;

uniform vec2 tluv;
uniform vec2 bruv;
uniform sampler2D font;
uniform vec4 basecolor;
uniform float alpha_discard_threshold;
uniform vec3 pos_offset;
uniform mat4 mvp;

)";
         const std::string _BITMAP_TEXT_GLSL_VERTEX = R"(
out vec2 frag_uv;

const vec3 v_rect_coords[4] = vec3[4](
    vec3(0.0,1.0,0.0), //top-left
    vec3(1.0,1.0,0.0), //top-right
    vec3(0.0,0.0,0.0), //bottom-left
    vec3(1.0,0.0,0.0)  //bottom-right
);

const vec2 v_rect_baseuvs[4] = vec2[4](
    vec2(0.0,0.0), //top-left
    vec2(1.0,0.0), //top-right
    vec2(0.0,1.0), //bottom-left
    vec2(1.0,1.0)  //bottom-right
);

const int v_rect_indices[6] = int[6](
    0,2,1,
    2,3,1
);

void main() {
    //frag_uv = mix(tluv, bruv, in_uv.st);
    int idx = v_rect_indices[gl_VertexID];
    vec3 vxyz = v_rect_coords[idx];
    vec2 vuv = v_rect_baseuvs[idx];
    frag_uv = mix(tluv, bruv, vuv);
    gl_Position = mvp * vec4(vxyz + pos_offset, 1.0);
}

)";
         const std::string _BITMAP_TEXT_GLSL_FRAGMENT = R"(
layout (location = 0) out vec4 frag_outcolor;
in vec2 frag_uv;
vec2 uv_to_xy(vec2 uv) {
    ivec2 ftsz = textureSize(font, 0);
    return clamp(vec2(uv * vec2(ftsz-1)), vec2(0), vec2(ftsz-1));
}
ivec2 uv_to_xyi(vec2 uv) {
    return ivec2((uv_to_xy(uv)));
}
vec4 get_font_texel(ivec2 pixelcoord) {
    return texelFetch(font, pixelcoord, 0);
}
ivec2 iv2_clamp(in ivec2 v, in ivec2 l, in ivec2 h){
    return ivec2(
        (v.x < l.x ? l.x : (v.x > h.x ? h.x : v.x)),
        (v.y < l.y ? l.y : (v.y > h.y ? h.y : v.y))
    );
}

void main() {
    ivec2 minxy = uv_to_xyi(vec2(tluv.x, bruv.y));
    ivec2 maxxy = uv_to_xyi(vec2(bruv.x, tluv.y));
    vec2 pxcoordf = uv_to_xy(frag_uv);
    ivec2 pixelcoord = ivec2(pxcoordf);
    vec2 lerpfactors = clamp(pxcoordf - vec2(pixelcoord), vec2(0), vec2(1));
    vec4 ftexel = mix(
        mix(get_font_texel(iv2_clamp(pixelcoord+ivec2(0,0), minxy, maxxy)), get_font_texel(iv2_clamp(pixelcoord+ivec2(1,0), minxy, maxxy)), vec4(lerpfactors.x)),
        mix(get_font_texel(iv2_clamp(pixelcoord+ivec2(0,1), minxy, maxxy)), get_font_texel(iv2_clamp(pixelcoord+ivec2(1,1), minxy, maxxy)), vec4(lerpfactors.x)),
        vec4(lerpfactors.y)
    );

    vec4 fragcolor = basecolor * ftexel;
    if (fragcolor.a < alpha_discard_threshold){discard;}
    frag_outcolor = fragcolor;
}

)";
     #endif
    #endif

    /*
    class _BaseRenderer {
    	private:
            EngineInstance* _einst = nullptr;
            #ifndef _GL45RW_HEADLESS
                SDL_Window* _sdl2win = nullptr;
                SDL_Renderer* _sdl2rend = nullptr;
                SDL_Texture* _sdl2tex = nullptr;
            #endif
        protected:
            virtual bool PRV_RendererShutdown(){return false;};
            virtual bool PRV_RendererStartup(const videomode_t& newmode){return false;};
            virtual bool PRV_ChangeWindowResolution(const videomode_t& newmode){return false;};
    	public:
    	    #ifndef _GL45RW_HEADLESS
    	        SDL_Window* const& sdl2win = _sdl2win;
    	        SDL_Renderer* const& sdl2rend = _sdl2rend;
    	        SDL_Texture* const& sdl2tex = _sdl2tex;
    	    #endif
            EngineInstance* const& instance = _einst;

    	    virtual gfxapi apitype() const {return gfxapi::none;}

    	     pubsetmode(const videomode_t& newmode) {
    	    	this->instance->
    	    }
    };
    */

    enum struct ccmdtype : int {
        unknown = 0,
        command = 1,
        cvar = 2,
        svar = 3,
        ivar = 4
    };

    struct CCMDInvokationExtraData {
        int _dummy;
        fscalar_t ivar_invalue;
    };

    class _BaseConsoleCommand {
        private:
            EngineInstance* _einst = nullptr;
            std::u32string _name;
            void _baseccmd_private_init();
        public:
            EngineInstance* const instance = _einst;
            const std::u32string& name = _name;

            virtual ccmdtype command_type() const {return ccmdtype::unknown;}
            virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata) {return 0;}
            _BaseConsoleCommand(EngineInstance* e, const std::u32string n) : _einst(e), _name(n) {this->_baseccmd_private_init();}

    };
    class ConsoleCommand : public _BaseConsoleCommand {
        using _BaseConsoleCommand::_BaseConsoleCommand;
        public:
            virtual ccmdtype command_type() const {return ccmdtype::command;}
    };

    class ConsoleCVAR : public _BaseConsoleCommand {
    	using _BaseConsoleCommand::_BaseConsoleCommand;
        public:
            virtual ccmdtype command_type() const {return ccmdtype::cvar;}
            virtual void getstr(std::u32string& s) {}
            virtual int setstr(const std::u32string& s) {return 0;}

            std::string getstr8() {std::u32string us; this->getstr(us); return djutil::ezstr::utf32_to_utf8(us);}
            int setstr8(const std::string& s) {return this->setstr(djutil::ezstr::utf8_to_utf32(s));}
            int _invoke_action(const std::u32string& args, CCMDInvokationExtraData& edata);
            virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata){return this->_invoke_action(args,edata);}

    };
    class ConsoleIVAR : public _BaseConsoleCommand {
    	//using _BaseConsoleCommand::_BaseConsoleCommand;
        private:
            void _console_ivar_private_init();
    	public:
    	    fscalar_t _value = 0;
    	    virtual ccmdtype command_type() const {return ccmdtype::ivar;}
    	    inline operator fscalar_t() const {return this->_value;}
    	    virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata) {
    	    	this->_value = std::max(this->_value, edata.ivar_invalue);
    	    	return 0;
    	    }
            ConsoleIVAR(EngineInstance* _ei, const std::u32string& _n) : _BaseConsoleCommand(_ei, _n) {this->_console_ivar_private_init();}
    };

    dictionary<uid_t, EngineInstance*>* _engine_instances_byid = nullptr;
    uid_t _next_engine_instance_id = 1;

    class BindCCMD;
    class UnbindCCMD;

    struct bindcmdnames_t {
		std::u32string
			bind = U"bind",
			unbind = U"unbind",
			unbindall = U"unbindall"
		;
	};



    class LightSource;

    class SoundFactory;
    class SoundHandle;

    enum struct sndfactype : int {
        none = 0,
        stream = 1,
        buffer = 2
    };
    struct matprops_t;
    #ifdef GL45RW_WITH_SW3D3
        struct _SW3D3_DrawMesh_ShaderData {
            matprops_t* mp = nullptr;
            EngineInstance* ei = nullptr;
            struct {
                bool use;
                sw3d3::texture_t* tex = nullptr;
                Texture2D* texalt = nullptr;
                int sampler_unit = 0;
            } texslots[8] = {};
        };
        struct _SW3D3_BitmapFont_ShaderData {
            BitmapFont* font = nullptr;
            EngineInstance* ei = nullptr;
            char32_t curchar;
        };

        bool _sw3d3_drawmesh_fmain(sw3d3::pixelshader_t& self);
        bool _sw3d3_bitmapfont_fmain(sw3d3::pixelshader_t& self);
    #endif

    #ifdef _GL45RW_WINDOW_GLFW3
        bool _glfw_calc_appropriate_fullscreen_resolution(int& ow, int& oh, GLFWmonitor* monitor, const int& cw, const int& ch) {
            int modecount = 0;
            const GLFWvidmode* modes = glfwGetVideoModes(monitor, &modecount);
            for (const auto* cmodep = modes; cmodep < (modes+modecount); cmodep++){
                const auto& cmode = *cmodep;
                if (cw == cmode.width && ch == cmode.height){
                    ow = cw;
                    oh = ch;
                    return true;
                }
            }
            const GLFWvidmode* mmode = glfwGetVideoMode(monitor);
            ow = mmode->width;
            oh = mmode->height;
            return false;
        }
    #endif

    class UniversalWindow {
        public:
            enum wintype : int {
                closed = 0,
                sw = 1,
                gl = 2
            };
            enum swpixelformat : int {
                none = 0,
                rgb24 = 1,
                rgba32 = 2
            };
        private:
            wintype _windowtype = wintype::closed;
            int _width = 0, _height = 0;
            fscalar_t _aspect = 1;
            swpixelformat _swpixfmt = swpixelformat::none;
            bool _fullscreen = false;
            //void* _swpixels = nullptr;
        public:
            const wintype& window_type = _windowtype;
            const int &width = _width, &height = _height;
            const fscalar_t& aspect_ratio = _aspect;
            const swpixelformat& swpixfmt = _swpixfmt;
            const bool& fullscreen = _fullscreen;
            std::string window_title = "Unnamed window";
            //void* const& software_pixelbuf = _swpixels;
            #if (defined(_GL45RW_WINDOW_SDL2))
             SDL_Renderer* sdl2rend = nullptr;
             SDL_Texture* sdl2tex = nullptr;
             SDL_Window* win = nullptr;
             //const uint8_t* sdl2keystates = nullptr;
             //int sdl2numkeys;
             #ifdef __GL45RW_HAS_OPENGL
              SDL_GLContext glctx = nullptr;
             #endif

            #elif (defined(_GL45RW_WINDOW_GLFW3))
             GLFWwindow* win = nullptr;
            #endif

            inline bool is_closed() const {return this->window_type == wintype::closed;}
            inline bool is_software() const {return this->window_type == wintype::sw;}
            inline bool is_opengl() const {return this->window_type == wintype::gl;}

            void close() {
                this->make_current();
                #if (defined(_GL45RW_WINDOW_SDL2))
                if (this->sdl2tex != nullptr) {
                    SDL_DestroyTexture(this->sdl2tex);
                    this->sdl2tex = nullptr;
                }
                if (this->sdl2rend != nullptr) {
                    SDL_DestroyRenderer(this->sdl2rend);
                    this->sdl2rend = nullptr;
                }
                #ifdef __GL45RW_HAS_OPENGL
                if (this->glctx != nullptr)
                {
                    SDL_GL_DeleteContext(this->glctx);
                    this->glctx = nullptr;
                }
                #endif
                if (this->win != nullptr) {
                    SDL_DestroyWindow(this->win);
                    this->win = nullptr;
                }
                //this->sdl2keystates = nullptr;
                #elif (defined(_GL45RW_WINDOW_GLFW3))
                if (this->win != nullptr){
                    glfwDestroyWindow(this->win);
                    this->win = nullptr;
                }
                #endif

                this->_aspect = 1.0f;
                this->_width = this->_height = 0;
                this->_windowtype = wintype::closed;
                this->_fullscreen = false;
                this->_swpixfmt = swpixelformat::none;
            }

            bool create_opengl_window(const int& win_w, const int& win_h, const bool win_fullscreen=false) {
                #if (!defined(__GL45RW_HAS_OPENGL))
                    throw std::runtime_error("OpenGL universal windows are unsupported in this build.");
                #else
                    bool new_glcontext = (this->_windowtype != wintype::gl);
                    if (new_glcontext){
                        this->close();
                        #if (defined(_GL45RW_WINDOW_SDL2))
                            uint32_t sdl2winflags = SDL_WINDOW_OPENGL;
                            SDL_SetHintWithPriority(SDL_HINT_MOUSE_TOUCH_EVENTS, "0", SDL_HINT_OVERRIDE);
                            #ifdef _GL45RW_GLES3
                             //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
                            #endif
                            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
                            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _GLVERSION[0]);
                            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _GLVERSION[1]);
                            if (win_fullscreen){sdl2winflags |= SDL_WINDOW_FULLSCREEN;}
                            if ((this->win = SDL_CreateWindow(
                                   this->window_title.c_str(),
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   win_w, win_h,
                                   sdl2winflags
                                )) == nullptr)
                            {
                                std::stringstream errss;
                                errss << "SDL_CreateWindow() failed: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }

                            if ((this->glctx = SDL_GL_CreateContext(this->win)) == nullptr)
                            {
                                std::stringstream errss;
                                errss << "SDL_GL_CreateContext() failed: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }

                            //SDL_GetWindowSize(this->win, &this->_width, &this->_height);
                            this->_width = win_w;
                            this->_height = win_h;
                            SDL_GL_MakeCurrent(this->win, this->glctx);
                            SDL_GL_SetSwapInterval(0);

                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                            #if (defined(_GL45RW_GLES3))
                                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                            #elif (defined(_GL45RW_GL2))
                                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
                                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
                            #endif
                            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GLVERSION[0]);
                            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GLVERSION[1]);
                            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                            GLFWmonitor* _monitor = (win_fullscreen ? glfwGetPrimaryMonitor() : nullptr);
                            int gw = win_w, gh = win_h;
                            if (win_fullscreen){_glfw_calc_appropriate_fullscreen_resolution(gw, gh, _monitor, win_w, win_h);}
                            if ((this->win = glfwCreateWindow(gw, gh, this->window_title.c_str(), _monitor, nullptr)) == nullptr){
                                throw std::runtime_error("glfwCreateWindow() failed.");
                            }
                            glfwMakeContextCurrent(this->win);
                            glfwSwapInterval(0);
                            glfwGetFramebufferSize(this->win, &this->_width, &this->_height);
                        #endif
                    }
                    else {
                        #if (defined(_GL45RW_WINDOW_SDL2))
                            SDL_SetWindowSize(this->win, win_w, win_h);
                            SDL_SetWindowFullscreen(this->win, win_fullscreen);
                            SDL_GetWindowSize(this->win, &this->_width, &this->_height);
                            SDL_SetWindowTitle(this->win, this->window_title.c_str());
                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                            glfwMakeContextCurrent(this->win);
                            //glfwSetWindowSize(this->win, win_w, win_h);
                            GLFWmonitor* _monitor = glfwGetPrimaryMonitor();
                            int gw = win_w, gh = win_h;
                            if (win_fullscreen){
                                _glfw_calc_appropriate_fullscreen_resolution(gw, gh, _monitor, win_w, win_h);
                                glfwSetWindowMonitor(this->win, _monitor, 0, 0, gw, gh, 0);
                            }
                            else {
                                const GLFWvidmode* mmode = glfwGetVideoMode(_monitor);
                                int winposx = mmode->width / 2;
                                int winposy = mmode->height / 2;
                                glfwSetWindowMonitor(this->win, nullptr, winposx, winposy, gw, gh, 0);
                            }
                            glfwGetFramebufferSize(this->win, &this->_width, &this->_height);
                        #endif
                    }
                    this->_aspect = this->_width / fscalar_t(this->_height);
                    this->_windowtype = wintype::gl;
                    this->_fullscreen = win_fullscreen;
                    this->make_current();
                    return new_glcontext;
                #endif
            }

            bool create_software_window(const int& win_w, const int& win_h, const bool win_fullscreen=false, const swpixelformat pixfmt=swpixelformat::rgb24) {
                #if (defined(_GL45RW_HEADLESS) || defined(_GL45RW_WINDOW_GLFW3))
                    throw std::runtime_error("Software universal windows are unsupported in this build.");
                #else
                    bool new_swcontext = (this->_windowtype != wintype::sw);
                    if (new_swcontext){
                        this->close();
                        #if (defined(_GL45RW_WINDOW_SDL2))
                            SDL_SetHintWithPriority(SDL_HINT_MOUSE_TOUCH_EVENTS, "0", SDL_HINT_OVERRIDE);
                            uint32_t sdl2winflags = 0;
                            if (win_fullscreen){sdl2winflags |= SDL_WINDOW_FULLSCREEN;}
                            SDL_CreateWindowAndRenderer(win_w, win_h, sdl2winflags, &(this->win), &(this->sdl2rend));
                            if (this->win == nullptr) {
                                std::stringstream errss;
                                errss << "SDL_CreateWindow() failed: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }
                            if (this->sdl2rend == nullptr) {
                                std::stringstream errss;
                                errss << "failed to create the SDL_Renderer: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }

                            SDL_SetWindowTitle(this->win, this->window_title.c_str());

                            uint32_t sdl2texfmt;
                            switch (pixfmt) {
                                case swpixelformat::rgb24: sdl2texfmt = SDL_PIXELFORMAT_RGB24; break;
                                case swpixelformat::rgba32: sdl2texfmt = SDL_PIXELFORMAT_RGBA32; break;
                                default: throw std::runtime_error("Bad pixelformat.");
                            }

                            this->sdl2tex = SDL_CreateTexture(
                                this->sdl2rend,
                                sdl2texfmt,
                                SDL_TEXTUREACCESS_STREAMING,
                                win_w,
                                win_h
                            );

                            if (this->sdl2tex == nullptr) {
                                std::stringstream errss;
                                errss << "failed to create the SDL_Texture: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }
                            SDL_QueryTexture(this->sdl2tex,
                                nullptr, nullptr,
                                &(this->_width),
                                &(this->_height)
                            );
                            SDL_SetRenderDrawColor(this->sdl2rend, 0, 0, 0, 0);
                            SDL_RenderClear(this->sdl2rend);
                        #endif
                    }
                    else {
                        #if (defined(_GL45RW_WINDOW_SDL2))
                            if (this->sdl2tex != nullptr) {
                                SDL_DestroyTexture(this->sdl2tex);
                                this->sdl2tex = nullptr;
                            }

                            SDL_SetWindowSize(this->win, win_w, win_h);
                            SDL_SetWindowFullscreen(this->win, win_fullscreen);
                            //SDL_GetWindowSize(this->win, &this->_width, &this->_height);
                            SDL_SetWindowTitle(this->win, this->window_title.c_str());

                            uint32_t sdl2texfmt;
                            switch (pixfmt) {
                                case swpixelformat::rgb24: sdl2texfmt = SDL_PIXELFORMAT_RGB24; break;
                                case swpixelformat::rgba32: sdl2texfmt = SDL_PIXELFORMAT_RGBA32; break;
                                default: throw std::runtime_error("Bad pixelformat.");
                            }

                            this->sdl2tex = SDL_CreateTexture(
                                this->sdl2rend,
                                sdl2texfmt,
                                SDL_TEXTUREACCESS_STREAMING,
                                win_w,
                                win_h
                            );

                            if (this->sdl2tex == nullptr) {
                                std::stringstream errss;
                                errss << "failed to create the SDL_Texture: " << SDL_GetError();
                                throw std::runtime_error(errss.str());
                            }
                            SDL_QueryTexture(this->sdl2tex,
                                nullptr, nullptr,
                                &(this->_width),
                                &(this->_height)
                            );
                        #endif
                    }
                    this->_swpixfmt = pixfmt;
                    this->_aspect = this->_width / fscalar_t(this->_height);
                    this->_windowtype = wintype::sw;
                    this->_fullscreen = win_fullscreen;
                    this->make_current();
                    return new_swcontext;
                #endif
            }

            void make_current(){
                switch (this->_windowtype){
                    case wintype::gl: {
                        #if (defined(_GL45RW_WINDOW_SDL2) && defined(__GL45RW_HAS_OPENGL))
                            SDL_GL_MakeCurrent(this->win, this->glctx);
                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                            glfwMakeContextCurrent(this->win);
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            void* lock_swpixels() {
                if (this->_windowtype != wintype::sw){return nullptr;}
                #if (defined(_GL45RW_WINDOW_SDL2))
                void* ret = nullptr; int pitch;
                SDL_LockTexture(this->sdl2tex, nullptr, &ret, &pitch);
                return ret;
                #else
                return nullptr;
                #endif
            }
            void unlock_swpixels() {
                if (this->_windowtype != wintype::sw){return;}
                #if (defined(_GL45RW_WINDOW_SDL2))
                SDL_UnlockTexture(this->sdl2tex);
                SDL_RenderCopy(this->sdl2rend, this->sdl2tex, nullptr, nullptr);
                #else
                return;
                #endif
            }

            void swap_buffers(const void* _sw_blit_this=nullptr, const bool clear=true){
                this->make_current();
                switch (this->_windowtype) {
                    case wintype::gl: {
                        #if (defined(_GL45RW_WINDOW_SDL2) && defined(__GL45RW_HAS_OPENGL))
                            SDL_GL_SwapWindow(this->win);
                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                            glfwSwapBuffers(this->win);
                        #endif
                        break;
                    }
                    case wintype::sw: {
                        #if (defined(_GL45RW_WINDOW_SDL2))
                            size_t cpp = 0;
                            switch (this->_swpixfmt){
                                case swpixelformat::rgb24: cpp = 3; break;
                                case swpixelformat::rgba32: cpp = 4; break;
                                default: throw std::runtime_error("Invalid pixelfmt.");
                            }
                            const size_t pxasz = this->_width * this->_height * cpp;
                            const size_t npx = this->_width * this->_height;
                            void* rpx = nullptr;
                            int pitch;
                            if (_sw_blit_this != nullptr){
                                SDL_LockTexture(this->sdl2tex, nullptr, &rpx, &pitch);
                                memcpy(rpx, _sw_blit_this, pxasz);
                                SDL_UnlockTexture(this->sdl2tex);
                                SDL_RenderCopy(this->sdl2rend, this->sdl2tex, nullptr, nullptr);
                            }
                            else if (clear) {
                                SDL_RenderClear(this->sdl2rend);
                            }
                            SDL_RenderPresent(this->sdl2rend);
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            ~UniversalWindow(){
                this->close();
            }

    };


    class EngineInstance {
        public:
            UniversalWindow uwin;
        private:
            uid_t _instance_id = 0;
            uid_t cur_assetid = 1;
            friend class AssetRef;
            friend class SoundFactory;
            friend class SoundHandle;
            std::unordered_set<uid_t> _EngineInstance_assets_marked_garbage;
            soundmode_t _cursndmode = {};
			//fscalar_t _lcg_curseed = 1.0f;
			size_t _rnd_curpos = 0;
			unsigned int _rnd_values[1024] = {};

            bool _mouse_locked = false;

            #ifdef GL45RW_WITH_OPENAL
             ALCdevice* _aldev = nullptr;
             ALCcontext* _alctx = nullptr;
            #endif

            videomode_t _curmode = {};
            #if (defined(_GL45RW_WINDOW_SDL2))
             SDL_Renderer* sdl2rend = nullptr;
             SDL_Texture* sdl2tex = nullptr;
             SDL_Window* win = nullptr;
            #elif (defined(_GL45RW_WINDOW_GLFW3))
             GLFWwindow* win = nullptr;
            #endif



            #ifdef __GL45RW_HAS_OPENGL
             #ifdef _GL45RW_WINDOW_SDL2
             SDL_GLContext glctx = nullptr;
             #endif
             AutoPointer<_glstuff::GLFramebufferObject> gl_screenfbo, gl_winfbo, gles3_deferred_screenfbo;
             int _gl_screenfbo_width = 0, _gl_screenfbo_height = 0;

             void _opengl_regen_screenfbo() {
                 if (!this->gl_screenfbo){this->gl_screenfbo();}
                 else {this->gl_screenfbo->clear_all_attachments();}

                 if (!this->gl_winfbo){this->gl_winfbo();}
                 else {this->gl_winfbo->clear_all_attachments();}

                 this->gl_winfbo->attach_tex2D(GL_COLOR_ATTACHMENT0, GL_RGBA8, this->_curmode.width, this->_curmode.height);
                 this->gl_winfbo->attach_rbuf(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT32F, this->_curmode.width, this->_curmode.height);
                 this->gl_winfbo->throw_if_incomplete();

                 this->_gl_screenfbo_width = glm::max((this->_curmode.width * this->_curmode.opengl_resolution_percent) / 100, 1);
                 this->_gl_screenfbo_height = glm::max((this->_curmode.height * this->_curmode.opengl_resolution_percent) / 100, 1);

                 this->gl_screenfbo->attach_tex2D(GL_COLOR_ATTACHMENT0, GL_RGBA8, this->_gl_screenfbo_width, this->_gl_screenfbo_height);
                 this->gl_screenfbo->attach_rbuf(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT32F, this->_gl_screenfbo_width, this->_gl_screenfbo_height);

                 #ifdef _GL45RW_GLES3
                     if (!this->gles3_deferred_screenfbo){this->gles3_deferred_screenfbo();}
                     else {this->gles3_deferred_screenfbo->clear_all_attachments();}
                     _glstuff::GLFramebufferObject* dsfbo = this->gles3_deferred_screenfbo;
                     dsfbo->attach_tex2D(GL_COLOR_ATTACHMENT0, GL_RGBA32F, this->_gl_screenfbo_width, this->_gl_screenfbo_height); //shadeless mask, diffuse intensity / 10, specular intensity / 10, specular hardness gbuffer
                     dsfbo->attach_tex2D(GL_COLOR_ATTACHMENT1, GL_RGBA32F, this->_gl_screenfbo_width, this->_gl_screenfbo_height); //ambient color gbuffer
                     dsfbo->attach_tex2D(GL_COLOR_ATTACHMENT2, GL_RGBA32F, this->_gl_screenfbo_width, this->_gl_screenfbo_height); //diffuse color gbuffer
                     dsfbo->attach_tex2D(GL_COLOR_ATTACHMENT3, GL_RGBA32F, this->_gl_screenfbo_width, this->_gl_screenfbo_height); //specular color gbuffer
                     dsfbo->throw_if_incomplete();
                 #endif

                 this->gl_screenfbo->throw_if_incomplete();
                 this->gl_screenfbo->bind(GL_DRAW_FRAMEBUFFER);
             }
            #endif
            uintmax_t _current_ticno = 0;
        protected:
            #ifdef __GL45RW_HAS_OPENGL
                _glstuff::GLShaderProgram* _bitmap_text_glshader = nullptr, *_window_blit_glshader = nullptr, *_gles3_conback_glshader = nullptr, *_gles3_testtri_glshader = nullptr;
                GLuint _gles3_attributeless_vao = 0;
            #endif
            #ifdef GL45RW_WITH_SW3D2
             dj_sw3d20::Material _bitmap_text_mat_sw3d2 = {};
            #endif

            djutil::virfs::VirtualFS* _main_vfs = nullptr;
            uid_t _current_eid = 1;
            //dictionary<uid_t, entity*> ents_byid = {};
            //vectorlist<entity*> ents_all = {}, _ents_pending_deletion = {};
            matstacks_t matrices = {};


            enum struct _constate : int {
                closed = 0,
                opening = 1,
                opened = 2,
                closing = 3
            };

            struct _consoledatatype {
                bool _init = false;
                bool allowconsole = true;
                bool _pending_refresh = false;
                _constate state = _constate::closed;
                fscalar_t openamount = 0.0f;
                fscalar_t openspeed = 8.0f, closespeed = 8.0f;
                size_t maxlines = 1;
                size_t linewrap = 200;
                vectorlist<std::u32string> lines = {};
                vectorlist<std::u32string> history = {};
                std::u32string curlines = U"";
                std::u32string curinput = U"", _curinput_hold = U"";
                uid_t fontid = 0;
                int font_npx = 30;
                fscalar_t opened_bottom_v = -0.75f;
                size_t _conmaxheight = 0;
                size_t _curhist = -1;
            };

            _consoledatatype console_data = {};

            void _deleteSlatedEntities() {
                /*
                for (entity*& e : this->_ents_pending_deletion) {
                    this->ents_all.eraseFirstOf(e);
                    this->ents_byid.popk(e.eid);
                    delete e; e = nullptr;
                }
                this->_ents_pending_deletion.clear();
                */
            }
            void _deleteAllEntities() {
                /*
                this->_ents_pending_deletion.clear();
                for (auto e : this->ents_all){this->_ents_pending_deletion.push_back(e); e->_pending_deletion = true;}
                this->_deleteSlatedEntities();
                */
            }

            void _resetConsoleData() {
                this->del_asset(this->console_data.fontid);
                this->console_data.fontid = 0;
            }
            void _createConsoleData();

            void _updateConsoleDataNewRes() {
                //this->console_data._conmaxheight = this->_curmode.height * (1.0f - ((this->console_data.opened_bottom_v - (-1.0f))*0.5f));
                const fscalar_t obvn = (this->console_data.opened_bottom_v);
                this->console_data._conmaxheight = this->_curmode.height * (1-obvn);
                int new_maxlines = (this->console_data._conmaxheight / this->console_data.font_npx) - 1;
                int new_linewrap = (this->_curmode.width / this->console_data.font_npx)-1;

                this->console_data.maxlines = new_maxlines;
                this->console_data.linewrap = new_linewrap;
                this->console_data._pending_refresh = true;
            }
            void _rebuild_all_materials();

            soundmode_t _full_SetSoundMode(const soundmode_t& newmode){
                this->S_MakeCurrent();
                std::vector<AssetBase*> factories = {}, handles = {};
                for (auto& iap : this->assets){
                    switch (iap.second->asset_type()){
                        case assettype::sndfactory: factories.push_back(iap.second); break;
                        case assettype::sndhandle: handles.push_back(iap.second); break;
                        default: ;
                    }
                }

                for (auto& hnd : handles){hnd->onSoundContextDelete();}
                for (auto& fac : factories){fac->onSoundContextDelete();}

                #if (defined(GL45RW_WITH_OPENAL))
                    if (this->_alctx != nullptr){
                        alcMakeContextCurrent(nullptr);
                        alcDestroyContext(this->_alctx);
                        this->_alctx = nullptr;
                    }
                    if (this->_aldev != nullptr){
                        alcCloseDevice(this->_aldev);
                        this->_aldev = nullptr;
                    }
                    //while (alGetError() != ALC_NO_ERROR){}
                #endif
                switch (newmode.renderer){
                    case sndapi::openal: {
                        #if (defined(GL45RW_WITH_OPENAL))
                            if ((this->_aldev = alcOpenDevice(nullptr)) == nullptr){
                                throw std::runtime_error("OpenAL init failed: couldn't open the selected device!");
                            }
                            //while (alGetError() != ALC_NO_ERROR){}
                            if (!alcMakeContextCurrent((this->_alctx = alcCreateContext(this->_aldev, nullptr)))){
                                alcCloseDevice(this->_aldev); this->_aldev = nullptr;
                                //while (alGetError() != ALC_NO_ERROR);
                                throw std::runtime_error("OpenAL init failed: context creation failed!");
                            }
                            //while (alGetError() != ALC_NO_ERROR){}
                            ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

                            alListener3f(AL_POSITION, 0, 0, 1.0f); alGetError();
                            // check for errors
                            alListener3f(AL_VELOCITY, 0, 0, 0); alGetError();
                            // check for errors
                            alListenerfv(AL_ORIENTATION, listenerOri); alGetError();
                            // check for errors
                            this->_cursndmode = newmode;
                        #else
                            throw std::runtime_error("OpenAL init failed: OpenAL support was disabled for this build.");
                        #endif
                        break;
                    }
                    case sndapi::none: {
                        this->_cursndmode = newmode;
                        break;
                    }
                    default: throw std::runtime_error("unknown sound backend.");
                }

                for (auto& fac : factories){fac->onSoundContextCreate();}
                for (auto& hnd : handles){hnd->onSoundContextCreate();}

                return this->_cursndmode;
            }
            #ifdef GL45RW_WITH_SW3D3
            static bool _sw3d3_testtri_fmain(sw3d3::pixelshader_t& self) {
                auto* pv = self.f_in.pv;
                self.f_out.fragcolor = sw3d3::TestContext::_bclerp(self.f_in.pc_bcw, pv[0].color, pv[1].color, pv[2].color);
                return true;
            }
            #endif

            videomode_t _full_SetVideoMode(const videomode_t& newmode)
            {
                this->R_MakeCurrent();
                for (auto& iap : this->assets)
                {
                    iap.second->onRenderContextDelete();
                }
                this->_resetConsoleData();
                #if (defined(_GL45RW_HEADLESS))
                    this->_curmode = videomode_t{
                        .renderer=gfxapi::none,
                        .width=0, .height=0
                    };
                #else

                    #if (defined(_GL45RW_WINDOW_SDL2))
                        if (this->win != nullptr){
                            _unregister_instance_window(SDL_GetWindowID(this->win));
                        }
                        this->sdl2tex = nullptr;
                        this->sdl2rend = nullptr;
                        this->win = nullptr;
                    #elif (defined(_GL45RW_WINDOW_GLFW3))
                        _unregister_instance_glfw_window(this->win);
                        this->win = nullptr;
                    #endif

                    #ifdef __GL45RW_HAS_OPENGL
                        if (this->uwin.is_opengl()){
                            delete this->_bitmap_text_glshader;
                            delete this->_window_blit_glshader;
                            delete this->_gles3_conback_glshader;
                            delete this->_gles3_testtri_glshader;
                            ~(this->gl_screenfbo);
                            ~(this->gl_winfbo);
                            ~(this->gles3_deferred_screenfbo);
                            if (this->_gles3_attributeless_vao != 0){
                                #if (defined(_GL45RW_GLES3))
                                this->_glcalls.glDeleteVertexArrays(1, &(this->_gles3_attributeless_vao));
                                #endif
                                this->_gles3_attributeless_vao = 0;
                            }
                        }
                        this->_bitmap_text_glshader = nullptr;
                        this->_window_blit_glshader = nullptr;
                        this->_gles3_conback_glshader = nullptr;
                        this->_gles3_testtri_glshader = nullptr;
                        this->_glcalls = _glstuff::GLcalls_t{};
                        if (_glstuff::curgl == &(this->_glcalls)) {
                            _glstuff::curgl = nullptr;
                        }
                        #ifdef _GL45RW_WINDOW_SDL2
                            this->glctx = nullptr;
                        #endif
                    #endif


                    #ifdef GL45RW_WITH_SW3D2
                        delete this->_sw3d2_screen_fbo;
                        this->_sw3d2_screen_fbo = nullptr;
                        this->_sw3d2_window_fbo = nullptr;
                        if (this->sw3d2ctx != nullptr) {
                            delete this->sw3d2ctx;
                        }
                        this->sw3d2ctx = nullptr;
                    #endif
                    #ifdef GL45RW_WITH_SW3D3
                        if (this->sw3d3ctx != nullptr){
                            this->_sw3d3_screen_fbo = nullptr;
                            this->_sw3d3_window_fbo = nullptr;
                            this->_sw3d3_screen_colortex = nullptr;
                            this->_sw3d3_screen_depthtex = nullptr;
                            this->_sw3d3_window_colortex = nullptr;
                            this->_sw3d3_window_depthtex = nullptr;

                            delete this->sw3d3ctx; this->sw3d3ctx = nullptr;
                        }
                    #endif
                    /*
                    if (this->win != nullptr)
                    {
                        _unregister_instance_window(SDL_GetWindowID(this->win));
                        SDL_DestroyWindow(this->win);
                        this->win = nullptr;
                    }
                    */
                    this->uwin.close();
                    this->_curmode = newmode;
                    std::string window_title = djutil::ezstr::utf32_to_utf8(newmode.title);
                    this->uwin.window_title = window_title;

                    switch (newmode.renderer)
                    {
                        case gfxapi::opengl:
                        {
                            #if (defined(__GL45RW_HAS_OPENGL))

                                this->uwin.create_opengl_window(newmode.width, newmode.height, newmode.fullscreen);

                                this->_curmode.width = this->uwin.width;
                                this->_curmode.height = this->uwin.height;

                                #if (defined(_GL45RW_WINDOW_SDL2))
                                    auto _glgetproc = SDL_GL_GetProcAddress;
                                #elif (defined(_GL45RW_WINDOW_GLFW3))
                                    auto _glgetproc = glfwGetProcAddress;
                                #endif

                                #if (defined(_GL45RW_GL2))
                                int _glver = gladLoadGLContext(&this->_glcalls._gladctx, (GLADloadfunc)_glgetproc);
                                #elif (defined(_GL45RW_GLES3))
                                int _glver = gladLoadGLES2Context(&this->_glcalls._gladctx, (GLADloadfunc)_glgetproc);
                                #endif

                                // load our OpenGL functions
                                #define _gl45rw_glc2(_rtype, _glname, _glargs, ...) \
                                _glstuff::_glcall_ric(this->_glcalls.gl ## _glname, this->_glcalls._gladctx._glname); \
                                if (this->_glcalls.gl ## _glname == nullptr){ \
                                    this->_glcalls.gl ## _glname = (_rtype (*) _glargs)_glgetproc("gl" #_glname); \
                                    std::cout << "[GL Info]: manually loaded gl" << #_glname << " because GLAD didn't\n"; \
                                } \

                                _GL45RW_OPENGL_CALLS_M(_gl45rw_glc2);
                                #undef _gl45rw_glc2
                                this->_glcalls.glFramebufferParameteriEXT = this->_glcalls.glFramebufferParameteri;

                                std::cout << "loaded OpenGL\n";


                                _glstuff::curgl = &(this->_glcalls);

                                _glstuff::GLShaderObject
                                    btv(GL_VERTEX_SHADER),
                                    btf(GL_FRAGMENT_SHADER)
                                ;


                                this->_bitmap_text_glshader = new _glstuff::GLShaderProgram(_glstuff::curgl->glCreateProgram(), true);

                                this->_bitmap_text_glshader->attach(btv(_BITMAP_TEXT_GLSL_COMMON + _BITMAP_TEXT_GLSL_VERTEX).throw_if_error());
                                this->_bitmap_text_glshader->attach(btf(_BITMAP_TEXT_GLSL_COMMON + _BITMAP_TEXT_GLSL_FRAGMENT).throw_if_error());

                                this->_bitmap_text_glshader->link().detach_all().throw_if_error();

                                std::cout << "built bitmap_text_glshader\n";

                                this->_window_blit_glshader = new _glstuff::GLShaderProgram(_glstuff::curgl->glCreateProgram(), true);
                                this->_gles3_attributeless_vao = 0;
                                #if (defined(_GL45RW_GLES3))
                                    _glstuff::curgl->glGenVertexArrays(1, &(this->_gles3_attributeless_vao));

                                    {
                                        this->_gles3_testtri_glshader = new _glstuff::GLShaderProgram(_glstuff::curgl->glCreateProgram(), true);

                                        _glstuff::GLSLCodeGenerator ttgen; ttgen.commonsrc.header();
                                        ttgen.uniform_def("mat4", "pvm");
                                        ttgen.varying_def("vec3", "v_color");
                                        ttgen.frag_header();
                                        ttgen.vertexsrc << R"(
const vec3 v_triangle_co[3] = vec3[3](
    vec3(-1.0, -1.0, 0.0),
    vec3(1.0, -1.0, 0.0),
    vec3(0.0, 1.0, 0.0)
);

const vec3 v_triangle_colors[3] = vec3[3](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);
                                        )";

                                        {_glstuff::GLSLAutoFuncDef f(ttgen.vertexsrc, "void", "main", {});
                                            f << "v_color = v_triangle_colors[gl_VertexID];"; f->startline();
                                            f << "gl_Position = pvm * vec4(v_triangle_co[gl_VertexID], 1.0);"; f->startline();
                                        }

                                        {_glstuff::GLSLAutoFuncDef f(ttgen.fragsrc, "void", "main", {});
                                            ttgen.frag_out(0, "vec4(v_color, 1.0);"); f->startline();
                                        }

                                        _glstuff::GLShaderObject
                                            ttv(GL_VERTEX_SHADER),
                                            ttf(GL_FRAGMENT_SHADER)
                                        ;

                                        this->_gles3_testtri_glshader->attach(ttv(ttgen.commonsrc.str() + ttgen.vertexsrc.str()).throw_if_error());
                                        this->_gles3_testtri_glshader->attach(ttf(ttgen.commonsrc.str() + ttgen.fragsrc.str()).throw_if_error());
                                        this->_gles3_testtri_glshader->link().detach_all().throw_if_error();
                                    }

                                #endif

                                _glstuff::GLSLCodeGenerator wbgen; wbgen.commonsrc.header();

                                wbgen.uniform_def("sampler2D", "screentex");
                                wbgen.uniform_def("sampler2D", "windowtex");
                                wbgen.uniform_def("float", "gamma");

                                wbgen.frag_header();

                                wbgen.varying_def("vec2", "v_texcoord");

                                #if (defined(_GL45RW_GLES3))
                                 wbgen.vertexsrc << R"(
const vec3 v_rect_coords[4] = vec3[4](
    vec3(-1.0,1.0,0.0), //top-left
    vec3(1.0,1.0,0.0), //top-right
    vec3(-1.0,-1.0,0.0), //bottom-left
    vec3(1.0,-1.0,0.0)  //bottom-right
);

const vec2 v_rect_baseuvs[4] = vec2[4](
    vec2(0.0,1.0), //top-left
    vec2(1.0,1.0), //top-right
    vec2(0.0,0.0), //bottom-left
    vec2(1.0,0.0)  //bottom-right
);

const int v_rect_indices[6] = int[6](
    0,2,1,
    2,3,1
);
)";
                                #endif

                                {_glstuff::GLSLAutoFuncDef f(wbgen.vertexsrc, "void", "main", {});
                                    #if (defined(_GL45RW_GLES3))
                                    f << "int idx = v_rect_indices[gl_VertexID];"; f->startline();
                                    f << "v_texcoord = v_rect_baseuvs[idx];"; f->startline();
                                    f << "gl_Position = vec4(v_rect_coords[idx].xyz, 1.0);"; f->startline();
                                    #else
                                    f << "v_texcoord = mix(vec2(0.5), vec2(1.0), gl_Vertex.xy);"; f->startline();
                                    f << "gl_Position = vec4(gl_Vertex.xyz, 1.0);"; f->startline();
                                    #endif
                                }

                                {_glstuff::GLSLAutoFuncDef f(wbgen.fragsrc, "void", "main", {});
                                    f->localvardef("vec4", "screencolor", "texture2D(screentex, v_texcoord)");
                                    f->localvardef("vec4", "wincolor", "texture2D(windowtex, v_texcoord)");
                                    wbgen.frag_out(0, "vec4(mix(pow(screencolor.xyz, vec3(gamma)), wincolor.xyz, wincolor.w), 1.0);"); f->startline();
                                    //wbgen.frag_out(0, "vec4(texture2D(windowtex, v_texcoord).xyz, 1.0);"); f->startline();
                                }

                                _glstuff::GLShaderObject
                                    wbv(GL_VERTEX_SHADER),
                                    wbf(GL_FRAGMENT_SHADER)
                                ;

                                this->_window_blit_glshader->attach(wbv(wbgen.commonsrc.str() + wbgen.vertexsrc.str()).throw_if_error());
                                this->_window_blit_glshader->attach(wbf(wbgen.commonsrc.str() + wbgen.fragsrc.str()).throw_if_error());
                                this->_window_blit_glshader->link().detach_all().throw_if_error();

                                std::cout << "built window_blit_glshader\n";

                                #ifdef _GL45RW_GLES3

                                _glstuff::GLSLCodeGenerator cbgen; cbgen.commonsrc.header();
                                cbgen.uniform_def("vec4", "color");
                                cbgen.uniform_def("float", "ypos");
                                cbgen.frag_header();
                                cbgen.vertexsrc << R"(
const vec2 v_rectcoords[6] = vec2[6](
    vec2(-1.0, -0.01),
    vec2(1.0, -0.01),
    vec2(-1.0, 2.0),
    vec2(-1.0, 2.0),
    vec2(1.0, 2.0),
    vec2(1.0, -0.01)
);
)";
                                {_glstuff::GLSLAutoFuncDef f(cbgen.vertexsrc, "void", "main", {});
                                    f << "gl_Position = vec4(vec3(v_rectcoords[gl_VertexID], -1) + vec3(0, ypos, 0), 1.0);";
                                }

                                {_glstuff::GLSLAutoFuncDef f(cbgen.fragsrc, "void", "main", {});
                                    cbgen.frag_out(0, "color");
                                }

                                this->_gles3_conback_glshader = new _glstuff::GLShaderProgram(_glstuff::curgl->glCreateProgram(), true);
                                _glstuff::GLShaderObject
                                    cbvs(GL_VERTEX_SHADER),
                                    cbfs(GL_FRAGMENT_SHADER)
                                ;

                                this->_gles3_conback_glshader->attach(cbvs(cbgen.commonsrc.str() + cbgen.vertexsrc.str()).throw_if_error());
                                this->_gles3_conback_glshader->attach(cbfs(cbgen.commonsrc.str() + cbgen.fragsrc.str()).throw_if_error());
                                this->_gles3_conback_glshader->link().detach_all().throw_if_error();

                                #endif


                                this->_glcalls.glClearColor(0,0,0,1);
                                std::cout << "call glClearColor\n";
                                this->_glcalls.glViewport(0,0, this->_curmode.width, this->_curmode.height);
                                std::cout << "call glViewport\n";
                                this->_glcalls.glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                                std::cout << "call glClear\n";

                                this->_opengl_regen_screenfbo();
                                std::cout << "regen_screenfbo\n";

                                this->uwin.swap_buffers();
                                std::cout << "uwin swap_buffers\n";
                                this->gl_screenfbo->bind(GL_DRAW_FRAMEBUFFER);
                                std::cout << "bind gl_screenfbo\n";

                                #if (defined(_GL45RW_WINDOW_SDL2))
                                    this->win = this->uwin.win;
                                    this->glctx = this->uwin.glctx;
                                #elif (defined(_GL45RW_WINDOW_GLFW3))
                                    this->win = this->uwin.win;
                                #endif

                                break;
                            #else
                                throw std::runtime_error("OpenGL window creation is disabled for this build.");
                            #endif
                            break;
                        }
                        case gfxapi::sw3d2: {
                            #if (defined(GL45RW_WITH_SW3D2))
                                this->uwin.create_software_window(newmode.width, newmode.height, newmode.fullscreen);

                                this->_curmode.width = this->uwin.width;
                                this->_curmode.height = this->uwin.height;

                                this->sw3d2ctx = new dj_sw3d20::Context();
                                glm::ivec2 scaled_size = glm::max((glm::ivec2{this->_curmode.width, this->_curmode.height}*this->_curmode.sw3d2_resolution_percent)/100, 0);
                                //this->sw3d2ctx->srContextInit(scaled_size.x, scaled_size.y, dj_sw3d20::enum_t::RGB);
                                //this->_sw3d2_screen_fbo = this->sw3d2ctx->srActiveFBO();
                                this->sw3d2ctx->srContextInit(this->_curmode.width, this->_curmode.height, dj_sw3d20::enum_t::RGB);
                                this->_sw3d2_window_fbo = this->sw3d2ctx->srActiveFBO();
                                this->_sw3d2_screen_fbo = this->sw3d2ctx->srCreateManagedFBO();
                                this->_sw3d2_screen_fbo->init(scaled_size.x, scaled_size.y);
                                this->sw3d2ctx->srActiveFBO(this->_sw3d2_screen_fbo);
                                this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, false);
                                this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING, false);
                                this->sw3d2ctx->srCullFace(dj_sw3d20::enum_t::BACK_FACE);
                                this->_sw3d2_winfbo_drawn = true;

                                #if (defined(_GL45RW_WINDOW_SDL2))
                                    this->win = this->uwin.win;
                                #elif (defined(_GL45RW_WINDOW_GLFW3))
                                    this->win = this->uwin.win;
                                #endif

                            #else
                                throw std::runtime_error("the SW3D 2 renderer was disabled in this build.\n(GL45RW_WITH_SW3D2 wasn't #define'd.)");
                            #endif
                            break;
                        }
                        case gfxapi::sw3d3: {
                            #if (defined(GL45RW_WITH_SW3D3))
                                this->uwin.create_software_window(newmode.width, newmode.height, newmode.fullscreen);

                                this->_curmode.width = this->uwin.width;
                                this->_curmode.height = this->uwin.height;

			                    this->sw3d3ctx = new sw3d3::TestContext();
			                    this->sw3d3ctx->setDimensions(this->_curmode.width, this->_curmode.height);

			                    this->_sw3d3_window_fbo = this->sw3d3ctx->getFBO(this->sw3d3ctx->getCurrentDrawFBO());

			                    this->_sw3d3_window_colortex = this->sw3d3ctx->getTexture(this->_sw3d3_window_fbo->attachments[0].texture);
			                    this->_sw3d3_window_depthtex = this->sw3d3ctx->getTexture(this->_sw3d3_window_fbo->attachments[1].texture);

                                //this->_sw3d3_screen_fbo = this->sw3d3ctx->getFBO(this->sw3d3ctx->newFBO());
                                //this->_sw3d3_screen_fbo->w = (this->_curmode.width * this->_curmode.sw3d3_resolution_percent) / 100;
                                //this->_sw3d3_screen_fbo->h = (this->_curmode.height * this->_curmode.sw3d3_resolution_percent) / 100;

                                //this->_sw3d3_screen_colortex = this->sw3d3ctx->getTexture(this->sw3d3ctx->newTexture());
                                //this->_sw3d3_screen_depthtex = this->sw3d3ctx->getTexture(this->sw3d3ctx->newTexture());

                                //this->_sw3d3_screen_colortex->texImage2D(sw3d3::textarget_e::target2d, this->_sw3d3_screen_fbo->w, this->_sw3d3_screen_fbo->h, sw3d3::texelfmt_e::rgb8, nullptr);
                                //this->_sw3d3_screen_depthtex->texImage2D(sw3d3::textarget_e::target2d, this->_sw3d3_screen_fbo->w, this->_sw3d3_screen_fbo->h, sw3d3::texelfmt_e::depth32f, nullptr);

                                //this->_sw3d3_screen_fbo->attach(sw3d3::fbo_attachment_e::color, this->_sw3d3_screen_colortex->idx);
                                //this->_sw3d3_screen_fbo->attach(sw3d3::fbo_attachment_e::depth, this->_sw3d3_screen_depthtex->idx);

                                this->_sw3d3_screen_fbo = this->sw3d3ctx->getFBO(this->sw3d3ctx->newFBO_CD(
                                    (this->_curmode.width * this->_curmode.sw3d3_resolution_percent) / 100,
                                    (this->_curmode.height * this->_curmode.sw3d3_resolution_percent) / 100
                                ));

                                this->_sw3d3_screen_colortex = this->sw3d3ctx->getTexture(this->_sw3d3_screen_fbo->attachments[0].texture);
                                this->_sw3d3_screen_depthtex = this->sw3d3ctx->getTexture(this->_sw3d3_screen_fbo->attachments[1].texture);

                                this->_sw3d3_testtri_ps = this->sw3d3ctx->getPixelShader(this->sw3d3ctx->newPixelShader());
                                this->_sw3d3_testtri_ps->fmain = EngineInstance::_sw3d3_testtri_fmain;

                                this->_sw3d3_drawmesh_ps = this->sw3d3ctx->getPixelShader(this->sw3d3ctx->newPixelShader());
                                this->_sw3d3_drawmesh_ps->fmain = _sw3d3_drawmesh_fmain;

                                this->_sw3d3_bitmapfont_ps = this->sw3d3ctx->getPixelShader(this->sw3d3ctx->newPixelShader());
                                this->_sw3d3_bitmapfont_ps->fmain = _sw3d3_bitmapfont_fmain;

                                this->sw3d3ctx->setCurrentDrawFBO(this->_sw3d3_screen_fbo->idx);

                                #if (defined(_GL45RW_WINDOW_SDL2))
                                    this->win = this->uwin.win;
                                #elif (defined(_GL45RW_WINDOW_GLFW3))
                                    this->win = this->uwin.win;
                                #endif

                            #else
                                throw std::runtime_error("the SW3D 3 renderer was disabled in this build.");
                            #endif
                            break;
                        }
                        case gfxapi::none:
                        {
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown renderer.");
                        }
                    }
                    if (this->win != nullptr)
                    {
                        //SDL_SetWindowFullscreen(this->win, this->_curmode.fullscreen);
                        #if (defined(_GL45RW_WINDOW_SDL2))
                        _register_instance_window(SDL_GetWindowID(this->win), this);
                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                        glfwSetWindowCloseCallback(this->win, _glfw_window_close_callback);
                        _register_instance_glfw_window(this->win, this);
                        #endif
                    }
                #endif
                this->R_MakeCurrent();
                this->_createConsoleData();
                this->_updateConsoleDataNewRes();

                return this->_curmode;
            }
            bool has_window() const {
                #if (defined(_GL45RW_HEADLESS))
                 return false;
                #else
                 //return (this->win != nullptr);
                 return (!this->uwin.is_closed());
                #endif
            }

            struct active_material_state {
                Material* mat = nullptr;
                #ifdef __GL45RW_HAS_OPENGL
                    _glstuff::GLProgramBinding* glbinding = nullptr;
                #endif
            };

            active_material_state _active_material_state = {};

            void _reset_active_material();
            void _set_active_material(Material* mat);


        public:
            const uintmax_t& current_ticno = _current_ticno;
            #ifdef __GL45RW_HAS_OPENGL
                _glstuff::GLcalls_t _glcalls = {};
                const GLuint& gles3_attributeless_vao = _gles3_attributeless_vao;
            #endif
            #ifdef GL45RW_WITH_SW3D2
              dj_sw3d20::Context* sw3d2ctx = nullptr;
              dj_sw3d20::Framebuffer
                  *_sw3d2_screen_fbo = nullptr,
                  *_sw3d2_window_fbo = nullptr
              ;
              bool _sw3d2_winfbo_drawn = false;
            #endif
            #ifdef GL45RW_WITH_SW3D3
             sw3d3::TestContext* sw3d3ctx = nullptr;
             sw3d3::fbo_t
                 *_sw3d3_screen_fbo = nullptr,
                 *_sw3d3_window_fbo = nullptr
             ;
             sw3d3::texture_t
                 *_sw3d3_screen_colortex = nullptr,
                 *_sw3d3_screen_depthtex = nullptr,

                 *_sw3d3_window_colortex = nullptr,
                 *_sw3d3_window_depthtex = nullptr
             ;
             sw3d3::pixelshader_t *_sw3d3_testtri_ps = nullptr, *_sw3d3_drawmesh_ps = nullptr, *_sw3d3_bitmapfont_ps = nullptr;
             sw3d3::vao_t *_sw3d3_rect_tl_vao = nullptr;
            #endif
            dictionary<char32_t, fvec4_t> textcolors = {
                {U'r', fvec4_t{1,0,0,1}},
                {U'g', fvec4_t{0,1,0,1}},
                {U'b', fvec4_t{0,0,1,1}},
                {U'y', fvec4_t{1,1,0,1}},
                {U'c', fvec4_t{0,1,1,1}},
                {U'm', fvec4_t{1,0,1,1}},
                {U'w', fvec4_t{1,1,1,1}},
                {U'0', fvec4_t{0,0,0,1}},
                {U'?', fvec4_t{0.5f,1.0f,0.5f,1.0f}}
            };

            struct texhnd_t {
                bool _dummy;
                #ifdef __GL45RW_HAS_OPENGL
                    GLuint gl;
                #endif
            };
            dictionary<uid_t, AssetBase*> assets = {};


            dictionary<std::u32string, _BaseConsoleCommand*> all_ccmds = {};
            dictionary<std::u32string, ConsoleIVAR*> ivar_ccmds = {};
            dictionary<std::u32string, fscalar_t> ivar_values = {};

            struct cmdbinding_t {
                std::u32string
                    command_name = U"",
                    command_args = U""
                ;
                fscalar_t
                    deadzone = 0.1,
                    altrange_min = 0,
                    altrange_max = 1,
                    exponent = 1,
                    sensitivity = 1
                ;
                bool _last_state = false;
            };
            enum struct bindcatflags : unsigned int {
                ignore_nonfocused = 0b00000001U,
                ignore_openconsole = 0b00000010U
            };
            struct bindcategory_t {
                dictionary<std::string, cmdbinding_t> binds = {};
                bindcatflags flags = bindcatflags(0U);
                BindCCMD *associated_bind_ccmd = nullptr;
                UnbindCCMD *associated_unbind_ccmd = nullptr;
            };

            dictionary<std::string, bindcategory_t*> bindcats = {};

			void _initializeRandomEngine() {
			    std::random_device rdev;
				for (int i = 0; i < 1024; i++){
					this->_rnd_values[i] = rdev();
				}
				this->_rnd_curpos = rdev() % 1024;
			}

			fscalar_t I_GetRandomFloat() {
                unsigned int curval = this->_rnd_values[this->_rnd_curpos++];
                this->_rnd_curpos %= 1024;
                return (curval % 2049)/fscalar_t(2048);
			}

            template <glm::length_t L>
            fvecL_t<L> I_GetRandomVector() {
                fvecL_t<L> res;
                for (glm::length_t i = 0; i < L; i++){res[i] = this->I_GetRandomFloat();}
                return res;
            }
            /*
            struct ivar_binding {
            	std::
            };

            dictionary<std::u32string,
            */
            struct _meshdraw_entry_t {
                fmat44_t model_matrix;
                uid_t mesh_handle;
            };

            //using renderbuckets_t = dictionary<uid_t, vectorlist<_meshdraw_entry_t>>;

            struct renderbuckets_t {
                dictionary<uid_t, vectorlist<_meshdraw_entry_t>*> _buckets = {};

                bool add_matid(const uid_t matidx) {
                    if (matidx == 0){return false;}
                    else if (this->_buckets.count(matidx) > 0){return false;}
                    this->_buckets[matidx] = new vectorlist<_meshdraw_entry_t>();
                    return true;
                }
                bool del_matid(const uid_t matidx) {
                    if (this->_buckets.count(matidx) == 0){return false;}
                    auto* bucket = this->_buckets[matidx];
                    delete bucket;
                    this->_buckets.erase(matidx);
                    return true;
                }

                bool clear_bucket(const uid_t matidx) {
                    auto* bucket = this->_buckets.getd(matidx, nullptr);
                    if (bucket == nullptr){return false;}
                    bucket->clear();
                    return true;
                }

                bool clear_all() {
                    for (auto& bp : this->_buckets) {
                        bp.second->clear();
                    }
                    return true;
                }

                renderbuckets_t() {}
                ~renderbuckets_t() {
                    while (this->_buckets.size() > 0) {
                        auto itr = this->_buckets.begin();
                        delete itr->second; itr->second = nullptr;
                        this->_buckets.erase(itr);
                    }
                }
            };

            struct rendergroup_t {
                renderbuckets_t
                    opaque_buckets,
                    translucent_buckets
                ;
            };

            rendergroup_t rendergroups[32] = {};

            void R_GetRenderGroupsForMask(size_t& count, rendergroup_t** groups, const uint32_t& mask) {
                count = 0;
                if (mask == 0){return;}
                else if (mask == uint32_t(-1)){
                    count = 32;
                    for (size_t i = 0; i < 32; i++){groups[i] = this->rendergroups+i;}
                    return;
                }
                for (uint32_t i = 0; i < 32; i++){
                    if (mask & (1 << i)){groups[count] = this->rendergroups+i; count++;}
                }
            }

            void R_EnsureAllGroupsHaveMaterialID(const uid_t& matidx, const bool opaque) {
                for (int i = 0; i < 32; i++){
                    if (opaque){this->rendergroups[i].opaque_buckets.add_matid(matidx);}
                    else {this->rendergroups[i].translucent_buckets.add_matid(matidx);}
                }
            }

            void R_EnsureAllGroupsRemovedMaterialID(const uid_t& matidx) {
                for (int i = 0; i < 32; i++){
                    this->rendergroups[i].opaque_buckets.del_matid(matidx);
                    this->rendergroups[i].translucent_buckets.del_matid(matidx);
                }
            }

            fvec2_t _rel_mouse_offset = {0,0};

            fscalar_t _rel_mouse_maxradius = 10;

            StopWatch _rel_mouse_updclock = {.interval=0.0167f};
            bool E_GetInputGrabbed() {
                return this->_mouse_locked;
            }
            void E_SetInputGrabbed(const bool grab){
                this->_mouse_locked = grab;
            }

            struct physobj_t {
                trs_t trs;
                sgphysshape pshape;
                sgphystype ptype;
                TriangleMesh* trimesh = nullptr;
            };

            using physgroup_t = vectorlist<physobj_t>;

            physgroup_t physgroups[32] = {};

            enum struct touchstate : int {
				up = 0,
				down = 1,
				motion = 2
			};

			struct touchevent_t {
				uid_t id = 0;
				touchstate state;
				fvec2_t
					oxy,
					cxy,
					dxy
				;
			};

			dictionary<uid_t, touchevent_t> touch_events_byid = {};
			vectorlist<touchevent_t> touch_events;

            void _i_register_engine_instance() {
                if (this->_instance_id != 0){return;}
                this->_instance_id = (gl45rw::_next_engine_instance_id++);
                gl45rw::_engine_instances_byid[0][this->_instance_id] = this;
            }
            void _i_unregister_engine_instance() {
                if (this->_instance_id == 0){return;}
                gl45rw::_engine_instances_byid->popk(this->_instance_id);
                this->_instance_id = 0;
            }

            inline uid_t I_GetInstanceID() const {return this->_instance_id;}

            void P_ClearPhysicsGroups(const uint32_t mask=uint32_t(-1)) {
                size_t ngroups = 0;
                physgroup_t* groups[32] = {};
                this->P_GetPhysicsGroupsForMask(ngroups, groups, mask);
                if (ngroups == 0){return;}
                for (size_t i = 0; i < ngroups; i++){groups[i]->clear();}
            }

            void P_GetPhysicsGroupsForMask(size_t& count, physgroup_t** groups, const uint32_t& mask) {
                count = 0;
                if (mask == 0){return;}
                else if (mask == uint32_t(-1)){
                    count = 32;
                    for (size_t i = 0; i < 32; i++){groups[i] = this->physgroups+i;}
                    return;
                }
                for (uint32_t i = 0; i < 32; i++){
                    if (mask & (1 << i)){groups[count] = this->physgroups+i; count++;}
                }
            }

            template <typename ITR>
            void P_PushPhysicsObjectsToGroups(ITR objs_start, ITR objs_end, const uint32_t& mask) {
                size_t ngroups = 0;
                physgroup_t* groups[32] = {};
                this->P_GetPhysicsGroupsForMask(ngroups, groups, mask);
                if (ngroups == 0){return;}
                for (ITR itr = objs_start; itr != objs_end; itr++){
                    auto obj = *itr;
                    for (size_t gi = 0; gi < ngroups; gi++){
                        groups[gi]->push_back(obj);
                    }
                }
            }



            /*
            void R_SetActiveRenderGroup(const std::string& n) {
                if (this->rendergroups.count(n) == 0){throw std::runtime_error("no rendergroup named \"" + n + "\".");}

            }
            */
            #if (!defined(_GL45RW_HEADLESS))
             #if (defined(_GL45RW_WINDOW_SDL2))
             inline SDL_Window* _get_sdl2_window() {return this->win;}
             inline uint32_t R_GetWindowID() const {
                return (this->win != nullptr ? uint32_t(SDL_GetWindowID(this->win)) : uint32_t(0));
             }
             #else
             inline uint32_t R_GetWindowID() const {return 0;}
             #endif
            #endif

            enum struct textinputclass : int {
                new_chars = 0,
                backspace = 1,
                enter = 2,
                hist_up = 3,
                hist_down = 4,
                tab = 5
            };
            struct textinputevent_t {
                textinputclass _class = textinputclass::new_chars;
                std::u32string newchars = U"";
            };

            vectorlist<textinputevent_t> text_events = {};

            virtual void _onCreate(void* params) {}
            virtual void _onDestroy() {}
            virtual void _onWindowCloseEvent() {}
            /*
            entity* I_GetEntityWithEID(const uid_t eid) {
                //return this->ents_byid.getd(eid, nullptr);
                return nullptr;
            }
            */

            struct activelight_t {
				LightSource* lightsrc = nullptr;
				fvec3_t pos = {0,0,0};
				fmat33_t orn = fmat33_t(1);
			};

            vectorlist<activelight_t> _active_lights = {};
            bool _active_lights_dirty = false;

             _consoledatatype& C_GetConsoleData() {return this->console_data;}

            fscalar_t E_GetInputValue(const inputeventinfo_t& iei) {
                switch (iei.iclass){
                    case inputclass::mouseaxis: {
                        fscalar_t axv = 0.0f;
                        switch (iei.mouseaxisno){
                            case 0: axv = this->_rel_mouse_offset.x / this->_rel_mouse_maxradius; break;
                            case 1: axv = -this->_rel_mouse_offset.y / this->_rel_mouse_maxradius; break;
                            default: ;
                        }
                        return std::max((iei.mouseaxisfwd ? axv : -axv), fscalar_t(0));
                    }
                    #ifdef _GL45RW_WINDOW_GLFW3
                    case inputclass::key: {
                        if (this->win == nullptr){return fscalar_t(0);}
                        auto _glfwkey = iei.keycode;
                        if (_glfwkey == GLFW_KEY_UNKNOWN){return fscalar_t(0);}
                        if (glfwGetKey(this->win, iei.keycode) != GLFW_RELEASE){return fscalar_t(1);}
                        else {return fscalar_t(0);}
                    }
                    #endif
                    default: return fscalar_t(iei);
                }
            }
            fscalar_t E_GetInputValue(const std::string& name) {
                return this->E_GetInputValue(E_GetInputInfo(name));
            }

            djutil::virfs::VirtualFSSourceBase* _I_CreateFilesystemSourceForFile(const djutil::virfs::fspath_t& fn) {
                if (this->_main_vfs == nullptr){return nullptr;}
                djutil::virfs::VirtualFSSourceBase* ret = nullptr;
                #define _hasext(_e) djutil::ezstr::endswith<char32_t>(fn, _e)
                if (_hasext(U".rpk") || _hasext(U".RPK")) {
                    auto* newsrc = new djutil::virfs::ResourcePackSource(this->_main_vfs, fn);
                    ret = reinterpret_cast<djutil::virfs::VirtualFSSourceBase*>(newsrc);
                }
                else if (_hasext(U".pk2") || _hasext(U".PK2")) {
                    auto* newsrc = new djutil::virfs::PK2Source(this->_main_vfs, fn);
                    ret = reinterpret_cast<djutil::virfs::VirtualFSSourceBase*>(newsrc);
                }
                #undef _hasext
                return ret;
            }

            template <class SLT>
            bool I_StartMainFS(const SLT& sources) {
                if (this->_main_vfs != nullptr){return false;}
                this->_main_vfs = new djutil::virfs::VirtualFS();
                SLT srclist = sources;
                while (srclist.size() > 0) {
                    const djutil::virfs::fspath_t ps = srclist.back();
                    const std::string psu8 = djutil::ezstr::utf32_to_utf8(ps);
                    srclist.pop_back();
                    if (djutil::pathman::is_dir(psu8)){
                        djutil::virfs::VirtualFS dummyfs;
                        auto* dummydir = new djutil::virfs::DirectorySource(&dummyfs, ps);
                        const std::u32string autoload_filename = U"/autoload.lst";
                        if (dummyfs.getEntityType(autoload_filename) == djutil::virfs::ent_type::file){
                            djutil::virfs::vfistream autoload_f(dummyfs, autoload_filename, std::ios::ate);
                            std::string autoload_u8; autoload_u8.resize(autoload_f.tellg());
                            autoload_f.seekg(0); autoload_f.read(&autoload_u8.front(), autoload_u8.size());
                            autoload_f.close();
                            const auto autoload = djutil::ezstr::utf8_to_utf32(autoload_u8);

                            vectorlist<std::u32string> autoload_l;
                            djutil::ezstr::splitlines(autoload_l, autoload);
                            bool added_parentdir = false;
                            for (auto& cmd : autoload_l){
                                std::u32string name, args;
                                djutil::ezstr::partition<char32_t>(name, args, cmd, U" ");
                                std::cout << "name=" << std::quoted(djutil::ezstr::utf32_to_utf8(name)) << ", args=" << std::quoted(djutil::ezstr::utf32_to_utf8(args)) << "\n";
                                if (djutil::ezstr::startswith(name, U"#")){continue;}
                                else if (name == U"add_parentdir" && !added_parentdir){
                                    added_parentdir = true;
                                    auto* dirsrc = new djutil::virfs::DirectorySource(this->_main_vfs, ps);
                                }
                                else if (name == U"addfile"){
                                    std::u32string realfn = dummydir->_getRealPath(U"/") + args;
                                    auto* newsrc = this->_I_CreateFilesystemSourceForFile(realfn);
                                }
                            }
                            if (!added_parentdir){
                                auto* dirsrc = new djutil::virfs::DirectorySource(this->_main_vfs, ps);
                            }
                        }
                        else {
                            auto* dirsrc = new djutil::virfs::DirectorySource(this->_main_vfs, ps);
                        }
                    }
                    else if (djutil::pathman::is_file(psu8)){
                        auto* arcsrc = this->_I_CreateFilesystemSourceForFile(ps);
                    }
                }
                return true;
            }
            bool I_CloseMainFS() {
                if (this->_main_vfs == nullptr){return false;}
                delete this->_main_vfs;
                this->_main_vfs = nullptr;
                return true;
            }
            djutil::virfs::VirtualFS* I_GetMainFS() {return this->_main_vfs;}
            void I_GetMatrices(fmat44_t& model, fmat44_t& view, fmat44_t& projection) const {
                model = this->matrices.model;
                view = this->matrices.view;
                projection = this->matrices.projection;
            }
            matstacks_t& I_GetMatrixStack() {return this->matrices;}

            struct calcmatrices_t {
				fmat44_t
					mdl = fmat44_t(1),
					mdl2 = fmat44_t(1),
					view = fmat44_t(1),
					view_inv = fmat44_t(1),
					projection = fmat44_t(1),
					projection_inv = fmat44_t(1)
				;
			};

			calcmatrices_t I_GetCalculatedMatrices() {
				calcmatrices_t cm;
				cm.mdl = this->matrices.model[-1];
				cm.mdl2 = fmat44_t(1);
				cm.view = this->matrices.view[-1];
				cm.view_inv = glm::inverse(cm.view);
				cm.projection = this->matrices.projection[-1];
				cm.projection_inv = glm::inverse(cm.projection);
				return cm;
			}

            bool R_HasActiveRenderer() {
                if ((!this->has_window()) || (this->_curmode.renderer == gfxapi::none)){return false;}
                return true;
            }
            void C_RefreshConsoleLayout() {
                this->_updateConsoleDataNewRes();
            }
            bool C_IsConsoleOpen() {
                auto& cd = this->console_data;
                return (cd.state == _constate::opening) || (cd.state == _constate::opened);
            }
            void C_SetConsoleOpen(const bool& isopen){
                auto& cd = this->console_data;
                if (isopen){
                    int curs = int(cd.state);
                    if (curs == 0 || curs == 3){cd.state = _constate::opening;}
                }
                else {
                    cd.state = _constate::closing;
                }
            }
            int C_ConsolePrint(const std::u32string& lines) {
				int nlines = 0;
				vectorlist<std::u32string> splits = {};
				djutil::ezstr::splitlines<char32_t>(
					splits,
					lines
				);

				for (auto& ln : splits){
					if (ln.size() == 0){continue;}
					this->console_data.lines.push_back(ln);
					nlines++;
				}
				return nlines;
			}

            int C_ConsolePrint(const std::string& lines){
				return this->C_ConsolePrint(djutil::ezstr::utf8_to_utf32(lines));
			}

            bindcategory_t* I_CreateBindCategory(const std::string& catname, const bindcmdnames_t& cmdnames, const bindcatflags flags=bindcatflags(0U));

            void I_UpdateAllBindings() {
                for (auto& p1 : this->bindcats) {
                    bindcategory_t* bcat = p1.second;
                    auto uiflags = (unsigned int)(bcat->flags);
                    bool flag_ignore_openconsole = (uiflags & ((unsigned int)bindcatflags::ignore_openconsole)) != 0U;
                    for (auto& p2 : bcat->binds) {
                        const std::string& key = p2.first;
                        cmdbinding_t& bnd = p2.second;
                        fscalar_t ev = 0.0f;
                        if (this->console_data.state == _constate::opened){ev = 0.0f;}
                        else {
                            ev = pow((this->E_GetInputValue(key)-bnd.altrange_min)/(bnd.altrange_max-bnd.altrange_min), bnd.exponent);
                            if (ev < bnd.deadzone){ev = 0.0f;}
                        }
                        if (this->ivar_ccmds.count(bnd.command_name) > 0){
                            CCMDInvokationExtraData ed = {.ivar_invalue=ev};
                            this->C_ExecCommand(bnd.command_name, U"", ed);
                        }
                        else {
                            if ((ev > 0.5f) && !bnd._last_state){
                                bnd._last_state = true;
                                std::cout << "bindexec\n";
                                this->C_ExecCommand(bnd.command_name, bnd.command_args);
                            }
                            else if (ev <= 0.5f && bnd._last_state){bnd._last_state = false;}
                        }
                    }
                }
            }

            fscalar_t I_GetIVARValue(const std::u32string& pname, const std::u32string& nname){
				return fscalar_t(this->ivar_ccmds[pname][0]) - fscalar_t(this->ivar_ccmds[nname][0]);
			}

            void S_MakeCurrent() {
                switch (this->_cursndmode.renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                        if (this->_aldev != nullptr && this->_alctx != nullptr){
                            alcMakeContextCurrent(this->_alctx);
                            //while (alGetError() != AL_NO_ERROR){}
                        }
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            void R_MakeCurrent() {
                if (!this->has_window()){return;}
                /*
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #ifdef __GL45RW_HAS_OPENGL
                            _glstuff::curgl = &(this->_glcalls);
                            SDL_GL_MakeCurrent(this->win, this->glctx);
                        #endif
                        return;
                    }
                    default: return;
                }
                */
                this->uwin.make_current();
            }

            dictionary<std::string, std::string> R_GetRendererInformation(){
                dictionary<std::string, std::string> infos = {
                	{"name", "headless"},
                	{"driver_vendor", "N/A"},
                	{"driver_renderer", "N/A"},
                	{"driver_version", "N/A"},
                	{"shading_language_version", "N/A"}
                };
                this->R_MakeCurrent();
                switch (this->_curmode.renderer){
                	case gfxapi::opengl: {
                		#ifdef __GL45RW_HAS_OPENGL
                		   auto& gl = this->_glcalls;
                		   infos["name"] = "opengl";
                		   if (gl.glGetString == nullptr){break;}
                		   infos["driver_vendor"] = (const char*)(gl.glGetString(GL_VENDOR));
                		   infos["driver_renderer"] = (const char*)(gl.glGetString(GL_RENDERER));
                		   infos["driver_version"] = (const char*)(gl.glGetString(GL_VERSION));
                		   infos["shading_language_version"] = (const char*)(gl.glGetString(GL_SHADING_LANGUAGE_VERSION));
                		#endif
                		break;
                	}
                	case gfxapi::sw3d2: {
                	    infos["name"] = "sw3d2";
                	    infos["driver_vendor"] = "Rudolph4268";
                	    infos["driver_renderer"] = "dj_sw3d20.hpp";
                	    infos["driver_version"] = "n/a";
                		break;
                	}
                	default: ;
                }
                return infos;
            }
            void R_SetViewportFromWindow() {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #ifdef __GL45RW_HAS_OPENGL
                            GLuint _cur_fbo = 0;
                            this->_glcalls.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&_cur_fbo);
                            if (_cur_fbo == this->gl_screenfbo->fbo_idx){
                                this->_glcalls.glViewport(0,0,this->_gl_screenfbo_width,this->_gl_screenfbo_height);
                            }
                            else {
                                this->_glcalls.glViewport(0,0,this->_curmode.width,this->_curmode.height);
                            }
                        #endif
                        return;
                    }
                    default: return;
                }
            }
            /*
            void R_ActiveTexture(
            void R_BindTexture2D(
            */
            void R_SetCapEnabled(const rendercap cap, const bool enable)
            {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #ifdef __GL45RW_HAS_OPENGL
                         GLenum glcap = _rendercap2gl(cap);
                         if (enable){this->_glcalls.glEnable(glcap);}
                         else       {this->_glcalls.glDisable(glcap);}
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        this->sw3d2ctx->srSetEnabled(
                            _RENDERCAP_TO_SW3D2[int(cap)],
                            enable
                        );
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                        switch (cap) {
                            case rendercap::depth_test: {
                                this->sw3d3ctx->drawparams.depthfunc = (enable ? sw3d3::depthfunc_e::less : sw3d3::depthfunc_e::always);
                                break;
                            }
                            default: ;
                        }
                        #endif
                        break;
                    }
                    default: return;
                }
            }

            active_material_state& R_GetActiveMaterialState(){
                return this->_active_material_state;
            }

            active_material_state& R_SetActiveMaterial(const uid_t matid);

            void R_UpdateMaterialMatrices(const calcmatrices_t& cm);

            bool R_IsCapEnabled(const rendercap cap)
            {
                if (!this->has_window()){return false;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    #ifdef __GL45RW_HAS_OPENGL
                    case gfxapi::opengl: return this->_glcalls.glIsEnabled(_rendercap2gl(cap));
                    #endif
                    #ifdef GL45RW_WITH_SW3D2
                    case gfxapi::sw3d2: return this->sw3d2ctx->srIsEnabled(_RENDERCAP_TO_SW3D2[int(cap)]); break;
                    #endif
                    #ifdef GL45RW_WITH_SW3D3
                    case gfxapi::sw3d3: {
                        switch (cap) {
                            case rendercap::depth_test: return (this->sw3d3ctx->drawparams.depthfunc != sw3d3::depthfunc_e::always);
                            default: return false;
                        }
                        break;
                    }
                    #endif
                    default: return false;
                }
                return false;
            }
            cullface R_GetFaceCullingMode() {
                if (!this->has_window()){return cullface::disabled;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        if (!this->_glcalls.glIsEnabled(GL_CULL_FACE)){return cullface::disabled;}
                        else {
                            GLint v = 0; this->_glcalls.glGetIntegerv(GL_CULL_FACE_MODE, &v);
                            if (v == GL_FRONT){return cullface::front;}
                            else if (v == GL_BACK){return cullface::back;}
                            return cullface::disabled;
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        if (this->sw3d2ctx->srIsEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING)){
                            auto v = this->sw3d2ctx->srCullFace();
                            if (v == dj_sw3d20::enum_t::FRONT_FACE){return cullface::front;}
                            else if (v == dj_sw3d20::enum_t::BACK_FACE){return cullface::back;}
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                        switch (this->sw3d3ctx->drawparams.cullface){
                            case sw3d3::cullface_e::back: return cullface::back;
                            case sw3d3::cullface_e::front: return cullface::front;
                            default: return cullface::disabled;
                        }
                        #endif
                        break;
                    }
                    default: ;
                }
                return cullface::disabled;
            }
            void R_SetFaceCullingMode(const cullface newmode) {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        switch (newmode) {
                            case cullface::disabled: this->_glcalls.glDisable(GL_CULL_FACE); break;
                            case cullface::back: this->_glcalls.glEnable(GL_CULL_FACE); this->_glcalls.glCullFace(GL_BACK); break;
                            case cullface::front: this->_glcalls.glEnable(GL_CULL_FACE); this->_glcalls.glCullFace(GL_FRONT); break;
                            default: ;
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        switch (newmode) {
                            case cullface::disabled: this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING, false); break;
                            case cullface::front: this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING, true); this->sw3d2ctx->srCullFace(dj_sw3d20::enum_t::FRONT_FACE); break;
                            case cullface::back: this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING, true); this->sw3d2ctx->srCullFace(dj_sw3d20::enum_t::BACK_FACE); break;
                            default: ;
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                        switch (newmode) {
                            case cullface::back: this->sw3d3ctx->drawparams.cullface = sw3d3::cullface_e::back; break;
                            case cullface::front: this->sw3d3ctx->drawparams.cullface = sw3d3::cullface_e::front; break;
                            default: this->sw3d3ctx->drawparams.cullface = sw3d3::cullface_e::disabled; break;
                        }
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            void R_SetClearColor(const fvec4_t& color) {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                     #ifdef __GL45RW_HAS_OPENGL
                      this->_glcalls.glClearColor(color.x, color.y, color.z, color.w);
                     #endif
                     break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                         this->sw3d2ctx->srClearColor(color.x, color.y, color.z, color.w);
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                         this->sw3d3ctx->clearcolor = glm::vec4(color);
                        #endif
                        break;
                    }
                    default: return;
                }
            }
            void R_ClearBuffers(const bool color=true, const bool depth=true)
            {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #ifdef __GL45RW_HAS_OPENGL
                            this->_glcalls.glClear((color ? GL_COLOR_BUFFER_BIT : GLbitfield(0)) | (depth ? GL_DEPTH_BUFFER_BIT : GLbitfield(0)));
                        #endif
                        return;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            this->sw3d2ctx->srClearBuffers(color, depth);
                        #endif
                        return;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            this->sw3d3ctx->clearBuffers(color, depth);
                        #endif
                        return;
                    }
                    default: return;
                }
            }
            fscalar_t R_WindowAspect() const
            {
                return fscalar_t(this->_curmode.width)/this->_curmode.height;
            }

            void R_DrawBitmapText(const uid_t font, const std::u32string& text);

            void _sw3d2_blit_screen2win() {
				#ifdef GL45RW_WITH_SW3D2
				djutil::imaging::scaled_copy_fixed<uint8_t>(
					this->_sw3d2_window_fbo->renderbuffer, this->_sw3d2_screen_fbo->renderbuffer,

					this->_curmode.width, this->_curmode.height,

					this->_sw3d2_screen_fbo->width, this->_sw3d2_screen_fbo->height,
					3
				);
				#endif
			}

            void R_SwapBuffers() {
                if (!this->has_window()){return;}
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #ifdef __GL45RW_HAS_OPENGL
                            #if (defined(_GL45RW_GL2))
                             #define _bind_fbo this->_glcalls.glBindFramebufferEXT
                            #elif (defined(_GL45RW_GLES3))
                             #define _bind_fbo this->_glcalls.glBindFramebuffer
                            #endif
                            GLuint _lastfbo = 0; this->_glcalls.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&_lastfbo);
                            _bind_fbo(GL_DRAW_FRAMEBUFFER, 0);
                            _bind_fbo(GL_READ_FRAMEBUFFER, 0);
                            this->_glcalls.glViewport(0, 0, this->_curmode.width, this->_curmode.height);

                            this->_glcalls.glActiveTexture(GL_TEXTURE0); this->_glcalls.glBindTexture(GL_TEXTURE_2D, this->gl_screenfbo->getAttachmentForLocation(GL_COLOR_ATTACHMENT0).glidx);
                            this->_glcalls.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (this->_curmode.opengl_linear_downscale ? GL_LINEAR : GL_NEAREST));
                            this->_glcalls.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (this->_curmode.opengl_linear_upscale ? GL_LINEAR : GL_NEAREST));
                            this->_glcalls.glActiveTexture(GL_TEXTURE1); this->_glcalls.glBindTexture(GL_TEXTURE_2D, this->gl_winfbo->getAttachmentForLocation(GL_COLOR_ATTACHMENT0).glidx);

                            this->_glcalls.glDisable(GL_DEPTH_TEST);
                            float _rect_verts[] = {
                                -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  -1.0f, 1.0f, -1.0f,
                                -1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f
                            };
                            {_glstuff::GLProgramBinding bnd(GLuint(*this->_window_blit_glshader));
                                int _texsamplers[2] = {0, 1};
                                bnd.setUniform(this->_window_blit_glshader->uniform_idx("screentex"), 1, _texsamplers+0);
                                bnd.setUniform(this->_window_blit_glshader->uniform_idx("windowtex"), 1, _texsamplers+1);
                                bnd.setUniform(this->_window_blit_glshader->uniform_idx("gamma"), 1, &(this->_curmode.gl_gamma));
                                #if (defined(_GL45RW_GL2))
                                this->_glcalls.glBegin(GL_TRIANGLES);
                                    for (size_t i = 0; i < 18; i+=3){
                                        this->_glcalls.glVertex3fv(_rect_verts+i);
                                    }
                                this->_glcalls.glEnd();
                                #elif (defined(_GL45RW_GLES3))
                                this->_glcalls.glBindVertexArray(this->_gles3_attributeless_vao);
                                this->_glcalls.glDrawArrays(GL_TRIANGLES, 0, 6);
                                this->_glcalls.glBindVertexArray(0);
                                #endif
                            }

                            this->_glcalls.glActiveTexture(GL_TEXTURE0); this->_glcalls.glBindTexture(GL_TEXTURE_2D, 0);
                            this->_glcalls.glActiveTexture(GL_TEXTURE1); this->_glcalls.glBindTexture(GL_TEXTURE_2D, 0);

                            //SDL_GL_SwapWindow(this->win);
                            this->uwin.swap_buffers();

                            _bind_fbo(GL_DRAW_FRAMEBUFFER, _lastfbo);
                            #undef _bind_fbo
                        #endif
                        return;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2

                            const size_t pxasz = this->_curmode.width * this->_curmode.height * 3;
                            const size_t npx = this->_curmode.width * this->_curmode.height;
                            void* rpx = this->uwin.lock_swpixels();
                            djutil::imaging::scaled_copy_fixed<uint8_t>(
                                (uint8_t*)rpx, this->_sw3d2_screen_fbo->renderbuffer,

                                this->_curmode.width, this->_curmode.height,

                                this->_sw3d2_screen_fbo->width, this->_sw3d2_screen_fbo->height,
                                3
                            );
                            //memcpy(rpx, this->_sw3d2_window_fbo->renderbuffer, pxasz);
                            if (this->_sw3d2_winfbo_drawn){
								for (size_t i = 0; i < npx; i++){
									if (this->_sw3d2_window_fbo->depthbuffer[i] < 1.0){
										std::copy_n(this->_sw3d2_window_fbo->renderbuffer+(i*3), 3, ((uint8_t*)rpx)+(i*3));
									}
								}
								std::fill_n(this->_sw3d2_window_fbo->depthbuffer, npx, 1.0);
								this->_sw3d2_winfbo_drawn = false;
							}

                            this->uwin.unlock_swpixels();
                            this->uwin.swap_buffers(nullptr, false);
                        #endif
                        return;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            void* rpx = this->uwin.lock_swpixels();
                            djutil::imaging::scaled_copy_fixed<uint8_t>(
                                (uint8_t*)rpx, this->_sw3d3_screen_colortex->targets[0]._texels_u8.pfront(),
                                this->_curmode.width, this->_curmode.height,
                                this->_sw3d3_screen_fbo->w, this->_sw3d3_screen_fbo->h,
                                3
                            );
                            this->uwin.unlock_swpixels();
                            this->uwin.swap_buffers(nullptr, false);
                        #endif
                        return;
                    }
                    default: return;
                }
            }

            soundmode_t S_GetCurrentSoundMode() const {
                return this->_cursndmode;
            }

            void S_SetSoundMode(const soundmode_t& newmode){
                this->_full_SetSoundMode(newmode);
            }

            videomode_t R_GetCurrentVideoMode() const {
                return this->_curmode;
            }

            void R_SetVideoMode(const videomode_t& newmode)
            {
                bool did_full = false;
                const videomode_t oldmode = this->_curmode;
                if (newmode.renderer != oldmode.renderer || !this->has_window())
                {
                    this->_full_SetVideoMode(newmode);
                    did_full = true;
                }
                else
                {
                    this->uwin.window_title = djutil::ezstr::utf32_to_utf8(this->_curmode.title);
                    if (this->_curmode.renderer == gfxapi::sw3d2 || this->_curmode.renderer == gfxapi::sw3d3) {
                        this->uwin.create_software_window(this->_curmode.width, this->_curmode.height, this->_curmode.fullscreen);
                    }
                    else if (this->_curmode.renderer == gfxapi::opengl){
                        this->uwin.create_opengl_window(this->_curmode.width, this->_curmode.height, this->_curmode.fullscreen);
                    }
                    this->_curmode.width = this->uwin.width;
                    this->_curmode.height = this->uwin.height;

                    if (this->_curmode.renderer == gfxapi::sw3d2) {
                        #ifdef GL45RW_WITH_SW3D2

                        this->_sw3d2_window_fbo->close();
                        this->_sw3d2_screen_fbo->close();
                        glm::ivec2 scaled_size = glm::max((glm::ivec2{this->_curmode.width, this->_curmode.height}*this->_curmode.sw3d2_resolution_percent)/100, 0);
                        this->_sw3d2_screen_fbo->init(scaled_size.x, scaled_size.y);
                        this->_sw3d2_window_fbo->init(this->_curmode.width, this->_curmode.height);
                        this->R_ClearBuffers(true,true);
                        this->R_SwapBuffers();
                        #endif
                    }
                    else if (this->_curmode.renderer == gfxapi::sw3d3) {
                        #ifdef GL45RW_WITH_SW3D3

                        this->sw3d3ctx->setDimensions(this->_curmode.width, this->_curmode.height);

                        this->sw3d3ctx->resizeFBO(
                            this->_sw3d3_screen_fbo->idx,
                            (this->_curmode.width * this->_curmode.sw3d3_resolution_percent) / 100,
                            (this->_curmode.height * this->_curmode.sw3d3_resolution_percent) / 100
                        );

                        this->R_ClearBuffers(true,true);
                        this->R_SwapBuffers();
                        #endif
                    }
                    else if (this->_curmode.renderer == gfxapi::opengl){
                        #ifdef GL45RW_WITH_OPENGL
                        this->_opengl_regen_screenfbo();
                        #endif
                    }
                }
                this->_updateConsoleDataNewRes();
                if (did_full){
                    vectorlist<AssetBase*> assetlist = {};
                    for (auto& iap : this->assets)
                    {
                        AssetBase* asset = iap.second;
                        assetlist.push_back(asset);
                    }
                    for (AssetBase*& asset : assetlist){
                        asset->onRenderContextCreate();
                    }
                    assetlist.clear();

                    this->_rebuild_all_materials();

                }
            }

            void R_DrawTestTriangle()
            {
                if (!this->has_window()){return;}
                fmat44_t
                    view = this->matrices.view,
                    model = this->matrices.model,
                    proj = this->matrices.projection
                ;
                this->R_MakeCurrent();
                switch (this->_curmode.renderer)
                {
                    case gfxapi::opengl:
                    {
                        #if (defined(_GL45RW_GL2))
                         this->_glcalls.glMatrixMode(GL_PROJECTION);
                          fmat44_t mvp = proj * glm::inverse(view) * model;
                          this->_glcalls.glLoadMatrixf(glm::value_ptr(mvp));
                         this->_glcalls.glMatrixMode(GL_MODELVIEW);
                          this->_glcalls.glLoadIdentity();
                         /*glMatrixMode(GL_MODELVIEW);
                          fmat44_t modelview = view * model;
                          glLoadMatrixf(glm::value_ptr(modelview));
                         */
                         this->_glcalls.glUseProgram(0);
                         this->_glcalls.glActiveTexture(GL_TEXTURE0);
                         this->_glcalls.glBindTexture(GL_TEXTURE_2D, 0);
                         this->_glcalls.glBegin(GL_TRIANGLES);
                          this->_glcalls.glColor4f(1,0,0,1); this->_glcalls.glTexCoord2f(0, 0); this->_glcalls.glVertex3f(-1,-1,0);
                          this->_glcalls.glColor4f(0,1,0,1); this->_glcalls.glTexCoord2f(1, 0); this->_glcalls.glVertex3f(1,-1,0);
                          this->_glcalls.glColor4f(0,0,1,1); this->_glcalls.glTexCoord2f(0.5f, 1); this->_glcalls.glVertex3f(0,1,0);
                         this->_glcalls.glEnd();
                        #elif (defined(_GL45RW_GLES3))
                         {_glstuff::GLProgramBinding bnd(GLuint(*this->_gles3_testtri_glshader));
                             fmat44_t pvm = proj * glm::inverse(view) * model;
                             bnd.setNamedUniform("pvm", 1, &pvm);
                             this->_glcalls.glBindVertexArray(this->_gles3_attributeless_vao);
                             this->_glcalls.glDrawArrays(GL_TRIANGLES, 0, 3);
                             this->_glcalls.glBindVertexArray(0);
                         }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        this->sw3d2ctx->srActiveMaterial(nullptr);
                        this->sw3d2ctx->srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX);
                          this->sw3d2ctx->srTopMatrix() = model;
                        this->sw3d2ctx->srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX);
                          this->sw3d2ctx->srTopMatrix() = view;
                        this->sw3d2ctx->srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX);
                          this->sw3d2ctx->srTopMatrix() = proj;
                        const dj_sw3d20::ffvertex_t _sw3d2_tri[3] = {
                            dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3{-1,-1,0}, .normal=dj_sw3d20::fpvec3{0,0,1}, .color=dj_sw3d20::fpvec4{1,0,0,1}},
                            dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3{1,-1,0}, .normal=dj_sw3d20::fpvec3{0,0,1}, .color=dj_sw3d20::fpvec4{0,1,0,1}},
                            dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3{0,1,0}, .normal=dj_sw3d20::fpvec3{0,0,1}, .color=dj_sw3d20::fpvec4{0,0,1,1}},
                        };
                        this->sw3d2ctx->srDrawElements(3, _sw3d2_tri);
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                        this->sw3d3ctx->drawparams.pixel_shader = this->_sw3d3_testtri_ps->idx;
                        this->sw3d3ctx->matrices.model = model;
                        this->sw3d3ctx->matrices.view = view;
                        this->sw3d3ctx->matrices.projection = proj;
                        const sw3d3::vertex_t _sw3d3_tri[3] = {
                            {.co=glm::vec4{-1,-1,0,1}, .color=glm::vec4{1,0,0,1}, .normal=glm::vec3{0,0,1}},
                            {.co=glm::vec4{1,-1,0,1}, .color=glm::vec4{0,1,0,1}, .normal=glm::vec3{0,0,1}},
                            {.co=glm::vec4{0,1,0,1}, .color=glm::vec4{0,0,1,1}, .normal=glm::vec3{0,0,1}}
                        };
                        this->sw3d3ctx->drawTriangles(_sw3d3_tri + 0, _sw3d3_tri + 3);
                        #endif
                        break;
                    }
                    default: return;
                }
            }
            template <class AT, typename... Args>
            uid_t new_asset(Args... args) {
                uid_t retid = this->cur_assetid;
                AT* asset = new AT(this, retid, args...);

                this->assets[retid] = asset;
                this->cur_assetid++;
                asset->onCreate();
                return retid;
            }
            template <class AT, typename... Args>
            AT* new_asset_rp(Args... args) {
                return this->get_asset<AT>(this->new_asset<AT,Args...>(args...));
            }
            bool del_asset(const uid_t idx) {
                if (this->assets.count(idx) == 0){return false;}
                //this->assets[idx].onDestroy();
                AssetBase* asset = this->assets[idx];
                asset->onDestroy();
                delete asset; asset = nullptr;
                this->assets.erase(idx);
                return true;
            }
            template <class AT=AssetBase>
            AT* get_asset(const uid_t idx) {
                if (this->assets.count(idx) == 0){return nullptr;}
                return reinterpret_cast<AT*>(this->assets[idx]);
            }

            assettype get_asset_type(const uid_t idx) {
                if (this->assets.count(idx) == 0){return assettype::none;}
                return this->assets[idx]->asset_type();
            }
            virtual bool update(const MainloopData& mld){return false;}
            void _release_garbage_assets() {
				for (auto gaid : this->_EngineInstance_assets_marked_garbage){
					this->del_asset(gaid);
					std::cout << "[INFO]: garbage collected assetid=" << gaid << " from instance=" << (void*)this << ".\n";
				}
				this->_EngineInstance_assets_marked_garbage.clear();
			}
            bool _do_update(const MainloopData& mld) {
				this->_release_garbage_assets();
            	for (auto& p : this->ivar_ccmds) {
            		p.second->_value = fscalar_t(0);
            	}
            	this->S_UpdateSoundSystem(mld);
                #ifndef _GL45RW_HEADLESS
                    if (this->has_window()){
                        bool mouselock = (this->_mouse_locked && !this->C_IsConsoleOpen());
                        #if (defined(_GL45RW_WINDOW_SDL2))
                        SDL_SetRelativeMouseMode((mouselock ? SDL_TRUE : SDL_FALSE));
                        #endif
                    }
                #endif
                //std::cout << "CALL UPDATE\n";
            	bool res = this->update(mld);
            	//std::cout << "UPDATE CALLED\n";
            	this->_release_garbage_assets();
            	return res;
            }
            void R_ClearAllRenderBuckets(const uint32_t grpmask=uint32_t(-1)) {
                size_t grpcount = 0;
                rendergroup_t* grps[32] = {}, *cgrp = nullptr;
                this->R_GetRenderGroupsForMask(grpcount, grps, grpmask);
                for (size_t i = 0; i < grpcount; i++){
                    cgrp = grps[i];
                    renderbuckets_t* buckets[2] = {
                        &(cgrp->opaque_buckets),
                        &(cgrp->translucent_buckets)
                    };

                    for (size_t j = 0; j < sizeof(buckets)/sizeof(buckets[0]); j++) {
                        buckets[j]->clear_all();
                    }
                }
            }

            void R_DrawAllRenderBuckets(const fclock_t curtime=0, const uint32_t grpmask=uint32_t(-1));


            void R_ClearActiveLights() {
				this->_active_lights.clear();
				this->_active_lights_dirty = true;
			}
			void R_UpdateLighting();

            template <class CCMDT, typename... Args>
            CCMDT* new_ccmd(const std::u32string& name, Args... args) {
                if (this->all_ccmds.count(name) > 0){
                    std::stringstream ess;
                    ess << "failed to create console command: name U" << std::quoted(djutil::ezstr::utf32_to_utf8(name)) << " already in use.";
                    throw std::runtime_error(ess.str());
                }
                auto* ccmd = new CCMDT(this, name, args...);
                //this->all_ccmds[name] = ccmd;
                return ccmd;
            }

            int C_ExecCommand(const std::u32string& name, const std::u32string& args, CCMDInvokationExtraData& extradata) {
                _BaseConsoleCommand* ccmd = this->all_ccmds.getd(name, nullptr);
                if (ccmd == nullptr){return -1;}
                return ccmd->invoke(args, extradata);
            }
            int C_ExecCommand(const std::u32string& name, const std::u32string& args) {
                CCMDInvokationExtraData ed = {};
                return this->C_ExecCommand(name, args, ed);
            }

            void C_ExecBatchString(const std::u32string& bat) {
                std::u32string name, args;
                std::vector<std::u32string> lines = {};
                djutil::ezstr::splitlines(lines, bat);
                for (auto& ln : lines) {
                    djutil::ezstr::partition<char32_t>(name, args, ln, U" ");
                    if (this->C_ExecCommand(name, args) == -1){
						std::cerr << "C_ExecBatchString(): " << djutil::ezstr::utf32_to_utf8(name) << ": command not found.\n";
						this->C_ConsolePrint(U"%cr" + name + U": command not found.%c?");
					}
                }
            }

            void C_ExecBatchFileFS(const std::u32string& fsp) {
                djutil::virfs::VirtualFS* vfs = this->I_GetMainFS();
                if (vfs == nullptr){throw std::runtime_error("C_ExecBatchFileFS: main FS must be initialized!");}
                djutil::virfs::vfistream istr(vfs[0], fsp, std::ios::ate);
                std::string rds; rds.resize(size_t(istr.tellg()));
                istr.seekg(0);
                istr.read(&rds.front(), rds.size());
                this->C_ExecBatchString(djutil::ezstr::utf8_to_utf32(rds));
            }

            void C_UpdateConsole(const MainloopData& mld);
            void R_RedrawConsole();

            /*
            void init(void* params=nullptr) {
                _register_engine_instance(this);
                this->_onCreate(params);
            }
            */
            enum struct soundhandlestate : int {
                invalid = 0,
                stopped = 1,
                paused = 2,
                playing = 3
            };
            struct playsoundargs_t {
                bool loop = false, keep = false;
                fscalar_t gain = 1.0f;
            };
            inline bool S_IsHandle(const uid_t idx) {
                return this->get_asset_type(idx) == assettype::sndhandle;
            }
            inline bool S_IsFactory(const uid_t idx) {
                return this->get_asset_type(idx) == assettype::sndfactory;
            }
            uid_t S_PlaySound(SoundFactory* fac, const playsoundargs_t& args);
            soundhandlestate S_GetHandleState(const uid_t hndidx);
            bool S_SetHandleState(const uid_t hndidx, const soundhandlestate newstate);

            bool S_StopSound(const uid_t hndidx) {
                return this->S_SetHandleState(hndidx, soundhandlestate::stopped);
            }
            std::vector<SoundHandle*> _sndsys_updhnds = {};
            bool S_UpdateSoundSystem(const MainloopData& mld);
    };



    bool AssetBase::_is_valid() const {
        return (this->_instance != nullptr && this->_instance->assets.count(this->_assetid) > 0);
    }
    void AssetBase::assertvalid() const {
        if (!this->_is_valid()){throw std::runtime_error("bad operation on an invalid asset.");}
        return;
    }

    bool dynassetref_t::valid() const {
		if (this->_ei == nullptr || this->_aid == 0){return false;}
		return (this->_ei->assets.count(this->_aid) > 0);
	}

	assettype dynassetref_t::type() const {
		if (this->_ei == nullptr || this->_aid == 0){return assettype::none;}
		return (this->_ei->get_asset_type(this->_aid));
	}

    dynassetref_t& dynassetref_t::operator=(const AssetBase* a) {
		if (a == nullptr){this->_ei = nullptr; this->_aid = 0; return *this;}
		this->_ei = a->instance;
		this->_aid = uid_t(*a);
		return *this;
	}

    template <class A>
    dynassetref_t::operator A*() const {
		if (this->_ei == nullptr || this->_aid == 0){return nullptr;}
		return this->_ei->get_asset<A>(this->_aid);
	}
    int ConsoleCVAR::_invoke_action(const std::u32string& args, CCMDInvokationExtraData& edata) {
        vectorlist<std::u32string> argv = {}; split_argstr(argv, args);
        if (argv.size() < 1){
            std::u32string vs = U""; this->getstr(vs);
            this->instance->C_ConsolePrint(U" %cg" + this->name + U"%cw = %c?" + vs + U"%c?");
        }
        else {
            this->setstr(argv[0]);
        }
        return 0;
    }
    /*
    bool AssetBase::delref(const uid_t uid) {
        this->assertvalid();
        if (this->refs_of.eraseFirstOf(uid))
        {
            AssetBase* ref = nullptr;
            if (this->_instance->assets.getv(ref, uid) && ref != nullptr)
            {
                ref->refd_by.eraseFirstOf(this->_assetid);
                if (ref->is_refcounting && ref->refd_by.size() == 0)
                {
                    delete ref; ref = nullptr;
                }
                return true;
            }
        }
        return false;
    }
    bool AssetBase::addref(const uid_t uid){
        this->assertvalid();
        if (this->refs_of.firstIndexOf(uid) == djutil::containers::npos)
        {
            return false;
        }
        AssetBase* toref = nullptr;
        if (!this->_instance->assets.getv(toref, uid) || toref == nullptr){return false;}
        if (toref->refd_by.firstIndexOf(this->_assetid) == djutil::containers::npos)
        {
            toref->refd_by.push_back(this->_assetid);
        }
        this->refs_of.push_back(uid);
        return true;
    }
    */
    AssetBase::~AssetBase() {
        if (this->_is_valid()){
            /*
            for (const auto& ref : this->refd_by)
            {
                AssetBase* refp = nullptr;
                if (this->_instance->assets.getv(refp, ref) && refp != nullptr)
                {
                    refp->refs_of.eraseFirstOf(this->_assetid);
                }
            }
            */
            //this->onDestroy();
            //this->_instance->assets.popk(this->_assetid);
            //const auto refs = this->_references;
            for (auto* ref : this->_references){
				ref->_asset = nullptr;
			}
			this->_references.clear();
			//this->_assetid = 0;
        }

    }

    void AssetRef::_unref() {
		if (this->_asset != nullptr){
			std::cout << "[INFO] " << uid_t(*this->_asset) << " lost a reference.\n";
			this->_asset->_AssetBase_decrref(this);
			if (this->_asset->_AssetBase_marked_as_garbage){
				this->_asset->instance->_EngineInstance_assets_marked_garbage.insert(uid_t(*this->_asset));
				std::cout << "[INFO] " << uid_t(*this->_asset) << " marked as garbage.\n";
			}
			this->_asset = nullptr;
		}
	}

	void AssetRef::_enref(AssetBase* a) {
		if (a != nullptr){
			this->_asset = a;
			if (this->_asset->_AssetBase_marked_as_garbage){
				this->_asset->instance->_EngineInstance_assets_marked_garbage.erase(uid_t(*this->_asset));
			}
			this->_asset->_AssetBase_incrref(this);
			std::cout << "[INFO] " << uid_t(*this->_asset) << " gained a new reference.\n";
		}
	}

    void _BaseConsoleCommand::_baseccmd_private_init(){
        this->_einst->all_ccmds[this->_name] = this;
        /*
        if (this->command_type() == ccmdtype::ivar){
        	ConsoleIVAR* ivthis = reinterpret_cast<ConsoleIVAR*>(this);
        	this->_einst->ivar_ccmds[this->_name] = ivthis;
        }
        */
    }

    void ConsoleIVAR::_console_ivar_private_init() {
        this->instance->ivar_ccmds[this->name] = this;
    }

    class TestAsset : public AssetBase {
        using AssetBase::AssetBase;
        public:
            virtual void onRenderContextCreate() {
                std::cout << "onRenderContextCreate()\n";
            }
            virtual void onRenderContextDelete() {
                std::cout << "onRenderContextDelete()\n";
            }

            virtual void onCreate() {
                std::cout << "onCreate()\n";
            }
            virtual void onDestroy() {
                std::cout << "onDestroy()\n";
            }
    };

    class SoundFactory : public AssetBase {
        using AssetBase::AssetBase;
        public:
            std::unordered_set<uid_t> active_handles = {};
            virtual assettype asset_type() const {return assettype::sndfactory;}
            virtual sndfactype factory_type() const {return sndfactype::none;}
            virtual void onSoundContextCreate() {std::cout << "SoundFactory[" << uid_t(*this) << "]->onSoundContextCreate();\n";}
            virtual void onSoundContextDelete() {std::cout << "SoundFactory[" << uid_t(*this) << "]->onSoundContextDelete();\n";}
            virtual void onCreate() {std::cout << "SoundFactory[" << uid_t(*this) << "]->onCreate();\n";}
            virtual void onDestroy() {std::cout << "SoundFactory[" << uid_t(*this) << "]->onDestroy();\n";}

            virtual uid_t create_new_handle() {return 0;}
    };
    class SoundHandle : public AssetBase {
        using AssetBase::AssetBase;
        private:
            uid_t _factoryidx = 0;
            SoundFactory* _factory = nullptr;
        public:
            const uid_t& factory_idx = _factoryidx;
            SoundFactory* const& factory = _factory;
            bool _keep = false;
            virtual assettype asset_type() const {return assettype::sndhandle;}
            inline bool is_handle_valid() const {
                return (this->_factory != nullptr && this->instance->get_asset<SoundFactory>(this->_factoryidx) == this->_factory);
            }
            virtual void onSoundContextCreate() {std::cout << "SoundHandle[" << uid_t(*this) << "]->onSoundContextCreate();\n";}
            virtual void onSoundContextDelete() {std::cout << "SoundHandle[" << uid_t(*this) << "]->onSoundContextDelete();\n";}
            virtual void onCreate() {std::cout << "SoundHandle[" << uid_t(*this) << "]->onCreate();\n";}
            virtual void onDestroy() {
                this->onSoundHandleDestroy();
                if (this->is_handle_valid()){
                    this->_factory->active_handles.erase(uid_t(*this));
                    this->_factory = nullptr;
                    this->_factoryidx = 0;
                }
            }
            virtual void onSoundHandleCreate(const EngineInstance::playsoundargs_t& args){
                ;
            }
            virtual void onSoundHandleDestroy() {;}


            virtual bool get_looping() const {return false;}
            virtual void set_looping(const bool l) {}
            virtual fclock_t duration() const {return 0.0f;}
            virtual fclock_t tell() const {return 0.0f;}
            virtual void seek(const fclock_t& newpos) {}

            virtual EngineInstance::soundhandlestate get_state() const {
                return EngineInstance::soundhandlestate::invalid;
            }
            virtual void set_state(const EngineInstance::soundhandlestate& s){;}

            virtual fscalar_t get_gain() const {return 0.0f;}
            virtual void set_gain(const fscalar_t& g) {;}

            virtual fscalar_t get_pitch() const {return 0.0f;}
            virtual void set_pitch(const fscalar_t& p){;}
            struct soundhandle_update_params_t {
                MainloopData mld;
            };

            virtual void onSoundHandleUpdate(soundhandle_update_params_t& up) {}

            virtual bool reached_end() {return false;}
            bool soundhandle_update(soundhandle_update_params_t& up){
                /*
                auto state = this->get_state();
                switch (state) {
                    case EngineInstance::soundhandlestate::stopped: {
                        if (!this->_keep){return false;}
                        break;
                    }
                    default: ;
                }
                */
                if (!this->_keep && this->reached_end()){
                    return false;
                }
                this->onSoundHandleUpdate(up);
                return true;
            }

            SoundHandle(EngineInstance* _einst, const uid_t _assetid, SoundFactory* _fac) :
                _factoryidx(*_fac),
                _factory(_fac),
                AssetBase(_einst, _assetid)
            {}

    };

    EngineInstance::soundhandlestate EngineInstance::S_GetHandleState(const uid_t hndidx) {
        if (!this->S_IsHandle(hndidx)){return EngineInstance::soundhandlestate::invalid;}
        auto* hnd = this->get_asset<SoundHandle>(hndidx);
        return hnd->get_state();
    }
    bool EngineInstance::S_SetHandleState(const uid_t hndidx, const EngineInstance::soundhandlestate newstate){
        if (!this->S_IsHandle(hndidx)){return false;}
        auto* hnd = this->get_asset<SoundHandle>(hndidx);
        hnd->set_state(newstate);
        return true;
    }

    bool EngineInstance::S_UpdateSoundSystem(const MainloopData& mld) {
        auto& handles = this->_sndsys_updhnds;
        for (auto& iap : this->assets) {
            if (iap.second->asset_type() == assettype::sndhandle){
                handles.push_back((SoundHandle*)iap.second);
            }
        }
        for (auto& hnd : handles){
            SoundHandle::soundhandle_update_params_t up = {};
            up.mld = mld;
            if (!hnd->soundhandle_update(up)){
                this->del_asset(uid_t(*hnd));
            }
        }
        handles.clear();
        return true;
    }

    class SoundBufferHandle;

    class SoundBufferFactory : public SoundFactory {
        using SoundFactory::SoundFactory;
        public:
            #ifdef GL45RW_WITH_OPENAL
                ALuint _albuf = 0;
                const ALenum _sbfac_alfmtmap[2] = {AL_FORMAT_MONO16, AL_FORMAT_STEREO16};
            #endif
        public:
            std::valarray<int16_t> samples;
            size_t rate = 44100, nchannels = 1;
            inline size_t nsamples() const {
                return this->samples.size() / this->nchannels;
            }
            inline fclock_t duration() const {return this->nsamples() / fclock_t(this->rate);}

            SoundBufferFactory& zeros(const fclock_t newduration=0.0f) {
                if (newduration <= 0.0f){this->samples.resize(0);}
                else {
                    size_t newsize = size_t(this->rate * newduration) * this->nchannels;
                    this->samples.resize(newsize, 0);
                }
                return this->reupload_sound();
            }

            SoundBufferFactory& reupload_sound() {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alBufferData(this->_albuf, this->_sbfac_alfmtmap[this->nchannels - 1], &(this->samples[0]), this->samples.size() * sizeof(int16_t), this->rate);
                            alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
                return *this;
            }

            SoundBufferFactory& sine(const fclock_t dur, const fclock_t freq=60.0f, const fscalar_t amplitude=1.0f, const size_t _rate=44100, const size_t _nchannels=1) {
                this->rate = _rate;
                this->nchannels = _nchannels;
                this->samples.resize(size_t(this->rate * dur) * this->nchannels, 0);
                size_t _nsamps = this->nsamples();
                std::cout << "SoundBufferFactory::sine() -> _nsamps=" << _nsamps << "\n";
                for (size_t i = 0; i < _nsamps; i++){
                    fscalar_t t = fscalar_t(i*freq) / _rate;
                    fscalar_t v = glm::clamp(sin(t * 2.0f * 3.14159f) * amplitude, -1.0f, 1.0f);
                    for (size_t j = 0; j < _nchannels; j++){this->samples[(i*_nchannels)+j] = int16_t(v * 32767.0f);}
                }
                return this->reupload_sound();
            }

            SoundBufferFactory& fromfile(std::istream& f) {
                auto fstart = f.tellg();
                char _pcm_ident_rd[16] = {}; f.read(_pcm_ident_rd, 16);
                if (memcmp(_pcm_ident_rd, "gl45_pcmfile_hdr", 16) == 0){
                    struct {uint32_t _version, _nchannels, _rate, _sampfmt, _nsamples;} _hdr;
                    djutil::binio::unpack(&_hdr._version, f, 5, true);
                    this->rate = _hdr._rate;
                    this->nchannels = _hdr._nchannels;
                    this->samples.resize(_hdr._nsamples * _hdr._nchannels, 0);

                    switch (_hdr._sampfmt) {
                        case 0: {
                            std::valarray<uint8_t> rsamples; rsamples.resize(_hdr._nsamples * _hdr._nchannels, 0);
                            f.read((char*)&(rsamples[0]), rsamples.size());
                            for (size_t i = 0; i < rsamples.size(); i++){
                                float sf = glm::clamp(glm::mix(-1.0f, 1.0f, rsamples[i]/255.0f), -1.0f, 1.0f);
                                this->samples[i] = int16_t(sf * 32767.0f);
                            }
                            break;
                        }
                        case 1:
                        case 2: {
                            djutil::binio::unpack(&(this->samples[0]), f, this->samples.size(), true);
                            break;
                        }

                        case 3:
                        case 4: {
                            std::valarray<int32_t> rsamples; rsamples.resize(_hdr._nsamples * _hdr._nchannels, 0);
                            djutil::binio::unpack(&(rsamples[0]), f, rsamples.size(), true);
                            for (size_t i = 0; i < rsamples.size(); i++){
                                double sf = glm::clamp(rsamples[i]/2147483647.0, -1.0, 1.0);
                                this->samples[i] = sf * 32767.0;
                            }
                            break;
                        }
                        case 5: {
                            std::valarray<float> rsamples; rsamples.resize(_hdr._nsamples * _hdr._nchannels, 0.0f);
                            djutil::binio::unpack(&(rsamples[0]), f, rsamples.size(), true);
                            for (size_t i = 0; i < rsamples.size(); i++){
                                this->samples[i] = glm::clamp(rsamples[i], -1.0f, 1.0f) * 32767.0f;
                            }
                            break;
                        }
                        default: ;
                    }

                    return this->reupload_sound();
                }
                f.seekg(fstart);
                char _vorbis_ident_rd[4] = {}; f.read(_vorbis_ident_rd, 4);
                if (memcmp(_vorbis_ident_rd, "OggS", 4) == 0){
                    #if (defined(GL45RW_WITH_STB_VORBIS))
                    f.seekg(0, std::ios::end);
                    auto fend = f.tellg();
                    f.seekg(fstart);
                    std::string oggdata; oggdata.resize(size_t(fend) - size_t(fstart));
                    f.read(&oggdata.front(), oggdata.size());
                    int _nchannels, _rate;
                    short* _oggsamples = nullptr;
                    int _nsamples = stb_vorbis_decode_memory((const uint8_t*)&oggdata.front(), oggdata.size(), &_nchannels, &_rate, &_oggsamples);
                    if (_nsamples == -1){
                        throw std::runtime_error("failed to decode the Ogg Vorbis file.");
                    }
                    this->samples.resize(_nsamples * _nchannels, 0); std::copy_n(_oggsamples, _nsamples * _nchannels, &(this->samples[0]));
                    this->rate = _rate;
                    this->nchannels = _nchannels;
                    free(_oggsamples); _oggsamples = nullptr;
                    return this->reupload_sound();
                    #else
                    throw std::runtime_error("Ogg Vorbis support was disabled in this build.");
                    #endif
                }
                f.seekg(fstart);
                throw std::runtime_error("Could not identify the file type.");
            }

            SoundBufferFactory& loadFromFS(const std::u32string& fsp) {
                auto* fs = this->instance->I_GetMainFS();
                if (fs == nullptr){return this->sine(0.1f, 120.0f);}
                try {
                    djutil::virfs::vfistream istr; istr.open(*fs, fsp, std::ios::in | std::ios::binary);
                    return this->fromfile(istr);
                } catch (const std::exception& exc){
                    return this->sine(0.1f, 120.0f);
                }
            }

            virtual uid_t create_new_handle();

            virtual void onSoundContextCreate() {
                std::cout << "SoundBufferFactory::onSoundContextCreate()\n";
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer){
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            this->_albuf = 0;
                            alGenBuffers(1, &this->_albuf); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
                this->reupload_sound();
            }

            virtual void onSoundContextDelete() {
                std::cout << "SoundBufferFactory::onSoundContextDelete()\n";
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer){
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alDeleteBuffers(1, &this->_albuf); alGetError();
                            alGetError();
                            this->_albuf = 0;
                        #endif
                        break;
                    }
                    default: ;
                }
                this->reupload_sound();
            }

            virtual void onCreate() {
                this->onSoundContextCreate();
            }
            virtual void onDestroy() {
                while (this->active_handles.size() > 0){
                    auto idx = *(this->active_handles.cbegin());
                    this->instance->del_asset(idx);
                }
                this->active_handles.clear();
                this->onSoundContextDelete();
            }

    };

    class SoundBufferHandle : public SoundHandle {
        using SoundHandle::SoundHandle;
        private:
            bool _replay_after_restart = false;
            #ifdef GL45RW_WITH_OPENAL
                ALuint _alsource = 0;
            #endif
            fclock_t _lastpos = 0, _lastpitch = 1;
            bool _lastlooping = false;
            fscalar_t _lastgain = 1, _duration = 0;
            EngineInstance::soundhandlestate _laststate;
            void _record_laststate() {
                this->_lastpos = this->tell();
                this->_lastpitch = this->get_pitch();
                this->_laststate = this->get_state();
                this->_lastlooping = this->get_looping();
                this->_lastgain = this->get_gain();
            }
            void _restore_laststate() {
                this->seek(this->_lastpos);
                this->set_pitch(this->_lastpitch);
                this->set_gain(this->_lastgain);
                this->set_looping(this->_lastlooping);
                this->set_state(this->_laststate);
            }
        public:
            const fscalar_t& duration = _duration;
            virtual void onSoundContextCreate() {
                this->instance->S_MakeCurrent();
                auto* fac = (SoundBufferFactory*)this->factory;
                this->_duration = fac->duration();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alGenSources(1, &this->_alsource); alGetError();
                            alSourcef(this->_alsource, AL_PITCH, 1); alGetError();
                            alSourcef(this->_alsource, AL_GAIN, 1); alGetError();
                            alSource3f(this->_alsource, AL_POSITION, 0, 0, 0); alGetError();
                            alSource3f(this->_alsource, AL_VELOCITY, 0, 0, 0); alGetError();
                            alSourcei(this->_alsource, AL_LOOPING, AL_FALSE); alGetError();
                            alSourcei(this->_alsource, AL_BUFFER, fac->_albuf);
                            alSourcePlay(this->_alsource); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
                if (this->_replay_after_restart){
                    this->_replay_after_restart = false;
                    this->_restore_laststate();
                }
            }
            virtual void onSoundHandleCreate(const EngineInstance::playsoundargs_t& args) {
                this->onSoundContextCreate();
                this->set_looping(args.loop);
            }
            virtual void onSoundHandleDestroy() {
                std::cout << "SoundBufferFactory::onSoundHandleDestroy()\n";
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alSourceStop(this->_alsource); alGetError();
                            alDeleteSources(1, &this->_alsource); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
            }
            virtual bool get_looping() const {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALint loop;
                            alGetSourcei(this->_alsource, AL_LOOPING, &loop); alGetError();
                            return (loop == AL_TRUE);
                        #endif
                        break;
                    }
                    default: ;
                }
                return false;
            }
            virtual void set_looping(const bool l){
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alSourcei(this->_alsource, AL_LOOPING, (l ? AL_TRUE : AL_FALSE)); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            virtual fscalar_t get_gain() const {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALfloat gain = 0;
                            alGetSourcef(this->_alsource, AL_GAIN, &gain); alGetError();
                            return fscalar_t(gain);
                        #endif
                        break;
                    }
                    default: ;
                }
                return false;
            }
            virtual void set_gain(const fscalar_t& g){
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alSourcef(this->_alsource, AL_GAIN, g); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            virtual fscalar_t get_pitch() const {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALfloat pitch = 0;
                            alGetSourcef(this->_alsource, AL_PITCH, &pitch); alGetError();
                            return fscalar_t(pitch);
                        #endif
                        break;
                    }
                    default: ;
                }
                return false;
            }
            virtual void set_pitch(const fscalar_t& p){
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            alSourcef(this->_alsource, AL_PITCH, p); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            virtual EngineInstance::soundhandlestate get_state() const {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALint s;
                            alGetSourcei(this->_alsource, AL_SOURCE_STATE, &s); alGetError();
                            switch (s) {
                                case AL_INITIAL:
                                case AL_PAUSED: return EngineInstance::soundhandlestate::paused;

                                case AL_PLAYING: return EngineInstance::soundhandlestate::playing;
                                default: return EngineInstance::soundhandlestate::stopped;
                            }
                        #endif
                        break;
                    }
                    default: ;
                }
                return EngineInstance::soundhandlestate::stopped;
            }

            virtual bool reached_end() {
                this->instance->S_MakeCurrent();
                if (this->get_looping()){return false;}
                /*
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALfloat s;
                            alGetSourcef(this->_alsource, AL_SEC_OFFSET, &s); alGetError();
                            if (s >= this->duration){return true;}
                        #endif
                        break;
                    }
                    default: ;
                }
                return false;
                */
                return (this->get_state() == EngineInstance::soundhandlestate::stopped);
            }

            virtual void set_state(const EngineInstance::soundhandlestate& s){
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            switch (s) {
                                case EngineInstance::soundhandlestate::playing: {alSourcePlay(this->_alsource); alGetError(); break;}
                                case EngineInstance::soundhandlestate::paused: {alSourcePause(this->_alsource); alGetError(); break;}
                                case EngineInstance::soundhandlestate::stopped: {alSourceStop(this->_alsource); alGetError(); alSourceRewind(this->_alsource); alGetError(); break;}
                                default: ;
                            }
                        #endif
                        break;
                    }
                    default: ;
                }
            }

            virtual fclock_t tell() const {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            ALfloat s; alGetSourcef(this->_alsource, AL_SEC_OFFSET, &s); alGetError();
                            return fclock_t(s);
                        #endif
                        break;
                    }
                    default: ;
                }
                return fclock_t(0);
            }

            virtual void seek(const fclock_t& newpos) {
                this->instance->S_MakeCurrent();
                switch (this->instance->S_GetCurrentSoundMode().renderer) {
                    case sndapi::openal: {
                        #ifdef GL45RW_WITH_OPENAL
                            const fclock_t p = glm::clamp(newpos, fclock_t(0), this->duration);
                            alSourcef(this->_alsource, AL_SEC_OFFSET, ALfloat(p)); alGetError();
                        #endif
                        break;
                    }
                    default: ;
                }
            }

    };

    uid_t SoundBufferFactory::create_new_handle(){return this->instance->new_asset<SoundBufferHandle>(this);}

    uid_t EngineInstance::S_PlaySound(SoundFactory* fac, const EngineInstance::playsoundargs_t& args) {
        if (fac == nullptr){return 0;}
        uid_t hndidx = fac->create_new_handle();
        if (hndidx == 0){return 0;}
        auto* hnd = this->get_asset<SoundHandle>(hndidx);
        if (hnd == nullptr){return 0;}
        fac->active_handles.insert(hndidx);
        hnd->_keep = args.keep;
        hnd->onSoundHandleCreate(args);
        hnd->set_looping(args.loop);
        hnd->set_gain(args.gain);
        return hndidx;
    }

    struct teximg8_t {
        struct {int w, h;} dims;
        bool has_alpha_channel = false; //false for RGB24, true for RGBA32
        std::valarray<uint8_t> pixels;
        bool _dirty = false;

        inline size_t xy2idx(const int x, const int y, const int c=0) const {
            const int nch = 3+int(this->has_alpha_channel);
            if (x < 0 || x >= this->dims.w || y < 0 || y >= this->dims.h || c < 0 || c >= nch){return (size_t)(-1);}
            return (((y * this->dims.w)+x)*nch)+c;
        }

        inline teximg8_t& xflip() {
            djutil::imaging::uflip_ip(&(this->pixels[0]), this->dims.w, this->dims.h, 3+int(this->has_alpha_channel));
            this->_dirty = true;
            return *this;
        }

        inline teximg8_t& yflip() {
            djutil::imaging::vflip_ip(&(this->pixels[0]), this->dims.w, this->dims.h, 3+int(this->has_alpha_channel));
            this->_dirty = true;
            return *this;
        }

        teximg8_t& reinit_blank(const int w, const int h, const bool a=false) {
            this->pixels.resize(w*h*(3+int(a)));
            std::fill_n(&(this->pixels[0]), this->pixels.size(), 0);
            this->dims.w = w;
            this->dims.h = h;
            this->has_alpha_channel = a;
            this->_dirty = true;
            return *this;
        }
        teximg8_t& solid_fill(const int w, const int h, const rgba32_t& fill, const bool a=true){
			this->reinit_blank(w,h,a);
			uint8_t* pxstart = &this->pixels[0];
			if (a){std::fill_n((rgba32_t*)pxstart, this->pixels.size()/4, fill);}
			else {std::fill_n((rgb24_t*)pxstart, this->pixels.size()/3, rgb24_t(fill));}
			return *this;
		}
        teximg8_t& reinit_fromimage(const int w, const int h, const bool a, const void* px) {
            this->reinit_blank(w,h,a);
            std::copy_n(reinterpret_cast<const uint8_t*>(px), this->pixels.size(), &(this->pixels[0]));
            return *this;
        }
        teximg8_t& radial_gradient(const int d, const rgba32_t& black, const rgba32_t& white, const bool a=false) {
            this->reinit_blank(d,d,a);
            fvec2_t centroid((d/2)-1);
            fscalar_t radius = (d/2);
            size_t idx = 0;
            const fvec4_t
                src(black),
                dst(white)
            ;
            rgba32_t col;
            for (int y = 0; y < d; y++)
             for (int x = 0; x < d; x++, idx += (3+int(a))){
                 fscalar_t dfac = glm::length(fvec2_t{x,y}-centroid)/radius;
                 col = rgba32_t(glm::clamp(glm::mix(dst, src, dfac), fvec4_t(0), fvec4_t(255)));
                 std::copy_n(&col.x, 3+int(a), &(this->pixels[idx]));
            }
            return *this;
        }
        teximg8_t& checkerboard(const int ncellsx, const int ncellsy, const int cellw, const int cellh, const rgba32_t& black, const rgba32_t& white, const bool a=false){
			const int
			    w = ncellsx*cellw,
			    h = ncellsy*cellh,
			    pxsz = 3+int(a)
			;
			this->reinit_blank(w, h, a);
			uint8_t* colptr = &(this->pixels[0]);
			for (int y=0,x; y < h; y++)
			 for (x=0; x < w; x++, colptr += pxsz){
				 const int
				     cx = x/cellw,
				     cy = y/cellh
				 ;
				 const rgba32_t color = ((cx+cy)%2 == 0 ? black : white);
				 std::copy_n(&color.x, pxsz, colptr);
			}
		    return *this;
		}
        teximg8_t& copyRectFrom(const teximg8_t& source, const ivec2_t& origin, const ivec2_t& size) {
            //if (size.x <= 0 || size.y <= 0){return *this;}
            this->reinit_blank(size.x, size.y, source.has_alpha_channel);
            const ivec2_t endpos = origin+size;
            for (int y = 0, x; y < size.y; y++) {
                int sy = origin.y + y;
                for (x = origin.x; x < size.x; x++) {
                    int sx = origin.x + x;
                    size_t didx = this->xy2idx(x,y);
                    size_t sidx = source.xy2idx(sx,sy);
                    if (sidx == size_t(-1) || didx == size_t(-1)){continue;}
                    std::copy_n(&(source.pixels[sidx]), 3+int(this->has_alpha_channel), &(this->pixels[didx]));
                }
            }
            return *this;
        }
        teximg8_t& operator=(const teximg8_t& other) {
            this->reinit_blank(other.dims.w, other.dims.h, other.has_alpha_channel);
            std::copy_n(&(other.pixels[0]), this->pixels.size(), &(this->pixels[0]));
            std::cout << "teximg8_t assn\n";
            return *this;
        }
        teximg8_t& from_rasterimage(const djutil::imaging::RasterImage& rimg) {
            this->reinit_blank(rimg.width, rimg.height, true);
            rimg.to_rgba32(&(this->pixels[0]));
            return *this;
        }
        void to_rasterimage(djutil::imaging::RasterImage& rimg) const {
            rimg.init_truecolor(this->dims.w, this->dims.h, (this->has_alpha_channel ? djutil::imaging::RasterImage::colorformat::rgba : djutil::imaging::RasterImage::colorformat::rgb), djutil::imaging::RasterImage::pxetype::u8, &(this->pixels[0]));
        }

        teximg8_t& from_dmdimage(dmd::teximglvl_t& lvl) {
            this->reinit_blank(lvl.width, lvl.height, true);
            size_t lvlcpp = lvl.cpp;
            for (size_t i = 0; i < this->pixels.size()/4; i++) {
                uint8_t* dst = &(this->pixels[i*4]);
                uint8_t* src = lvl.pixels + (i*lvlcpp);

                switch (lvlcpp) {
                    case size_t(1U): dst[0] = dst[1] = dst[2] = src[0]; dst[1] = 255; break;
                    case size_t(2U): dst[0] = dst[1] = dst[2] = src[0]; dst[1] = src[1]; break;
                    case size_t(3U): dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = 255; break;
                    case size_t(4U): dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; break;
                    default: ;
                }
            }
            return *this;
        }

        void rot90ccw(teximg8_t& dst) const {
            /*
            dst.reinit_blank(this->dims.h, this->dims.w, this->has_alpha_channel);
            for (int y = 0; y < this->dims.h; y++){
             for (int x = 0; x < this->dims.w; x++){
                 std::copy_n(&(this->pixels[this->xy2idx(x,y)]), 3+int(this->has_alpha_channel), &(dst.pixels[dst.xy2idx(y, dst.dims.h-1-x)]));
             }
            }
            */
            this->rot90cw(dst); dst.yflip();
        }

        void rot90cw(teximg8_t& dst) const {
            dst.reinit_blank(this->dims.h, this->dims.w, this->has_alpha_channel);
            for (int y = 0; y < this->dims.h; y++){
             for (int x = 0; x < this->dims.w; x++){
                 std::copy_n(&(this->pixels[this->xy2idx(x,y)]), 3+int(this->has_alpha_channel), &(dst.pixels[dst.xy2idx(y, x)]));
             }
            }
        }

        void rot180(teximg8_t& dst) const {
            dst = *this;
            dst.xflip().yflip();
        }

        teximg8_t() : dims{1, 1}, _dirty(true), has_alpha_channel(false), pixels{0,0,0,0} {}
        teximg8_t(const teximg8_t& o) {(*this) = o;}
        ~teximg8_t() {
            std::cout << "gl45rw::teximg8_t deleted!\n";
            this->pixels.resize(0);
        }

    };

    class Texture2D : public AssetBase {
        using AssetBase::AssetBase;
        private:
            bool _renderdata_initialized = false;
            vectorlist<teximg8_t> _mips;
        public:
            virtual assettype asset_type() const {return assettype::texture;}

            texfilter _minfilter = texfilter::linear;
            texfilter _magfilter = texfilter::linear;

            texwrap
                _wrap_s = texwrap::repeat,
                _wrap_t = texwrap::repeat,
                _wrap_r = texwrap::repeat
            ;

            #ifdef __GL45RW_HAS_OPENGL
                GLuint gltexid = 0;
            #endif
            #ifdef GL45RW_WITH_SW3D2
                dj_sw3d20::Texture* sw3d2tex = nullptr;
                dj_sw3d20::EnvironmentMap* sw3d2envtex = nullptr;
            #endif
            #ifdef GL45RW_WITH_SW3D3
                sw3d3::texture_t* sw3d3tex = nullptr;
            #endif
            #ifdef GL45RW_WITH_SW3D3
            inline glm::vec4 _sw3d3_texture2D_alt(const glm::vec2& st){
                /*
                static const sw3d3::texwrap_e _wrapmap[] = {
                    sw3d3::texwrap_e::repeat,
                    sw3d3::texwrap_e::clamp_to_edge
                };

                const glm::vec2 uv = sw3d3::texture_t::_texwrapst(_wrapmap[int(this->_wrap_s)], _wrapmap[int(this->_wrap_t)], st);
                */
                auto& mip0 = this->getMipLevel();
                const glm::vec2 uv = glm::repeat(st);
                const int
                    x = (mip0.dims.w-1) * uv.x,
                    y = (mip0.dims.h-1) * uv.y
                ;
                const size_t idx = (y*mip0.dims.w)+x;

                auto* pxrgb = reinterpret_cast<sw3d3::rgb24_t*>(&(mip0.pixels[0]));
                auto* pxrgba = reinterpret_cast<sw3d3::rgba32_t*>(&(mip0.pixels[0]));
                return (mip0.has_alpha_channel ? glm::vec4(pxrgba[idx])/255.0f : glm::vec4(glm::vec3(pxrgb[idx])/255.0f, 1.0f));
            }
            #endif

            void update_texparams(const bool _bind=true) {
                this->instance->R_MakeCurrent();
                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        auto& gl = *(_glstuff::curgl);
                        if (this->gltexid != 0) {
                            gl.glActiveTexture(GL_TEXTURE0);
                            if (_bind){gl.glBindTexture(GL_TEXTURE_2D, this->gltexid);}

                            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _texfilter2gl(this->_minfilter));
                            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _texfilter2gl(this->_magfilter));

                            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _TEXWRAP_TO_GL[int(this->_wrap_s)]);
                            gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _TEXWRAP_TO_GL[int(this->_wrap_t)]);
                            //gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, _TEXWRAP_TO_GL[int(this->_wrap_r)]);

                            if (_bind){gl.glBindTexture(GL_TEXTURE_2D, 0);}
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            if (this->sw3d2tex != nullptr){
                                this->sw3d2tex->filter = _TEXFILTER_TO_SW3D2[int(this->_magfilter)];

                                this->sw3d2tex->wrap_s = _TEXWRAP_TO_SW3D2[int(this->_wrap_s)];
                                this->sw3d2tex->wrap_t = _TEXWRAP_TO_SW3D2[int(this->_wrap_t)];
                            }
                            if (this->sw3d2envtex != nullptr){
                                auto* env0 = this->sw3d2envtex[0][dj_sw3d20::enum_t::ENVTG_EQUIRECT];
                                env0->filter = _TEXFILTER_TO_SW3D2[int(this->_magfilter)];
                                env0->wrap_s = _TEXWRAP_TO_SW3D2[int(this->_wrap_s)];
                                env0->wrap_t = _TEXWRAP_TO_SW3D2[int(this->_wrap_t)];

                            }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            if (this->sw3d3tex != nullptr){
                                for (auto& tg : this->sw3d3tex->targets) {
                                    tg.filter = sw3d3::texfilter_e::nearest;
                                    tg.wrap_s = _TEXWRAP_TO_SW3D3[int(this->_wrap_s)];
                                    tg.wrap_t = _TEXWRAP_TO_SW3D3[int(this->_wrap_t)];
                                }
                            }
                        #endif
                        break;
                    }
                    default: ;
                }
            }
            virtual void onRenderContextDelete() {
                auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                this->instance->R_MakeCurrent();
                std::cout << "tex2d(" << uid_t(*this) << ")->onRenderContextDelete()\n";
                switch (rend) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        //if (!this->instance->R_HasActiveRenderer()){this->gltexid = 0;}
                        /*else*/ if (this->gltexid != 0) {
                            auto& gl = *(_glstuff::curgl);
                            std::cout << "tex2d(" << uid_t(*this) << ": deleting OpenGL texture id=" << this->gltexid << "\n";
                            gl.glDeleteTextures(1, &(this->gltexid));
                            this->gltexid = 0;
                        }

                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            //if (!this->instance->R_HasActiveRenderer()){this->sw3d2tex = nullptr;}
                            /*else*/ {
                                if (this->sw3d2tex != nullptr) {
                                    delete this->sw3d2tex;
                                    this->sw3d2tex = nullptr;
                                }
                                if (this->sw3d2envtex != nullptr){
                                    delete this->sw3d2envtex;
                                    this->sw3d2envtex = nullptr;
                                }
                            }

                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            if (this->sw3d3tex != nullptr){
                                auto* ctx = this->instance->sw3d3ctx;
                                ctx->delTexture(this->sw3d3tex->idx);
                                this->sw3d3tex = nullptr;
                            }
                        #endif
                        break;
                    }
                    default: ;
                }
                this->_renderdata_initialized = false;
            }
            virtual void onRenderContextCreate() {
                //if (!this->instance->R_HasActiveRenderer() || this->_renderdata_initialized){return;}
                const auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                this->instance->R_MakeCurrent();
                std::cout << "tex2d(" << uid_t(*this) << ")->onRenderContextCreate()\n";
                if (!this->_renderdata_initialized){
                    switch (rend) {
                        case gfxapi::opengl: {
                            #ifdef __GL45RW_HAS_OPENGL

                            if (this->gltexid == 0) {
                                auto& gl = *(_glstuff::curgl);
                                gl.glGenTextures(1, &(this->gltexid));
                                this->_renderdata_initialized = true;
                                std::cout << "tex2d(" << uid_t(*this) << ": created OpenGL texture id=" << this->gltexid << "\n";
                            }

                            #endif
                            break;
                        }
                        case gfxapi::sw3d2: {
                            #ifdef GL45RW_WITH_SW3D2
                            if (this->sw3d2tex == nullptr) {
                                auto& ctx = this->instance->sw3d2ctx[0];
                                this->sw3d2tex = ctx.srCreateManagedTexture();
                                this->_renderdata_initialized = true;
                            }
                            if (this->sw3d2envtex == nullptr){
                                auto& ctx = this->instance->sw3d2ctx[0];
                                this->sw3d2envtex = ctx.srCreateManaged<dj_sw3d20::EnvironmentMap>();
                                this->_renderdata_initialized = true;
                            }
                            #endif
                            break;
                        }
                        case gfxapi::sw3d3: {
                            #ifdef GL45RW_WITH_SW3D3
                            auto* ctx = this->instance->sw3d3ctx;
                            if (this->sw3d3tex == nullptr){
                                this->sw3d3tex = ctx->getTexture(ctx->newTexture());
                                this->_renderdata_initialized = true;
                            }
                            #endif
                            break;
                        }
                        default: ;
                    }
                }
                for (size_t i = 0; i < this->_mips.size(); i++) {
                    this->reupload_texture(i, true);
                }
            }

            teximg8_t& getMipLevel(const size_t level=0) {return this->_mips[level];}

            void reupload_texture(const size_t level=0, const bool override_dirty=false) {
                if ((!this->_renderdata_initialized) || level >= this->_mips.size()){
                    std::cout << "reupload_texture() fail\n";
                    return;
                }
                auto& mip = this->_mips[level];
                if (mip.pixels.size() > 0){
                    std::cout << "mip.pixels[0] = " << int(mip.pixels[0]) << "\n";
                }
                else {
                    std::cout << "mip.pixels is empty!\n";
                }
                auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                switch (rend) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL

                        this->instance->R_MakeCurrent();
                        auto& gl = *(_glstuff::curgl);
                        gl.glActiveTexture(GL_TEXTURE0);
                        gl.glBindTexture(GL_TEXTURE_2D, this->gltexid);
                        this->update_texparams(false);
                        if (override_dirty || mip._dirty) {
                            mip._dirty = false;
                            std::cout << "reloading opengl\n";
                            gl.glTexImage2D(GL_TEXTURE_2D, level, (mip.has_alpha_channel ? GL_RGBA8 : GL_RGB8), mip.dims.w, mip.dims.h, 0, (mip.has_alpha_channel ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, &(mip.pixels[0]));
                            gl.glFlush(); gl.glFinish();
                        }
                        gl.glBindTexture(GL_TEXTURE_2D, 0);
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                        if (level > 0){break;}
                        auto* ctx = this->instance->sw3d3ctx;
                        this->update_texparams(false);
                        auto swcolfmt = (mip.has_alpha_channel ? sw3d3::texelfmt_e::rgba8 : sw3d3::texelfmt_e::rgb8);
                        if (override_dirty || mip._dirty){
                            mip._dirty = false;
                            auto& tg2d = this->sw3d3tex->targets[0];
                            tg2d.flip_u_axis = false;
                            tg2d.flip_v_axis = false;
                            this->sw3d3tex->texImage2D(sw3d3::textarget_e::target2d, mip.dims.w, mip.dims.h, swcolfmt, &(mip.pixels[0]), false);
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        if (level > 0){break;}
                        this->instance->R_MakeCurrent();
                        auto& ctx = this->instance->sw3d2ctx[0];
                        this->update_texparams(false);
                        auto swcolfmt = (mip.has_alpha_channel ? dj_sw3d20::enum_t::RGBA : dj_sw3d20::enum_t::RGB);
                        if (override_dirty || mip._dirty) {
                            std::cout << "reloading sw3d2\n";
                            mip._dirty = false;

                            this->sw3d2tex->texImage2D(
                                &(mip.pixels[0]),
                                mip.dims.w, mip.dims.h,
                                swcolfmt,
                                0, false
                            );
                            this->sw3d2tex->mipmaps[0].origin_tl = true;
                            //djutil::imaging::vflip_ip(this->sw3d2tex->texels, mip.dims.w, mip.dims.h, 3+int(mip.has_alpha_channel));
                            if (level == 0){
                                auto& env0 = this->sw3d2envtex[0][dj_sw3d20::enum_t::ENVTG_EQUIRECT][0];
                                env0.mipmaps[0].origin_tl = true;
                                env0.texImage2D(
                                    &(mip.pixels[0]),
                                    mip.dims.w, mip.dims.h,
                                    swcolfmt,
                                    0, false
                                );
                                //djutil::imaging::vflip_ip(env0.texels, mip.dims.w, mip.dims.h, 3+int(mip.has_alpha_channel));
                            }
                        }
                        #endif
                        break;
                    }
                    default: break;
                }
            }
            virtual void onDestroy() {
                std::cout << "Texture2D<" << uid_t(*this) << ">::onDestroy();\n";
                this->onRenderContextDelete();
                this->_mips.clear();

            }
            virtual void onCreate() {
                this->onRenderContextCreate();
            }

            template <class PLT>
            void loadFromFS(const PLT& lvlpaths) {
                assert(lvlpaths.size() == this->_mips.size());
                djutil::virfs::vfistream istr;
                auto* fs = this->instance->I_GetMainFS();
                djutil::imaging::RasterImage lrimg;
                for (size_t i = 0; i < this->_mips.size(); i++) {
                    auto& mip = this->_mips[i];
                    istr.open(*fs, lvlpaths[i], std::ios::binary);
                    //std::stringstream istr;
                    //fs->open(istr, lvlpaths[i]);
                    lrimg.load_stbi(istr);
                    istr.close();
                    //lrimg.vflip();
                    mip.from_rasterimage(lrimg).yflip();
                    this->reupload_texture(i, true);
                }
            }
            template <class PT, typename... Paths>
            void loadFromFS_A(PT level0, Paths... levels) {
                vectorlist<std::u32string> lvls = {level0, levels...};
                this->loadFromFS(lvls);
            }
            Texture2D(EngineInstance* _einst, const uid_t _assetid, const size_t nmips=1) : AssetBase(_einst, _assetid) {
                this->_mips.resize(nmips);
            }
    };
    class CubemapTexture : public AssetBase {
        using AssetBase::AssetBase;
        public:
            bool _renderdata_init = false;
            teximg8_t packedfaces = {};
            int base_width = 0, base_height = 0;
            #ifdef __GL45RW_HAS_OPENGL
                GLuint gltexid = 0;
            #endif
            #ifdef GL45RW_WITH_SW3D2
                dj_sw3d20::EnvironmentMap* sw3d2envtex = nullptr;
            #endif
        public:
            virtual assettype asset_type() const {return assettype::cubemap;}
            bool uses_bilinear_filtering = false;
            teximg8_t& getPackedFacesInternal() {return this->packedfaces;}
            virtual void onRenderContextDelete() {
                auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                this->instance->R_MakeCurrent();
                switch (rend) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        if (!this->instance->R_HasActiveRenderer()){this->gltexid = 0;}
                        else if (this->gltexid != 0) {
                            auto& gl = *(_glstuff::curgl);
                            gl.glDeleteTextures(1, &(this->gltexid));
                            this->gltexid = 0;
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        if (!this->instance->R_HasActiveRenderer()){this->sw3d2envtex = nullptr;}
                        else if (this->sw3d2envtex != nullptr){
                            delete this->sw3d2envtex;
                            this->sw3d2envtex = nullptr;
                        }
                        #endif
                        break;
                    }
                    default: ;
                }
                this->_renderdata_init = false;
            }
            virtual bool reupload_cubemap(const bool override_dirty=false) {
                if (!this->_renderdata_init){return false;}
                if (!(override_dirty || this->packedfaces._dirty)){return false;}
                this->instance->R_MakeCurrent();
                auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                switch (rend) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        auto& gl = *(_glstuff::curgl);
                        gl.glActiveTexture(GL_TEXTURE0);
                        gl.glBindTexture(GL_TEXTURE_CUBE_MAP, this->gltexid);
                        gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                            gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                            gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                            gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                            for (int face = 0; face < 6; face++) {
                                GLenum glface = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
                                //size_t cellidx = this->packedfaces.xy2idx(0, this->base_height * face);
                                //uint8_t* cellptr = &(this->packedfaces.pixels[cellidx]);

                                teximg8_t faceimg, cellimg;
                                faceimg.copyRectFrom(this->packedfaces, glm::ivec2{0, this->base_height * face}, glm::ivec2{this->base_width, this->base_height});
                                if (glface == GL_TEXTURE_CUBE_MAP_NEGATIVE_X){
                                    faceimg.rot90ccw(cellimg);
                                    cellimg.xflip();
                                }
                                else if (glface == GL_TEXTURE_CUBE_MAP_POSITIVE_X){
                                    faceimg.rot90cw(cellimg);
                                    //cellimg.xflip();
                                }
                                else if (glface == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y || glface == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z){
                                    faceimg.rot180(cellimg);
                                    cellimg.yflip();
                                }
                                else {
                                    cellimg = faceimg;
                                    cellimg.yflip();
                                }
                                gl.glTexImage2D(
                                    glface, 0,
                                    (this->packedfaces.has_alpha_channel ? GL_RGBA : GL_RGB),
                                    this->base_width, this->base_height, 0,
                                    (this->packedfaces.has_alpha_channel ? GL_RGBA : GL_RGB),
                                    GL_UNSIGNED_BYTE, &(cellimg.pixels[0])
                                );
                                gl.glFlush(); gl.glFinish();
                            }
                        gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            const dj_sw3d20::enum_t _sw3d2facemap[6] = {
                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_POSX,
                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_NEGX,

                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_POSY,
                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_NEGY,

                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_POSZ,
                                dj_sw3d20::enum_t::ENVTG_CUBEMAP_NEGZ
                            };
                            //this->instance->R_MakeCurrent();
                            auto& ctx = this->instance->sw3d2ctx[0];
                            //this->update_texparams(false);
                            for (int face = 0; face < 6; face++) {
                                auto swface = _sw3d2facemap[face];
                                size_t cellidx = this->packedfaces.xy2idx(0, this->base_height * face);
                                uint8_t* cellptr = &(this->packedfaces.pixels[cellidx]);
                                auto& envtg = this->sw3d2envtex[0][swface][0];
                                envtg.wrap_s = envtg.wrap_t = dj_sw3d20::enum_t::CLAMP_TO_EDGE;
                                envtg.filter = dj_sw3d20::enum_t::NEAREST;
                                envtg.texImage2D(
                                    cellptr,
                                    this->base_width, this->base_height,
                                    (this->packedfaces.has_alpha_channel ? dj_sw3d20::enum_t::RGBA : dj_sw3d20::enum_t::RGB),
                                    0, true
                                );
                                //djutil::imaging::vflip_ip(envtg.texels, this->base_width, this->base_height, 3+int(this->packedfaces.has_alpha_channel));
                            }
                        #endif
                        break;
                    }
                    default: ;
                }
                this->packedfaces._dirty = false;
                return true;
            }
            virtual void onRenderContextCreate() {
                if (!this->instance->R_HasActiveRenderer()){return;}
                this->instance->R_MakeCurrent();
                auto rend = this->instance->R_GetCurrentVideoMode().renderer;
                switch (rend) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                        if (this->gltexid == 0) {
                            auto& gl = *(_glstuff::curgl);
                            gl.glGenTextures(1, &(this->gltexid));
                            this->_renderdata_init = true;
                        }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                        if (this->sw3d2envtex == nullptr){
                            auto* ctx = this->instance->sw3d2ctx;
                            this->sw3d2envtex = ctx->srCreateManaged<dj_sw3d20::EnvironmentMap>();
                            this->_renderdata_init = true;
                        }
                        #endif
                        break;
                    }
                    default: ;
                }
                this->reupload_cubemap(true);
            }

            void resetToDefault() {
                const uint8_t nupixels[] = {
                    255,000,000,
                    000,255,000,
                    000,000,255,
                    255,255,000,
                    255,000,255,
                    255,255,255
                };
                this->base_width = this->base_height = 1;
                this->packedfaces.reinit_blank(1,6,false);
                std::copy_n(nupixels, sizeof(nupixels), &(this->packedfaces.pixels[0]));
            }

            void setInternalPackedImageRaw(const size_t nbw, const size_t nbh, const bool is_rgba, const uint8_t* npx){
                this->base_width = nbw;
                this->base_height = nbh;
                this->packedfaces.reinit_blank(nbw, nbh*6, is_rgba);
                std::copy_n(npx, (nbw*nbh*6*(3+int(is_rgba))), &(this->packedfaces.pixels[0]));
            }

            void loadPackedFromFS_IF(const fspath_t& pf) {
                djutil::virfs::vfistream istr; istr.open(this->instance->I_GetMainFS()[0], pf, std::ios::binary);
                djutil::imaging::RasterImage rimg; rimg.load_stbi(istr);
                //if (rimg.height % 6 != 0){throw std::runtime_error("new image's height is not divisible by 6!");}
                this->packedfaces.from_rasterimage(rimg);
                this->base_width = rimg.width;
                this->base_height = rimg.height / 6;
                this->reupload_cubemap(true);
            }

            void loadFacesFromFS_6I(
                const fspath_t& posx,
                const fspath_t& negx,
                const fspath_t& posy,
                const fspath_t& negy,
                const fspath_t& posz,
                const fspath_t& negz
            )
            {
                const fspath_t paths[6] = {
                    posx, negx,
                    posy, negy,
                    posz, negz
                };
                for (int i = 0; i < 6; i++){
                    const auto& fsp = paths[i];
                    teximg8_t faceimg = {};
                    djutil::virfs::vfistream istr;
                    istr.open(this->instance->I_GetMainFS()[0], fsp, std::ios::binary);
                    djutil::imaging::RasterImage rimg;
                    rimg.load_stbi(istr);
                    istr.close();
                    faceimg.from_rasterimage(rimg);
                    if (i == 0){
                        this->base_width = faceimg.dims.w;
                        this->base_height = faceimg.dims.h;
                        this->packedfaces.reinit_blank(this->base_width, this->base_height*6, faceimg.has_alpha_channel);
                    }
                    else {
                        if (faceimg.dims.w != this->base_width){throw std::runtime_error("face width && base_width mismatch!");}
                        if (faceimg.dims.h != this->base_height){throw std::runtime_error("face height && base_height mismatch!");}
                        if (faceimg.has_alpha_channel != this->packedfaces.has_alpha_channel){throw std::runtime_error("color format mismatch!");}
                    }
                    std::copy_n(&(faceimg.pixels[0]), (faceimg.dims.w*faceimg.dims.h*(3+int(faceimg.has_alpha_channel))), &(this->packedfaces.pixels[this->packedfaces.xy2idx(0, i*this->base_height)]));
                }
                this->reupload_cubemap(true);
            }

            virtual void onCreate() {
                this->resetToDefault();
                this->onRenderContextCreate();
            }
            virtual void onDestroy() {
                this->onRenderContextDelete();
            }
    };

    class BitmapFont : public AssetBase {
        using AssetBase::AssetBase;
        public:
            Texture2D* font_texture = nullptr;

            struct charmap_entry {
                fvec2_t tluv, bruv;
            };

            dictionary<char32_t, charmap_entry> character_map = {};
            fvec2_t spacing = {1.0f, 1.0f};

            #ifdef GL45RW_WITH_SW3D2
             dj_sw3d20::Material sw3d2_font_mat = {};
            #endif

            virtual assettype asset_type() const {return assettype::bitmapfont;}

            void _rebuild() {
            	#ifdef GL45RW_WITH_SW3D2
            	 dj_sw3d20::Material nfm = {};
            	 nfm.shadeless = true;
            	 nfm.uses_transparency = false;
            	 nfm.alpha_threshold = 0.2f;
            	 nfm.base_diffuse_color = dj_sw3d20::fpvec4{1,1,1,0};
            	 dj_sw3d20::MaterialTextureSlot nft = {};
            	 nft.src_image = (this->font_texture != nullptr ? this->font_texture->sw3d2tex : nullptr);
            	 nft.affects_diffuse_color.use = false;
            	 nft.affects_diffuse_alpha.use = true;
            	 nft.affects_diffuse_alpha.amount = 1.0;
            	 nfm.texture_slots.push_back(nft);
            	 this->sw3d2_font_mat = nfm;
            	#endif
            }

            void mapToQ3Bigchars1() {
                this->character_map.clear();
                for (size_t i = 0; i < 256; i++){
                    size_t row = i/16;
                    size_t col = i%16;
                    charmap_entry e = {};
                    const fvec2_t topuv = fvec2_t{col/16.0f, 1.0f-(row/16.0f)};
                    e.tluv = glm::trunc(topuv*16.0f)/16.0f;
                    e.bruv = (e.tluv + fvec2_t{1/17.0f, -1/17.0f});
                    this->character_map[char32_t(i)] = e;
                }
            }
            void mapToQ3Bigchars(const int width_hint, const int height_hint, const int ncols=16, const int nrows=16) {
                this->character_map.clear();
                const int width_incr = (width_hint/ncols);
                const int height_incr = (height_hint/nrows);
                char32_t chr = 0;
                int ix = 0, iy = 0, dx = width_incr, dy = height_incr;
                const fvec2_t wh = {float(width_hint), float(height_hint)};
                const fvec2_t whincr = wh/fvec2_t{ncols, nrows};
                for (int row = 0; row < nrows; row++, iy += height_incr, dy += height_incr) {
                    ix = 0; dx = width_incr;
                    float fix = 0.0f;
                    float fiy = whincr.y*row;
                    for (int col = 0; col < ncols; col++, ix += width_incr, dx += width_incr, chr++) {
                        fix = whincr.x*col;
                        charmap_entry e = {};
                        const fvec2_t topuv = fvec2_t{roundf(fix)/wh.x, 1.0f - (roundf(fiy)/wh.y)};
                        const fvec2_t btmuv = fvec2_t{roundf(fix+whincr.x-1)/wh.x, 1.0f - (roundf(fiy+whincr.y-1)/wh.y)};
                        e.tluv = topuv;
                        e.bruv = btmuv;
                        this->character_map[chr] = e;
                    }
                }
            }
            void mapToQ3Bigchars() {
                auto& mip = this->font_texture->getMipLevel(0);
                this->mapToQ3Bigchars(mip.dims.w, mip.dims.h, 16, 16);
            }
            void load_from_file(const std::u32string& path) {
                this->create_texture();
                /*
                auto& mip = this->font_texture->getMipLevel(0);
                djutil::ufpio::ufstream istr; istr.open(path, std::ios::in | std::ios::binary);
                imaging::RasterImage ri; ri.load_stbi(istr);
                mip.from_rasterimage(ri).yflip();
                this->font_texture->reupload_texture(0, true);
                */
                this->font_texture->loadFromFS_A(path);
                this->font_texture->reupload_texture(0, true);
                this->_rebuild();
            }
            void load_default_font() {
                this->create_texture();
                auto* tex = this->font_texture;
                auto& mip = tex->getMipLevel();
                //const int _dims[2] = {160,160};
                const int _w = _default_bitmap_font::dims[0], _h = _default_bitmap_font::dims[1];
                const int _col_incr = _w/16, _row_incr = _h/16;
                const size_t npx = _w*_h;
                mip.reinit_blank(_w, _h, true);
                for (size_t i = 0; i < npx;) {
                    uint8_t* pxptr = &(mip.pixels[0]) + (i * 4);
                    uint8_t pbit = _default_bitmap_font::packed_bits[i/8];
                    for (uint8_t bit = 0; bit < 8; bit++, i++, pxptr+=4) {
                        uint8_t _value = ((pbit & (uint8_t(1) << bit)) ? 255U : 0U);
                        pxptr[0] = pxptr[1] = pxptr[2] = pxptr[3] = _value;
                    }
                }
                mip.yflip();
                tex->_minfilter = tex->_magfilter = texfilter::nearest;
                this->font_texture->reupload_texture(0, true);

                this->character_map.clear();

                char32_t chr = 0;
                int cx = 0, cy = 0;
                for (int row = 0; row < 16; row++, cy+=_row_incr) {
                    cx = 0;
                    for (int col = 0; col < 16; col++, cx+=_col_incr, chr++){
                        this->character_map[chr] = charmap_entry{
                            .tluv=fvec2_t{(cx/float(_w)), 1.0f - (cy/float(_h))},
                            .bruv=fvec2_t{((cx+(_col_incr))/float(_w)), 1.0f - ((cy+(_row_incr - 1))/float(_h))},
                        };
                    }
                }
                this->spacing = fvec2_t{0.8f, 1.0f};
                //this->mapToQ3Bigchars();
                this->_rebuild();
            }
            void create_texture() {
                if (this->font_texture != nullptr){return;}
                uid_t idx = this->instance->new_asset<Texture2D>(1);
                auto* tex = this->font_texture = this->instance->get_asset<Texture2D>(idx);
                tex->_minfilter = tex->_magfilter = texfilter::linear;
                tex->_wrap_s = tex->_wrap_t = texwrap::clamp_to_edge;
                this->_rebuild();
            }

            virtual void onCreate() {
                //this->font_texture->
            }
            virtual void onDestroy() {
                if (this->font_texture != nullptr){this->instance->del_asset(uid_t(*(this->font_texture)));}
                this->font_texture = nullptr;
            }
    };
    class UnbindCCMD : public ConsoleCommand {
		using ConsoleCommand::ConsoleCommand;
		private:
		    std::string _catname = "";
		public:
		    const std::string& category_name = _catname;
		    virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata){
				EngineInstance::bindcategory_t* bcat = this->instance->bindcats.getd(this->_catname, nullptr);
				if (bcat == nullptr){return 0;}
				std::string keyname = djutil::ezstr::utf32_to_utf8(args);
				auto& cd = this->instance->C_GetConsoleData();
				if (E_GetInputInfo(keyname) == _null_input_event_info){
					cd.lines.push_back(U"%cr[ERROR]%cw " + args + U"%c? isn't a valid key!");
					return 0;
				}
				bcat->binds.popk(keyname);
				return 0;
			}
			UnbindCCMD(EngineInstance* _einst, const std::u32string& _name, const std::string& _bindcatname) :
                ConsoleCommand(_einst, _name),
                _catname(_bindcatname)
            {}
	};
    class BindCCMD : public ConsoleCommand {
        using ConsoleCommand::ConsoleCommand;
        private:
            std::string _catname = "";
            EngineInstance::bindcategory_t* _catptr = nullptr;
        public:
            const std::string& category_name = _catname;
            EngineInstance::bindcategory_t* const& category = _catptr;

            virtual int invoke(const std::u32string& args, CCMDInvokationExtraData& edata) {
                //const inputeventinfo_t& iei = E_GetInputInfo("");
                //this->_catptr->binds;
                EngineInstance::bindcategory_t* bcat = this->instance->bindcats.getd(this->_catname, nullptr);
                auto& cd = this->instance->C_GetConsoleData();
                if (bcat == nullptr){return 0;}
                else if (false) {
					vectorlist<std::u32string> argv = {};
					split_argstr<char32_t>(argv, args);
					std::string keyname;


					if (argv.size() == 0){return 0;}
					if (argv.size() >= 1){
						keyname = djutil::ezstr::utf32_to_utf8(argv[0]);
						if (E_GetInputInfo(keyname) == _null_input_event_info){
							cd.lines.push_back(U"%cr[ERROR]%cw " + argv[0] + U"%c? isn't a valid key!");
							return 0;
						}
					}
					if (argv.size() >= 2){
						const std::u32string command = argv[1];
						EngineInstance::cmdbinding_t newbind = {};
						djutil::ezstr::partition<char32_t>(newbind.command_name, newbind.command_args, command, U" ");
						bcat->binds[keyname] = newbind;
						/*
						auto argit = argv.cbegin()+2;

						while (argit < argv.cend()){
							auto arg = *argit; argit++;

						}
						*/
					}

					else if (argv.size() == 1){
						if (bcat->binds.count(keyname) == 0){
							cd.lines.push_back(U"%cw" + argv[0] + U"%c? isn't bound.");
						}
						else {
							auto& binding = bcat->binds[keyname];
							cd.lines.push_back(U"%cw" + argv[0] + U"%c? = " + binding.command_name + U" " + binding.command_args + U"%c?");
						}
					}
				}
				else {
				    int oargno = 0;
				    std::string keyname = "";
				    EngineInstance::cmdbinding_t newbind = {};

				    ArgumentParser<char32_t> ap; ap.load_argstr(args);
				    for (parsedarg_t<char32_t> parg; ap.adv(parg);){
						switch (parg.prefix) {
							case argprefix::none: {
							    switch (oargno) {
									case 0: {
										keyname = djutil::ezstr::utf32_to_utf8(parg.value);
										if (E_GetInputInfo(keyname) == _null_input_event_info){
											cd.lines.push_back(U"%cr[ERROR]%cw " + parg.value + U"%c? isn't a valid key!");
											return 0;
										}
										break;
									}
									case 1: {
										djutil::ezstr::partition<char32_t>(
										    newbind.command_name,
										    newbind.command_args,
										    parg.value,
										    U" "
										);
										break;
									}
									default: ;
								}
								oargno++;
								break;
							}
							case argprefix::dash: {
								if (parg.name == U"deadzone"){
									newbind.deadzone = fscalar_t(std::stof(djutil::ezstr::utf32_to_utf8(parg.value)));
								}
								break;
							}
							default: ;
						}
					}
					std::u32string ukeyname = djutil::ezstr::utf8_to_utf32(keyname);
					if (oargno == 1){
						if (bcat->binds.count(keyname) == 0){
							cd.lines.push_back(U"%cw" + ukeyname + U"%c? isn't bound.");
						}
						else {
							auto& binding = bcat->binds[keyname];
							cd.lines.push_back(U"%cw" + ukeyname + U"%c? = " + binding.command_name + U" " + binding.command_args + U"%c?");
						}
					}
					else if (oargno >= 2){
						bcat->binds[keyname] = newbind;
					}
				}
                return 0;
            }

            BindCCMD(EngineInstance* _einst, const std::u32string& _name, const std::string& _bindcatname) :
                ConsoleCommand(_einst, _name),
                _catname(_bindcatname)
            {}
    };

    class ConsoleTextSize_CVAR : public ConsoleCVAR {
        using ConsoleCVAR::ConsoleCVAR;
        public:
            virtual void getstr(std::u32string& s) {
                s = djutil::ezstr::utf8_to_utf32(std::to_string(this->instance->C_GetConsoleData().font_npx));
            }

            virtual int setstr(const std::u32string& s) {
                try {
                    this->instance->C_GetConsoleData().font_npx = std::stoi(djutil::ezstr::utf32_to_utf8(s));
                    this->instance->C_RefreshConsoleLayout();
                } catch (const std::exception& e){;}
                return 0;
            }
    };


    EngineInstance::bindcategory_t* EngineInstance::I_CreateBindCategory(const std::string& catname, const bindcmdnames_t& cmdnames, const bindcatflags flags) {
        EngineInstance::bindcategory_t* cur = this->bindcats.getd(catname, nullptr);
        if (cur != nullptr){return nullptr;}
        this->bindcats[catname] = cur = new EngineInstance::bindcategory_t();
        cur->flags = flags;
        cur->associated_bind_ccmd = this->new_ccmd<BindCCMD>(cmdnames.bind, catname);
        cur->associated_unbind_ccmd = this->new_ccmd<UnbindCCMD>(cmdnames.unbind, catname);
        return cur;
    }

    void EngineInstance::_createConsoleData() {
        auto* font = this->new_asset_rp<BitmapFont>();
        font->load_default_font();
        font->font_texture->_minfilter = font->font_texture->_magfilter = texfilter::nearest;
        this->console_data.fontid = uid_t(*font);
        this->console_data._init = true;
        this->console_data._pending_refresh = true;
    }
    void EngineInstance::C_UpdateConsole(const MainloopData& mld) {
        auto& cd = this->console_data;
        switch (cd.state){
            case EngineInstance::_constate::opened: {
                for (auto& tievt : this->text_events) {
                    if (int(tievt._class) == 0){
                        for (auto& nc : tievt.newchars) {
                            if (nc == U'`'){continue;}
                            cd._curhist = size_t(-1);
                            cd.curinput.push_back(nc);
                        }
                        //cd.curinput.append(tievt.newchars);
                    }
                    else if (int(tievt._class) == 1){
                        if (cd.curinput.size() > 0){cd.curinput.pop_back();}
                        cd._curhist = size_t(-1);
                    }
                    else if (int(tievt._class) == 2){
                        if (cd.curinput.size() > 0){cd.lines.push_back(U"%c?>%c?"+cd.curinput+U"%c?");}
                        std::u32string cmdn, cmda;
                        djutil::ezstr::partition<char32_t>(cmdn, cmda, cd.curinput, U" ");
                        CCMDInvokationExtraData cmdid = {};
                        int status = this->C_ExecCommand(cmdn, cmda, cmdid);
                        if (status == -1 && cd.curinput.size() > 0) {
                            cd.lines.push_back(U"%cr[ERROR] Invalid command.%c?");
                        }
                        if (cd.history.size() == 0 || cd.curinput != cd.history.back()) {
                            cd.history.push_back(cd.curinput);
                        }
                        cd._curhist = size_t(-1);
                        cd._curinput_hold = cd.curinput = U"";
                    }
                    else if (int(tievt._class) == 3) {
                        if (cd._curhist == size_t(-1) && cd.history.size() > 0){
                            cd._curinput_hold = cd.curinput;
                            cd._curhist = 0;
                        }
                        else if (cd.history.size() > 0) {
                            cd._curhist = std::min(cd.history.size() - 1, (cd._curhist+1));
                        }
                        if (cd._curhist != size_t(-1)){cd.curinput = *(cd.history.rbegin() + cd._curhist);}
                    }
                    else if (int(tievt._class) == 4) {
                        if (cd._curhist == 0) {
                            cd._curhist = size_t(-1);
                            cd.curinput = cd._curinput_hold;
                        }
                        else if (cd._curhist > 0 && cd._curhist < size_t(-1)) {
                            cd._curhist = std::min(cd.history.size() - 1, (cd._curhist-1));
                            cd.curinput = *(cd.history.rbegin() + cd._curhist);
                        }
                    }
                    else if (int(tievt._class) == 5) {
                    	auto spaceitr = std::find(cd.curinput.begin(), cd.curinput.end(), U' ');
                    	if (spaceitr != cd.curinput.end()){continue;}
                    	vectorlist<std::u32string> results = {};
                    	for (auto& ep : this->all_ccmds) {
                    		if (djutil::ezstr::startswith<char32_t>(ep.first, cd.curinput)){
                    			results.push_back(ep.first);
                    		}
                    	}
                    	if (results.size() == 0){continue;}
                    	else if (results.size() == 1){
                    		cd.curinput = results[0] + (cd.curinput.size() > 0 ? U" " : U"");
                    		continue;
                    	}
                    	cd.lines.push_back(U"%cw\\%c?");
                    	cd.lines.back() += cd.curinput;
                    	cd.lines.back() += U"%c?";
                    	for (auto& res : results) {
                    		cd.lines.push_back(U"%cw -> %c?");
                    		cd.lines.back() += res;
                    		cd.lines.back() += U"%c?";
                    	}
                    	std::u32string common_chars = U"";
                    	for (size_t cci = 0; true; cci++) {
							bool mustbreak = false;
							char32_t c;
							for (auto& res : results){
								if (cci >= res.size()){mustbreak = true; break;}
								c = res[cci];
								if (c != results[0][cci]){mustbreak = true; break;}
							}
							if (mustbreak){break;}
							common_chars.push_back(c);
						}
						cd.curinput = common_chars;
                    }
                }
                break;
            }
            case EngineInstance::_constate::opening: {
                #ifndef _GL45RW_HEADLESS
                 #if (defined(_GL45RW_WINDOW_SDL2))
                 SDL_StartTextInput();
                 #endif
                #endif
                cd.openamount += (cd.openspeed * mld.ticdelta);
                if (cd.openamount >= 1.0f){
                    cd.openamount = 1.0f;
                    cd.state = EngineInstance::_constate::opened;
                }
                break;
            }
            case EngineInstance::_constate::closing: {
                #ifndef _GL45RW_HEADLESS
                 #if (defined(_GL45RW_WINDOW_SDL2))
                 SDL_StopTextInput();
                 #endif
                #endif
                cd.openamount -= (cd.closespeed * mld.ticdelta);
                if (cd.openamount <= 0.0f){
                    cd.openamount = 0.0f;
                    cd.state = EngineInstance::_constate::closed;
                }
                break;
            }
            default: ;
        }
    }
    void EngineInstance::R_RedrawConsole() {
        //if (!this->has_window() || this->console_data.state == _constate::closed){return;}
        if (!this->has_window()){return;}
        if (this->_curmode.renderer != gfxapi::opengl && this->console_data.state == _constate::closed){return;}
        auto& cd = this->console_data;
        const fscalar_t aspect = this->R_WindowAspect();
        const fvec4_t bgcolor = {0.125f, 0.25f, 0.375f, /*0.75f*/ 0.95f};
        const fvec2_t textsize = fvec2_t(cd.font_npx) / fvec2_t{this->_curmode.width, this->_curmode.height};
        const fscalar_t con_vpos = glm::mix(1.0f, cd.opened_bottom_v, cd.openamount);
        //const fscalar_t con_vpos_top = con_vpos + (1.0f - cd.opened_bottom_v);
        const fscalar_t con_vpos_top = con_vpos + (1.0f - cd.opened_bottom_v);
        this->R_MakeCurrent();
        switch (this->_curmode.renderer) {
            case gfxapi::opengl: {
                #ifdef __GL45RW_HAS_OPENGL
                 auto& gl = this->_glcalls;
                  this->gl_winfbo->bind(GL_DRAW_FRAMEBUFFER);
                  gl.glClearColor(0,0,0,0);
                  gl.glClear(GL_COLOR_BUFFER_BIT);
                  if (this->console_data.state != _constate::closed){
                      gl.glDisable(GL_DEPTH_TEST); gl.glDepthMask(0);
                      gl.glDisable(GL_CULL_FACE);

                      gl.glUseProgram(0);
                      gl.glActiveTexture(GL_TEXTURE0);
                      #if (defined(_GL45RW_GL2))
                      gl.glBindTexture(GL_TEXTURE_2D, 0);
                      gl.glMatrixMode(GL_MODELVIEW); gl.glLoadIdentity();
                      gl.glTranslatef(0.0f, con_vpos, -0.09f);
                      gl.glScalef(aspect, 1.0f, 1.0f);
                      gl.glMatrixMode(GL_PROJECTION); gl.glLoadIdentity(); gl.glOrtho(-aspect, aspect, -1.0f, 1.0f, 0.01f, 10.0f);
                      #endif
                      this->R_SetViewportFromWindow();
                      //gl.glEnable(GL_BLEND);
                      //gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                      //gl.glBlendEquation(GL_FUNC_ADD);
                      #if (defined(_GL45RW_GL2))
                      gl.glBegin(GL_TRIANGLES);
                      gl.glColor4fv(glm::value_ptr(bgcolor));

                      gl.glVertex2f(-1.0f, -0.01f);
                      gl.glVertex2f(1.0f, -0.01f);
                      gl.glVertex2f(-1.0f, 2.0f);
                      gl.glVertex2f(-1.0f, 2.0f);
                      gl.glVertex2f(1.0f, 2.0f);
                      gl.glVertex2f(1.0f, -0.01f);

                      gl.glEnd();
                      #elif (defined(_GL45RW_GLES3))
                      {_glstuff::GLProgramBinding glb1(*this->_gles3_conback_glshader);
                          gl.glBindVertexArray(this->_gles3_attributeless_vao);
                          glb1.setUniform(this->_gles3_conback_glshader->uniform_idx("color"), 1, &bgcolor);
                          glb1.setUniform(this->_gles3_conback_glshader->uniform_idx("ypos"), 1, &con_vpos);
                          gl.glDrawArrays(GL_TRIANGLES, 0, 6);
                          gl.glBindVertexArray(0);
                      }
                      #endif
                      gl.glDisable(GL_BLEND);
                      gl.glColorMask(true, true, true, false);
                      this->matrices.projection.pushMatrix(); this->matrices.projection.loadIdentity();
                      this->matrices.view.pushMatrix(); this->matrices.view.loadIdentity();
                      this->matrices.model.pushMatrix(); this->matrices.model.loadIdentity();

                      gl.glEnable(GL_BLEND);
                      gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                      //gl.glBlendFunc(GL_DST_ALPHA, GL_ONE);
                      gl.glBlendEquation(GL_FUNC_ADD);


                      this->matrices.model[-1] = fmat44_t(trs_t(
                          fvec3_t{(-1.0f), con_vpos_top, -0.03f},
                          fmat33_t(1),
                          fvec3_t(textsize, 1.0f)
                      ));

                      //this->R_DrawBitmapText(this->console_data.fontid, U"%cwRoses are %crred%cw\nViolets are %cbblue%cw\nThis console is %cgcool%cw,\nand so are %c0you.");

                      int maxlines = cd.maxlines;
                      int nlines = cd.lines.size();
                      int startidx = nlines - maxlines;
                      std::u32string catlines = U"%c?";
                      for (int idx = startidx; idx < nlines; idx++) {
                          if (idx < 0 || idx >= nlines) {catlines.append(U"\n");}
                          else {catlines.append(cd.lines[idx]); catlines.push_back(U'\n');}
                      }
                      this->R_DrawBitmapText(this->console_data.fontid, catlines);

                      this->matrices.model[-1] = fmat44_t(trs_t(
                          fvec3_t{-1.0f, con_vpos+textsize.y, -0.03f},
                          fmat33_t(1),
                          fvec3_t(textsize, 1.0f)
                      ));
                      std::u32string inpline = U"%cw>%cd";
                      for (auto& chr : cd.curinput) {
                          if (chr == U'%'){inpline.append(U"%%");}
                          else {inpline.push_back(chr);}
                      }
                      this->R_DrawBitmapText(this->console_data.fontid, inpline);

                      this->matrices.projection.popMatrix();
                      this->matrices.view.popMatrix();
                      this->matrices.model.popMatrix();
                      gl.glColorMask(true, true, true, true);
                      gl.glDepthMask(1);
                      gl.glDisable(GL_BLEND);
                  }
                  this->gl_screenfbo->bind(GL_DRAW_FRAMEBUFFER);
                  this->R_SetViewportFromWindow();
                #endif
                break;
            }
            case gfxapi::sw3d2: {
                #ifdef GL45RW_WITH_SW3D2
                auto& ctx = this->sw3d2ctx[0];
                auto* oldfbo = ctx.srActiveFBO(this->_sw3d2_window_fbo);
                //this->_sw3d2_blit_screen2win();
                dj_sw3d20::Material _conback_mat = {
                    .base_diffuse_color=dj_sw3d20::fpvec4(bgcolor),
                    .uses_transparency=false,
                    .blendtype=dj_sw3d20::enum_t::BLEND_MIX
                };
                ctx.srActiveMaterial(&_conback_mat);
                //ctx.srClearBuffers(false, true);
                bool depthtest = ctx.srIsEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST); ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, false);
                bool depthdraw = ctx.srIsEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER); ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, false);
                /*
                ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX);
                  ctx.srLoadIdentity();
                  ctx.srScalef(aspect, 1, 1);
                  ctx.srTranslatef(0.0, con_vpos, -0.006);
                ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX);
                  ctx.srLoadIdentity();
                ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX);
                  ctx.srTopMatrix() = glm::ortho<dj_sw3d20::fpscalar>(-aspect, aspect, -1.0, 1.0, 0.005, 10.0);
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_FACE_CULLING, false);
                //ctx.srClearBuffers(false, true);
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, false);
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, false);
                const dj_sw3d20::ffvertex_t _conback_mesh[] = {
                    dj_sw3d20::ffvertex_t{.co={-1.0, -0.01, -0.01}},
                    dj_sw3d20::ffvertex_t{.co={1.0, -0.01, -0.01}},
                    dj_sw3d20::ffvertex_t{.co={-1.0, 2.0, -0.01}},

                    dj_sw3d20::ffvertex_t{.co={1.0, -0.01, -0.01}},
                    dj_sw3d20::ffvertex_t{.co={1.0, 2.0, -0.01}},
                    dj_sw3d20::ffvertex_t{.co={-1.0, 2.0, -0.01}}
                };
                ctx.srDrawElements(6, _conback_mesh);
                */
                dj_sw3d20::u8rgb bgcolor_u8 = dj_sw3d20::_math::fp2u8(dj_sw3d20::fpvec3(bgcolor));
                //const glm::ivec2 cb_tl = dj_sw3d20::_math::ndc2xy(dj_sw3d20::fpvec4{-1, con_vpos_top, 0, 0}, this->_sw3d2_screen_fbo->width, this->_sw3d2_screen_fbo->height);
                //const glm::ivec2 cb_br = dj_sw3d20::_math::ndc2xy(dj_sw3d20::fpvec4{1, con_vpos, 0, 0}, this->_sw3d2_screen_fbo->width, this->_sw3d2_screen_fbo->height);

                for (int y = std::max(0, int(this->_sw3d2_window_fbo->height * (0.5 + (0.5 * con_vpos)))); y < this->_sw3d2_window_fbo->height; y++)
                {
                	std::fill_n((dj_sw3d20::u8rgb*)(this->_sw3d2_window_fbo->colorptr(0, y)), this->_sw3d2_window_fbo->width, bgcolor_u8);
                	std::fill_n(this->_sw3d2_window_fbo->depthptr(0, y), this->_sw3d2_window_fbo->width, 0.0);
                	this->_sw3d2_winfbo_drawn = true;
                }

                //this->matrices.projection.pushMatrix(); this->matrices.projection.loadIdentity();
                fmat44stack::automatrix_t autoprj1(this->matrices.projection);
                fmat44stack::automatrix_t autoview1(this->matrices.view); autoview1 = fmat44_t(1);
                fmat44stack::automatrix_t automdl1(this->matrices.model); automdl1 = fmat44_t(1);
                this->matrices.projection[-1] = glm::orthoRH(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
                //this->matrices.view.pushMatrix(); this->matrices.view.loadIdentity();
                //this->matrices.model.pushMatrix(); this->matrices.model.loadIdentity();
                this->matrices.model[-1] = fmat44_t(trs_t(
                    fvec3_t{(-1.0f), con_vpos_top, -1.03f},
                    fmat33_t(1),
                    fvec3_t(textsize, 1.0f)
                ));

                //this->R_DrawBitmapText(this->console_data.fontid, U"%cwRoses are %crred%cw\nViolets are %cbblue%cw\nThis console is %cgcool%cw,\nand so are %c0you.");

                int maxlines = cd.maxlines;
                int nlines = cd.lines.size();
                int startidx = nlines - maxlines;
                std::u32string catlines = U"%c?";
                for (int idx = startidx; idx < nlines; idx++) {
                    if (idx < 0 || idx >= nlines) {catlines.append(U"\n");}
                    else {catlines.append(cd.lines[idx]); catlines.push_back(U'\n');}
                }
                this->R_DrawBitmapText(this->console_data.fontid, catlines);
                this->matrices.model[-1] = fmat44_t(trs_t(
                    fvec3_t{-1.0f, con_vpos+textsize.y, -1.03f},
                    fmat33_t(1),
                    fvec3_t(textsize, 1.0f)
                ));
                std::u32string inpline = U"%cw>%cd";
                for (auto& chr : cd.curinput) {
                    if (chr == U'%'){inpline.append(U"%%");}
                    else {inpline.push_back(chr);}
                }
                this->R_DrawBitmapText(this->console_data.fontid, inpline);

                //this->matrices.projection.popMatrix();
                //this->matrices.view.popMatrix();
                //this->matrices.model.popMatrix();

                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, depthdraw);
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, depthtest);
                ctx.srActiveFBO(oldfbo);
                #endif
                break;
            }
            default: ;
        }
    }
    void EngineInstance::R_DrawBitmapText(const uid_t font, const std::u32string& text) {
        if (!this->has_window()){return;}
        auto* fontobj = this->get_asset<BitmapFont>(font);
        if (fontobj == nullptr){return;}
        const fmat44_t mvp = fmat44_t(this->matrices.projection) * glm::inverse(fmat44_t(this->matrices.view)) * fmat44_t(this->matrices.model);
        fvec3_t curofs = fvec3_t(0,0,0);
        const fvec4_t _defaultcolor = this->textcolors.getd(U'?', fvec4_t{0,0.5f,0,1});
        fvec4_t curcolor = _defaultcolor;
        this->R_MakeCurrent();
        switch (this->_curmode.renderer) {
            case gfxapi::opengl: {
                #ifdef __GL45RW_HAS_OPENGL
                auto& gl = *(_glstuff::curgl);
                gl.glActiveTexture(GL_TEXTURE0);
                gl.glBindTexture(GL_TEXTURE_2D, fontobj->font_texture->gltexid);
                gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                auto* glsp = this->_bitmap_text_glshader;
                float _alpha_threshold = 0.2f;
                {_glstuff::GLProgramBinding _mbind(*glsp);
                    const GLint
                        tluv_u = glsp->uniform_idx("tluv"),
                        bruv_u = glsp->uniform_idx("bruv"),
                        texsampler_u = glsp->uniform_idx("font"),
                        basecolor_u = glsp->uniform_idx("basecolor"),
                        threshold_u = glsp->uniform_idx("alpha_discard_threshold"),
                        offset_u = glsp->uniform_idx("pos_offset"),
                        mvp_u = glsp->uniform_idx("mvp"),
                        font_width_u = glsp->uniform_idx("font_width"),
                        font_height_u = glsp->uniform_idx("font_height")
                    ;
                    _mbind.setUniform(mvp_u, 1, &mvp);
                    _mbind.setUniform(threshold_u, 1, &_alpha_threshold);
                    int samplerid = 0;
                    _mbind.setUniform(texsampler_u, 1, &samplerid);
                    _mbind.setUniform(basecolor_u, 1, &curcolor);
                    #ifdef _GL45RW_GL2
                    _mbind.setUniform(font_width_u, 1, &(fontobj->font_texture->getMipLevel(0).dims.w));
                    _mbind.setUniform(font_height_u, 1, &(fontobj->font_texture->getMipLevel(0).dims.h));
                    #endif
                    for (DrawTextParser parser(text); bool(parser); curofs.x = float(parser.column)*fontobj->spacing.x, curofs.y = -float(parser.line)*fontobj->spacing.y) {
                        switch (parser.action) {
                            case DrawTextParser::action_type::setfgcolor: {
                                curcolor = this->textcolors.getd(parser.fgcolorcode, _defaultcolor);
                                _mbind.setUniform(basecolor_u, 1, &curcolor);
                                break;
                            }
                            case DrawTextParser::action_type::drawchar: {
                                _mbind.setUniform(offset_u, 1, &curofs);
                                BitmapFont::charmap_entry
                                    fallback = {},
                                    entry = fontobj->character_map.getd(parser.char2draw, fallback)
                                ;
                                _mbind.setUniform(tluv_u, 1, &entry.tluv);
                                _mbind.setUniform(bruv_u, 1, &entry.bruv);

                                #if (defined(_GL45RW_GL2))
                                    gl.glBegin(GL_TRIANGLES);
                                     gl.glTexCoord2f(0,0); gl.glVertex2f(0,0);
                                     gl.glTexCoord2f(0,1); gl.glVertex2f(0,-1);
                                     gl.glTexCoord2f(1,1); gl.glVertex2f(1,-1);

                                     gl.glTexCoord2f(1,1); gl.glVertex2f(1,-1);
                                     gl.glTexCoord2f(1,0); gl.glVertex2f(1,0);
                                     gl.glTexCoord2f(0,0); gl.glVertex2f(0,0);
                                    gl.glEnd();
                                #elif (defined(_GL45RW_GLES3))
                                    gl.glBindVertexArray(this->_gles3_attributeless_vao);
                                    gl.glDrawArrays(GL_TRIANGLES, 0, 6);
                                    gl.glBindVertexArray(0);
                                #endif

                                break;
                            }
                            default: ;
                        }
                    }

                }
                #endif
                break;
            }
            case gfxapi::sw3d2: {
                #ifdef GL45RW_WITH_SW3D2
                auto& ctx = this->sw3d2ctx[0];
                ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(this->matrices.model[-1]);
                ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(this->matrices.view[-1]);
                ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(this->matrices.projection[-1]);

                auto fontmat = (fontobj->sw3d2_font_mat);
                ctx.srActiveMaterial(&fontmat);
                fontmat.base_diffuse_color = dj_sw3d20::fpvec4(curcolor);
                for (DrawTextParser parser(text); bool(parser); curofs.x = float(parser.column)*fontobj->spacing.x, curofs.y = -float(parser.line)*fontobj->spacing.y) {
                    switch (parser.action) {
                        case DrawTextParser::action_type::setfgcolor: {
                            curcolor = this->textcolors.getd(parser.fgcolorcode, _defaultcolor);
                            fontmat.base_diffuse_color = dj_sw3d20::fpvec4(curcolor);
                            break;
                        }
                        case DrawTextParser::action_type::drawchar: {
                            BitmapFont::charmap_entry
                                fallback = {},
                                entry = fontobj->character_map.getd(parser.char2draw, fallback)
                            ;
                            const dj_sw3d20::ffvertex_t chr_rect[6] = {
                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{0,0,0}), .uv={{entry.tluv.x, entry.tluv.y}}},
                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{0,-1,0}), .uv={{entry.tluv.x, entry.bruv.y}}},
                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{1,-1,0}), .uv={{entry.bruv.x, entry.bruv.y}}},

                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{1,-1,0}), .uv={{entry.bruv.x, entry.bruv.y}}},
                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{1,0,0}), .uv={{entry.bruv.x, entry.tluv.y}}},
                            	dj_sw3d20::ffvertex_t{.co=dj_sw3d20::fpvec3(curofs+fvec3_t{0,0,0}), .uv={{entry.tluv.x, entry.tluv.y}}}
                            };

                            ctx.srDrawElements(6, chr_rect);
                            break;
                        }
                        default: ;
                    }
                }
                #endif
            	break;
            }
            default: ;
        }
    }

    enum bumpmapspace : int {
        object = 0,
        tangent = 1
    };
    enum struct textype : int {
        image = 0,
        envmap = 1
    };

    enum struct envmaptype : int {
        equirect = 0,
        cubemap = 1
    };

    enum struct cubemap_layout : int {
        blender = 0
    };
    enum struct texblendtype : int {
        mix = 0,
        add = 1,
        mul = 2
    };

    enum struct matblendtype : int {
        opaque = 0,
        alpha_blend = 1,
        add = 2
    };

    enum struct texmaptype : int {
        uvmap = 0,
        reflection = 1,
        view = 2
    };

    enum struct mat_diffuse_model : int {
        none = 0,
        lambert = 1
    };

    enum struct mat_specular_model : int {
        none = 0,
        phong = 1
    };
    struct fxpointer_t {
        bool has_fx = false;
        uint32_t fxcat = 0, fxnum = 0;
        union {
            int _dummy;
            union {
                struct {
                    fscalar_t linv[3];
                } c0n10;
                struct {
                    fscalar_t linv0[3], linv1[3], linv2[3], factors[3];
                    int blendtypes[3];
                } c0n11;
            } texture;
        } fxargs;
    };
    struct matprops_t {
        template <class A=fscalar_t>
        struct _mataffect {
            bool use;
            A amount;
        };
        struct _textureslot {
            bool use = false;
            textype type = textype::image;
            struct {
                _mataffect<fvec3_t>
                    diffuse_color,
                    specular_color,
                    texcoord_offset,
                    normal,
                    ambient_color
                ;
                _mataffect<fscalar_t>
                    diffuse_alpha,
                    specular_alpha,
                    diffuse_intensity,
                    specular_intensity,
                    specular_hardness,
                    emit_intensity,
                    ambient_alpha
                ;
            } affects = {};

            texmaptype maptype = texmaptype::uvmap;

            size_t uvmapno = 0;
            uid_t texture = 0;

            texfilter
                min_filter = texfilter::nearest,
                mag_filter = texfilter::nearest
            ;
            bumpmapspace bump_space = bumpmapspace::object;
            texblendtype blendtype = texblendtype::mix;
            fxpointer_t texfx = {};
            envmaptype envtype = envmaptype::equirect;

            bool use_rgb2intensity = false;
            fvec3_t rgb2intensity_color = {1,1,1};

        };

        struct {
            mat_diffuse_model model;
            fvec3_t color;
            fscalar_t alpha, intensity;
        } diffuse = {mat_diffuse_model::none, fvec3_t(0.8), 1, 1};

        struct {
            mat_specular_model model;
            fvec3_t color;
            fscalar_t alpha, intensity, hardness;
        } specular = {mat_specular_model::none, fvec3_t(0.8), 1, 1, 16};

        _mataffect<fvec4_t>
            vertex_color_light = {false, fvec4_t(1)},
            vertex_color_paint = {false, fvec4_t(1)}
        ;

        matblendtype blendtype = matblendtype::opaque;

        _textureslot texture_slots[8] = {};
        cullface face_culling_mode = cullface::disabled;
        bool shadeless = true;
        fscalar_t emit_intensity = 0.0f;
        struct {
            fvec3_t color;
            fscalar_t alpha;
        } ambient = {fvec3_t(0), fscalar_t(1)};
    };
    class Material : public AssetBase {
        using AssetBase::AssetBase;
        private:
            #ifdef GL45RW_WITH_SW3D2
            template <class D, class S>
            static void _texaffect_to_sw3d2(dj_sw3d20::_MaterialTexAffect<D>& dst, const matprops_t::_mataffect<S>& src){
				dst.use = src.use;
				dst.amount = D(src.amount);
			}
            #endif
        public:
            #ifdef __GL45RW_HAS_OPENGL
                _glstuff::GLShaderProgram* _glsp = nullptr;
                struct gl_light_uniforms {
					GLint
					    lighttype,
					    lightfalloff,
					    diffuse_color,
					    diffuse_intensity,
					    specular_color,
					    specular_intensity,
					    energy,
					    radius,
					    cone_angle,
					    pos,
					    orn
					;
				};
                struct {
                    GLint
                        model_matrix,
                        view_matrix,
                        view_matrix_inv,
                        projection_matrix,
                        projection_matrix_inv,
                        time,
                        num_active_lights
                    ;
                    vectorlist<gl_light_uniforms> lights = {};
                } _glsl_uniform_locations;
                vectorlist<GLint> _active_texture_samplers = {};
            #endif
            #ifdef GL45RW_WITH_SW3D2
                dj_sw3d20::Material _sw3d2mat = {};
            #endif
            #ifdef GL45RW_WITH_SW3D3
                _SW3D3_DrawMesh_ShaderData _sw3d3shdat = {};

            #endif
            //vectorlist<int> _active_texture_slots = {}, _active_uvtexture_slots = {};
            struct processed_texslot_t {
                int texslotidx = 0;
                int sampler_unit = 0;
                matprops_t::_textureslot *mts = nullptr;
                int uvmapno = 0;
                uid_t texture = 0;
                std::string
                    sampler_uniform_name = "",
                    varying_uv_name = ""
                ;
                #ifdef __GL45RW_HAS_OPENGL
                    GLuint gltexid = 0;
                    GLint glsamplerloc = -1;
                #endif

            };
            vectorlist<processed_texslot_t> active_textures = {};
        private:
            bool _has_shader = false;
            //uid_t _sgmodel_managed_idx = 0, _managing_sgmodel_id = 0;
            void _destroy_shader() {
                //if (!this->instance->R_HasActiveRenderer()){return;}
                if (!this->_has_shader){return;}
                this->instance->R_MakeCurrent();
                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                            delete this->_glsp;
                            this->_glsp = nullptr;
                            this->_active_texture_samplers.clear();
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            //this->_sw3d3shdat.mp = nullptr;
                            //this->_sw3d3shdat.ei = nullptr;
                            this->_sw3d3shdat = _SW3D3_DrawMesh_ShaderData{};
                        #endif
                        break;
                    }
                    default: break;
                }
                //this->instance->opaque_buckets.del_matid(uid_t(*this));
                //this->instance->translucent_buckets.del_matid(uid_t(*this));
                this->instance->R_EnsureAllGroupsRemovedMaterialID(uid_t(*this));
                this->active_textures.clear();
                this->_has_shader = false;
            }
            template <glm::length_t L>
            inline void _vec2glsl(std::ostream& ostr, const glm::vec<L,fscalar_t>& v) const {
                bool first = true;
                ostr << "vec" << L << "(";
                for (size_t i = 0; i < L; i++){
                    if (!first){ostr << ',';}
                    first = false;
                    ostr << v[i];
                }
                ostr << ')';
            }
            #ifdef GL45RW_WITH_OPENGL
            template <glm::length_t L>
            inline void _vec2glsl(_glstuff::GLSLCodeStream& ostr, const glm::vec<L,fscalar_t>& v) const {
                ostr << v;
            }
            #endif
            void _build_shader() {
                std::cout << "called _build_shader()\n";
                //if (!this->instance->R_HasActiveRenderer()){return;}
                this->instance->R_MakeCurrent();
                this->_destroy_shader();
                bool has_mirrorball_textures = false;
                this->active_textures.clear();
                for (int i = 0; i < 8; i++) {
                    processed_texslot_t pts = {.texslotidx=i};

                    auto& mts = this->data.texture_slots[i];
                    if (!mts.use || mts.texture == 0){continue;}
                    //if (!mts.affects.diffuse_color.use && !mts.affects.texcoord_offset.use){continue;}

                    std::cout << "process texslot " << i << '\n';

                    pts.sampler_unit = i;
                    pts.texture = mts.texture;
                    //pts._mts = mts;
                    pts.mts = this->data.texture_slots+i;
                    pts.uvmapno = mts.uvmapno;
                    pts.sampler_uniform_name = "tex"+std::to_string(pts.sampler_unit);
                    //pts.varying_uv_name = "gl_TexCoord["+std::to_string(pts.uvmapno)+"]";
                    pts.varying_uv_name = "v_texcoord"+std::to_string(pts.uvmapno);
                    if (mts.maptype == texmaptype::reflection && mts.type == textype::image){has_mirrorball_textures = true;}
                    std::cout << "adding pts..\n";
                    this->active_textures.push_back(pts);
                }

                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            this->_sw3d3shdat.mp = &(this->data);
                            this->_sw3d3shdat.ei = this->instance;

                            for (int i = 0; i < 8 && i < this->active_textures.size(); i++){
                                auto& sts = this->_sw3d3shdat.texslots[i];
                                auto& atex = this->active_textures[i];
                                sts.use = true;
                                auto* gtex = this->instance->get_asset<Texture2D>(atex.texture);
                                sts.tex = gtex->sw3d3tex;
                                sts.texalt = gtex;
                                sts.sampler_unit = atex.sampler_unit;
                            }

                        #endif
                        break;
                    }
                    case gfxapi::opengl: {
                        #ifdef __GL45RW_HAS_OPENGL
                            std::cout << "_build_shader(): OpenGL mode\n";
                            if (_glstuff::curgl == nullptr || _glstuff::curgl->glCompileShader == nullptr){
                                std::cout << "_build_shader(): curgl is null!\n";
                                break;
                            }
                            auto& gl = *(_glstuff::curgl);
                            #if (defined(_GL45RW_GL2))
                            int maxlights = GL45RW_GL2_MAXLIGHTS;
                            #elif (defined(_GL45RW_GLES3))
                            int maxlights = GL45RW_GLES3_MAXFWDLIGHTS;
                            #endif
                            this->_glsp = new _glstuff::GLShaderProgram(gl.glCreateProgram(), true);
                            //std::stringstream csrc, vsrc, fsrc;
                            _glstuff::GLSLCodeGenerator msgen; msgen.commonsrc.header();

                            msgen.uniform_def("float", "time");
                            msgen.uniform_def("mat4", "model_matrix");
                            msgen.uniform_def("mat4", "view_matrix");
                            msgen.uniform_def("mat4", "view_matrix_inv");
                            msgen.uniform_def("mat4", "projection_matrix");
                            msgen.uniform_def("mat4", "projection_matrix_inv");

                            msgen.frag_header();


                            auto& csrc = msgen.commonsrc;
                            auto& vsrc = msgen.vertexsrc;
                            auto& fsrc = msgen.fragsrc;
                            csrc << R"""(

#define BUMPMAPSPACE_OBJECT 0
#define BUMPMAPSPACE_TANGENT 1

)""";

                                if (!this->data.shadeless){
									csrc << R"""(
#define LIGHTTYPE_POINT 0
#define LIGHTTYPE_SPOT 1
#define LIGHTTYPE_AMBIENT 2
#define LIGHTTYPE_HEMI 3

#define LIGHTFALLOFF_CONSTANT 0
#define LIGHTFALLOFF_LINEAR 1
#define LIGHTFALLOFF_SQUARE 2

struct gl45rw_LightSource {
    int type;
    int falloff;
    vec4 diffuse_color;
    vec4 specular_color;
    float diffuse_intensity;
    float specular_intensity;
    float energy;
    float radius;
    float cone_angle;
    vec3 pos;
    mat3 orn;
};
)""";
                                 msgen.uniform_def("gl45rw_LightSource", "light_sources", maxlights, 11);
                                 msgen.uniform_def("int", "num_active_lights");
     /*
     << "uniform gl45rw_LightSource light_sources[" << maxlights << "];\n"
     << "uniform int num_active_lights;\n\n";
     */
								}
                                for (auto& atex : this->active_textures) {
                                    std::string sampler_type = "sampler2D";
                                    if (atex.mts->type == textype::envmap && atex.mts->envtype == envmaptype::cubemap) {
                                        sampler_type = "samplerCube";
                                    }
                                    //csrc << "uniform " << sampler_type << " " << atex.sampler_uniform_name << ";\n";
                                    msgen.uniform_def(sampler_type, atex.sampler_uniform_name);

                                }

                                msgen.varying_def("vec3", "v_viewpos");
                                msgen.varying_def("vec3", "v_worldpos");
                                msgen.varying_def("vec3", "v_viewnormal");
                                msgen.varying_def("vec3", "v_worldnormal");
                                msgen.varying_def("vec3", "v_modelnormal");
                                msgen.varying_def("vec4", "v_diffuse");
                                msgen.varying_def("vec4", "v_clippos");
                                msgen.varying_def("vec3", "v_camdir_view");
                                msgen.varying_def("vec3", "v_camdir_world");

                                msgen.varying_def("vec2", "v_texcoord0");
                                msgen.varying_def("vec2", "v_texcoord1");
                                msgen.varying_def("vec2", "v_texcoord2");
                                msgen.varying_def("vec2", "v_texcoord3");
                                msgen.varying_def("vec2", "v_texcoord4");
                                msgen.varying_def("vec2", "v_texcoord5");
                                msgen.varying_def("vec2", "v_texcoord6");
                                msgen.varying_def("vec2", "v_texcoord7");

                                #ifdef _GL45RW_GLES3

                                msgen.attrib_def("vec3", "vao_co");
                                msgen.attrib_def("vec3", "vao_normal");
                                msgen.attrib_def("vec4", "vao_color");

                                msgen.attrib_def("vec2", "vao_uv0");
                                msgen.attrib_def("vec2", "vao_uv1");
                                msgen.attrib_def("vec2", "vao_uv2");
                                msgen.attrib_def("vec2", "vao_uv3");
                                msgen.attrib_def("vec2", "vao_uv4");
                                msgen.attrib_def("vec2", "vao_uv5");
                                msgen.attrib_def("vec2", "vao_uv6");
                                msgen.attrib_def("vec2", "vao_uv7");

                                #endif

                                csrc << R"""(

//utility functions
//from the original gl45

#define PI 3.14159

int longest_vec3_axis(vec3 v)
{
    int la = 0;
    if (abs(v.y) > abs(v[la])){la = 1;}
    if (abs(v.z) > abs(v[la])){la = 2;}
    return la;
}

#define CUBEFACE_NEGX 0 //left
#define CUBEFACE_POSX 1 //right
#define CUBEFACE_NEGY 2 //back
#define CUBEFACE_POSY 3 //front
#define CUBEFACE_NEGZ 4 //bottom
#define CUBEFACE_POSZ 5 //top


int vec2cubeface(vec3 v)
{
    int vla = longest_vec3_axis(v), _add = 0;
    if (vla == 0){_add = (v.x >= 0.0f) ? 1 : 0;}
    else if (vla == 1){_add = (v.y >= 0.0f) ? 1 : 0;}
    else if (vla == 2){_add = (v.z >= 0.0f) ? 1 : 0;}
    return (2*vla)+_add;
}

void vec2cubemap(in vec3 v, out int face, out vec2 uvloc)
{
    face = vec2cubeface(v);
    vec3 nv = normalize(v)*vec3(1.41421356237);
    if (face == CUBEFACE_NEGX){uvloc = (nv.yz*vec2(0.5))+vec2(0.5);}
    else if (face == CUBEFACE_POSX){uvloc = vec2(1.0-((nv.y*0.5)+0.5), (nv.z*0.5)+0.5);}
    else if (face == CUBEFACE_NEGY){uvloc = vec2(1.0-((nv.x*0.5)+0.5), (nv.z*0.5)+0.5);}
    else if (face == CUBEFACE_POSY){uvloc = (nv.xz*vec2(0.5))+vec2(0.5);}
    else if (face == CUBEFACE_NEGZ){uvloc = (nv.xy*vec2(0.5))+vec2(0.5);}
    else if (face == CUBEFACE_POSZ){uvloc = vec2((nv.x*0.5)+0.5, 1.0-((nv.y*0.5)+0.5));}
}

float altrange(in float v, in float zero, in float one){return (v-zero)/(one-zero);}


// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated seeding method.

#define PHI 1.61803398874989484820459  // Φ = Golden Ratio

float gold_noise(in vec2 xy, in float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

float frand(in vec2 iseed, out vec2 oseed) // it's an LCG!
{
    float v = gold_noise(iseed, fract(time));
    oseed = iseed*vec2(v);
    return v;
}

vec3 hsv2rgb(vec3 c) //https://github.com/hughsk/glsl-hsv2rgb/blob/master/index.glsl
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec2 equirect2uv(in vec3 er)
{
    float vx, vy, vz, dx, dy, dz;
    vec3 _n3 = normalize(er);
    dx = vx = _n3.x;
    dz = vy = _n3.y;
    vz = _n3.z;
    dy = -vz;
    return vec2(0.5+(atan(dx, dz)/(2.0*PI)), 0.5-(asin(dy)/PI));
}

vec3 apply_trs(in vec3 p, in vec3 t, in mat3 r, in vec3 s){return t+(r*(p*s));}


#define BLENDTYPE_mix 0
#define BLENDTYPE_add 1
#define BLENDTYPE_mul 2

vec4 blendcolors(int blendtype, vec4 src, vec4 dst, vec4 fac) {
    if (blendtype == BLENDTYPE_add) {return src+(dst*fac);}
    else if (blendtype == BLENDTYPE_mul) {return src*(dst*fac);}
    return mix(src, dst, fac);
}

vec3 blendcolors(int blendtype, vec3 src, vec3 dst, vec3 fac) {
    if (blendtype == BLENDTYPE_add) {return src+(dst*fac);}
    else if (blendtype == BLENDTYPE_mul) {return src*(dst*fac);}
    return mix(src, dst, fac);
}

float blendscalars(int blendtype, float src, float dst, float fac) {
    if (blendtype == BLENDTYPE_add) {return src+(dst*fac);}
    else if (blendtype == BLENDTYPE_mul) {return src*(dst*fac);}
    return mix(src, dst, fac);
}

vec2 calc_mirrorball_uv(vec3 view, vec3 normal) {
    vec3 r = reflect(view, -normal);
    return vec2(
        0.5 + (0.4*r.x),
        0.5 + (0.4*r.y)
    );
}

vec3 cubemap_rh2lh(in vec3 rh) {
    return rh * vec3(1,1,1);
}

float rgb2gray(in vec3 rgb) {
    return dot(rgb, vec3(0.2989, 0.5870, 0.1140));
}

vec4 texfx_0_11_sample2d(in sampler2D tex, in vec2 base_uv, in vec2 linvs[3], in int blendtypes[3], in float factors[3]) {
    vec4 texel = vec4(0);
    for (int i = 0; i < 3; i++) {
        texel = blendcolors(blendtypes[i], texel, texture2D(tex, base_uv + (linvs[i] * time)), vec4(factors[i]));
    }
    return texel;
}

)""";


                                vsrc << R"""(

void main() {
    mat4 vm = view_matrix_inv * model_matrix;
    mat4 pvm = projection_matrix * vm;
    vec3 gvertex = )""" <<
    #if (defined(_GL45RW_GL2))
    "gl_Vertex.xyz;\n"
    #elif (defined(_GL45RW_GLES3))
    "vao_co.xyz;\n"
    #endif
    << "\n" <<
"    vec3 mat_diffuse_color = " << this->data.diffuse.color << ";\n" <<
"    float mat_diffuse_alpha = float(" << this->data.diffuse.alpha << ");\n"
    << R"""(
    v_viewpos = vec3(view_matrix_inv * model_matrix * vec4(gvertex, 1.0));
    v_worldpos = vec3(model_matrix * vec4(gvertex, 1.0));


    v_diffuse = vec4(mat_diffuse_color )""";
                                if (this->data.vertex_color_light.use){
                                    #if (defined(_GL45RW_GL2))
                                    vsrc << " * gl_Color.xyz";
                                    #elif (defined(_GL45RW_GLES3))
                                    vsrc << " * vao_color.xyz";
                                    #endif
                                }
                                vsrc << ", mat_diffuse_alpha);\n    vec3 vmnormal = " <<
                                #if (defined(_GL45RW_GL2))
                                "gl_Normal.xyz;\n"
                                #elif (defined(_GL45RW_GLES3))
                                "vao_normal.xyz;\n"
                                #endif
                                << R"""(
    v_modelnormal = normalize(vmnormal);
    vec3 vwnorm = vec3(model_matrix * vec4(vmnormal, 0.0)); v_worldnormal = vwnorm;
    v_viewnormal = vec3(view_matrix_inv * model_matrix * vec4(vmnormal, 0.0));

    v_clippos = pvm * vec4(gvertex, 1.0);

)""" <<
    #if (defined(_GL45RW_GL2))
    R"(
    v_texcoord0 = gl_MultiTexCoord0.st;
    v_texcoord1 = gl_MultiTexCoord1.st;
    v_texcoord2 = gl_MultiTexCoord2.st;
    v_texcoord3 = gl_MultiTexCoord3.st;
    v_texcoord4 = gl_MultiTexCoord4.st;
    v_texcoord5 = gl_MultiTexCoord5.st;
    v_texcoord6 = gl_MultiTexCoord6.st;
    v_texcoord7 = gl_MultiTexCoord7.st;
)"
    #elif (defined(_GL45RW_GLES3))
    R"(
    v_texcoord0 = vao_uv0;
    v_texcoord1 = vao_uv1;
    v_texcoord2 = vao_uv2;
    v_texcoord3 = vao_uv3;
    v_texcoord4 = vao_uv4;
    v_texcoord5 = vao_uv5;
    v_texcoord6 = vao_uv6;
    v_texcoord7 = vao_uv7;
)"
    #endif
R"""(
    gl_Position = v_clippos;
}
)""";

                                fsrc << R"""(

float calc_light_factor(in int falloff, in float d, in float radius_min, in float radius_max) {
	if (falloff == 0){return (d <= radius_max ? 1.0 : 0.0);}
	float basefac = clamp((d - radius_min) / (radius_max - radius_min), 0.0, 1.0);
	float factor = 0.0;
	if (falloff == 2){
		factor = pow(1.0 - (basefac), 2.0);
	}
	else {
	    factor = 1.0 - basefac;
	}
	return factor;
}

vec4 calc_rgb2intensity(in vec4 ftexel, in vec3 color){
    vec3 quot = clamp(ftexel.rgb / color, vec3(0), vec3(1));
    return vec4(1) * rgb2gray(quot);
}

void main() {
    bool is_frontface = gl_FrontFacing;
    float frontface_sign = (-1.0) + (2.0 * float(is_frontface));
    vec2 frseed = gl_FragCoord.xy;
    float noise1 = frand(frseed, frseed);

    vec3 f_viewnormal = normalize(v_viewnormal * frontface_sign);
    vec3 f_worldnormal = normalize(v_worldnormal * frontface_sign);

    //vec3 f_viewnormal_flat = normalize(cross(dFdx(v_viewpos), dFdy(v_viewpos)));
    //vec3 f_worldnormal_flat = normalize(cross(dFdx(v_worldpos), dFdy(v_worldpos)));

    vec3 dposx = dFdx(v_worldpos);
    vec3 dposy = dFdy(v_worldpos);

    vec3 tc_offset = vec3(0);
    vec3 ltcoffs = vec3(0);
    vec3 rel_diffuse_color = vec3(0);
    vec3 diffuse_color = v_diffuse.rgb;
    vec3 specular_color = )""" << this->data.specular.color << R"""(;
    vec3 ambient_color = )""" << this->data.ambient.color << R"""(;
    vec3 rel_specular_color = vec3(0);
    float diffuse_intensity = )""" << "float(" << this->data.diffuse.intensity << ");" << R"""(
    float specular_intensity = )""" << "float(" << this->data.specular.intensity << ");" << R"""(
    float specular_hardness = )""" << "float(" << this->data.specular.hardness << ");" << R"""(
    /*
    vec3 objspace_x_vector = normalize(vec3(model_matrix * vec4(1,0,0,0)));
    vec3 objspace_y_vector = normalize(vec3(model_matrix * vec4(0,1,0,0)));
    vec3 objspace_z_vector = normalize(vec3(model_matrix * vec4(0,0,1,0)));
    */
    float diffuse_alpha = v_diffuse.a;
    vec3 tgnormal;
    vec4 fragcolor = vec4(0);
    vec4 ftexel = vec4(0);

    mat3 tbn;
    vec2 duvx;
    vec2 duvy;
)""";
                                if (has_mirrorball_textures) {
                                    //fsrc << "    vec3 mbreflvec = normalize(reflect(normalize(v_worldpos - view_matrxix[3].xyz), normalize(v_worldnormal)));\n";
                                    //fsrc << "    vec2 mbrefluv = vec2(0.5 + (0.25 * mbreflvec.x), 0.5 + (0.25 * mbreflvec.y));\n";
                                    fsrc << "    vec2 mbrefluv = calc_mirrorball_uv(normalize(v_viewpos), f_viewnormal);\n";
                                }
                                fsrc << "    vec3 vvec_w = normalize(v_worldpos - vec3(view_matrix[3]));\n";
                                fsrc << "    vec3 vrefl_w = reflect(vvec_w, f_worldnormal);\n";
                                for (auto& atex : this->active_textures) {
                                      fsrc << "    ltcoffs = tc_offset;\n";
                                      bool has_fx_0_11 = false;

                                      std::unordered_map<std::string, std::string> tfx0_11_arrays = {
                                          {"linvs", "vec2[3](vec2(0), vec2(0), vec2(0))"},
                                          {"factors", "float[3](0.3, 0.3, 0.3)"},
                                          {"blendtypes", "int[3](1, 1, 1)"}
                                      };
                                      if (atex.mts->texfx.has_fx) {
                                          auto texfx = atex.mts->texfx;
                                          if (texfx.fxcat == 0 && texfx.fxnum == 10) {
                                              const float* _linv = texfx.fxargs.texture.c0n10.linv;
                                              fsrc << "    ltcoffs += (vec3(" << _linv[0] << "," << _linv[1] << "," << _linv[2] << ")*time);\n";
                                          }
                                          else if (texfx.fxcat == 0 && texfx.fxnum == 11) {
                                              has_fx_0_11 = true;
                                              const auto& c0n11 = texfx.fxargs.texture.c0n11;
                                              for (auto& ssp : tfx0_11_arrays) {
                                                  std::stringstream ass;
                                                  if (ssp.first == "linvs") {
                                                      ass << "vec2[3](";
                                                      ass << "vec2(" << c0n11.linv0[0] << ',' << c0n11.linv0[1] << "),";
                                                      ass << "vec2(" << c0n11.linv1[0] << ',' << c0n11.linv1[1] << "),";
                                                      ass << "vec2(" << c0n11.linv2[0] << ',' << c0n11.linv2[1] << ")";
                                                      ass << ")";
                                                  }
                                                  else if (ssp.first == "factors") {
                                                      ass << "float[3](" << c0n11.factors[0] << ',' << c0n11.factors[1] << ',' << c0n11.factors[2] << ")";
                                                  }
                                                  else if (ssp.first == "blendtypes") {
                                                      ass << "int[3](" << c0n11.blendtypes[0] << ',' << c0n11.blendtypes[1] << ',' << c0n11.blendtypes[2] << ")";
                                                  }
                                                  ssp.second = ass.str();
                                              }
                                          }
                                      }
                                      switch (atex.mts->type) {
                                          case textype::image: {
                                              fsrc << "    ftexel = ";
                                              //Texture2D* tex = this->instance->get_asset<Texture2D>(atex.texture);
                                              switch (atex.mts->maptype) {
                                                  case texmaptype::uvmap: {
                                                      if (has_fx_0_11) {
                                                          fsrc << "texfx_0_11_sample2d("
                                                               << atex.sampler_uniform_name << ','
                                                               << atex.varying_uv_name << ".st + ltcoffs.st,"
                                                               << tfx0_11_arrays["linvs"] << ','
                                                               << tfx0_11_arrays["blendtypes"] << ','
                                                               << tfx0_11_arrays["factors"]
                                                               << ");\n"
                                                          ;
                                                      }
                                                      else {
                                                          fsrc << "texture2D("
                                                               << atex.sampler_uniform_name << ','
                                                               << atex.varying_uv_name << ".st + ltcoffs.st);\n"
                                                          ;
                                                      }
                                                      break;
                                                  }
                                                  case texmaptype::reflection: {
                                                      fsrc << "texture2D("
                                                           << atex.sampler_uniform_name << ','
                                                           << "calc_mirrorball_uv(normalize(v_viewpos), f_viewnormal) + ltcoffs.st);\n";
                                                      break;
                                                  }
                                                  default: fsrc << "vec4(0);\n";
                                              }
                                              break;
                                          }
                                          case textype::envmap: {
                                              switch (atex.mts->envtype){
                                                  case envmaptype::equirect: {
                                                      switch (atex.mts->maptype){
                                                          case texmaptype::reflection: {
                                                              fsrc << "ftexel = texture2D("
                                                                   << atex.sampler_uniform_name << ","
                                                                   << "equirect2uv(vrefl_w) + ltcoffs.st);\n"
                                                              ;
                                                              break;
                                                          }
                                                          case texmaptype::view: {
                                                              fsrc << "ftexel = texture2D("
                                                                   << atex.sampler_uniform_name << ","
                                                                   << "equirect2uv(vvec_w) + ltcoffs.st);\n"
                                                              ;
                                                              break;
                                                          }
                                                          default: fsrc << "ftexel = vec4(0);\n";
                                                      }
                                                      break;
                                                  }
                                                  case envmaptype::cubemap: {
                                                      switch (atex.mts->maptype) {
                                                          case texmaptype::reflection: {
                                                              fsrc << "ftexel = textureCube("
                                                                   << atex.sampler_uniform_name << ","
                                                                   << "cubemap_rh2lh(normalize(vrefl_w + ltcoffs)));\n"
                                                              ;
                                                              break;
                                                          }
                                                          case texmaptype::view: {
                                                              fsrc << "ftexel = textureCube("
                                                                   << atex.sampler_uniform_name << ","
                                                                   << "cubemap_rh2lh(normalize(vvec_w + ltcoffs)));\n"
                                                              ;
                                                              break;
                                                          }
                                                          default: fsrc << "ftexel = vec4(0);\n";
                                                      }
                                                      break;
                                                  }
                                                  default: fsrc << "ftexel = vec4(0);\n";
                                              }
                                              break;
                                          }
                                          default: fsrc << "ftexel = vec4(0);\n";
                                      }

                                      if (atex.mts->use_rgb2intensity){
                                          fsrc << "    ftexel = calc_rgb2intensity(ftexel, " << atex.mts->rgb2intensity_color << ");\n";
                                      }



                                      if (atex.mts->affects.diffuse_color.use) {
                                          fsrc << "    diffuse_color = blendcolors("
                                               << int(atex.mts->blendtype)
                                               << ", diffuse_color, ftexel.rgb, ";
                                          this->_vec2glsl(fsrc, atex.mts->affects.diffuse_color.amount);
                                          fsrc << ");\n";
                                      }
                                      if (atex.mts->affects.specular_color.use) {
                                          fsrc << "    specular_color = blendcolors("
                                               << int(atex.mts->blendtype)
                                               << ", specular_color, ftexel.rgb, ";
                                          this->_vec2glsl(fsrc, atex.mts->affects.specular_color.amount);
                                          fsrc << ");\n";
                                      }
                                      if (atex.mts->affects.ambient_color.use) {
                                          fsrc << "    ambient_color = blendcolors("
                                               << int(atex.mts->blendtype)
                                               << ", ambient_color, ftexel.rgb, ";
                                          this->_vec2glsl(fsrc, atex.mts->affects.ambient_color.amount);
                                          fsrc << ");\n";
                                      }
                                      if (atex.mts->affects.diffuse_intensity.use) {
                                          fsrc << "    diffuse_intensity = blendscalars("
                                               << int(atex.mts->blendtype)
                                               << ", diffuse_intensity, rgb2gray(ftexel.rgb), float(";
                                          fsrc << atex.mts->affects.diffuse_intensity.amount;
                                          fsrc << "));\n";
                                      }
                                      if (atex.mts->affects.specular_intensity.use) {
                                          fsrc << "    specular_intensity = blendscalars("
                                               << int(atex.mts->blendtype)
                                               << ", specular_intensity, rgb2gray(ftexel.rgb), float(";
                                          fsrc << atex.mts->affects.specular_intensity.amount;
                                          fsrc << "));\n";
                                      }
                                      if (atex.mts->affects.specular_hardness.use) {
                                          fsrc << "    specular_hardness = blendscalars("
                                               << int(atex.mts->blendtype)
                                               << ", specular_hardness, 511.0 * rgb2gray(ftexel.rgb), float(";
                                          fsrc << atex.mts->affects.specular_hardness.amount;
                                          fsrc << "));\n";
                                      }
                                      if (atex.mts->affects.texcoord_offset.use) {
                                          fsrc << "    tc_offset = blendcolors("
                                               << int(atex.mts->blendtype)
                                               << ", tc_offset.xyz, mix(vec3(-1), vec3(1), ftexel.rgb), ";
                                          this->_vec2glsl(fsrc, atex.mts->affects.texcoord_offset.amount);
                                          fsrc << ");\n";
                                      }
                                      if (atex.mts->affects.diffuse_alpha.use) {
                                          fsrc << "    diffuse_alpha = blendscalars("
                                               << int(atex.mts->blendtype)
                                               << ", diffuse_alpha, ftexel.a, float("
                                               << atex.mts->affects.diffuse_alpha.amount
                                               << "));\n"
                                          ;
                                      }
                                      if (atex.mts->affects.normal.use) {
                                          if (atex.mts->bump_space == bumpmapspace::object){
                                              fsrc << "    tgnormal = normalize(vec3(model_matrix * vec4(mix(vec3(-1), vec3(1), ftexel.xyz), 0)));\n";
                                          }
                                          else if (atex.mts->bump_space == bumpmapspace::tangent){
                                              fsrc << "    duvx = dFdx(v_texcoord" << atex.mts->uvmapno << ".st);\n";
                                              fsrc << "    duvy = dFdy(v_texcoord" << atex.mts->uvmapno << ".st);\n";

                                              fsrc << "    tbn = mat3(normalize(dposx*duvy.y - dposy*duvx.y), -normalize(-dposx*duvy.x + dposy*duvx.x), f_worldnormal);\n";

                                              fsrc << "    tgnormal = normalize(tbn * mix(vec3(-1), vec3(1), ftexel.xyz));\n";
                                          }
                                          fsrc << "    f_worldnormal = normalize(blendcolors(" << int(atex.mts->blendtype) << ", f_worldnormal, tgnormal, "; this->_vec2glsl(fsrc, atex.mts->affects.normal.amount); fsrc << "));\n";
                                          fsrc << "    f_viewnormal = normalize(vec3(view_matrix_inv * vec4(f_worldnormal, 0)));\n";
                                          fsrc << "    vrefl_w = reflect(vvec_w, f_worldnormal);\n";
                                      }
                                }
                                if (this->data.blendtype != matblendtype::opaque){
									fsrc << "    if (diffuse_alpha <= 0.001){discard;}\n";
								}
								fsrc << "    fragcolor.rgb = diffuse_color; fragcolor.a = diffuse_alpha;\n";
								if (!this->data.shadeless){
								    fsrc << R"""(
    //bool frontface = dot(vvec_w, normalize(v_worldnormal)) <= 0.0;
    //bool frontface = gl_FrontFacing;
    //bool frontface = true;
    //rel_diffuse_color = ambient_color;
    float lightshine = 0.0;
    for (int lightnum = 0; lightnum < num_active_lights; lightnum++){
        gl45rw_LightSource light = light_sources[lightnum];
        float ldistance = 0.0;
        float lfactor = 0.0;
        float ldot;
        //float frontfacemul = (-1.0 + (2.0 * float(frontface)));
        vec3 lvector = vec3(0);
        vec3 ldir = normalize(light.orn * vec3(0,0,-1));
        vec3 fragnormw = f_worldnormal;
        bool do_spec = true;
        if (light.type == LIGHTTYPE_POINT){
            ldistance = length(v_worldpos - light.pos);
            if (ldistance >= light.radius){continue;}
            lvector = normalize(light.pos - v_worldpos);
            ldot = dot(lvector, fragnormw);
            if (ldot <= 0.0){continue;}
            lfactor = ldot * calc_light_factor(light.falloff, ldistance, 0.0, light.radius);
	    }
	    else if (light.type == LIGHTTYPE_SPOT){
	        vec3 ldelta = light.pos - v_worldpos;
	        lvector = normalize(ldelta);
	        ldot = dot(lvector, fragnormw);
	        float spotdot = dot(-lvector, ldir);
	        float lvecangle = degrees(acos(spotdot));
	        if (ldot <= 0.0 || spotdot <= 0.0 || lvecangle > (light.cone_angle * 0.5)){continue;}
	        ldistance = dot(ldir, -ldelta);
	        if (ldistance < 0.0 || ldistance >= light.radius){continue;}
	        lfactor = max(ldot * (1.0 - (lvecangle / (light.cone_angle*0.5))), 0.0) * calc_light_factor(light.falloff, ldistance, 0.0, light.radius);
	    }
	    else if (light.type == LIGHTTYPE_HEMI){
            lvector = -ldir;
            lfactor = max(mix(0.5, 1.0, dot(lvector, fragnormw)), 0.0);
	    }
	    else if (light.type == LIGHTTYPE_AMBIENT){
            lfactor = 1.0;
            lvector = fragnormw;
            do_spec = false;
	    }
	    else {continue;}
	    if (lfactor <= 0.0){continue;}
	    float _ldfac = max(diffuse_intensity * lfactor * (light.energy * 0.1) * light.diffuse_intensity, 0.0);
	    float _lsfac = (do_spec ? max(specular_intensity * lfactor * (light.energy * 0.1) * pow(clamp(dot(vrefl_w, lvector), 0.0, 1.0), specular_hardness), 0.0) : 0.0);
	    rel_diffuse_color += (light.diffuse_color.rgb * _ldfac);
	    rel_specular_color += (light.specular_color.rgb * _lsfac);
	    lightshine = max(lightshine + lfactor, 0.0);
    }
    //diffuse_color *= rel_diffuse_color;
    diffuse_color = mix(ambient_color, rel_diffuse_color*diffuse_color, vec3(lightshine));
    specular_color *= rel_specular_color;
    fragcolor.rgb = diffuse_color + specular_color;
)""";
							    }
                                //fsrc << "    gl_FragColor = fragcolor;\n}\n";
                                msgen.frag_out(0, "fragcolor");
                                fsrc << "}\n";
                            /*
                            std::cout << " [COMMON]:\n" << csrc.str() << "\n";
                            std::cout << " [VERTEX]:\n" << vsrc.str() << "\n";
                            */
                            //std::cout << " [FRAGMENT]:\n" << fsrc.str() << "\n";


                            //std::cout << "[VERTEX]:\n" << _fmt_with_linenums(csrc.str() + vsrc.str()) << "\n";
                            //std::cout << "[FRAGMENT]:\n" << _fmt_with_linenums(csrc.str() + fsrc.str()) << "\n";
                            //std::cout.flush();

                            _glstuff::GLShaderObject
                                vs1(GL_VERTEX_SHADER),
                                fs1(GL_FRAGMENT_SHADER)
                            ;
                            this->_glsp->attach(vs1(csrc.str() + vsrc.str()).throw_if_error());
                            this->_glsp->attach(fs1(csrc.str() + fsrc.str()).throw_if_error());
                            this->_glsp->link().throw_if_error().detach_all();

                            //std::cout << "glsl compilation successful!\n";

                            #define _gl_uniform(_t, _n) this->_glsl_uniform_locations._t = this->_glsp->uniform_idx(_n)
                                _gl_uniform(model_matrix, "model_matrix");
                                _gl_uniform(view_matrix, "view_matrix");
                                _gl_uniform(view_matrix_inv, "view_matrix_inv");
                                _gl_uniform(projection_matrix, "projection_matrix");
                                _gl_uniform(projection_matrix_inv, "projection_matrix_inv");

                                _gl_uniform(time, "time");
                                _gl_uniform(num_active_lights, "num_active_lights");
                            #undef _gl_uniform
                            this->_glsl_uniform_locations.lights.clear();
                            if (!this->data.shadeless){
								std::string lightnamebase = "";
								#define _lightuniform(_t, _n) \
								    lud._t = this->_glsp->uniform_idx(lightnamebase + _n); \
								    \

                                //std::cout << "lights[" << lightnum << "]." << #_t << " = " << lightnamebase << _n << " = " << lud._t << "\n" \

								for (int lightnum = 0; lightnum < maxlights; lightnum++){
									this->_glsl_uniform_locations.lights.emplace_back();
									auto& lud = this->_glsl_uniform_locations.lights.back();
									lightnamebase = "light_sources[" + std::to_string(lightnum) + "].";
									_lightuniform(lighttype, "type");
									_lightuniform(lightfalloff, "falloff");
									_lightuniform(diffuse_color, "diffuse_color");
									_lightuniform(specular_color, "specular_color");
									_lightuniform(diffuse_intensity, "diffuse_intensity");
									_lightuniform(specular_intensity, "specular_intensity");
									_lightuniform(energy, "energy");
									_lightuniform(radius, "radius");
									_lightuniform(cone_angle, "cone_angle");
									_lightuniform(pos, "pos");
									_lightuniform(orn, "orn");
								}
								#undef _lightuniform
							}
                            for (auto& atex : this->active_textures) {
                                atex.glsamplerloc = this->_glsp->uniform_idx(atex.sampler_uniform_name);
                                if (atex.mts->type == textype::envmap && atex.mts->envtype == envmaptype::cubemap){
                                    atex.gltexid = this->instance->get_asset<CubemapTexture>(atex.texture)->gltexid;
                                }
                                else {
                                    atex.gltexid = this->instance->get_asset<Texture2D>(atex.texture)->gltexid;
                                }
                            }
                            gl.glFlush(); gl.glFinish();
                            this->_has_shader = true;
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            auto& vcl = this->data.vertex_color_light;
                            auto& vcp = this->data.vertex_color_paint;
                            this->_sw3d2mat = dj_sw3d20::Material{
                                .base_diffuse_color=dj_sw3d20::fpvec4(dj_sw3d20::fpvec3(this->data.diffuse.color), this->data.diffuse.alpha),
                                .base_specular_color=dj_sw3d20::fpvec4(dj_sw3d20::fpvec3(this->data.specular.color), this->data.specular.alpha),
                                .base_diffuse_intensity=this->data.diffuse.intensity,
                                .base_specular_intensity=this->data.specular.intensity,
                                .base_specular_hardness=this->data.specular.hardness,
                                .vertex_color_paint={.use=vcp.use, .amount=dj_sw3d20::fpvec4(vcp.amount)},
                                .vertex_color_light={.use=vcl.use, .amount=dj_sw3d20::fpvec4(vcl.amount)},
                                .shadeless=this->data.shadeless,
                                .alpha_threshold=2.0/255.0
                            };

                            if (true) {
                                switch (this->data.blendtype) {
                                    case matblendtype::alpha_blend: this->_sw3d2mat.uses_transparency = true; this->_sw3d2mat.blendtype = dj_sw3d20::enum_t::BLEND_MIX; break;
                                    case matblendtype::add: this->_sw3d2mat.uses_transparency = true; this->_sw3d2mat.blendtype = dj_sw3d20::enum_t::BLEND_ADD; break;
                                    default: this->_sw3d2mat.uses_transparency = false; this->_sw3d2mat.alpha_threshold = 0;
                                }
                            }

                            for (auto& atex : this->active_textures) {
                                //if (atex.mts->type != textype::image){continue;}
                                dj_sw3d20::MaterialTextureSlot smts = {};
                                smts.use = true;

                                smts.src_image = nullptr;
                                smts.src_envmap = nullptr;

                                smts.mapping_type = dj_sw3d20::enum_t::TEXMAP_UVMAP;
                                if (atex.mts->maptype == texmaptype::reflection) {
                                    smts.mapping_type = dj_sw3d20::enum_t::TEXMAP_REFLECTION;
                                }
                                else if (atex.mts->maptype == texmaptype::view){
                                    smts.mapping_type = dj_sw3d20::enum_t::TEXMAP_VIEWVECTOR;
                                }

                                smts.textype = dj_sw3d20::enum_t::TEXTYPE_IMAGE;
                                smts.bumpspace = (dj_sw3d20::enum_t)(int)(atex.mts->bump_space);
                                if (atex.mts->type == textype::envmap){
                                    smts.textype = dj_sw3d20::enum_t::TEXTYPE_ENVMAP;
                                    smts.envmaptype = dj_sw3d20::enum_t::ENVMAP_EQUIRECT;
                                    if (atex.mts->envtype == envmaptype::cubemap){
                                        std::cout << "********* GOT THE CUBE MAP\n";
                                        smts.envmaptype = dj_sw3d20::enum_t::ENVMAP_CUBEMAP;
                                        smts.src_envmap = this->instance->get_asset<CubemapTexture>(atex.mts->texture)->sw3d2envtex;
                                    }
                                    else {smts.src_envmap = this->instance->get_asset<Texture2D>(atex.mts->texture)->sw3d2envtex;}
                                }
                                else {
                                    smts.src_image = this->instance->get_asset<Texture2D>(atex.mts->texture)->sw3d2tex;
                                }

                                /*
                                smts.affects_diffuse_color.use = atex.mts->affects.diffuse_color.use;
                                smts.affects_diffuse_color.amount = dj_sw3d20::fpvec3(atex.mts->affects.diffuse_color.amount);

                                smts.affects_diffuse_alpha.use = atex.mts->affects.diffuse_alpha.use;
                                smts.affects_diffuse_alpha.amount = (atex.mts->affects.diffuse_alpha.amount);

                                smts.affects_specular_color.use = atex.mts->affects.specular_color.use;
                                smts.affects_specular_color.amount = dj_sw3d20::fpvec3(atex.mts->affects.specular_color.amount);

                                smts.affects_texcoord_offset.use = atex.mts->affects.texcoord_offset.use;
                                smts.affects_texcoord_offset.amount = dj_sw3d20::fpvec3(atex.mts->affects.texcoord_offset.amount);
                                */

                                Material::_texaffect_to_sw3d2(smts.affects_diffuse_color, atex.mts->affects.diffuse_color);
                                Material::_texaffect_to_sw3d2(smts.affects_diffuse_alpha, atex.mts->affects.diffuse_alpha);
                                Material::_texaffect_to_sw3d2(smts.affects_diffuse_intensity, atex.mts->affects.diffuse_intensity);
                                Material::_texaffect_to_sw3d2(smts.affects_specular_color, atex.mts->affects.specular_color);
                                Material::_texaffect_to_sw3d2(smts.affects_specular_intensity, atex.mts->affects.specular_intensity);
                                Material::_texaffect_to_sw3d2(smts.affects_specular_hardness, atex.mts->affects.specular_hardness);
                                Material::_texaffect_to_sw3d2(smts.affects_texcoord_offset, atex.mts->affects.texcoord_offset);
                                Material::_texaffect_to_sw3d2(smts.affects_normal_vector, atex.mts->affects.normal);

                                smts.layerblendtype = _TEXBLEND_TO_SW3D2[int(atex.mts->blendtype)];
                                smts.uvmapno = atex.mts->uvmapno;

                                //smts.alpha_channel = 0;
                                this->_sw3d2mat.texture_slots.push_back(smts);
                            }
                            this->_has_shader = true;
                        #endif
                        break;
                    }
                    default: ;
                }

                //EngineInstance::renderbuckets_t* newbucket = &((this->data.blendtype == matblendtype::opaque ? this->instance->opaque_buckets : this->instance->translucent_buckets));
                //newbucket->add_matid(uid_t(*this));
                this->instance->R_EnsureAllGroupsHaveMaterialID(uid_t(*this), (this->data.blendtype == matblendtype::opaque));
            }
            /*
            void _register_with_scenegraph_model(const uid_t& sgmdl, const uid_t& midx);
            void _register_with_scenegraph_model();
            */
        public:
            //const uid_t& sgmodel_managed_idx = _sgmodel_managed_idx;
            //const uid_t& managing_sgmodel_id = _managing_sgmodel_id;
            matprops_t data = {};
            virtual assettype asset_type() const {return assettype::material;}

            virtual void rebuild() {
                //this->_destroy_shader();
                this->_build_shader();
            }

            virtual void onRenderContextDelete() {
                if (!this->instance->R_HasActiveRenderer()){return;}
                this->_destroy_shader();
            }
            virtual void onRenderContextCreate() {
                std::cout << "Material::onRenderContextCreate()\n";
                //if (!this->instance->R_HasActiveRenderer()){return;}
                //this->_build_shader();
            }
            virtual void onCreate() {
                //this->onRenderContextCreate();
                this->rebuild();
            }
            virtual void onDestroy() {
                this->onRenderContextDelete();
            }
            Material(EngineInstance* _inst, const uid_t _uid, const matprops_t& _data) : AssetBase(_inst, _uid) {this->data = _data;}
    };

    enum class lightsrctype : unsigned int {
		point = 0,
		spot = 1,
		ambient = 2,
		hemi = 3
	};

	enum class lightfallofftype : unsigned int {
		constant = 0,
		linear = 1,
		square = 2
	};

    struct lightprops_t {
	    struct {
			bool enabled = true;
			fvec3_t color = {1,1,1};
			fscalar_t factor = 1.0f;
		} diffuse = {};
		struct {
			bool enabled = false;
			fvec3_t color = {1,1,1};
			fscalar_t factor = 1.0f;
		} specular = {};

		fscalar_t energy = 1.0f, radius = 1.0f, cone_angle = 90.0f;
		lightsrctype type = lightsrctype::point;
		lightfallofftype falloff = lightfallofftype::linear;
	};

    class LightSource : public AssetBase {
		using AssetBase::AssetBase;
		public:
		    struct addparams_t {
				fvec3_t pos = {0,0,0};
				fmat33_t orn = fmat33_t(1);
			};
		    lightprops_t data = {};
		    virtual assettype asset_type() {return assettype::lightsource;}

		    void addlight(const addparams_t& p) {
				this->instance->_active_lights.push_back(EngineInstance::activelight_t{
					.lightsrc=this,
					.pos=p.pos,
					.orn=p.orn
				});
				this->instance->_active_lights_dirty = true;
			}

		    LightSource(EngineInstance* _inst, const uid_t _uid, const lightprops_t& _data) :
		        AssetBase(_inst, _uid),
		        data(_data)
		    {}
	};

    void EngineInstance::_rebuild_all_materials() {
        for (auto p : this->assets) {
            if (p.second->asset_type() == gl45rw::assettype::material){
                ((Material*)p.second)->rebuild();
            }
        }
    }
    void EngineInstance::_reset_active_material() {
        this->R_MakeCurrent();
        auto& ams = this->_active_material_state;
        switch (this->_curmode.renderer) {
            case gfxapi::opengl: {
                #ifdef __GL45RW_HAS_OPENGL
                    auto& gl = this->_glcalls;
                    for (int i = 0; i < 8; i++){
                        gl.glActiveTexture(GL_TEXTURE0+i);
                        gl.glBindTexture(GL_TEXTURE_2D, 0);
                        gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                    }
                    gl.glActiveTexture(GL_TEXTURE0);
                    gl.glDepthMask(1);
                    delete this->_active_material_state.glbinding;
                    this->_active_material_state.glbinding = nullptr;
                #endif
                break;
            }
            case gfxapi::sw3d2: {
                #ifdef GL45RW_WITH_SW3D2
                    this->sw3d2ctx->srActiveMaterial(nullptr);
                #endif
                break;
            }
            default: ;
        }
        this->_active_material_state.mat = nullptr;
    }

    void EngineInstance::_set_active_material(Material* mat) {
        if (mat == this->_active_material_state.mat){return;}
        this->_reset_active_material();
        if (mat == nullptr){return;}
        auto& ams = this->_active_material_state;
        switch (this->_curmode.renderer) {
            case gfxapi::opengl: {
                #ifdef __GL45RW_HAS_OPENGL
                    auto& gl = this->_glcalls;
                    int maxlights = GL45RW_GL2_MAXLIGHTS;

                    switch (mat->data.blendtype) {
                        case matblendtype::alpha_blend: {
                            gl.glEnable(GL_BLEND);
                            gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            gl.glBlendEquation(GL_FUNC_ADD);
                            break;
                        }
                        case matblendtype::add: {
                            gl.glEnable(GL_BLEND);
                            gl.glBlendFunc(GL_ONE, GL_ONE);
                            gl.glBlendEquation(GL_FUNC_ADD);
                            gl.glDepthMask(0);
                            break;
                        }
                        default: gl.glDisable(GL_BLEND); gl.glDepthMask(1);
                    }

                    this->R_SetFaceCullingMode(mat->data.face_culling_mode);
                    ams.glbinding = new _glstuff::GLProgramBinding(*(mat->_glsp));
                    auto& _mbind = ams.glbinding[0];
                    /*
                    _mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_color, 1, &mat->data.diffuse.color);
                    _mbind.setUniform(mat->_glsl_uniform_locations.mat_specular_color, 1, &mat->data.specular.color);
                    _mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_intensity, 1, &mat->data.diffuse.intensity);
                    _mbind.setUniform(mat->_glsl_uniform_locations.mat_specular_intensity, 1, &mat->data.specular.intensity);
                    _mbind.setUniform(mat->_glsl_uniform_locations.mat_specular_hardness, 1, &mat->data.specular.hardness);
                    */
                    for (auto& atex : mat->active_textures) {
                        GLenum target = GL_TEXTURE_2D;
                        if (atex.mts->type == textype::envmap && atex.mts->envtype == envmaptype::cubemap){
                            target = GL_TEXTURE_CUBE_MAP;
                        }
                        gl.glActiveTexture(GL_TEXTURE0+atex.sampler_unit);
                        gl.glBindTexture(target, atex.gltexid);
                        _mbind.setUniform(atex.glsamplerloc, 1, &atex.sampler_unit);
                    }
                    int nlights = 0;
                    for (auto& lud : mat->_glsl_uniform_locations.lights) {
						if (nlights >= this->_active_lights.size()){break;}
						auto& alight = this->_active_lights[nlights];
						auto& ald = alight.lightsrc->data;
						#define _setlud(_t, _num, _data) _mbind.setUniform(lud._t, _num, _data)
						int ltype = int(ald.type), lfall = int(ald.falloff);
						fvec4_t ldc = fvec4_t(ald.diffuse.color, 0), lsc = fvec4_t(ald.specular.color, 1);
						_setlud(lighttype, 1, &ltype);
						_setlud(lightfalloff, 1, &lfall);
						_setlud(diffuse_color, 1, &ldc);
						_setlud(specular_color, 1, &lsc);
						_setlud(diffuse_intensity, 1, &ald.diffuse.factor);
						_setlud(specular_intensity, 1, &ald.specular.factor);

						_setlud(energy, 1, &ald.energy);
						_setlud(radius, 1, &ald.radius);
						_setlud(cone_angle, 1, &ald.cone_angle);
						_setlud(pos, 1, &alight.pos);
						_setlud(orn, 1, &alight.orn);
						#undef _setlud
						nlights++;
					}
                    _mbind.setUniform(mat->_glsl_uniform_locations.num_active_lights, 1, &nlights);
                    //std::string uniform_name =

                #endif
                break;
            }
            case gfxapi::sw3d2: {
                #ifdef GL45RW_WITH_SW3D2
                    this->sw3d2ctx->srActiveMaterial(&(mat->_sw3d2mat));
                    this->R_SetFaceCullingMode(mat->data.face_culling_mode);
                    if (mat->data.blendtype == matblendtype::add){
                        this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, false);
                    }
                    else {
                        this->sw3d2ctx->srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, true);
                    }
                #endif
                break;
            }
            default: ;
        }
    }

    EngineInstance::active_material_state& EngineInstance::R_SetActiveMaterial(const uid_t matid){
        this->_set_active_material(this->get_asset<Material>(matid));
        return this->_active_material_state;
    }

    void EngineInstance::R_UpdateLighting() {
		if (!this->_active_lights_dirty){return;}
		this->_active_lights_dirty = false;
		this->R_MakeCurrent();
		switch (this->_curmode.renderer){
			case gfxapi::opengl: {
				#ifdef __GL45RW_HAS_OPENGL
				 auto& gl = _glstuff::curgl[0];
				#endif
				break;
			}
			case gfxapi::sw3d2: {
				#ifdef GL45RW_WITH_SW3D2
				auto& ctx = this->sw3d2ctx[0];
				ctx.srResetLightSources();
				for (auto& alight : this->_active_lights){
					LightSource* glight = alight.lightsrc;
					dj_sw3d20::LightSource* slight = ctx.srCreateLightSource();
					if (slight == nullptr){break;}
					slight->pos = dj_sw3d20::fpvec3(alight.pos);
					slight->orientation = dj_sw3d20::fpmat3(alight.orn);
					slight->falloff = dj_sw3d20::light_falloff_model((unsigned int)(glight->data.falloff));
					slight->type = dj_sw3d20::light_type((unsigned int)(glight->data.type));
					slight->spot_max_distance = slight->point_radius_max = dj_sw3d20::fpscalar(glight->data.radius);
					slight->spot_cone_angle = dj_sw3d20::fpscalar(glight->data.cone_angle);
					slight->diffuse_color = dj_sw3d20::fpvec4(dj_sw3d20::fpvec3(glight->data.diffuse.color), 0);
					slight->specular_color = dj_sw3d20::fpvec4(dj_sw3d20::fpvec3(glight->data.specular.color), 1);
					slight->diffuse_intensity = glight->data.energy * glight->data.diffuse.factor;
					slight->specular_intensity = glight->data.energy * glight->data.specular.factor;
				}
				#endif
				break;
			}
			default: ;
		}
	}

    void EngineInstance::R_UpdateMaterialMatrices(const EngineInstance::calcmatrices_t& cm){
		if (this->_active_material_state.mat == nullptr){return;}
		auto* mat = this->_active_material_state.mat;
		const fmat44_t final_mdl = cm.mdl2 * cm.mdl;
		switch (this->_curmode.renderer) {
			case gfxapi::opengl: {
				#ifdef __GL45RW_HAS_OPENGL

			    auto& _mbind = this->_active_material_state.glbinding[0];
				_mbind.setUniform(mat->_glsl_uniform_locations.projection_matrix, 1, &cm.projection);
				_mbind.setUniform(mat->_glsl_uniform_locations.projection_matrix_inv, 1, &cm.projection_inv);
				_mbind.setUniform(mat->_glsl_uniform_locations.view_matrix, 1, &cm.view);
				_mbind.setUniform(mat->_glsl_uniform_locations.view_matrix_inv, 1, &cm.view_inv);
				_mbind.setUniform(mat->_glsl_uniform_locations.model_matrix, 1, &final_mdl);

				//_mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_color, 1, &mat->data.diffuse.color);
				//_mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_alpha, 1, &mat->data.diffuse.alpha);
				//_mbind.setUniform(mat->_glsl_uniform_locations.time, 1, &curtime);
				#endif
				break;
			}
			case gfxapi::sw3d2: {
				#ifdef GL45RW_WITH_SW3D2
				auto& ctx = *this->sw3d2ctx;
				ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX);
                 ctx.srTopMatrix() = cm.view;
                ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX);
                 ctx.srTopMatrix() = cm.projection;
                ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX);
                 ctx.srTopMatrix() = final_mdl;
				#endif
				break;
			}
			default: ;
		}
	}

	#ifdef GL45RW_WITH_SW3D3
	    template <class VT>
        VT _sw3d3_texelblend(const texblendtype& bt, const VT& src, const VT& dst, const VT& fac) {
            static const void* _jmps[] = {
                &&_tbt_mix,
                &&_tbt_add,
                &&_tbt_mul
            };
            goto *_jmps[int(bt)];
            _tbt_mix: return glm::mix(src,dst,fac);
            _tbt_add: return glm::mix(src,src+dst,fac);
            _tbt_mul: return glm::mix(src,src*dst,fac);
            return src;
        }
        struct _sw3d3_drawmesh_sampletex_args_t {
            glm::vec3 pviewdir, wviewdir, pvrefl, wvrefl;
            int slot;
        };
        #define _sw3d3_bclerp sw3d3::TestContext::_bclerp
        glm::vec4 _sw3d3_drawmesh_sampletex(sw3d3::pixelshader_t& self, _SW3D3_DrawMesh_ShaderData& ud, _sw3d3_drawmesh_sampletex_args_t& args) {
            auto& sts = ud.texslots[args.slot];
            auto* pv = self.f_in.pv;
            auto& gts = ud.mp->texture_slots[args.slot];
            auto* tex = sts.tex;
            static const void* _jmptable_t2d[] = {
                &&_t2d_uvmap,
                &&_t2d_reflect,
                &&_t2d_view
            };
            glm::vec3 mbreflvec = args.pvrefl * args.pvrefl.z;
            goto *_jmptable_t2d[int(gts.maptype)];
            _t2d_uvmap: return tex->texture2D(_sw3d3_bclerp(self.f_in.pc_bcw, pv[0].uv[gts.uvmapno], pv[1].uv[gts.uvmapno], pv[2].uv[gts.uvmapno]));
            _t2d_reflect: return tex->texture2D(glm::vec2{
                0.5f + (0.6f * mbreflvec.x),
                0.5f - (0.6f * mbreflvec.y)
            });
            _t2d_view: return glm::vec4(0);
            return glm::vec4(0);
        }
        template <typename T>
        inline T _sw3d3_faceforward(const bool& frontface, const T& v) {
            return (frontface ? v : -v);
        }
	    bool _sw3d3_drawmesh_fmain(sw3d3::pixelshader_t& self) {
            auto* pv = self.f_in.pv;
            auto* wvn = self.f_in.world_vertex_normals;
            auto* wvc = self.f_in.world_vertex_coords;
            auto& ud = *reinterpret_cast<_SW3D3_DrawMesh_ShaderData*>(self.userdata);
            glm::vec4 vcolor = _sw3d3_bclerp(self.f_in.pc_bcw, pv[0].color, pv[1].color, pv[2].color);
            glm::vec4 fragcolor = vcolor * glm::vec4(ud.mp->diffuse.color, 1.0f);
            glm::vec3 pnormal = glm::normalize(_sw3d3_faceforward(self.f_in.frontface, _sw3d3_bclerp(self.f_in.pc_bcw, pv[0].normal, pv[1].normal, pv[2].normal)));
            glm::vec3 wnormal = glm::normalize(_sw3d3_faceforward(self.f_in.frontface, _sw3d3_bclerp(self.f_in.pc_bcw, wvn[0], wvn[1], wvn[2])));
            //glm::vec3 pfragpos(_sw3d3_bclerp(self.f_in.pc_bcw, pv[0].co, pv[1].co, pv[2].co));
            glm::vec3 wfragpos(_sw3d3_bclerp(self.f_in.pc_bcw, wvc[0], wvc[1], wvc[2]));
            glm::vec3 pfragpos(self.f_in.pvm * glm::vec4(wfragpos, 1));
            glm::vec3 wcampos(self.ctx->matrices.view[3]);
            glm::vec3 wviewdir = glm::normalize(wfragpos - wcampos);
            glm::vec3 pviewdir = glm::normalize(pfragpos);
            _sw3d3_drawmesh_sampletex_args_t stargs;
            stargs.pviewdir = pviewdir;
            stargs.wviewdir = wviewdir;
            stargs.pvrefl = glm::normalize(glm::reflect(pviewdir, pnormal));
            stargs.wvrefl = glm::normalize(glm::reflect(wviewdir, wnormal));

            for (int i = 0; i < 8; i++){
                stargs.slot = i;
                auto& sts = ud.texslots[i];
                if (!sts.use){break;}
                auto& gts = ud.mp->texture_slots[i];
                if (!gts.affects.diffuse_color.use){continue;}
                const glm::vec4 texel = _sw3d3_drawmesh_sampletex(self, ud, stargs);
                fragcolor = _sw3d3_texelblend(gts.blendtype, fragcolor, texel, glm::vec4(gts.affects.diffuse_color.amount, 1));
            }

            if (!ud.mp->shadeless) {
                fragcolor = glm::vec4(glm::vec3(fragcolor) * glm::mix(0.25f, 1.0f, glm::abs(glm::dot(glm::vec3{0,1,0}, pnormal))), fragcolor.a);
            }

            switch (ud.mp->blendtype){
                case matblendtype::add: {
                    self.f_out.fragcolor = self.f_in.curfragcolor + fragcolor;
                    break;
                }
                default: self.f_out.fragcolor = fragcolor;
            }
            return true;
        }
        bool _sw3d3_bitmapfont_fmain(sw3d3::pixelshader_t& self){

            return true;
        }
        #undef _sw3d3_bclerp
	#endif

    class TriangleMesh : public AssetBase {
        using AssetBase::AssetBase;
        private:
            bool _renderdata_init = false;
        public:
            enum struct drawmethod : int {
                no_draw = -1, //the mesh isn't for the renderer (i.e. a collision mesh)
                static_draw = 0, //full VAO's with an index buffer are used in OpenGL ES 3, most suitable for non-animated meshes.
                stream_draw = 1 //dynamically streams batches of up to a certain number of tris (i.e. max. 150 tris) to a VAO then renders them a batch at a time.
            };
            struct drawparams_t {
				#ifdef __GL45RW_HAS_OPENGL
				    _glstuff::GLProgramBinding* glbinding = nullptr;
				#endif
				#ifdef GL45RW_WITH_SW3D2
				    dj_sw3d20::Material* sw3d2mat = nullptr;
				#endif
				#ifdef GL45RW_WITH_SW3D3
                    _SW3D3_DrawMesh_ShaderData* _sw3d3_shader_data = nullptr;
				#endif
				int _dummy;
			};
            struct vertex_t {
                fvec3_t co = {0,0,0}, normal = {0,0,0};
                fvec4_t color = {1,1,1,1};
                fvec2_t uv[8] = {};
                fvec3_t tangent = {0,0,0}, binormal = {0,0,0};
            };
            struct shapekey_vertex_entry {
                vertex_t deltas;
            };
            struct shapekey {
                fscalar_t frame = 0;
                dictionary<unsigned int, shapekey_vertex_entry> entries;
            };

            struct shapekey_animation {
                vectorlist<shapekey> keyframes = {};
            };

            struct meshdata {
                uint8_t active_uvmaps_mask = 1;
                size_t _num_active_uvmaps = 1;
                vectorlist<vertex_t> vertices = {};
                vectorlist<std::array<unsigned int, 3>> triangles = {};
                dictionary<std::u32string, shapekey_animation> shapeanims = {};
                /*
                meshdata() {}
                meshdata(const meshdata& o) :
                    active_uvmaps_mask(o.active_uvmaps_mask),
                    _num_active_uvmaps(o._num_active_uvmaps),
                    vertices(o.vertices),
                    triangles(o.triangles)
                {}
                */

                bool getface(const vertex_t** vptrs, std::array<unsigned int, 3>& vinds, const size_t tri_idx) const {
                    if (tri_idx >= this->triangles.size()){return false;}
                    vinds = this->triangles[tri_idx];
                    vptrs[0] = &(this->vertices[vinds[0]]);
                    vptrs[1] = &(this->vertices[vinds[1]]);
                    vptrs[2] = &(this->vertices[vinds[2]]);
                    return true;
                }
                meshdata& reset() {
                    this->active_uvmaps_mask = 1;
                    this->_num_active_uvmaps = 1;
                    this->vertices.clear();
                    this->triangles.clear();
                    this->shapeanims.clear();
                    return *this;
                }
                meshdata& prim_tri1(const fvec4_t acolor={1,0,0,1}, const fvec4_t bcolor={0,1,0,1}, const fvec4_t ccolor={0,0,1,1}) {
                    auto& self = this->reset();
                    self.vertices.push_back(vertex_t{.co=fvec3_t{-0.5f, -0.5f, 0.0f}, .normal=fvec3_t{0,0,1}, .color=acolor, .uv={fvec2_t{0,0}}});
                    self.vertices.push_back(vertex_t{.co=fvec3_t{0.5f, -0.5f, 0.0f}, .normal=fvec3_t{0,0,1}, .color=bcolor, .uv={fvec2_t{1,0}}});
                    self.vertices.push_back(vertex_t{.co=fvec3_t{0.0f, 0.5f, 0.0f}, .normal=fvec3_t{0,0,1}, .color=ccolor, .uv={fvec2_t{0.5f,1.0f}}});
                    self.triangles.push_back(std::array<unsigned int, 3>{0,1,2});
                    return self.recalc_tangents_and_binormals();
                }
                meshdata& prim_circle_xy(const unsigned int res=15, const fscalar_t radius=0.5f, const fvec3_t origin={0,0,0}){
					if (res < 3){throw std::runtime_error("res must be >= 3!");}
					auto& self = this->reset();
					vertex_t centervert = {.co=origin, .normal=fvec3_t{0,0,1}, .color=fvec4_t(1), .uv={fvec2_t(0.5f)}};
					self.vertices.push_back(centervert);
					fscalar_t cangle = 0, anglestep = glm::radians(360.0f/res);
					for (unsigned int i = 0; i < res; i++, cangle+=anglestep){
						fvec3_t avec = {glm::sin(cangle), glm::cos(cangle), 0.0f};
						fvec2_t uv = glm::mix(fvec2_t(0.5f), fvec2_t(1.0f), fvec2_t(avec));
						self.vertices.push_back(vertex_t{.co=origin+(avec*radius), .normal=fvec3_t{0,0,1}, .color=fvec4_t(1), .uv={uv}});
						unsigned int j = (i+1)%res;
						self.triangles.push_back(std::array<unsigned int, 3>{0, i+1, j+1});
					}

					return self.recalc_tangents_and_binormals();
				}
                meshdata& recalc_tangents_and_binormals(const unsigned int uvmapno=0){
                    for (auto& tri : this->triangles){
                        vertex_t& v0 = this->vertices[tri[0]];
                        vertex_t& v1 = this->vertices[tri[1]];
                        vertex_t& v2 = this->vertices[tri[2]];
                        fvec3_t dpos1 = v1.co - v0.co, dpos2 = v2.co - v0.co;
                        fvec2_t duv1 = v1.uv[uvmapno] - v0.uv[uvmapno], duv2 = v2.uv[uvmapno] - v0.uv[uvmapno];
                        fscalar_t r = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x);
                        v0.tangent = v1.tangent = v2.tangent = (dpos1 * duv2.y  - dpos2 * duv1.y)*r;
                        v0.binormal = v1.binormal = v2.binormal = (dpos2 * duv1.x - dpos1 * duv2.x)*r;
                    }
                    return *this;
                }
				/*
				meshdata& prim_sphere(const unsigned int u_res=16, const unsigned int v_res=8, const fscalar_t radius=0.5f){
					if (v_res <= 2){throw std::runtime_error("v_res >= 3!");}
					else if (u_res < 2){throw std::runtime_error("u_res >= 2!");}
					auto& self = this->reset();
					vertex_t
					    topvert = {.co=fvec3_t{0,0,radius}, .normal=fvec3_t{0,0,1}, .color=fvec4_t(1)},
					    btmvert = {.co=fvec3_t{0,0,-radius}, .normal=fvec3_t{0,0,-1}, .color=fvec4_t(1)}
					;
					self.vertices.push_back(topvert);
					self.vertices.push_back(btmvert);
					for (unsigned int j = 1; j < v_res-1; j++){
						fscalar_t vangle = (180.0f/v_res)*j;
						bool connect_with_top = (j == 1);
						bool connect_with_btm = (j == (v_res-1));
						for (unsigned int i = 0; i < u_res; i++){
							fscalar_t uangle = (360.0f/u_res)*i;
							unsigned int
						}
					}

					return self;
				}
				*/
                meshdata& prim_unitcube(const fvec3_t origin_offs={0,0,0}) {
                    auto& self = this->reset();
                    const fvec3_t _cube_coords[24] = {
                        {-0.5,-0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5,-0.5,-0.5}, {-0.5, 0.5,-0.5},
                        { 0.5, 0.5, 0.5}, { 0.5,-0.5, 0.5}, { 0.5, 0.5,-0.5}, { 0.5,-0.5,-0.5},
                        { 0.5,-0.5, 0.5}, {-0.5,-0.5, 0.5}, { 0.5,-0.5,-0.5}, {-0.5,-0.5,-0.5},
                        {-0.5, 0.5, 0.5}, { 0.5, 0.5, 0.5}, {-0.5, 0.5,-0.5}, { 0.5, 0.5,-0.5},
                        {-0.5, 0.5,-0.5}, { 0.5, 0.5,-0.5}, {-0.5,-0.5,-0.5}, { 0.5,-0.5,-0.5},
                        {-0.5,-0.5, 0.5}, { 0.5,-0.5, 0.5}, {-0.5, 0.5, 0.5}, { 0.5, 0.5, 0.5}
                    };
                    /*
                    const fvec3_t _cube_normals[24] = {
                        [0 ... 3] = {-1, 0, 0},
                        [4 ... 7] = { 1, 0, 0},
                        [8 ... 11] = { 0,-1, 0},
                        [12 ... 15] = { 0, 1, 0},
                        [16 ... 19] = { 0, 0,-1},
                        [20 ... 23] = { 0, 0, 1}
                    };

                    fvec3_t _cube_normals[24] = {};

                    for (int idx = 0; idx <= 3; idx++){_cube_normals[idx] = -fvec3_t{-1,0,0};}
                    for (int idx = 4; idx <= 7; idx++){_cube_normals[idx] = -fvec3_t{1,0,0};}
                    for (int idx = 8; idx <= 11; idx++){_cube_normals[idx] = -fvec3_t{0,-1,0};}
                    for (int idx = 12; idx <= 15; idx++){_cube_normals[idx] = -fvec3_t{0,1,0};}
                    for (int idx = 16; idx <= 19; idx++){_cube_normals[idx] = -fvec3_t{0,0,-1};}
                    for (int idx = 20; idx <= 23; idx++){_cube_normals[idx] = -fvec3_t{0,0,1};}
                    */

                    const fvec2_t _cube_uvmap0[24] = {
                        {0, 1}, {1, 1}, {0, 0}, {1, 0},
                        {0, 1}, {1, 1}, {0, 0}, {1, 0},
                        {0, 1}, {1, 1}, {0, 0}, {1, 0},
                        {0, 1}, {1, 1}, {0, 0}, {1, 0},
                        {0, 1}, {1, 1}, {0, 0}, {1, 0},
                        {0, 1}, {1, 1}, {0, 0}, {1, 0}
                    };

                    const fvec2_t _cube_uvmap1[24] = {
                        {1, 1}, {0, 1}, {1, 0}, {0, 0},
                        {1, 1}, {0, 1}, {1, 0}, {0, 0},
                        {1, 1}, {0, 1}, {1, 0}, {0, 0},
                        {1, 1}, {0, 1}, {1, 0}, {0, 0},
                        {1, 1}, {0, 1}, {1, 0}, {0, 0},
                        {1, 1}, {0, 1}, {1, 0}, {0, 0}
                    };
                    const unsigned int _cube_vertex_indices[36] = {
                        3,0,2,    3,1,0,
                        7,4,6,    7,5,4,
                        11,8,10,  11,9,8,
                        15,12,14, 15,13,12,
                        19,16,18, 19,17,16,
                        23,20,22, 23,21,20
                    };

                    for (int i = 0; i < 24; i++){
                        self.vertices.push_back(vertex_t{
                            .co=_cube_coords[i]+origin_offs,
                            //.normal=_cube_normals[i],
                            .normal=fvec3_t(0),
                            .color=fvec4_t(1),
                            .uv={_cube_uvmap0[i], _cube_uvmap1[i]}
                        });
                    }
                    const unsigned int* vip = _cube_vertex_indices;
                    for (int i = 0; i < 12; i++, vip+=3){
                        self.triangles.push_back(reinterpret_cast<const std::array<unsigned int, 3>&>(*vip));
                        const fvec3_t normal = glm::normalize(glm::cross((_cube_coords[vip[1]] - _cube_coords[vip[0]]), (_cube_coords[vip[2]] - _cube_coords[vip[0]])));
                        self.vertices[vip[0]].normal = self.vertices[vip[1]].normal = self.vertices[vip[2]].normal = normal;
                    }

                    this->_num_active_uvmaps = 2;
                    this->active_uvmaps_mask = 0x1U | 0x2U;
                    return self.recalc_tangents_and_binormals();
                }
                meshdata& prim_suzanne() {
                    auto& self = this->reset();
                    return self;
                }
                meshdata& prim_unitrect_xy(const fvec3_t origin={0.5,0.5,0}, const fvec2_t uvorigin={0,0}) {
					auto& self = this->reset();
					const fvec3_t pco[] = {
						{0,0,0}, {1,0,0},
						{0,1,0}, {1,1,0}
					};
					const fvec2_t puv[] = {
						{0,0}, {1,0},
						{0,1}, {1,1}
					};
					for (int i = 0; i < 4; i++){
						self.vertices.push_back(vertex_t{
							.co=pco[i]-origin,
							.normal=fvec3_t{0,0,1},
							.color=fvec4_t(1),
							.uv={puv[i]-uvorigin}
						});
					}
					self.triangles.push_back(std::array<unsigned int, 3>{0,1,3});
					self.triangles.push_back(std::array<unsigned int, 3>{2,3,0});
					self._num_active_uvmaps = 1;
					self.active_uvmaps_mask = 0x1U;
					return self.recalc_tangents_and_binormals();
				}
                _mathstuff::aabb3d_t bbox() const {
                    _mathstuff::aabb3d_t box;
                    if (this->triangles.size() == 0){box.pmin = box.pmax = fvec3_t(0);}
                    else {
                        for (auto t : this->triangles){
                            box.add_tri_ip(this->vertices[t[0]].co, this->vertices[t[1]].co, this->vertices[t[2]].co);
                        }
                    }
                    return box;
                }
            };


            class _trimeshgrid_userdata {
                TriangleMesh* trimesh = nullptr;
            };

            class _trimeshgrid_entry {
                private:
                    unsigned int _faceidx = 0;
                    TriangleMesh* const _trimesh = nullptr;
                    inline std::array<vertex_t*, 3> _get_vertices() const {
                        std::array<vertex_t*, 3> tri = {};
                        auto& tmd = this->_trimesh->data;
                        auto& triangle = tmd.triangles[this->_faceidx];
                        for (size_t i = 0; i < 3; i++){
                            tri[i] = &(tmd.vertices[triangle[i]]);
                        }
                        return tri;
                    }
                    inline void _calc_triangle() {
                        std::array<vertex_t*, 3> verts = this->_get_vertices();
                        fvec3_t vcsum = {0,0,0};
                        this->fminbounds = fvec3_t(INFINITY);
                        this->fmaxbounds = fvec3_t(-INFINITY);
                        for (int i = 0; i < 3; i++){
                            this->vcoords[i] = verts[i]->co;
                            this->vnormals[i] = glm::normalize(verts[i]->normal);
                            vcsum += this->vcoords[i];
                            fminbounds = glm::min(this->fminbounds, this->vcoords[i]);
                            fmaxbounds = glm::max(this->fmaxbounds, this->vcoords[i]);
                        }
                        this->fnormal = glm::normalize(glm::cross(this->vcoords[1] - this->vcoords[0], this->vcoords[2] - this->vcoords[0]));
                        this->fcentroid = vcsum / fvec3_t(3);

                        this->iminbounds = glm::ivec3(this->fminbounds);
                        this->imaxbounds = glm::ivec3(this->fmaxbounds);
                    }
                public:
                    const unsigned int& face_index = _faceidx;
                    TriangleMesh* const& trimesh = _trimesh;

                    std::array<fvec3_t, 3> vcoords = {}, vnormals = {};
                    fvec3_t fnormal = {}, fcentroid = {};
                    fvec3_t fminbounds = {}, fmaxbounds = {};
                    glm::ivec3 iminbounds = {}, imaxbounds = {};

                    bool operator==(const _trimeshgrid_entry& other) const {
                        return (this->trimesh == other.trimesh) && (this->face_index == other.face_index);
                    }

                    static void cellmap3d_calc_aabb(glm::ivec3& oa, glm::ivec3& ob, const _trimeshgrid_entry& self, _CellMap3D_DefaultUserdata& userdata) {
                        oa = self.iminbounds;
                        ob = self.imaxbounds;
                    }

                    static bool cellmap3d_intersects_aabb(const glm::ivec3& ia, const glm::ivec3& ib, const _trimeshgrid_entry& self, _CellMap3D_DefaultUserdata& userdata) {
                        return (glm::all(glm::greaterThanEqual(self.iminbounds, ib)) && glm::all(glm::lessThan(self.imaxbounds, ia)));
                    }

                    _trimeshgrid_entry() {}
                    _trimeshgrid_entry(const _trimeshgrid_entry& o) :
                        _trimesh(o._trimesh),
                        _faceidx(o._faceidx),

                        vcoords(o.vcoords),
                        vnormals(o.vnormals),
                        fnormal(o.fnormal),
                        fcentroid(o.fcentroid),

                        fminbounds(o.fminbounds),
                        fmaxbounds(o.fmaxbounds),

                        iminbounds(o.iminbounds),
                        imaxbounds(o.imaxbounds)
                    {}
                    _trimeshgrid_entry(TriangleMesh* const& tm, const unsigned int& _face) :
                        _trimesh(tm),
                        _faceidx(_face)
                    {this->_calc_triangle();}
            };

            glm::ivec3 physgrid_maxncells = {10,10,10};
            _mathstuff::aabb3d_t meshbox = {fvec3_t(0), fvec3_t(0)};
            meshdata data = {};
            drawmethod draw_method = drawmethod::static_draw;
            CellMap3D<_trimeshgrid_entry>* physgrid = nullptr;
            #ifdef __GL45RW_HAS_OPENGL
                GLuint
                    gles3_vao_buf = 0,
                    gles3_vao = 0,
                    gles3_ibo = 0,

                    gl2_vbo = 0,
                    gl2_nbo = 0,
                    gl2_cbo = 0,
                    gl2_uvbo[8] = {}
                ;

            #endif
            #ifdef GL45RW_WITH_SW3D2
                dj_sw3d20::ffvertexlist_t sw3d2_vao = {};
            #endif
            #ifdef GL45RW_WITH_SW3D3
                sw3d3::vao_t* sw3d3_vao = nullptr;
            #endif

            void create_physdata() {
                if (this->physgrid != nullptr){return;}

                this->meshbox = this->data.bbox();
                fvec3_t mbdelta = this->meshbox.dims();
                glm::ivec3 celldims = glm::ivec3(glm::max(fvec3_t(1), mbdelta / fvec3_t(this->physgrid_maxncells)));
                this->physgrid = new CellMap3D<_trimeshgrid_entry>(celldims);
                for (unsigned int i = 0; i < this->data.triangles.size(); i++) {
                    _trimeshgrid_entry trient(this, i);
                    this->physgrid->add(trient);
                }
            }
            void delete_physdata() {
                delete this->physgrid;
                this->physgrid = nullptr;
            }
            void _create_renderdata() {
                if (this->_renderdata_init || this->draw_method == drawmethod::no_draw){return;}
                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::opengl: {
                        #if (defined(_GL45RW_GL2))
                            ;
                        #elif (defined(_GL45RW_GLES3))
                            this->instance->R_MakeCurrent();
                            auto& gl = _glstuff::curgl[0];
                            gl.glGenVertexArrays(1, &(this->gles3_vao));
                            gl.glGenBuffers(1, &(this->gles3_vao_buf));
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            this->sw3d2_vao.clear();
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            auto* ctx = this->instance->sw3d3ctx;
                            this->sw3d3_vao = ctx->getVAO(ctx->newVAO());
                        #endif
                        break;
                    }
                    default: return;
                }
                this->_renderdata_init = true;
            }
            void _destroy_renderdata() {
                if (!this->_renderdata_init || this->draw_method == drawmethod::no_draw){return;}
                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::opengl: {
                        this->instance->R_MakeCurrent();
                        #if (defined(_GL45RW_GL2))
                            auto& gl = _glstuff::curgl[0];
                            gl.glDeleteBuffers(11, &this->gl2_vbo); std::fill_n(&this->gl2_vbo, 11, GLuint(0));
                        #elif (defined(_GL45RW_GLES3))
                            auto& gl = _glstuff::curgl[0];
                            gl.glDeleteVertexArrays(1, &this->gles3_vao);
                            gl.glDeleteBuffers(1, &this->gles3_vao_buf);
                            this->gles3_vao = 0;
                            this->gles3_vao_buf = 0;
                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            this->sw3d2_vao.clear();
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            auto* ctx = this->instance->sw3d3ctx;
                            if (this->sw3d3_vao == nullptr){break;}
                            ctx->delVAO(this->sw3d3_vao->idx);
                            this->sw3d3_vao = nullptr;
                        #endif
                        break;
                    }
                    default: ;
                }
                this->_renderdata_init = false;
            }
            const void* gles3_ofs(const size_t& v) {
                return reinterpret_cast<const void*>(v);
            }
            void refresh() {
                if (!this->_renderdata_init || this->draw_method == drawmethod::no_draw){return;}
                switch (this->instance->R_GetCurrentVideoMode().renderer) {
                    case gfxapi::opengl: {
                        this->instance->R_MakeCurrent();
                        #if (defined(_GL45RW_GL2))
                            auto& gl = _glstuff::curgl[0];
                            if (this->draw_method == drawmethod::static_draw){
                                this->data.recalc_tangents_and_binormals();
                                gl.glGenBuffers(11, &(this->gl2_vbo));
                                bytevector vbo_bv = {}, nbo_bv = {}, cbo_bv = {}, uvbo_bvs[8] = {};
                                const size_t nverts = this->data.triangles.size() * 3;

                                for (auto& tri : this->data.triangles){
                                    fvec3_t _vcos[3];
                                	for (size_t tv = 0; tv < 3; tv++){
                                		unsigned int vidx = tri[tv];
                                		auto& gv = this->data.vertices[vidx];
                                		_vcos[tv] = gv.co;
                                		bytevector_add(vbo_bv, gv.co);
                                		bytevector_add(nbo_bv, gv.normal);
                                		bytevector_add(cbo_bv, gv.color);
                                		for (int i = 0; i < 8; i++){bytevector_add(uvbo_bvs[i], gv.uv[i]);}
                                	}
                                }
                                gl.glEnableClientState(GL_VERTEX_ARRAY);
                                 gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_vbo);
                                 gl.glBufferData(GL_ARRAY_BUFFER, vbo_bv.size(), vbo_bv.data(), GL_STATIC_DRAW);
                                gl.glEnableClientState(GL_NORMAL_ARRAY);
                                 gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_nbo);
                                 gl.glBufferData(GL_ARRAY_BUFFER, nbo_bv.size(), nbo_bv.data(), GL_STATIC_DRAW);
                                gl.glEnableClientState(GL_COLOR_ARRAY);
                                 gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_cbo);
                                 gl.glBufferData(GL_ARRAY_BUFFER, cbo_bv.size(), cbo_bv.data(), GL_STATIC_DRAW);
                                gl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                                for (int i = 0; i < 8; i++){
                                	gl.glClientActiveTexture(GL_TEXTURE0 + i);
	                                gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_uvbo[i]);
	                                gl.glBufferData(GL_ARRAY_BUFFER, uvbo_bvs[i].size(), uvbo_bvs[i].data(), GL_STATIC_DRAW);
	                                //gl.glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
                                }
                                gl.glFlush(); gl.glFinish();
                                gl.glBindBuffer(GL_ARRAY_BUFFER, 0);
                            }
                        #elif (defined(_GL45RW_GLES3))
                            auto& gl = _glstuff::curgl[0];
                            gl.glBindBuffer(GL_ARRAY_BUFFER, this->gles3_vao_buf);
                            bytevector vao_buf;
                            const size_t nverts = this->data.triangles.size() * 3;
                            for (auto& tri : this->data.triangles){
                                for (size_t tv = 0; tv < 3; tv++){
                                    auto& vidx = tri[tv];
                                    auto& gv = this->data.vertices[vidx];
                                    vao_buf.add(gv.co);
                                    vao_buf.add(gv.normal);
                                    vao_buf.add(gv.color);

                                    vao_buf.add(gv.uv[0]);
                                    vao_buf.add(gv.uv[1]);
                                    vao_buf.add(gv.uv[2]);
                                    vao_buf.add(gv.uv[3]);
                                    vao_buf.add(gv.uv[4]);
                                    vao_buf.add(gv.uv[5]);
                                    vao_buf.add(gv.uv[6]);
                                    vao_buf.add(gv.uv[7]);
                                }
                            }
                            gl.glBufferData(GL_ARRAY_BUFFER, vao_buf.size(), vao_buf.data(), GL_STATIC_DRAW);
                            const size_t stride = sizeof(fscalar_t) * 26;
                            gl.glBindVertexArray(this->gles3_vao);
                            gl.glEnableVertexAttribArray(0); gl.glVertexAttribPointer(0, 3, GL_FLOAT, 0, stride, this->gles3_ofs(0)); // co
                            gl.glEnableVertexAttribArray(1); gl.glVertexAttribPointer(1, 3, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 3)); // normal
                            gl.glEnableVertexAttribArray(2); gl.glVertexAttribPointer(2, 4, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 6)); // color
                            gl.glEnableVertexAttribArray(3); gl.glVertexAttribPointer(3, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 10)); // uv0
                            gl.glEnableVertexAttribArray(4); gl.glVertexAttribPointer(4, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 12)); // uv1
                            gl.glEnableVertexAttribArray(5); gl.glVertexAttribPointer(5, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 14)); // uv2
                            gl.glEnableVertexAttribArray(6); gl.glVertexAttribPointer(6, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 16)); // uv3
                            gl.glEnableVertexAttribArray(7); gl.glVertexAttribPointer(7, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 18)); // uv4
                            gl.glEnableVertexAttribArray(8); gl.glVertexAttribPointer(8, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 20)); // uv5
                            gl.glEnableVertexAttribArray(9); gl.glVertexAttribPointer(9, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 22)); // uv6
                            gl.glEnableVertexAttribArray(10); gl.glVertexAttribPointer(10, 2, GL_FLOAT, 0, stride, this->gles3_ofs(sizeof(fscalar_t) * 24)); // uv7
                            gl.glFlush(); gl.glFinish();
                            gl.glBindVertexArray(0);
                            gl.glBindBuffer(GL_ARRAY_BUFFER, 0);

                        #endif
                        break;
                    }
                    case gfxapi::sw3d2: {
                        #ifdef GL45RW_WITH_SW3D2
                            this->sw3d2_vao.clear();
                            if (this->draw_method == drawmethod::static_draw){
                                this->sw3d2_vao.resize(this->data.triangles.size() * 3, dj_sw3d20::ffvertex_t{});
                                size_t tri_idx = 0;
                                for (auto& tri : this->data.triangles) {
                                    for (size_t tv = 0; tv < 3; tv++, tri_idx++){
                                        unsigned int vidx = tri[tv];
                                        auto& gvertex = this->data.vertices[vidx];
                                        auto& svertex = this->sw3d2_vao[tri_idx];
                                        svertex.co = dj_sw3d20::fpvec3(gvertex.co);
                                        svertex.normal = dj_sw3d20::fpvec3(gvertex.normal);
                                        svertex.color = dj_sw3d20::fpvec4(gvertex.color);
                                        //svertex.tangent = dj_sw3d20::fpvec3(gvertex.tangent);
                                        //svertex.binormal = dj_sw3d20::fpvec3(gvertex.binormal);
                                        for (int i = 0; i < 8; i++){
                                            svertex.uv[i] = dj_sw3d20::fpvec2(gvertex.uv[i]);
                                        }
                                    }
                                }
                            }
                        #endif
                        break;
                    }
                    case gfxapi::sw3d3: {
                        #ifdef GL45RW_WITH_SW3D3
                            auto* ctx = this->instance->sw3d3ctx;
                            this->sw3d3_vao->vertices.reset();
                            this->sw3d3_vao->indices.reset();

                            this->sw3d3_vao->vertices.resize(this->data.vertices.size());
                            this->sw3d3_vao->indices.resize(this->data.triangles.size() * 3);
                            //std::copy_n(&(this->data.triangles.front()[0]), this->sw3d3_vao->indices.size(), this->sw3d3_vao->indices.pfront());
                            for (size_t i = 0; i < this->sw3d3_vao->indices.size()/3; i++){
                                auto& gt = this->data.triangles[i];
                                for (int j = 0; j < 3; j++){this->sw3d3_vao->indices[(i*3)+j] = gt[j];}
                            }
                            for (size_t i = 0; i < this->sw3d3_vao->vertices.size(); i++){
                                auto& gv = this->data.vertices[i];
                                auto& sv = this->sw3d3_vao->vertices[i];
                                sv.co = glm::vec4(glm::vec3(gv.co), 1);
                                sv.color = glm::vec4(gv.color);
                                sv.normal = glm::vec3(gv.normal);
                                for (int j = 0; j < 8; j++){
                                    sv.uv[j] = glm::vec2(gv.uv[j]);
                                }
                            }
                        #endif
                        break;
                    }
                    default: ;
                }

            }

			void _drawmesh_opengl(const drawparams_t& dp) {
				#ifdef __GL45RW_HAS_OPENGL
                auto& gl = *(_glstuff::curgl);
                #if (defined(_GL45RW_GL2))
				if (dp.glbinding == nullptr){return;}
				auto& glb = dp.glbinding;
				auto* mesh = this;

				if (this->draw_method == TriangleMesh::drawmethod::static_draw){
				  gl.glEnableClientState(GL_VERTEX_ARRAY);
                    gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_vbo);
                    gl.glVertexPointer(3, GL_FLOAT, 0, nullptr);
                   gl.glEnableClientState(GL_NORMAL_ARRAY);
                    gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_nbo);
                    gl.glNormalPointer(GL_FLOAT, 0, nullptr);
                   gl.glEnableClientState(GL_COLOR_ARRAY);
                    gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_cbo);
                    gl.glColorPointer(4, GL_FLOAT, 0, nullptr);
                   gl.glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                   for (int i = 0; i < 8; i++){
                   	gl.glClientActiveTexture(GL_TEXTURE0 + i);
                    gl.glBindBuffer(GL_ARRAY_BUFFER, this->gl2_uvbo[i]);
                    gl.glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
                   }

                   gl.glDrawArrays(GL_TRIANGLES, 0, this->data.triangles.size() * 3);
                   gl.glBindBuffer(GL_ARRAY_BUFFER, 0);

				}
				else {
					gl.glBegin(GL_TRIANGLES);
					for (const auto& tri : mesh->data.triangles) {
						for (int tvn = 0; tvn < 3; tvn++) {
							const auto& vertex = mesh->data.vertices[tri[tvn]];


							//glTexCoord2f(0.5f, 0.25f);

							gl.glNormal3fv(&(vertex.normal[0]));
							gl.glColor4fv(&(vertex.color[0]));

							gl.glMultiTexCoord2fv(GL_TEXTURE0, glm::value_ptr(vertex.uv[0]));
							gl.glMultiTexCoord2fv(GL_TEXTURE1, glm::value_ptr(vertex.uv[1]));
							gl.glMultiTexCoord2fv(GL_TEXTURE2, glm::value_ptr(vertex.uv[2]));
							gl.glMultiTexCoord2fv(GL_TEXTURE3, glm::value_ptr(vertex.uv[3]));
							gl.glMultiTexCoord2fv(GL_TEXTURE4, glm::value_ptr(vertex.uv[4]));
							gl.glMultiTexCoord2fv(GL_TEXTURE5, glm::value_ptr(vertex.uv[5]));
							gl.glMultiTexCoord2fv(GL_TEXTURE6, glm::value_ptr(vertex.uv[6]));
							gl.glMultiTexCoord2fv(GL_TEXTURE7, glm::value_ptr(vertex.uv[7]));

							gl.glVertex3fv(&(vertex.co[0]));
						}
					}
					gl.glEnd();
				}
				#elif (defined(_GL45RW_GLES3))
				gl.glBindVertexArray(this->gles3_vao);
				gl.glDrawArrays(GL_TRIANGLES, 0, this->data.triangles.size() * 3);
				gl.glBindVertexArray(0);
				#endif
				#endif
			}

            void _drawmesh_sw3d2(const drawparams_t& dp){
				#ifdef GL45RW_WITH_SW3D2
				if (dp.sw3d2mat == nullptr){return;}
				auto& ctx = this->instance->sw3d2ctx[0];
				auto* mesh = this;
				if (mesh->draw_method == TriangleMesh::drawmethod::static_draw){
					ctx.srDrawElements(mesh->sw3d2_vao);
				}
				else {
					dj_sw3d20::ffvertex_t swtri[3] = {};
					for (const auto& tri : mesh->data.triangles) {
						for (int tvn = 0; tvn < 3; tvn++) {
							const auto& vertex = mesh->data.vertices[tri[tvn]];
							dj_sw3d20::ffvertex_t& swvertex = swtri[tvn];
							swvertex = dj_sw3d20::ffvertex_t{};
							swvertex.co = dj_sw3d20::fpvec3(vertex.co);
							swvertex.normal = dj_sw3d20::fpvec3(vertex.normal);
							//swvertex.color = dj_sw3d20::fpvec4(swcolors[tvn], 1);
							swvertex.color = dj_sw3d20::fpvec4(vertex.color);
							for (int uvn = 0; uvn < 8; uvn++) {
								swvertex.uv[uvn] = dj_sw3d20::fpvec2(vertex.uv[uvn]);
							}
						}
						ctx.srDrawElements(3, swtri);
					}
				}
				#endif
			}

			void _drawmesh_sw3d3(const drawparams_t& dp) {
			    #ifdef GL45RW_WITH_SW3D3
                if (dp._sw3d3_shader_data == nullptr){return;}
                auto* shdat = dp._sw3d3_shader_data;
                auto* ctx = this->instance->sw3d3ctx;
                ctx->drawparams.pixel_shader = this->instance->_sw3d3_drawmesh_ps->idx;
                this->instance->_sw3d3_drawmesh_ps->userdata = shdat;
                ctx->drawTrianglesIndexed(this->sw3d3_vao->idx);
			    #endif
			}

			void drawmesh(const drawparams_t& dp) {
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: this->_drawmesh_opengl(dp); break;
					case gfxapi::sw3d2: this->_drawmesh_sw3d2(dp); break;
					case gfxapi::sw3d3: this->_drawmesh_sw3d3(dp); break;
					default: ;
				}
			}
			void drawparams_from_amat(drawparams_t& dp){
				auto& ams = this->instance->R_GetActiveMaterialState();
				if (ams.mat == nullptr){return;}
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: {
						#ifdef __GL45RW_HAS_OPENGL
						dp.glbinding = ams.glbinding;
						#endif
						break;
					}
					case gfxapi::sw3d2: {
						#ifdef GL45RW_WITH_SW3D2
						dp.sw3d2mat = &ams.mat->_sw3d2mat;
						#endif
						break;
					}
					case gfxapi::sw3d3: {
					    #ifdef GL45RW_WITH_SW3D3
                        dp._sw3d3_shader_data = &ams.mat->_sw3d3shdat;
					    #endif
					    break;
					}
					default: ;
				}
			}
            virtual assettype asset_type() const {return assettype::trimesh;}
            virtual void onRenderContextCreate() {
                this->_create_renderdata();
                this->refresh();
            }
            virtual void onRenderContextDelete() {
                this->_destroy_renderdata();
            }
            virtual void onCreate() {this->onRenderContextCreate();}
            virtual void onDestroy() {this->delete_physdata(); this->onRenderContextDelete();}

            TriangleMesh(EngineInstance* _inst, const uid_t _newid, const meshdata& proto, const drawmethod dm=drawmethod::static_draw) : AssetBase(_inst, _newid), data(proto), draw_method(drawmethod::static_draw) {}

    };
    void trimeshdata_from_dmdmesh(TriangleMesh::meshdata& tmd, dmd::DMD_Mesh*& dmdmesh) {
        size_t nuvs = std::min(dmdmesh->uvmaps.size(), size_t(8U));
        size_t nverts = dmdmesh->vertices.size();
        size_t nidxs = dmdmesh->indices.size();
        size_t ntris = nidxs / 3U;

        tmd.active_uvmaps_mask = ~((uint8_t(255U) << nuvs));
        tmd._num_active_uvmaps = nuvs;

        tmd.vertices.resize(nverts);
        tmd.triangles.resize(ntris);

        for (size_t vn = 0; vn < nverts; vn++) {

            TriangleMesh::vertex_t& tmv = tmd.vertices[vn];

            //std::copy_n(&(dmdmesh->vertices[vn][0]), 3, &(tmv.co[0]));
            //std::copy_n(&(dmdmesh->normals[vn][0]), 3, &(tmv.normal[0]));
            //std::copy_n(&(dmdmesh->vertex_colors[vn][0]), 4, &(tmv.color[0]));
            tmv.co = _dmdvec2glm(dmdmesh->vertices[vn]);
            tmv.normal = _dmdvec2glm(dmdmesh->normals[vn]);
            tmv.color = fvec4_t(_dmdvec2glm(dmdmesh->vertex_colors[vn]), 1);
            //tmv.tangent = _dmdvec2glm(dmdmesh->tangents[vn]);
            //tmv.binormal = _dmdvec2glm(dmdmesh->binormals[vn]);
            std::fill_n(tmv.uv, 8, fvec2_t(0));
            for (size_t uvn = 0; uvn < nuvs; uvn++) {
                //std::copy_n(&(dmdmesh->uvmaps[uvn][vn][0]), 2, &(tmv.uv[uvn][0]));
                tmv.uv[uvn] = _dmdvec2glm(dmdmesh->uvmaps[uvn][vn]);
            }
        }

        for (size_t tn = 0; tn < ntris; tn++) {
            auto& tri = tmd.triangles[tn];
            const auto* dmdtri = &(dmdmesh->indices[tn*3]);
            std::copy_n(dmdtri, 3, &(tri[0]));
        }
        tmd.recalc_tangents_and_binormals();
    }
    class SceneGraphModel : public AssetBase {
        using AssetBase::AssetBase;
        private:
            uid_t _cur_meshid = 1, _cur_nodeid = 1;
        public:

            enum struct sgnodetype : int {
                empty = 0,
                mesh = 1
            };


            //dictionary<uid_t, sgmesh_t*> _meshes_byid;
            vectorlist<uid_t> owns_assets;
            /*
            sgmesh_t* new_mesh(const size_t nverts=50, const size_t ntris=0) {
                sgmesh_t* numesh = new sgmesh_t{};
                numesh->id = this->_cur_meshid;
                numesh->vertices.resize(nverts);
                numesh->triangles.resize(ntris);
                this->_meshes_byid[numesh->id] = numesh;
                this->_cur_meshid++;
                return numesh;
            }

            bool reupload_mesh(const uid_t idx) {
                if (idx == 0){return false;}
                sgmesh_t* mesh = this->_meshes_byid.getd(idx, nullptr);
                if (mesh == nullptr){return false;}
                return true;
            }

            bool deupload_mesh(const uid_t idx) {
                if (idx == 0){return false;}
                sgmesh_t* mesh = this->_meshes_byid.getd(idx, nullptr);
                if (mesh == nullptr){return false;}
                return true;
            }

            bool del_mesh(const uid_t idx) {
                sgmesh_t* dmesh = this->_meshes_byid.getd(idx, nullptr);
                if (dmesh == nullptr){return false;}
                this->deupload_mesh(idx);
                delete dmesh; dmesh = nullptr;
                this->_meshes_byid.erase(idx);
                return true;
            }
            */



            struct sgsubmdlent_t {uid_t mesh = 0, material = 0;};
            struct sgnodedata_t {
                uid_t id = 0, parent = 0;
                vectorlist<uid_t> children = {};

                sgnodetype nodetype = sgnodetype::empty;
                trs_t trs;
                vectorlist<sgsubmdlent_t> submdl_entries = {};
                bool visible = true;

                sgphystype physics_type = sgphystype::none;
                sgphysshape physics_shape = sgphysshape::none;
                uint32_t collision_groups = 1, collision_mask = 1;
                void trs_from_dmd(dmd::DMD_Node* dmdnode) {
                    if (dmdnode == nullptr){return;}
                    this->trs = trs_t(
                        _dmdvec2glm(dmdnode->local_position),
                        glm::transpose(fmat33_t{
                            _dmdvec2glm(dmdnode->local_orientation[0]),
                            _dmdvec2glm(dmdnode->local_orientation[1]),
                            _dmdvec2glm(dmdnode->local_orientation[2])
                        }),
                        _dmdvec2glm(dmdnode->local_scale)
                    );
                }
                sgnodedata_t() {}
                ~sgnodedata_t() = default;
            };

            dictionary<uid_t, sgnodedata_t*> _nodes_byid;
            uid_t _rootnode_idx = 0;
            sgnodedata_t* new_node(const uid_t parentidx=0) {
                auto* node = new sgnodedata_t{};
                node->id = this->_cur_nodeid;
                this->_cur_nodeid++;

                if (parentidx != node->id && parentidx != 0 && this->_nodes_byid.count(parentidx) > 0) {
                    node->parent = parentidx;
                    auto* parnode = this->_nodes_byid[parentidx];
                    parnode->children.push_back(node->id);
                }
                else if (parentidx == 0 && this->_rootnode_idx == 0) {
                    this->_rootnode_idx = node->id;
                    node->parent = 0;
                }
                this->_nodes_byid[node->id] = node;
                return node;
            }

            void del_node(const uid_t idx) {
                auto* node = this->_nodes_byid.getd(idx, nullptr);
                if (node == nullptr){return;}
                const auto children_copy = node->children;
                for (auto& cn : children_copy) {this->del_node(cn);}
                //children_copy.clear();
                node->children.clear();

                auto* parnode = this->_nodes_byid.getd(idx, nullptr);
                if (parnode != nullptr && parnode != node) {
                    parnode->children.eraseFirstOf(idx);
                }
                node->parent = 0;

                this->_nodes_byid.erase(idx);
                delete node; node = nullptr;
                if (this->_rootnode_idx != 0 && idx == this->_rootnode_idx) {
                    this->_rootnode_idx = 0;
                }
            }

            TriangleMesh* new_managed_trimesh(const TriangleMesh::meshdata& data, const TriangleMesh::drawmethod draw_method) {
                uid_t idx = this->instance->new_asset<TriangleMesh>(data, draw_method);
                auto* mesh = this->instance->get_asset<TriangleMesh>(idx);
                this->owns_assets.push_back(idx);
                return mesh;
            }

            Material* new_managed_material(const matprops_t mp=matprops_t{}) {
                uid_t idx = this->instance->new_asset<Material>(mp);
                auto* mat = this->instance->get_asset<Material>(idx);
                //mat->data = mp;
                this->owns_assets.push_back(idx);
                return mat;
            }

            Texture2D* new_managed_tex2D(const size_t nmips=1) {
                uid_t idx = this->instance->new_asset<Texture2D>(nmips);
                this->owns_assets.push_back(idx);
                return this->instance->get_asset<Texture2D>(idx);
            }
            bool del_managed_asset(const uid_t idx) {
                //size_t i = this->owns_assets.firstIndexOf(idx);
                //if (i == djutil::containers::npos){return false;}
                auto it = this->owns_assets.iterToFirstOf(idx);
                if (it == this->owns_assets.cend()){return false;}
                std::cout << "SceneGraphModel::del_managed_asset(" << idx << ");\n";
                this->instance->del_asset(idx);
                this->owns_assets.erase(it);
                return true;
            }

            void clear() {
                /*
                while (this->_meshes_byid.size() > 0){
                    this->del_mesh(this->_meshes_byid.begin()->first);
                }
                this->_meshes_byid.clear();
                */

                this->del_node(this->_rootnode_idx);
                this->_nodes_byid.clear();
                /*
                for (uid_t& maid : this->owns_assets) {
                    this->instance->del_asset(maid);
                }
                */
                while (this->owns_assets.size() > 0){
                    this->del_managed_asset(this->owns_assets.front());
                }

                this->owns_assets.clear(); this->owns_assets.shrink_to_fit();
                std::cout << "cleared sgmodel\n";
                std::cout.flush();
            }

            virtual assettype asset_type() const {return assettype::sgmodel;}

            virtual void onDestroy() {
                this->onRenderContextDelete();
                this->clear();
            }

            virtual void onCreate() {}

            virtual void onRenderContextCreate() {
                /*
                for (const auto& imp : this->_meshes_byid) {
                    this->reupload_mesh(imp.first);
                }
                */
            }

            virtual void onRenderContextDelete() {
                /*
                for (const auto& imp : this->_meshes_byid) {
                    this->deupload_mesh(imp.first);
                }
                */
            }

            void _merge_dmdlf(dmd::DMDFile& dmdlf) {
                //dmdlf.ensuresAllDataInLists();
                if (this->_rootnode_idx == 0){
                    auto* nroot = this->new_node(0);
                    nroot->nodetype = sgnodetype::empty;
                    nroot->trs_from_dmd(dmdlf.dmdnodes[0]);
                }

                std::unordered_map<dmd::dataid_t, Texture2D*> teximgmap = {};
                for (dmd::DMD_TexImage*& dmdtex : dmdlf.dmdteximages) {
                    auto* tex = this->new_managed_tex2D(1);
                    teximgmap[dmdtex->uid] = tex;
                    for (size_t mipn = 0; mipn < 1; mipn++) {
                        auto& dmdmip = dmdtex->images[mipn];
                        auto& mip = tex->getMipLevel(mipn);
                        mip.from_dmdimage(dmdmip);
                        tex->reupload_texture(mipn, true);
                    }

                }

                std::unordered_map<dmd::dataid_t, Material*> matmap = {};
                for (dmd::DMD_Material*& dmdmat : dmdlf.dmdmaterials) {
                    matprops_t mp = {};

                    mp.diffuse.alpha = dmdmat->base_diffuse_alpha;
                    mp.diffuse.color = _dmdvec2glm(dmdmat->base_diffuse_color);
                    mp.diffuse.intensity = dmdmat->base_diffuse_intensity;
                    mp.diffuse.model = mat_diffuse_model::none;

                    mp.specular.alpha = dmdmat->base_specular_alpha;
                    mp.specular.color = _dmdvec2glm(dmdmat->base_specular_color);
                    mp.specular.intensity = dmdmat->base_specular_intensity;
                    mp.specular.hardness = dmdmat->base_specular_hardness*511.0f;

                    mp.vertex_color_light.use = true;
                    mp.vertex_color_light.amount = fvec4_t(1);

                    mp.shadeless = dmdmat->shadeless;

                    mp.emit_intensity = dmdmat->base_emit_amount;

                    switch (dmdmat->facecull) {
                        case dmd::facecullmode::back: mp.face_culling_mode = cullface::back; break;
                        case dmd::facecullmode::front: mp.face_culling_mode = cullface::front; break;
                        default: mp.face_culling_mode = cullface::disabled;
                    }

                    switch (dmdmat->blendtype) {
                        case dmd::matblend::none: mp.blendtype = matblendtype::opaque; break;
                        case dmd::matblend::alpha_blend: mp.blendtype = matblendtype::alpha_blend; break;
                        case dmd::matblend::add: mp.blendtype = matblendtype::add; break;
                        default: ;
                    }

                    for (size_t tsn = 0; tsn < 8; tsn++) {
                        dmd::dmd_texslot_t& dmdts = dmdmat->texture_slots[tsn];
                        matprops_t::_textureslot& ts = mp.texture_slots[tsn];
                        ts.use = false;
                        if (!dmdts.use){continue;}
                        dmd::DMD_TexData* dmdtd = dmdts.texdata;

                        ts.affects.diffuse_color.use = dmdtd->affects_diffuse_color.use;
                        ts.affects.diffuse_color.amount = fvec3_t(dmdtd->affects_diffuse_color.value);

                        ts.affects.diffuse_alpha.use = dmdtd->affects_diffuse_alpha.use;
                        ts.affects.diffuse_alpha.amount = dmdtd->affects_diffuse_alpha.value;

                        ts.affects.texcoord_offset.use = dmdtd->affects_texcoord_offset.use;
                        ts.affects.texcoord_offset.amount = fvec3_t(dmdtd->affects_texcoord_offset.value);

                        ts.affects.specular_color.use = dmdtd->affects_specular_color.use;
                        ts.affects.specular_color.amount = fvec3_t(dmdtd->affects_specular_color.value);

                        ts.affects.ambient_color.use = dmdtd->affects_ambient_color.use;
                        ts.affects.ambient_color.amount = fvec3_t(dmdtd->affects_ambient_color.value);

                        ts.affects.normal.use = dmdtd->affects_normal_vector.use;
                        ts.affects.normal.amount = fvec3_t(dmdtd->affects_normal_vector.value);

                        ts.affects.emit_intensity.use = dmdtd->affects_emit_amount.use;
                        ts.affects.emit_intensity.amount = dmdtd->affects_emit_amount.value;

                        ts.bump_space = (bumpmapspace)(int)(dmdtd->bumpspace);

                        ts.uvmapno = dmdtd->uvmap_index;

                        ts.use_rgb2intensity = dmdtd->use_rgb2intensity;
                        ts.rgb2intensity_color = _dmdvec2glm(dmdtd->rgb2intensity_color);

                        switch (dmdtd->texsource){
                            case dmd::texsrc::image: ts.type = textype::image; break;
                            case dmd::texsrc::equirect: ts.type = textype::envmap; ts.envtype = envmaptype::equirect; break;
                            default: continue;
                        }
                        ts.use = dmdts.use;
                        switch (dmdtd->blendtype) {
                            case dmd::texblend::add: ts.blendtype = texblendtype::add; break;
                            case dmd::texblend::mul: ts.blendtype = texblendtype::mul; break;
                            default: ts.blendtype = texblendtype::mix;
                        }

                        switch (dmdtd->maptype) {
                            case dmd::texmap::mirror_ball_reflection: ts.maptype = texmaptype::reflection; break;
                            case dmd::texmap::view: ts.maptype = texmaptype::view; break;
                            default: ts.maptype = texmaptype::uvmap;
                        }

                        if (dmdtd->texfx.has_fx) {
                            auto& dmdtfx = dmdtd->texfx;
                            auto& texfx = ts.texfx;
                            texfx.has_fx = true;
                            texfx.fxcat = dmdtfx.fxcat;
                            texfx.fxnum = dmdtfx.fxnum;
                            if (texfx.fxcat == 0 && texfx.fxnum == 10) {
                                auto& fxargs = texfx.fxargs.texture.c0n10;
                                dmdtfx.getargval<fscalar_t>(fxargs.linv, "linv", 0, 3);
                            }
                            else if (texfx.fxcat == 0 && texfx.fxnum == 11) {
                                auto& fxargs = texfx.fxargs.texture.c0n11;
                                dmdtfx.getargval<fscalar_t>(fxargs.linv0, "linv0", 0, 3);
                                dmdtfx.getargval<fscalar_t>(fxargs.linv1, "linv1", 0, 3);
                                dmdtfx.getargval<fscalar_t>(fxargs.linv2, "linv2", 0, 3);
                                dmdtfx.getargval<fscalar_t>(fxargs.factors, "factors", 0, 3);
                                dmdtfx.getargval<int>(fxargs.blendtypes, "blendtypes", 0, 3);
                            }
                        }

                        Texture2D* t2d = teximgmap[dmdtd->teximg->uid];
                        ts.texture = uid_t(*t2d);
                    }

                    auto* mat = this->new_managed_material(mp);
                    matmap[dmdmat->uid] = mat;
                }

                std::unordered_map<dmd::dataid_t, TriangleMesh*> meshmap = {};
                for (dmd::DMD_Mesh*& dmdmesh : dmdlf.dmdmeshes) {
                    TriangleMesh::meshdata tmd = {};
                    trimeshdata_from_dmdmesh(tmd, dmdmesh);
                    auto* trimesh = this->new_managed_trimesh(tmd, TriangleMesh::drawmethod::static_draw);
                    meshmap[dmdmesh->uid] = trimesh;
                }

                std::unordered_map<dmd::dataid_t, sgnodedata_t*> nodemap = {};
                struct _noderecurs_t {
                    int level;
                    dmd::DMD_Node* dmdnode;
                    sgnodedata_t* node;
                };

                vectorlist<_noderecurs_t> nodeque = {_noderecurs_t{.level=0, .dmdnode=dmdlf.dmdnodes[0], .node=this->_nodes_byid[this->_rootnode_idx]}};
                while (nodeque.size() > 0) {
                    _noderecurs_t cur = nodeque.front(); nodeque.erase(nodeque.begin());
                    nodemap[cur.dmdnode->uid] = cur.node;
                    cur.node->trs_from_dmd(cur.dmdnode);
                    cur.node->visible = cur.dmdnode->visible;
                    cur.node->physics_shape = sgphysshape(int(cur.dmdnode->physics_shape));
                    cur.node->physics_type = (cur.node->physics_shape != sgphysshape::none ? sgphystype::static_collision : sgphystype::none);
                    if (cur.dmdnode->nodetype == dmd::_NODETYPE_MESH) {
                        cur.node->nodetype = sgnodetype::mesh;
                        dmd::DMD_SubModel* dmdsubmdl = cur.dmdnode->submdl;
                        for (dmd::DMD_Mesh*& dmesh : dmdsubmdl->meshes) {
                            cur.node->submdl_entries.push_back(sgsubmdlent_t{
                                .mesh=uid_t(meshmap[dmesh->uid][0]),
                                .material=uid_t(matmap[dmesh->material->uid][0])
                            });
                            if (cur.node->physics_shape == sgphysshape::trimesh){
                                auto* tm = meshmap[dmesh->uid];
                                std::cout << "[INFO] building trimesh collision grid for TriangleMesh " << (void*)tm << '\n';
                                tm->create_physdata();
                            }
                        }
                    }
                    else {
                        cur.node->nodetype = sgnodetype::empty;
                    }
                    for (dmd::DMD_Node*& dchild : cur.dmdnode->children) {
                        _noderecurs_t nrc = {};
                        nrc.level = cur.level + 1;
                        nrc.dmdnode = dchild;
                        nrc.node = this->new_node(nodemap[dchild->parent->uid]->id);
                        nodeque.push_back(nrc);
                    }
                }
                std::cout << "merged dmd into sgmodel\n";
            }

            void merge_dmd(const fspath_t& fsp) {
                virfs::vfistream hnd; hnd.open(*(this->instance->I_GetMainFS()), fsp);
                dmd::DMDFile _dmdlf; _dmdlf.load(hnd); hnd.close();
                this->_merge_dmdlf(_dmdlf);
            }

            struct enque_args {
                trs_t base_trs;
                bool
                    enque_renderobjs = true,
                    enque_physobjs = false
                ;
                uint32_t rgrpmask = 2;
            };

            void _merge_msdf(std::istream& msdf, const uid_t parentnode=0) {
                if (this->_rootnode_idx == 0){
                    auto* nroot = this->new_node(0);
                    nroot->nodetype = sgnodetype::empty;
                    nroot->trs = trs_t();
                }
                auto* pnode = this->_nodes_byid.getd(parentnode, this->_nodes_byid[this->_rootnode_idx]);
                auto* mnode = this->new_node(pnode->id);

                mnode->nodetype = sgnodetype::mesh;
                mnode->trs = trs_t();

                auto fstart = msdf.tellg();
                char id_rd[4] = {};
                struct {
                	uint32_t version, nindices, nvertices, nuvmaps, nvcolors, ntextures;
                } hdr = {};

                if (msdf.read(id_rd, 4).gcount() != 4 || memcmp(id_rd, "MSD\0", 4) != 0) {
                	throw std::runtime_error("Invalid MSD identifier!");
                }

                djutil::binio::unpack(&hdr.version, msdf, 6, true);

                TriangleMesh::meshdata tmd = {};

                size_t nuvs = hdr.nuvmaps;
                size_t nverts = hdr.nvertices;
                size_t nidxs = hdr.nindices;
                size_t ntris = nidxs / 3U;

                tmd.active_uvmaps_mask = ~((uint8_t(255U) << nuvs));
                tmd._num_active_uvmaps = nuvs;

                tmd.vertices.resize(nverts);
                tmd.triangles.resize(ntris);

                fvec2_t _dummy_uv;
                for (auto& tri : tmd.triangles) {
                	djutil::binio::unpack<unsigned int, uint32_t>(&(tri[0]), msdf, 3, true);
                }
                for (auto& vert : tmd.vertices) {djutil::binio::unpack<float, fscalar_t>(glm::value_ptr(vert.co), msdf, 3, true);}
                for (size_t i = 0; i < nuvs; i++) {
                    for (auto& vert : tmd.vertices) {
                        djutil::binio::unpack<fscalar_t, float>(glm::value_ptr((i < 8 ? vert.uv[i] : _dummy_uv)), msdf, 2, true);
                    }
                }
                for (auto& vert : tmd.vertices) {
                	fvec4_t cvc(0);
                	vert.color = fvec4_t(1);
                	for (size_t i = 0; i < hdr.nvcolors; i++) {
                		djutil::binio::unpack<fscalar_t, float>(glm::value_ptr(cvc), msdf, 4, true);
                	    vert.color *= cvc;
                	}
                }
                for (auto& vert : tmd.vertices) {djutil::binio::unpack<fscalar_t, float>(glm::value_ptr(vert.normal), msdf, 3, true);}
                matprops_t mp = {};
                mp.shadeless = false;
                mp.diffuse.alpha = mp.specular.alpha = 1;
                mp.diffuse.color = mp.specular.color = fvec3_t(1);

                mp.vertex_color_light.use = mp.vertex_color_paint.use = false;
                mp.vertex_color_light.amount = fvec4_t(0);

                mp.face_culling_mode = cullface::back;

                for (size_t i = 0; i < hdr.ntextures; i++)
                {
                	djutil::imaging::RasterImage rim;
                	msdf.seekg(6, std::ios::cur);
                	rim.load_rim(msdf);

                	if (i == 0) {
                	    auto* tex = this->new_managed_tex2D(1);
                	    tex->getMipLevel(0).from_rasterimage(rim).yflip();
                	    tex->reupload_texture(0, true);
                	    auto& mts = mp.texture_slots[0];
                	    mts.use = true;
                        mts.blendtype = texblendtype::mul;
                	    mts.affects.diffuse_color.use = mts.affects.specular_color.use = true;
                	    mts.affects.diffuse_color.amount = fvec3_t(1);
                	    mts.affects.specular_color.amount = fvec3_t(0.5f);
                	    mts.texture = uid_t(*tex);
                	}
                }

                auto* mat = this->new_managed_material(mp);
            	auto* tm = this->new_managed_trimesh(tmd, TriangleMesh::drawmethod::static_draw);
            	mnode->visible = true;
                mnode->submdl_entries.push_back(sgsubmdlent_t{.mesh=uid_t(*tm), .material=uid_t(*mat)});
            }

            void merge_msd(const fspath_t& fsp, const uid_t parentnode=0) {
            	virfs::vfistream hnd; hnd.open(*(this->instance->I_GetMainFS()), fsp);
            	this->_merge_msdf(hnd, parentnode);
            }

            enum struct modelfmt : int {
            	meshdata = 0,
            	djmodel = 1
            };

            struct merge_rawmeshes_entry {
                TriangleMesh::meshdata trimeshproto = {};
                matprops_t matproto = {};
                TriangleMesh::drawmethod tmdrawmethod = TriangleMesh::drawmethod::static_draw;
            };

            using merge_rawmeshes_list = std::vector<merge_rawmeshes_entry>;

            uid_t merge_rawmeshes(const merge_rawmeshes_list& submodels, const uid_t parentnode=0) {
                if (this->_rootnode_idx == 0){
                    auto* nroot = this->new_node(0);
                    nroot->nodetype = sgnodetype::empty;
                    nroot->trs = trs_t();
                }
                auto* pnode = this->_nodes_byid.getd(parentnode, this->_nodes_byid[this->_rootnode_idx]);
                auto* mnode = this->new_node(pnode->id);

                mnode->nodetype = sgnodetype::mesh;
                mnode->trs = trs_t();

                for (auto ent : submodels) {
                    auto* tm = this->new_managed_trimesh(ent.trimeshproto, ent.tmdrawmethod);
                    auto* mat = this->new_managed_material(ent.matproto);
                    mnode->submdl_entries.push_back(sgsubmdlent_t{
                        .mesh=uid_t(*tm),
                        .material=uid_t(*mat)
                    });
                }

                return mnode->id;
            }

            modelfmt merge_modelf(std::istream& f, const uid_t parentnode=0) {
            	auto start = f.tellg();
            	char id_rd[128] = {};
            	if (f.read(id_rd, 4).gcount() == 4 && !memcmp(id_rd, "MSD\0", 4)){
            		f.seekg(start);
            		this->_merge_msdf(f,parentnode);
            		return modelfmt::meshdata;
            	}
            	f.seekg(start);
            	memset(id_rd, '\0', 128);
            	if (f.read(id_rd, 8).gcount() == 8 && !memcmp(id_rd, "DJMODEL\0", 8)) {
            		f.seekg(start);
            		dmd::DMDFile _dmdlf; _dmdlf.load(f);
            	    this->_merge_dmdlf(_dmdlf);
            	    return modelfmt::djmodel;
            	}
            	throw std::runtime_error("Couldn't identify the model file format.");
            }

            modelfmt merge_model(const fspath_t& fsp, const uid_t parentnode=0) {
            	virfs::vfistream hnd; hnd.open(*(this->instance->I_GetMainFS()), fsp);
            	return this->merge_modelf(hnd, parentnode);
            }

            bool enque(enque_args& args) {
                if (this->_rootnode_idx == 0){return false;}
                _enque_node_internal iargs = {};
                iargs.args_ptr = &args;
                iargs.level = 0;
                iargs.node = this->_nodes_byid[this->_rootnode_idx];
                iargs.ctrs = args.base_trs;
                iargs.curmdl = fmat44_t(args.base_trs);
                this->_enque_node(iargs);
                return true;
            }

        private:
            struct _enque_node_internal {
                enque_args* args_ptr;
                int level;
                sgnodedata_t* node;
                trs_t ctrs;
                fmat44_t curmdl;
            };
            void _enque_node(_enque_node_internal& iargs) {
                enque_args& eargs = *(iargs.args_ptr);
                uint32_t rgrpmask = eargs.rgrpmask;
                sgnodedata_t* node = iargs.node;
                if (node == nullptr){return;}
                const trs_t cur_trs = iargs.ctrs.transform(node->trs);
                //const fmat44_t cur_mdlmat = iargs.curmdl * fmat44_t(node->trs);
                const fmat44_t cur_mdlmat(cur_trs);

                if (iargs.args_ptr->enque_physobjs && node->physics_shape == sgphysshape::trimesh && node->nodetype == sgnodetype::mesh){
                    size_t grpcount = 0;
                    EngineInstance::physgroup_t* pgrps[32] = {};
                    this->instance->P_GetPhysicsGroupsForMask(grpcount, pgrps, node->collision_groups);
                    if (grpcount != 0){
                        for (auto& smdlent : node->submdl_entries){
                            EngineInstance::physobj_t pobj = {};
                            pobj.trs = cur_trs;
                            pobj.pshape = sgphysshape::trimesh;
                            pobj.ptype = sgphystype::static_collision;
                            pobj.trimesh = this->instance->get_asset<TriangleMesh>(smdlent.mesh);
                            for (size_t gi = 0; gi < grpcount; gi++){
                                pgrps[gi]->push_back(pobj);
                            }
                        }
                    }
                }
                if (node->visible) {
                    size_t rgrpcount = 0; EngineInstance::rendergroup_t* rgrps[32] = {}, *cgrp = nullptr;
                    this->instance->R_GetRenderGroupsForMask(rgrpcount, rgrps, rgrpmask);
                    switch (node->nodetype) {
                        case sgnodetype::mesh: {
                            auto* ctx = this->instance;
                            for (auto& smdlent : node->submdl_entries) {
                                Material* mat = this->instance->get_asset<Material>(smdlent.material);
                                if (mat == nullptr){continue;}
                                for (size_t rgno = 0; rgno < rgrpcount; rgno++){
                                    cgrp = rgrps[rgno];
                                    EngineInstance::renderbuckets_t
                                        &rbs = (mat->data.blendtype == matblendtype::opaque ? cgrp->opaque_buckets : cgrp->translucent_buckets)
                                    ;
                                    auto* bucket = rbs._buckets.getd(smdlent.material, nullptr);
                                    if (bucket == nullptr){std::cout << "no bucket\n"; continue;}
                                    bucket->push_back(EngineInstance::_meshdraw_entry_t{});
                                    auto& mde = bucket->back();
                                    mde.model_matrix = cur_mdlmat;
                                    //mde.model_trs = cur_trs;
                                    mde.mesh_handle = smdlent.mesh;
                                }
                            }
                            break;
                        }
                        default: ;
                    }
                }
                for (const auto& chid : node->children) {
                    _enque_node_internal ciargs = iargs;
                    ciargs.level += 1;
                    ciargs.node = this->_nodes_byid.getd(chid, nullptr);
                    ciargs.ctrs = cur_trs;
                    ciargs.curmdl = cur_mdlmat;
                    _enque_node(ciargs);
                }
            }
    };

    void EngineInstance::R_DrawAllRenderBuckets(const fclock_t curtime, const uint32_t grpmask) {
        this->R_MakeCurrent();
        renderbuckets_t* buckets[2] = {};
        const fmat44_t
            base_mdl = this->matrices.model,
            view = this->matrices.view,
            view_inv = glm::inverse(view),
            projection = this->matrices.projection,
            projection_inv = glm::inverse(projection)
        ;
        size_t grpcount = 0; EngineInstance::rendergroup_t* grps[32] = {}, *cgrp = nullptr;
        this->R_GetRenderGroupsForMask(grpcount, grps, grpmask);

        cullface oldcull = this->R_GetFaceCullingMode();
        switch (this->R_GetCurrentVideoMode().renderer) {

            case gfxapi::opengl: {
                #ifdef __GL45RW_HAS_OPENGL
                    auto& gl = *(_glstuff::curgl);
                    for (size_t gno = 0; gno < grpcount; gno++) {
                        cgrp = grps[gno];
                        buckets[0] = &(cgrp->opaque_buckets);
                        buckets[1] = &(cgrp->translucent_buckets);
                        //gl.glShadeModel(GL_SMOOTH);
                        for (size_t bucket_idx = 0; bucket_idx < sizeof(buckets)/sizeof(buckets[0]); bucket_idx++) {
                            auto& bucket = buckets[bucket_idx][0];
                            gl.glEnable(GL_DEPTH_TEST);
                            gl.glActiveTexture(GL_TEXTURE0);
                            gl.glEnable(GL_TEXTURE_2D);

                            if (bucket_idx == 0){gl.glDisable(GL_BLEND);}
                            else if (bucket_idx == 1){gl.glEnable(GL_BLEND);}

                            for (auto& bp : bucket._buckets) {
                                Material* mat = this->get_asset<Material>(bp.first);
                                if (mat == nullptr || mat->_glsp == nullptr){continue;}
                                auto* ents = bp.second;
                                if (ents == nullptr || ents->size() == 0){continue;}
                                auto& ams = this->R_SetActiveMaterial(bp.first);

                                _glstuff::GLProgramBinding& _mbind = *ams.glbinding;

                                _mbind.setUniform(mat->_glsl_uniform_locations.projection_matrix, 1, &projection);
                                _mbind.setUniform(mat->_glsl_uniform_locations.projection_matrix_inv, 1, &projection_inv);
                                _mbind.setUniform(mat->_glsl_uniform_locations.view_matrix, 1, &view);
                                _mbind.setUniform(mat->_glsl_uniform_locations.view_matrix_inv, 1, &view_inv);
                                //_mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_color, 1, &mat->data.diffuse.color);
                                //_mbind.setUniform(mat->_glsl_uniform_locations.mat_diffuse_alpha, 1, &mat->data.diffuse.alpha);
                                _mbind.setUniform(mat->_glsl_uniform_locations.time, 1, &curtime);

                                for (auto& entry : *ents) {
                                    auto* mesh = this->get_asset<TriangleMesh>(entry.mesh_handle);
                                    const fmat44_t final_mdlmat = entry.model_matrix * base_mdl;
                                    _mbind.setUniform(mat->_glsl_uniform_locations.model_matrix, 1, &final_mdlmat);

                                    mesh->_drawmesh_opengl(TriangleMesh::drawparams_t{.glbinding=&_mbind});

                                }
                                this->R_SetActiveMaterial(0);
                            }
                        }
                        gl.glDisable(GL_BLEND);
                        gl.glDepthMask(1);
                    }
                #endif
                break;
            }
            case gfxapi::sw3d2: {
                #ifdef GL45RW_WITH_SW3D2
                auto& ctx = *(this->sw3d2ctx);
                dj_sw3d20::ffvertex_t swtri[3] = {};
                dj_sw3d20::fpmat3 swcolors(1);
                ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX);
                 ctx.srTopMatrix() = view;
                ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX);
                 ctx.srTopMatrix() = projection;

                for (size_t gno = 0; gno < grpcount; gno++) {
                    cgrp = grps[gno];
                    buckets[0] = &(cgrp->opaque_buckets);
                    buckets[1] = &(cgrp->translucent_buckets);
                    for (size_t bucket_idx = 0; bucket_idx < sizeof(buckets)/sizeof(buckets[0]); bucket_idx++) {
                        auto& bucket = buckets[bucket_idx][0];
                        ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, true);
                        ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, true);
                        for (auto& bp : bucket._buckets) {
                            Material* mat = this->get_asset<Material>(bp.first);
                            if (mat == nullptr){continue;}
                            auto* ents = bp.second;
                            if (ents == nullptr || ents->size() == 0){continue;}
                            for (int tsno = 0; tsno < 8; tsno++){
                                auto& smts = mat->_sw3d2mat.texture_slots[tsno];
                                auto& gmts = mat->data.texture_slots[tsno];
                                auto& gtexfx = gmts.texfx;
                                if (gtexfx.has_fx){
                                    if (gtexfx.fxcat == 0 && gtexfx.fxnum == 10){
                                        const float* _linv = gtexfx.fxargs.texture.c0n10.linv;
                                        for (int di = 0; di < 3; di++){
                                            smts.texcoord_offset[di] = (_linv[di] * curtime);
                                        }
                                    }
                                    else if (gtexfx.fxcat == 0 && gtexfx.fxnum == 11){
                                        const float* _linv0 = gtexfx.fxargs.texture.c0n11.linv0;
                                        const float* _linv1 = gtexfx.fxargs.texture.c0n11.linv1;
                                        const float* _linv2 = gtexfx.fxargs.texture.c0n11.linv2;
                                        for (int di = 0; di < 3; di++){
                                            smts.texcoord_offset[di] = ((_linv0[di]) * curtime);
                                        }
                                    }
                                }
                            }
                            auto& ams = this->R_SetActiveMaterial(bp.first);
                            for (auto& entry : *ents) {
                                auto* mesh = this->get_asset<TriangleMesh>(entry.mesh_handle);
                                const fmat44_t final_mdlmat = entry.model_matrix * base_mdl;
                                ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX);
                                 ctx.srTopMatrix() = final_mdlmat;
                                mesh->_drawmesh_sw3d2(TriangleMesh::drawparams_t{.sw3d2mat=&mat->_sw3d2mat});
                            }
                        }
                    }
                }
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DEPTH_TEST, true);
                ctx.srSetEnabled(dj_sw3d20::enum_t::CAP_DRAW_DEPTH_BUFFER, true);
                #endif
                break;
            }
            case gfxapi::sw3d3: {
                #ifdef GL45RW_WITH_SW3D3
                auto* ctx = this->sw3d3ctx;
                ctx->matrices.view = glm::mat4(view);
                //ctx->matrices.model = glm::mat4(model);
                ctx->matrices.projection = glm::mat4(projection);
                for (size_t gno = 0; gno < grpcount; gno++) {
                    cgrp = grps[gno];
                    buckets[0] = &(cgrp->opaque_buckets);
                    buckets[1] = &(cgrp->translucent_buckets);
                    for (size_t bucket_idx = 0; bucket_idx < sizeof(buckets)/sizeof(buckets[0]); bucket_idx++) {
                        auto& bucket = buckets[bucket_idx][0];

                        for (auto& bp : bucket._buckets) {
                            ctx->drawparams.depthfunc = sw3d3::depthfunc_e::less;
                            ctx->drawparams.depth_mask = true;
                            Material* mat = this->get_asset<Material>(bp.first);
                            if (mat == nullptr){continue;}
                            auto* ents = bp.second;
                            if (ents == nullptr || ents->size() == 0){continue;}
                            auto& ams = this->R_SetActiveMaterial(bp.first);
                            if (mat->data.blendtype == matblendtype::add){
                                ctx->drawparams.depthfunc = sw3d3::depthfunc_e::less;
                                ctx->drawparams.depth_mask = false;
                            }
                            for (auto& entry : *ents) {
                                auto* mesh = this->get_asset<TriangleMesh>(entry.mesh_handle);
                                const fmat44_t final_mdlmat = entry.model_matrix * base_mdl;
                                ctx->matrices.model = glm::mat4(final_mdlmat);

                                mesh->_drawmesh_sw3d3(TriangleMesh::drawparams_t{._sw3d3_shader_data=&mat->_sw3d3shdat});
                            }
                        }
                    }
                }
                ctx->drawparams.depthfunc = sw3d3::depthfunc_e::less;
                ctx->drawparams.depth_mask = true;
                #endif
                break;
            }
            default: ;
        }
        this->R_SetFaceCullingMode(oldcull);
    }

    #ifdef GL45RW_WITH_LUAJIT

    class LuaException : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
    class LuaRuntimeError : public LuaException {
        using LuaException::LuaException;
    };

    class LuaEngineBase;
    dictionary<lua_State*, LuaEngineBase*>* _lua_engines_bystate = nullptr;

    template <class LET=LuaEngineBase> LET* E_GetLuaEngineForState(lua_State* L);

    class LuaEngineBase : public AssetBase {
        using AssetBase::AssetBase;

        private:
            lua_State* _lua = nullptr;
            //std::string _eid_str = "0", _uid_str = "0";
            static int _gl45rw_lua_apicall_test(lua_State* L) {
                auto& self = *E_GetLuaEngineForState(L);
                std::cout << "[Lua] gl45rw.test() called from luaengine " << uid_t(self) << "!\n";
                return 0;
            }
            /*
            static int _gl45rw_lua_apicall_L_GetCurrentLuaEngineID(lua_State* L) {
                auto& self = *E_GetLuaEngineForState(L);

                return 1;
            }
            */
        public:

            lua_State* const& luastate = _lua;
            operator lua_State*() {return this->_lua;}

            virtual assettype asset_type() const {return assettype::luaengine;}

            template <glm::length_t M, glm::length_t N, class T, glm::qualifier Q=glm::defaultp>
            glm::mat<M,N,T,Q> tomatrix(const int idx, const int startindex=1){
                const glm::length_t S = M*N;
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto* L = this->_lua;
                if (!lua_istable(L, idx)){throw LuaException("Lua value is not a table");}
                else if (lua_objlen(L, idx) != S){
                    std::stringstream errss;
                    errss << "vector size mismatch, expected a table with " << S << "elements.";
                    throw LuaException(errss.str());
                }
                glm::mat<N,M,T,Q> retmat;
                auto* components = &reinterpret_cast<T&>(retmat);
                for (int i = startindex, j = 0; j < S; i++, j++){
                    lua_pushinteger(L, i); lua_gettable(L, idx);
                    components[j] = T(lua_tonumber(L, -1)); lua_pop(L, 1);
                }
                return glm::transpose(retmat);
            }

            template <glm::length_t M, glm::length_t N, class T, glm::qualifier Q=glm::defaultp>
            void frommatrix(const glm::mat<M,N,T,Q>& m, const int idx=-1, const int startindex=1) {
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto* L = this->_lua;
                if (!lua_istable(L, -1)){throw LuaException("Lua value is not a table");}
                const glm::mat<N,M,T,Q> tm = glm::transpose(m);
                lua_pushvalue(L, idx);
                 lua_pushstring(L, "_nrows");
                 lua_pushinteger(L, N);
                 lua_settable(L, -3);

                 lua_pushstring(L, "_ncols");
                 lua_pushinteger(L, M);
                 lua_settable(L, -3);
                const int S = M*N;
                const T* vals = &tm[0][0];
                for (int i = startindex, j = 0; j < S; i++, j++) {
                    lua_pushinteger(L, i); lua_pushnumber(L, vals[j]);
                    lua_settable(L, -3);
                }
                lua_pop(L, 1);
            }

            template <glm::length_t S, class T, glm::qualifier Q=glm::defaultp>
            glm::vec<S,T,Q> tovector(const int idx, const int startindex=1) {
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto* L = this->_lua;
                if (!lua_istable(L, idx)){throw LuaException("Lua value is not a table");}
                else if (lua_objlen(L, idx) != S){
                    std::stringstream errss;
                    errss << "vector size mismatch, expected a table with " << S << "elements.";
                    throw LuaException(errss.str());
                }
                glm::vec<S,T,Q> retvec;
                for (int i = startindex, j = 0; j < S; i++, j++){
                    lua_pushinteger(L, i); lua_gettable(L, idx);
                    retvec[j] = T(lua_tonumber(L, -1)); lua_pop(L, 1);
                }
                return retvec;
            }

            template <glm::length_t S, class T, glm::qualifier Q=glm::defaultp>
            void fromvector(const glm::vec<S,T,Q>& vec, const int idx=-1, const int startindex=1) {
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto* L = this->_lua;
                if (!lua_istable(L, -1)){throw LuaException("Lua value is not a table");}
                lua_pushvalue(L, idx);
                for (int i = startindex, j = 0; j < S; i++, j++){
                    lua_pushinteger(L, i); lua_pushnumber(L, vec[j]);
                    lua_settable(L, -3);
                }
                lua_pop(L, 1);
            }

            uid_t touid(const int idx=-1){
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto resptr = (const uid_t*)lua_tostring(this->_lua, idx);
                return (resptr == nullptr ? uid_t(0) : *resptr);
            }

            template <class AT>
            AT* toassetptr(const int idx=-1) {
                return this->instance->get_asset<AT>(this->touid(idx));
            }

            int pcall(const int nargs, const int nresults=LUA_MULTRET) {
                if (this->_lua == nullptr){throw LuaException("Lua state is not initialized!");}
                auto* L = this->_lua;
                int oldtop = lua_gettop(L)-nargs-1;
                int callres;
                if ((callres = lua_pcall(L, nargs, nresults, 0)) != 0){
                    throw LuaRuntimeError(lua_tostring(L, -1));
                }
                int numreturned = lua_gettop(L)-oldtop;
                return numreturned;
            }

            virtual void onCreate() {
                this->_lua = luaL_newstate();
                if (this->_lua == nullptr){throw LuaException("Couldn't create a lua state!");}
                //auto** edataptr = reinterpret_cast<LuaEngineBase**>(lua_getextraspace(this->_lua));
                //edataptr[0] = this;
                gl45rw::_lua_engines_bystate[0][this->_lua] = this;
                luaL_openlibs(this->_lua);
                /*
                luaL_requiref(this->_lua, "gl45rw", [](lua_State* L)->int{
                    const luaL_Reg gl45rw_apicalls[] = {
                        {"test", _gl45rw_lua_apicall_test},
                        {nullptr, nullptr}
                    };
                    luaL_newlib(L, gl45rw_apicalls);
                    return 1;
                }, 1);
                */
                const luaL_Reg gl45rw_apicalls[] = {
                    {"test", _gl45rw_lua_apicall_test},
                    {"L_GetLuaEngineID", [](lua_State* L)->int{
                        auto& self = *E_GetLuaEngineForState(L);
                        lua_pushstring(L, std::to_string(uid_t(self)).c_str());
                        return 1;
                    }},
                    {"I_GetInstanceID", [](lua_State* L)->int{
                        auto& self = *E_GetLuaEngineForState(L);
                        lua_pushstring(L, std::to_string(self.instance->I_GetInstanceID()).c_str());
                        return 1;
                    }},
                    {"C_ExecuteString", [](lua_State* L)->int{
                        auto& self = *E_GetLuaEngineForState(L);
                        int nargs = lua_gettop(L);
                        if (nargs != 1){lua_pushstring(L, "wrong # of args!"); lua_error(L);}
                        if (!lua_isstring(L, 1)){lua_pushstring(L, "invalid argument"); lua_error(L);}
                        std::stringstream sh; sh.str(lua_tostring(L, 1));
                        while (sh) {
                            std::string cmdn = "", cmda = "";
                            sh >> cmdn; sh.get();
                            std::getline(sh, cmda);
                            if (cmda.size() > 0 && cmda.back() == '\r'){cmda.pop_back();}
                            if (cmdn == ""){continue;}
                            CCMDInvokationExtraData _edata = {};
                            self.instance->C_ExecCommand(djutil::ezstr::utf8_to_utf32(cmdn), djutil::ezstr::utf8_to_utf32(cmda), _edata);
                        }
                        return 0;
                    }},
                    {"FS_ReadMemberFile", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        auto* mainfs = self->I_GetMainFS();
                        int nargs = lua_gettop(L);
                        const char* fsp8 = nullptr;
                        if (nargs != 1){
                            lua_pushfstring(L, "bad # of arguments! (got %i, expected 1)", nargs);
                            return lua_error(L);
                        }
                        else if (mainfs == nullptr){
                            lua_pushstring(L, "main filesystem not initialized");
                            return lua_error(L);
                        }
                        else if ((fsp8 = lua_tostring(L, 1)) == nullptr){
                            lua_pushstring(L, "cannot convert arg 1 to a string");
                            return lua_error(L);
                        }
                        try {
                            const std::u32string fsp = djutil::ezstr::utf8_to_utf32(fsp8);
                            djutil::virfs::vfistream hnd(*mainfs, fsp, std::ios::in | std::ios::binary | std::ios::ate);
                            const size_t sz = hnd.tellg(); hnd.seekg(0);
                            std::string rd_s; rd_s.resize(sz);
                            if (rd_s.size() == 0){
                                hnd.close();
                                lua_pushstring(L, "");
                                return 1;
                            }
                            else {
                                hnd.read(&rd_s.front(), sz);
                                hnd.close();
                                //lua_settop(L, 0);
                                lua_pushlstring(L, &rd_s.front(), sz);
                                return 1;
                            }
                        }
                        catch (const std::exception& e){
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"FS_GetDirectoryListing", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "missing first argument."); return lua_error(L);}
                        try {
                            djutil::virfs::fsdirlist_t
                                _dirs = {},
                                _files = {},
                                *_lists[2] = {&_dirs, &_files}
                            ;
                            auto* fs = self->I_GetMainFS();
                            if (fs == nullptr){throw LuaException("main filesystem is not initialized!");}
                            const char* fsp8 = lua_tostring(L, 1);
                            if (fsp8 == nullptr){throw LuaException("cannot convert arg 1 to a string.");}
                            fs->listdir(_dirs, _files, djutil::ezstr::utf8_to_utf32(fsp8));
                            for (int l = 0; l < 2; l++){
                                auto& list = _lists[l][0];
                                lua_createtable(L, list.size(), 0);
                                int lidx = 0;
                                for (auto& elem : list) {
                                    const auto fullname = djutil::ezstr::utf32_to_utf8(elem.fullname);
                                    const auto relname = djutil::ezstr::utf32_to_utf8(elem.relname);
                                    lua_pushinteger(L, lidx+1);
                                     lua_createtable(L, 0, 0);
                                       lua_pushstring(L, "fullname");
                                       lua_pushstring(L, fullname.c_str());
                                       lua_settable(L, -3);

                                       lua_pushstring(L, "relname");
                                       lua_pushstring(L, relname.c_str());
                                       lua_settable(L, -3);
                                    lua_settable(L, -3);
                                    lidx++;
                                }
                            }
                            return 2;
                        }
                        catch (const std::exception& e){
                            return luaL_error(L, e.what());
                        }
                    }},
                    {"T_Vec3ConversionTest1", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){return 0;}
                        fvec3_t vec = luaengine->tovector<3,fscalar_t>(1);
                        auto& cd = self->C_GetConsoleData();
                        cd.lines.push_back(djutil::ezstr::utf8_to_utf32(glm::to_string(vec)));
                        return 0;
                    }},
                    {"M_NormalizeVec3D", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "need 1 argument."); return lua_error(L);}
                        try {
                            fvec3_t vec = luaengine->tovector<3,fscalar_t>(1);
                            lua_createtable(L, 3, 0);
                            luaengine->fromvector(glm::normalize(vec));
                            return 1;
                        }
                        catch (const LuaException& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"M_NormalizeVec3D_IP", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = luaengine->instance;
                        int nargs = lua_gettop(L);
                        if (nargs != 1){
                            lua_pushstring(L, "need 1 argument");
                            return lua_error(L);
                        }
                        try {
                            fvec3_t vec = glm::normalize(luaengine->tovector<3,fscalar_t>(1));
                            luaengine->fromvector(vec, 1);
                            return 0;
                        }
                        catch (const LuaException& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"M_Mat4x4Test1", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = luaengine->instance;
                        int nargs = lua_gettop(L);
                        if (nargs != 1){
                            lua_pushstring(L, "need 1 argument");
                            return lua_error(L);
                        }
                        try {
                            fmat44_t m44 = luaengine->tomatrix<4,4,fscalar_t>(1);
                            auto& cd = self->C_GetConsoleData();
                            cd.lines.push_back(djutil::ezstr::utf8_to_utf32("m44 = " + glm::to_string(m44)));
                            std::cout << "m44 = " << glm::to_string(m44) << "\n";
                            return 0;
                        }
                        catch (const LuaException& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"L_LoadModule", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        auto* mainfs = self->I_GetMainFS();
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "expected at least 1 argument!"); return lua_error(L);}
                        const char* fsp8c = lua_tostring(L, 1);
                        if (fsp8c == nullptr){lua_pushstring(L, "cannot convert arg1 to a string."); return lua_error(L);}
                        lua_getglobal(L, "package");
                        lua_getfield(L, -1, "loaded");
                        lua_getfield(L, -1, "gl45rw");
                        lua_getfield(L, -1, "_loaded_modules");
                        lua_getfield(L, -1, fsp8c); //check if a module by the name of fsp8c already exists, if so, return that.
                        if (!lua_isnil(L, -1)){std::cout << "is not nil\n"; return 1;}
                        lua_pop(L, 1); //pop off the nil value
                        if (mainfs == nullptr){lua_pushstring(L, "main fs not initialized!"); return lua_error(L);}

                        lua_newtable(L); //module table
                        lua_pushstring(L, fsp8c);
                        lua_setfield(L, -2, "__PATH__");
                        lua_pushvalue(L, -1);
                        lua_setfield(L, -3, fsp8c); //register module

                        std::string script = "";

                        try {
                            const std::u32string fsp = djutil::ezstr::utf8_to_utf32(fsp8c);
                            djutil::virfs::vfistream hnd(*mainfs, fsp, std::ios::in | std::ios::binary | std::ios::ate);
                            script.resize(hnd.tellg()); hnd.seekg(0);
                            hnd.read(&script.front(), script.size());
                            hnd.close();
                            std::cout << script << '\n';
                        }
                        catch (const std::runtime_error& e){
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                        int loadres = 0;
                        if ((loadres = luaL_loadstring(L, script.c_str())) != 0){
                            //lua_pushstring(L, "failed to load chunk");
                            return lua_error(L);
                        }
                        lua_pushvalue(L, -2); //push the module as the first argument.
                        lua_call(L, 1, 0); //invoke the chunk with the module table as its argument.
                        //lua_pop(L, 1); //pop the chunk, leaving the module table to return
                        //lua_pushvalue(L, -2);
                        return 1; //return the module table.
                    }},
                    {"C_ConsolePrint", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);

                        int nargs = lua_gettop(L);
                        if (nargs < 1){return 0;}
                        //std::string str = lua_tostring(L, 1);

                        auto& cd = self->C_GetConsoleData();
                        std::stringstream ss;
                        for (int i = 1; i <= nargs; i++){
                            ss << lua_tostring(L, i);
                            if (i < nargs){ss << " ";}
                        }
                        ss << "\n";
                        vectorlist<std::u32string> splits = {};
                        djutil::ezstr::splitlines<char32_t>(
                            splits,
                            djutil::ezstr::utf8_to_utf32(ss.str())
                        );

                        for (auto& ln : splits){
                            if (ln.size() == 0){continue;}
                            cd.lines.push_back(ln);
                        }
                        return 0;
                    }},
                    {"R_ClearBuffers", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        bool clearcolbuf = true, clearzbuf = true;
                        if (nargs >= 1){clearcolbuf = lua_toboolean(L, 1);}
                        if (nargs >= 2){clearzbuf = lua_toboolean(L, 2);}
                        self->R_ClearBuffers(clearcolbuf, clearzbuf);
                        return 0;
                    }},
                    {"R_ClearAllRenderBuckets", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        self->R_ClearAllRenderBuckets();
                        return 0;
                    }},
                    {"R_DrawAllRenderBuckets", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        fclock_t _uptime = 0;
                        if (nargs >= 1){_uptime = lua_tonumber(L, 1);}
                        self->R_DrawAllRenderBuckets(_uptime);
                        return 0;
                    }},
                    {"I_CreateAsset", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "need atl. 1 arg"); return lua_error(L);}
                        const char* asset_type_s = lua_tostring(L, 1);
                        uid_t assetid = 0;
                        if (asset_type_s == nullptr){lua_pushstring(L, "arg 1 cannot be converted to string"); return lua_error(L);}
                        else if (!strcmp(asset_type_s, "sgmodel")) {
                            try {
                                assetid = self->new_asset<SceneGraphModel>();
                            }
                            catch (const std::exception& e){
                                lua_pushstring(L, e.what());
                                return lua_error(L);
                            }
                        }
                        else {
                            lua_pushfstring(L, "unknown asset type: %s", asset_type_s);
                            return lua_error(L);
                        }
                        lua_pushlstring(L, (const char*)&assetid, sizeof(assetid)); //push the raw byte reperesentation as a Lua string
                        return 1;
                    }},
                    {"I_DeleteAsset", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "needs 1 argument."); return lua_error(L);}
                        try {
                            lua_pushboolean(L, self->del_asset(luaengine->touid(1)));
                            return 1;
                        } catch (std::exception& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"I_SceneGraphModelClear", [](lua_State* L){
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "need 1 arg!"); return lua_error(L);}
                        AssetBase* _abase = luaengine->toassetptr<AssetBase>(1);
                        if (_abase == nullptr || _abase->asset_type() != assettype::sgmodel){lua_pushboolean(L, false); return 1;}
                        auto* asset = (SceneGraphModel*)_abase;
                        try {
                            asset->clear();
                        }
                        catch (const std::exception& e){
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                        lua_pushboolean(L, true);
                        return 1;
                    }},
                    {"I_SceneGraphModelMerge", [](lua_State* L){
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 2){lua_pushstring(L, "need 2 args!"); return lua_error(L);}

                        AssetBase* _abase = luaengine->toassetptr<AssetBase>(1);
                        if (_abase == nullptr || _abase->asset_type() != assettype::sgmodel){lua_pushboolean(L, false); return 1;}
                        auto* asset = (SceneGraphModel*)_abase;

                        const char* fsp8 = lua_tostring(L, 2);
                        if (fsp8 == nullptr){lua_pushboolean(L, false); return 1;}

                        try {
                            const std::u32string fsp = djutil::ezstr::utf8_to_utf32(fsp8);
                            asset->merge_model(fsp);
                        }
                        catch (const std::exception& e){
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                        lua_pushboolean(L, true);
                        return 1;
                    }},
                    {"R_EnqueSceneGraphModel", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "need atl. 1 arg!"); return lua_error(L);}
                        AssetBase* _abase = luaengine->toassetptr<AssetBase>(1);
                        if (_abase == nullptr || _abase->asset_type() != assettype::sgmodel){lua_pushboolean(L, false); return 1;}
                        auto* asset = (SceneGraphModel*)_abase;
                        try {
                            SceneGraphModel::enque_args eargs = {};
                            if (nargs >= 2){eargs.base_trs.pos = luaengine->tovector<3,fscalar_t>(2);}
                            if (nargs >= 3){eargs.base_trs.orn = luaengine->tomatrix<3,3,fscalar_t>(3);}
                            if (nargs >= 4){eargs.base_trs.scale = luaengine->tovector<3,fscalar_t>(4);}
                            if (nargs >= 5 && lua_istable(L, 5)){
                                ;
                            }
                            asset->enque(eargs);
                        }
                        catch (const std::exception& e){
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                        lua_pushboolean(L, true);
                        return 1;
                    }},
                    {"I_IsValidAssetID", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "needs 1 argument."); return lua_error(L);}
                        try {
                            lua_pushboolean(L, (luaengine->toassetptr<AssetBase>(1) != nullptr));
                            return 1;
                        } catch (std::exception& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {"I_GetAssetType", [](lua_State* L)->int{
                        auto* luaengine = gl45rw::E_GetLuaEngineForState(L);
                        auto* self = (luaengine->instance);
                        int nargs = lua_gettop(L);
                        if (nargs < 1){lua_pushstring(L, "needs 1 argument."); return lua_error(L);}
                        try {
                            assettype atype = self->get_asset_type(luaengine->touid(1));
                            auto atypei = size_t(atype);
                            if (atypei >= (sizeof(_GL45RW_ASSET_TYPE_NAMES)/sizeof(_GL45RW_ASSET_TYPE_NAMES[0]))){
                                atypei = 0;
                            }
                            lua_pushstring(L, _GL45RW_ASSET_TYPE_NAMES[atypei]);
                            return 1;
                        } catch (std::exception& e) {
                            lua_pushstring(L, e.what());
                            return lua_error(L);
                        }
                    }},
                    {nullptr, nullptr}
                };
                luaL_register(this->_lua, "gl45rw", gl45rw_apicalls);
                /*
                int oldtop = lua_gettop(this->_lua);
                lua_getglobal(this->_lua, "package");
                lua_getfield(this->_lua, -1, "loaded");
                lua_newtable(this->_lua);
                lua_setfield(this->_lua, -2, "_loaded_modules");
                lua_settop(this->_lua, oldtop);
                */
                luaL_dostring(this->_lua, R"""(
package.loaded.gl45rw._loaded_modules = {}
)""");
            }

            virtual void onDestroy() {
                if (this->_lua != nullptr) {
                    lua_close(this->_lua);
                    this->_lua = nullptr;
                    gl45rw::_lua_engines_bystate->popk(this->_lua);
                }
            }
    };

    template <class LET>
    LET* E_GetLuaEngineForState(lua_State* L) {
        if (gl45rw::_lua_engines_bystate == nullptr){return nullptr;}
        return reinterpret_cast<LET*>(gl45rw::_lua_engines_bystate->getd(L, nullptr));
    }
    #endif
/*
    const std::string
        _TCSYS_GLSL_COMMON_SRC = R"(#version 120
uniform sampler2D texture;
uniform vec4 basecolor;
uniform mat4 projection;
uniform mat4 viewinv;
uniform mat4 model;
varying vec2 v_uv;
)",
       _TCSYS_GLSL_VERTEX_SRC = R"(
void main() {
    mat4 mvp = projection * viewinv * model;
    gl_Position = mvp * vec4(gl_Vertex.xyz, 1.0);
    v_uv = gl_MultiTexCoord0.st;
}
)",
       _TCSYS_GLSL_FRAGMENT_SRC = R"(
void main() {
    gl_FragColor = basecolor * texture2D(texture, v_uv);
}
)";

*/

    class TouchControlSystem : public AssetBase {
		using AssetBase::AssetBase;
		public:
		    bool changed_window = false;
		    class TouchControlBase {
				private:
				    TouchControlSystem* _tcsys = nullptr;
				public:
				    std::string classname = "";
				    TouchControlSystem* const& tcsystem = _tcsys;
				    virtual bool contains_point(const fvec2_t& p) {return false;}

				    virtual void on_create() {
						;
					}

					virtual bool on_touch_event(EngineInstance::touchevent_t& tevent){
						return true;
					}

					virtual void on_update(const MainloopData& mld){}
					virtual void on_redraw(EngineInstance::calcmatrices_t& cm){}

					virtual void on_destroy() {
						;
					}
				    TouchControlBase(TouchControlSystem* t) : _tcsys(t) {t->_widgets.push_back(this);}
				    virtual ~TouchControlBase() {
						this->on_destroy();
						auto fit = std::find(this->_tcsys->_widgets.begin(), this->_tcsys->_widgets.end(), this);
						if (fit != this->_tcsys->_widgets.end()){this->_tcsys->_widgets.erase(fit);}
					}

			};
			class TestJoystick1 : TouchControlBase {
				using TouchControlBase::TouchControlBase;
				private:
				    bool pressed = false;
				    uid_t activating_touchid = 0;
				    TriangleMesh *backmesh = nullptr, *frontmesh = nullptr;
				    fvec2_t _cstickrelpos = {0,0};
				public:
				    Texture2D *bg_image = nullptr, *fg_image = nullptr;
				    fvec2_t origin = {0,0};
				    fscalar_t outer_radius = 0.375f, inner_radius = 0.075f;
				    std::u32string
				        px_ivar = U"", nx_ivar = U"",
				        py_ivar = U"", ny_ivar = U""
				    ;
				    virtual void on_create() {
						this->classname = "TestJoystick1";
						auto* tcs = this->tcsystem;
						TriangleMesh::meshdata btmd; btmd.prim_circle_xy(16, 1.0f);
						TriangleMesh::meshdata ftmd; ftmd.prim_circle_xy(16, 1.0f);

						this->backmesh = tcs->new_managed_asset<TriangleMesh>(btmd);
						this->frontmesh = tcs->new_managed_asset<TriangleMesh>(ftmd);
					}
					virtual bool on_touch_event(EngineInstance::touchevent_t& tevt){
						auto* tcs = this->tcsystem;
						switch (tevt.state){
							case EngineInstance::touchstate::down: {
								if (this->pressed){break;}
								this->pressed = true;
								this->activating_touchid = tevt.id;
								this->_cstickrelpos = tcs->rel2ndc(tevt.cxy)-this->origin;
								return true;
							}
							case EngineInstance::touchstate::up: {
								if (tevt.id != this->activating_touchid){break;}
								this->pressed = false;
								this->activating_touchid = 0;
								this->_cstickrelpos = fvec2_t(0);
								break;
							}
							case EngineInstance::touchstate::motion: {
								if (tevt.id != this->activating_touchid || !this->pressed){break;}
								this->_cstickrelpos = tcs->rel2ndc(tevt.cxy)-this->origin;
								break;
							}
							default: ;
						}
						return false;
					}
					virtual void on_destroy() {
						this->tcsystem->del_managed_asset(uid_t(*this->backmesh)); this->backmesh = nullptr;
						this->tcsystem->del_managed_asset(uid_t(*this->frontmesh)); this->frontmesh = nullptr;
					}
					void _updivar(const std::u32string& n, const fscalar_t& v){
						if (n.size() == 0){return;}
						CCMDInvokationExtraData ed = {};
						ed.ivar_invalue = glm::clamp(v, 0.0f, 1.0f);
						this->tcsystem->instance->C_ExecCommand(n, U"", ed);
					}
					virtual void on_update(const MainloopData& mld){
						if (this->tcsystem->changed_window){this->pressed = false; this->activating_touchid = 0; this->_cstickrelpos = fvec2_t(0);}
						fscalar_t maxdist = this->outer_radius - this->inner_radius;
						fscalar_t curdist = glm::length(this->_cstickrelpos);
						if (curdist > maxdist){
							this->_cstickrelpos = glm::normalize(this->_cstickrelpos)*maxdist;
						}
						fscalar_t xvalue = glm::clamp(this->_cstickrelpos.x/maxdist, -1.0f, 1.0f);
						fscalar_t yvalue = glm::clamp(this->_cstickrelpos.y/maxdist, -1.0f, 1.0f);

						this->_updivar(this->px_ivar, xvalue); this->_updivar(this->nx_ivar, -xvalue);
						this->_updivar(this->py_ivar, yvalue); this->_updivar(this->ny_ivar, -yvalue);

					}
					virtual bool contains_point(const fvec2_t& p) {
						return glm::length(p-(this->origin)) <= this->outer_radius;
					}
					virtual void on_redraw(EngineInstance::calcmatrices_t& cm) {
						auto* ei = this->tcsystem->instance;
						this->tcsystem->draw_ctrlmesh(this->backmesh, glm::translate(fvec3_t(this->origin, 0))*glm::scale(fvec3_t(this->outer_radius)), fvec4_t(fvec3_t(1.0f), 1.0f), this->bg_image);
						this->tcsystem->draw_ctrlmesh(this->frontmesh, glm::translate(fvec3_t(this->origin + this->_cstickrelpos, 0))*glm::scale(fvec3_t(this->inner_radius)), fvec4_t(fvec3_t(0.75f + (int(this->pressed) * 0.25f)), 1.0f), this->fg_image);
					}
			};
			class TestButton1 : TouchControlBase {
				using TouchControlBase::TouchControlBase;
				private:
				    bool pressed = false;
				    uid_t activating_touchid = 0;
				    TriangleMesh* btn_mesh = nullptr;
				    //Material *btn_offmat = nullptr, *btn_onmat = nullptr;
				    //SceneGraphModel *offmdl = nullptr, *onmdl = nullptr;
				public:
				    Texture2D* panel_image = nullptr;
				    fvec2_t origin = {0,0};
				    fscalar_t radius = 0.1f;
				    std::u32string ivar_name = U"";
				    virtual void on_create() {
						auto* tcs = this->tcsystem;
						this->classname = "TestButton1";

						//matprops_t mpbase = {};
						//mpbase.shadeless = true;

						//matprops_t mpoff = mpbase; mpoff.diffuse.color = fvec3_t(0.375f);
						//matprops_t mpon = mpbase; mpon.diffuse.color = fvec3_t(1.0f);


						TriangleMesh::meshdata tmd; tmd.prim_circle_xy(32, 1.0f);

						this->btn_mesh = tcs->new_managed_asset<TriangleMesh>(tmd);
						/*
						this->btn_offmat = tcs->new_managed_asset<Material>(mpoff);
						this->btn_onmat = tcs->new_managed_asset<Material>(mpon);

						this->offmdl = tcs->new_managed_asset<SceneGraphModel>();
						this->onmdl = tcs->new_managed_asset<SceneGraphModel>();

						SceneGraphModel::merge_rawmeshes_list offml, onml;
						offml.resize(1); onml.resize(1);
						offml[0].matproto = mpoff; offml[0].trimeshproto.prim_circle_xy(32, this->radius);
						onml[0].matproto = mpon; onml[0].trimeshproto.prim_circle_xy(32, this->radius);

						this->offmdl->merge_rawmeshes(offml);
						this->onmdl->merge_rawmeshes(onml);
						*/
					}
					virtual bool on_touch_event(EngineInstance::touchevent_t& tevt){
						switch (tevt.state){
							case EngineInstance::touchstate::down: {
								if (this->pressed){break;}
								this->pressed = true;
								this->activating_touchid = tevt.id;
								std::cout << "TestButton1 pressed!\n";
								return true;
							}
							case EngineInstance::touchstate::up: {
								if (tevt.id != this->activating_touchid){break;}
								this->pressed = false;
								this->activating_touchid = 0;
								std::cout << "TestButton1 released!\n";
								break;
							}
							default: ;
						}
						return false;
					}
					virtual void on_destroy() {
						/*
						this->tcsystem->del_managed_asset(uid_t(*this->offmdl)); this->offmdl = nullptr;
						this->tcsystem->del_managed_asset(uid_t(*this->onmdl)); this->onmdl = nullptr;
						*/
						this->tcsystem->del_managed_asset(uid_t(*this->btn_mesh)); this->btn_mesh = nullptr;
					}
					virtual void on_update(const MainloopData& mld){
						if (this->tcsystem->changed_window){this->pressed = false; this->activating_touchid = 0;}
						if (this->ivar_name.size() > 0){
							CCMDInvokationExtraData ed;
							ed.ivar_invalue = float(int(this->pressed));
							this->tcsystem->instance->C_ExecCommand(this->ivar_name, U"", ed);
						}
					}
					virtual bool contains_point(const fvec2_t& p) {
						return glm::length(p-(this->origin)) <= this->radius;
					}
					virtual void on_redraw(EngineInstance::calcmatrices_t& cm) {
						auto* ei = this->tcsystem->instance;
						this->tcsystem->draw_ctrlmesh(this->btn_mesh, glm::translate(fvec3_t(this->origin, 0))*glm::scale(fvec3_t(this->radius)), (this->pressed ? fvec4_t(fvec3_t(1), 1.0f) : fvec4_t(fvec3_t(0.25f), 0.5f)), this->panel_image);
					}
			};
		private:
		    Texture2D* _default_texture = nullptr;
		    #ifdef __GL45RW_HAS_OPENGL
		        _glstuff::GLShaderProgram* _tcs_glsp = nullptr;
		    #endif
		    #ifdef GL45RW_WITH_SW3D2
		        dj_sw3d20::Material _tcs_sw3d2mat;
		    #endif
		    std::unordered_set<uid_t> _owned_assets;
		    vectorlist<TouchControlBase*> _widgets;
		    vectorlist<EngineInstance::touchevent_t*> _teque = {}, _absorbed = {};
		public:
		    Texture2D* const& default_texture = _default_texture;

		    fmat44_t ortho_matrix = fmat44_t(1);
		    virtual assettype asset_type() {return assettype::touchctrlsys;}
		    fvec2_t rel2ndc(const fvec2_t& r){
				fscalar_t aspect = this->instance->R_WindowAspect();
				return glm::mix(fvec2_t{-aspect, 1.0f}, fvec2_t{aspect, -1.0f}, r);
			}
		    void update_events() {

				auto& eq = this->_teque;
				auto& absorbed = this->_absorbed;
				for (auto& te : this->instance->touch_events){
					eq.push_back(&te);
				}

				for (auto* wid : this->_widgets){
					for (auto* tep : eq){
						fvec2_t coord = this->rel2ndc(tep->cxy);
					    if ((tep->state != EngineInstance::touchstate::down) || wid->contains_point(coord)){
							if (wid->on_touch_event(*tep)){
								absorbed.push_back(tep);
							}
						}
					}
					for (auto* tep : absorbed){eq.eraseFirstOf(tep);}
					absorbed.clear();
				}
				eq.clear();
				absorbed.clear();
			}

			void update_controls(const MainloopData& mld){
				for (auto* wid : this->_widgets){
					wid->on_update(mld);
				}
				this->changed_window = false;
			}

			void draw_ctrlmesh(TriangleMesh* mesh, const fmat44_t& mdl2, const fvec4_t& color, Texture2D* image=nullptr){
				Texture2D* itex = (image != nullptr ? image : this->_default_texture);
		        auto& ms = this->instance->I_GetMatrixStack();
		        ms.model.pushMatrix(); ms.model.multMatrix(mdl2);
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: {
						#ifdef __GL45RW_HAS_OPENGL
                        #if (defined(_GL45RW_GL2))
						_glstuff::GLProgramBinding _mbind(*this->_tcs_glsp);
						const int _sampleunit = 0;
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("model"), 1, &(ms.model[-1]));
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("basecolor"), 1, &color);
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("texture"), 1, &_sampleunit);
						auto& gl = *_glstuff::curgl;
						gl.glEnable(GL_BLEND);
						gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        gl.glBlendEquation(GL_FUNC_ADD);
						gl.glActiveTexture(GL_TEXTURE0); gl.glBindTexture(GL_TEXTURE_2D, itex->gltexid);
						mesh->_drawmesh_opengl(TriangleMesh::drawparams_t{.glbinding=&_mbind});
						gl.glBindTexture(GL_TEXTURE_2D, 0);
						gl.glDisable(GL_BLEND);
						#endif
						#endif
						break;
					}
					case gfxapi::sw3d2: {
						#ifdef GL45RW_WITH_SW3D2
						auto& ctx = *this->instance->sw3d2ctx;
						auto& ts0 = this->_tcs_sw3d2mat.texture_slots[0];
						ts0 = dj_sw3d20::MaterialTextureSlot{.use=true, .src_image=itex->sw3d2tex, .layerblendtype=dj_sw3d20::enum_t::BLEND_MUL};
						ts0.affects_diffuse_alpha.use = true;
						ts0.affects_diffuse_alpha.amount = 1.0;

						this->_tcs_sw3d2mat.base_diffuse_color = dj_sw3d20::fpvec4(color);
						this->_tcs_sw3d2mat.uses_transparency = true;

						ctx.srActiveMaterial(&this->_tcs_sw3d2mat);
						ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(ms.projection[-1]);
						ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(ms.view[-1]);
						ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(ms.model[-1]);
						mesh->_drawmesh_sw3d2(TriangleMesh::drawparams_t{.sw3d2mat=&this->_tcs_sw3d2mat});
						ctx.srActiveMaterial(nullptr);
						#endif
						break;
					}
					default: ;
				}
				ms.model.popMatrix();
			}
			void redraw_controls() {
				this->instance->R_ClearBuffers(false,true);
				auto& ms = this->instance->I_GetMatrixStack();
				fscalar_t aspect = this->instance->R_WindowAspect();
				this->ortho_matrix = glm::orthoRH(-aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f);
				ms.projection.pushMatrix(); ms.projection[-1] = this->ortho_matrix;
				ms.view.pushMatrix(); ms.view.loadIdentity(); ms.view.translate(fvec3_t{0,0,0.5});
				ms.model.pushMatrix(); ms.model.loadIdentity();
				bool olddte = this->instance->R_IsCapEnabled(rendercap::depth_test);
				this->instance->R_SetCapEnabled(rendercap::depth_test, false);
				auto cm = this->instance->I_GetCalculatedMatrices();
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: {
						#ifdef __GL45RW_HAS_OPENGL
						#if (defined(_GL45RW_GL2))
						_glstuff::GLProgramBinding _mbind(*this->_tcs_glsp);
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("projection"), 1, &cm.projection);
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("viewinv"), 1, &cm.view_inv);
						_mbind.setUniform(this->_tcs_glsp->uniform_idx("model"), 1, &cm.mdl);
						#endif
						#endif
						break;
					}
					case gfxapi::sw3d2: {
						#ifdef GL45RW_WITH_SW3D2
						auto& ctx = *this->instance->sw3d2ctx;
						ctx.srActiveMaterial(&this->_tcs_sw3d2mat);
						ctx.srMatrixMode(dj_sw3d20::enum_t::PROJECTION_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(cm.projection);
						ctx.srMatrixMode(dj_sw3d20::enum_t::VIEW_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(cm.view);
						ctx.srMatrixMode(dj_sw3d20::enum_t::MODEL_MATRIX); ctx.srTopMatrix() = dj_sw3d20::fpmat4(cm.mdl);
						#endif
						break;
					}
					default: ;
				}
				for (auto* wid : this->_widgets){
					wid->on_redraw(cm);
				}
				ms.model.popMatrix();
				ms.view.popMatrix();
				ms.projection.popMatrix();
				this->instance->R_SetCapEnabled(rendercap::depth_test, olddte);
			}

			template <class AT, typename... Args>
			AT* new_managed_asset(Args... args){
				AT* a = this->instance->new_asset_rp<AT>(args...);
				//this->_owned_assets.emplace_back();
				//this->_owned_assets.back() = (AssetBase*)a;
				this->_owned_assets.insert(uid_t(*a));
				return a;
			}
			void del_managed_asset(const uid_t& aid){
				if (this->_owned_assets.count(aid) == 0){return;}
				this->_owned_assets.erase(aid);
				this->instance->del_asset(aid);
			}

			template <class WT, typename... Args>
			WT* new_widget(Args... args){
				WT* w = new WT(this, args...);
				w->on_create();
				return w;
			}
			void _delete_shader() {
				this->instance->R_MakeCurrent();
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: {
						#ifdef __GL45RW_HAS_OPENGL
						delete this->_tcs_glsp;
						this->_tcs_glsp = nullptr;
						#endif
						break;
					}
					default: ;
				}
			}
			void _rebuild_shader() {
				this->_delete_shader();
				switch (this->instance->R_GetCurrentVideoMode().renderer){
					case gfxapi::opengl: {
						#ifdef __GL45RW_HAS_OPENGL
                        #if (defined(_GL45RW_GL2))
						auto& gl = *_glstuff::curgl;
						this->_tcs_glsp = new _glstuff::GLShaderProgram(gl.glCreateProgram(), true);
						_glstuff::GLShaderObject
							vs1(GL_VERTEX_SHADER),
							fs1(GL_FRAGMENT_SHADER)
						;
						/*
						this->_tcs_glsp->attach(vs1(_TCSYS_GLSL_COMMON_SRC + _TCSYS_GLSL_VERTEX_SRC).throw_if_error());
						this->_tcs_glsp->attach(fs1(_TCSYS_GLSL_COMMON_SRC + _TCSYS_GLSL_FRAGMENT_SRC).throw_if_error());
						*/
                        _glstuff::GLSLCodeGenerator gen;
                        gen.commonsrc.header();
                        gen.frag_header();

                        gen.uniform_def("sampler2D", "texture");
                        gen.uniform_def("vec4", "basecolor");
                        gen.uniform_def("mat4", "projection");
                        gen.uniform_def("mat4", "viewinv");
                        gen.uniform_def("mat4", "model");

                        gen.varying_def("vec2", "v_uv");

                        {_glstuff::GLSLAutoFuncDef f(gen.vertexsrc, "void", "main", {});
                            f->localvardef("mat4", "mvp", "projection * viewinv * model");
                            f << "v_uv = gl_MultiTexCoord0.st;"; f->startline();
                            f << "gl_Position = mvp * vec4(gl_Vertex.xyz, 1.0);";
                        }

                        //std::cout << "\nvertex=\n" << gen.vertexsrc.str() << "\n";

                        {_glstuff::GLSLAutoFuncDef f(gen.fragsrc, "void", "main", {});
                            gen.frag_out(0, "texture2D(texture, v_uv)");
                        }

                        this->_tcs_glsp->attach(vs1(gen.vertexstr()).throw_if_error());
                        this->_tcs_glsp->attach(fs1(gen.fragstr()).throw_if_error());
						this->_tcs_glsp->link().throw_if_error().detach_all();

						#endif
						#endif
						break;
					}
					case gfxapi::sw3d2: {
						#ifdef GL45RW_WITH_SW3D2
						auto& ctx = *this->instance->sw3d2ctx;
						this->_tcs_sw3d2mat = dj_sw3d20::Material{};
						this->_tcs_sw3d2mat.shadeless = true;
						this->_tcs_sw3d2mat.blendtype = dj_sw3d20::enum_t::BLEND_MIX;
						this->_tcs_sw3d2mat.texture_slots.emplace_back();
						#endif
						break;
					}
					default: ;
				}
			}

			virtual void onRenderContextCreate() {
				this->_rebuild_shader();
			}
			virtual void onRenderContextDelete() {
				this->_delete_shader();
				this->changed_window = true;
			}
			virtual void onDestroy() {
				while (this->_widgets.size() > 0){
				    delete this->_widgets.front();
				}
				for (auto aid : this->_owned_assets){
					this->instance->del_asset(aid);
				}
				this->_owned_assets.clear();
			}
			virtual void onCreate() {
				this->_default_texture = this->instance->new_asset_rp<Texture2D>(1);
				this->_default_texture->_minfilter = this->_default_texture->_magfilter = texfilter::nearest;
				auto& mip0 = this->_default_texture->getMipLevel(0);
				//mip0.reinit_blank(2, 2, true);
				//std::fill_n(&(mip0.pixels[0]), mip0.pixels.size(), uint8_t(255));
				mip0.radial_gradient(64, rgba32_t{255,255,255,0}, rgba32_t(255), true);
				this->_default_texture->reupload_texture(0,true);
				std::cout << "set to white\n";
				this->_rebuild_shader();
			}
	};

	class Scene {
        public:
            static const uint32_t
                SCENE_VIEWMODEL = 1,
                SCENE_WORLDMODEL = 2
            ;
            using dargs_t = dictionary<std::string, std::string>;
            using entid_t = uid_t;

            class EntityClass;
            dictionary<std::string, EntityClass*> entity_classes = {};

            struct entity {
                Scene* scene = nullptr;
                entid_t eid = 0;
                uid_t egroup = 0;
                std::string classname = "", tagname = "";
                trs_t trs;
                void (*logic)(entity&, const gl45rw::MainloopData&) = nullptr;
                void (*render)(entity&) = nullptr;
                void (*on_delete)(entity&) = nullptr;
                void (*physics_upload)(entity&, const gl45rw::MainloopData&) = nullptr;
                void (*physics_step)(entity&, const gl45rw::MainloopData&) = nullptr;
                void* userdata = nullptr;
            };
            dictionary<std::string, std::unordered_set<entid_t>> tagged_ents = {};
            class EntityClass {
                private:
                    Scene* _scene = nullptr;
                public:
                    Scene* const& scene = _scene;
                    virtual std::string classname() = 0;
                protected:
                    virtual entity* prtspawn(void* args) {return nullptr;}
                    virtual entity* prtspawn_da(const dictionary<std::string, std::string>& dict) {return nullptr;}
                public:
                    entity* spawn(void* args){return this->prtspawn(args);}
                    entity* spawn(const dictionary<std::string, std::string>& dargs) {return this->prtspawn_da(dargs);}

                    virtual void onClassCreate() {}
                    virtual void onClassDestroy() {}

                    EntityClass(Scene* s) : _scene(s) {}
                    virtual ~EntityClass() {this->onClassDestroy();}
            };
            struct loadspr_args {
                fvec3_t basecolor = {1,1,1};
                fvec3_t scale = {1,1,1};
                fvec3_t ofs = {0.0f,0.0f,0.1f};

                matblendtype blendtype = matblendtype::add;
            };

            class ParticleSystem {
                private:
                    Scene* _scene = nullptr;
                    uid_t _psid = 0;

                    void _ptmp_randattr(fscalar_t& dst, const std::array<fscalar_t, 2>& range) {dst = glm::mix(range[0], range[1], this->scene->instance->I_GetRandomFloat());}

                    template <glm::length_t L>
                    void _ptmp_randattr(fvecL_t<L>& dst, const std::array<fvecL_t<L>, 2>& range) {
                        fvecL_t<L> fac = this->scene->instance->I_GetRandomVector<L>();
                        dst = glm::mix(range[0], range[1], fac);
                    }
                public:
                    trs_t base_etrs;
                    Scene* const& scene = _scene;
                    const uid_t& id = _psid;

                    enum particle_type : int {
                        sgmdl = 0
                    };
                    struct particle_t {
                        particle_type type = particle_type::sgmdl;
                        uid_t sgmodel = 0;
                        fclock_t lifespan = 0, lifeclock = 0;
                        fvec3_t linv = {0,0,0}, gravity = {0,0,0};
                        trs_t base_trs, ofs_trs;
                        bool sgmdl_align2camera = true;
                        fvec3_t sgmdl_alignment_axis = {1,0,0}, sgmdl_alignment_upaxis = {0,0,1};
                        bool ptmp_owns_sgmodel = true;

                        std::array<fvec3_t, 2> ptmp_linvs = {}, ptmp_spawnofs = {};
                        std::array<fclock_t, 2> ptmp_lifespans = {};

                    };



                    size_t numspawned = 0;
                    std::vector<particle_t> particles = {}, _particles_keep = {}, particle_templates = {};
                    size_t new_template() {this->particle_templates.emplace_back(); return this->particle_templates.size() - 1;}
                    particle_t& getptmp(const int& idx) {
                        if (idx < 0) {return this->particle_templates[this->particle_templates.size() + idx];}
                        return this->particle_templates[idx];
                    }
                    void ptmp_resetSGModel(const int& idx) {
                        auto& ptmp = this->getptmp(idx);
                        if (ptmp.ptmp_owns_sgmodel){
                            this->scene->instance->del_asset(ptmp.sgmodel);
                        }
                        ptmp.sgmodel = 0;
                    }
                    void ptmp_mergeSGModel(const int& idx, const std::u32string& fspath) {
                        auto& ptmp = this->getptmp(idx);
                        this->ptmp_resetSGModel(idx);
                        ptmp.ptmp_owns_sgmodel = true;
                        ptmp.sgmodel = this->scene->instance->new_asset<SceneGraphModel>();
                        auto* sgmodel = this->scene->instance->get_asset<SceneGraphModel>(ptmp.sgmodel);
                        sgmodel->merge_model(fspath);
                    }


                    void ptmp_loadSprite(const int& idx, const std::u32string& fspath, const loadspr_args& args) {
                        auto& ptmp = this->getptmp(idx);
                        this->ptmp_resetSGModel(idx);
                        ptmp.ptmp_owns_sgmodel = true;
                        ptmp.sgmodel = this->scene->instance->new_asset<SceneGraphModel>();
                        auto* sgmodel = this->scene->instance->get_asset<SceneGraphModel>(ptmp.sgmodel);
                        SceneGraphModel::merge_rawmeshes_list mrl; mrl.emplace_back();
                        auto& ent0 = mrl.front();
                        ent0.trimeshproto.prim_unitrect_xy();
                        for (auto& v : ent0.trimeshproto.vertices) {
                            v.co = (v.co * args.scale) + args.ofs;
                        }
                        auto& mat0 = ent0.matproto;
                        mat0.diffuse.color = args.basecolor;
                        mat0.diffuse.alpha = 1;
                        mat0.blendtype = args.blendtype;
                        auto& m0t0 = mat0.texture_slots[0];
                        m0t0.use = true;

                        m0t0.texture = this->scene->instance->new_asset<Texture2D>();
                        auto* m0t0tex = this->scene->instance->get_asset<Texture2D>(m0t0.texture);
                        m0t0tex->loadFromFS_A(fspath);

                        m0t0.affects.diffuse_color = matprops_t::_mataffect<fvec3_t>{.use=true, .amount={1,1,1}};
                        m0t0.affects.diffuse_alpha = matprops_t::_mataffect<fscalar_t>{.use=true, .amount=1};
                        m0t0.blendtype = texblendtype::mul;

                        sgmodel->merge_rawmeshes(mrl);

                        ptmp.sgmdl_align2camera = true;
                        ptmp.sgmdl_alignment_axis = fvec3_t{0,0,1};
                        ptmp.sgmdl_alignment_upaxis = fvec3_t{0,1,0};

                    }

                    void ptmp_delete(const int& idx) {
                        this->ptmp_resetSGModel(idx);
                        int realidx = (idx < 0 ? this->particle_templates.size() + idx : idx);
                        this->particle_templates.erase(this->particle_templates.begin() + realidx);
                    }


                    particle_t* emit_particle(const trs_t& etrs, const fvec3_t ofsmin={0,0,0}, const fvec3_t ofsmax={0,0,0}) {
                        trs_t etrs2 = etrs;
                        auto* ei = this->scene->instance;
                        auto& ptmp = this->particle_templates[(this->numspawned++) % (this->particle_templates.size())];
                        fvec3_t tl1; this->_ptmp_randattr(tl1, ptmp.ptmp_spawnofs);
                        etrs2.translate(tl1, true);
                        etrs2.translate(glm::mix(ofsmin, ofsmax, ei->I_GetRandomVector<3>()), true);
                        this->particles.push_back(ptmp);
                        auto& newpart = this->particles.back();
                        this->_ptmp_randattr(newpart.linv, ptmp.ptmp_linvs);
                        this->_ptmp_randattr(newpart.lifespan, ptmp.ptmp_lifespans);
                        newpart.base_trs = etrs2;
                        return &newpart;
                    }

                    void logic(const MainloopData& mld) {
                        this->_particles_keep.clear();
                        for (auto& p : this->particles) {
                            p.lifeclock += mld.ticdelta;
                            if (p.lifeclock < p.lifespan) {
                                //p.linv += (p.gravity * mld.ticdelta);
                                fvec3_t wlinv = p.base_trs.getaxisvect(p.linv) + (p.gravity * p.lifeclock);
                                p.base_trs.translate(wlinv * mld.ticdelta, false);
                                this->_particles_keep.push_back(p);
                            }
                        }
                        this->particles.clear();
                        this->particles.insert(this->particles.end(), this->_particles_keep.begin(), this->_particles_keep.end());
                    }

                    void render() {
                        auto* ei = this->scene->instance;
                        fmat44_t mdlmat, viewmat, projmat;
                        ei->I_GetMatrices(mdlmat, viewmat, projmat);
                        fvec3_t campos(viewmat[3]);
                        fvec3_t camup = glm::normalize(fvec3_t(viewmat * fvec4_t{0,1,0,0}));
                        for (auto& p : this->particles) {
                            trs_t ptrs = this->base_etrs.transform(p.base_trs.transform(p.ofs_trs));
                            switch (p.type) {
                                case particle_type::sgmdl: {
                                    auto* sgmdl = ei->get_asset<SceneGraphModel>(p.sgmodel);
                                    if (sgmdl == nullptr){break;}
                                    SceneGraphModel::enque_args ea;
                                    ea.base_trs = ptrs;
                                    if (p.sgmdl_align2camera){
                                        ea.base_trs.align_axis_to_vec(glm::normalize(p.sgmdl_alignment_upaxis), camup);
                                        ea.base_trs.align_axis_to_vec(glm::normalize(p.sgmdl_alignment_axis), glm::normalize(campos - ea.base_trs.pos));
                                    }
                                    sgmdl->enque(ea);
                                    break;
                                }
                                default: ;
                            }
                        }
                    }

                    ParticleSystem(Scene* s) : _scene(s) {
                        this->_psid = s->_cur_psid++;
                        s->particle_systems.push_back(this);
                        s->particle_systems_byid[this->_psid] = this;
                    }

                    ~ParticleSystem() {
                        while (this->particle_templates.size() > 0) {
                            this->ptmp_delete(-1);
                        }
                        this->particles.clear();
                        this->_particles_keep.clear();
                        this->_scene->particle_systems.eraseFirstOf(this);
                        this->_scene->particle_systems_byid.erase(this->_psid);
                    }
            };
            uid_t _cur_psid = 1;
            vectorlist<ParticleSystem*> particle_systems = {};
            dictionary<uid_t, ParticleSystem*> particle_systems_byid = {};

            uid_t psys_new() {
                auto* psys = new ParticleSystem(this);
                return psys->id;
            }
            ParticleSystem* psys_new_rp() {
                return new ParticleSystem(this);
            }
            ParticleSystem::particle_t* psys_emitParticle(const uid_t& psid, const trs_t& etrs, const fvec3_t ofsmin={0,0,0}, const fvec3_t ofsmax={0,0,0}) {
                try {
                    auto* psys = this->particle_systems_byid.at(psid);
                    return psys->emit_particle(etrs, ofsmin, ofsmax);
                } catch (const std::out_of_range& exc) {
                    return nullptr;
                }
            }
            bool psys_delete(const uid_t& psid) {
                try {
                    auto* psys = this->particle_systems_byid.at(psid);
                    delete psys; psys = nullptr;
                    return true;
                } catch (const std::out_of_range& exc) {
                    return false;
                }
            }
        private:
            entid_t _cur_entid = 1;
            dictionary<entid_t, entity*> _ents_by_id = {};
            vectorlist<entity*> _newly_spawned_entities = {}, _active_ents = {};
            vectorlist<entid_t> _entids_pending_removal = {};
            EngineInstance* _einst = nullptr;
            bool _currently_iterating_entsbyid = false;
            bool _process_spawned_ents_list() {
                if (this->_currently_iterating_entsbyid){return false;}
                for (auto& e : this->_newly_spawned_entities){
                    this->_active_ents.push_back(e);
                }
                this->_newly_spawned_entities.clear();
                return true;
            }
            bool _process_deleted_ents_list() {
                if (this->_currently_iterating_entsbyid){return false;}
                for (auto& eid : this->_entids_pending_removal){
                    entity* e = this->_ents_by_id.getd(eid, nullptr);
                    if (e != nullptr){
                        if (e->on_delete != nullptr){e->on_delete(*e);}
                        this->_ents_by_id.erase(eid);
                        this->_active_ents.eraseFirstOf(e);
                        delete e; e = nullptr;
                    }
                }
                this->_entids_pending_removal.clear();
                return true;
            }
            class _EntityLoopLock {
                private:
                    Scene* scene = nullptr;
                public:
                    _EntityLoopLock(Scene* s) : scene(s) {s->_currently_iterating_entsbyid = true;}
                    ~_EntityLoopLock() {this->scene->_currently_iterating_entsbyid = false;}
            };
        public:
            EngineInstance* const& instance = _einst;

            int num_physics_steps = 1, num_logic_steps = 1;

            entity* getent(const entid_t& eid) {return this->_ents_by_id.getd(eid, nullptr);}

            template <class CLS=EntityClass>
            CLS* classof(const entity* e) {
                if (e == nullptr){return nullptr;}
                return reinterpret_cast<CLS*>(this->entity_classes.getd(e->classname, nullptr));
            }

            template <class ECT=EntityClass>
            ECT* newclass() {
                ECT* ec = new ECT(this);
                this->entity_classes[ec->classname()] = ec;
                ec->onClassCreate();
                return ec;
            }

            entity* spawn() {
                entity* ent = new entity;
                ent->scene = this;
                ent->eid = this->_cur_entid++;
                this->_newly_spawned_entities.push_back(ent);
                this->_ents_by_id[ent->eid] = ent;
                this->_process_spawned_ents_list();
                return ent;
            }

            entity* spawn(const std::string& classname, void* spawnargs){
                EntityClass* ecls = this->entity_classes.at(classname);
                return ecls->spawn(spawnargs);
            }
            entity* spawn(const std::string& classname, const dictionary<std::string, std::string>& dargs){
                EntityClass* ecls = this->entity_classes.at(classname);
                return ecls->spawn(dargs);
            }

            void set_tag(entity* e, const std::string& ntag) {
                if (e == nullptr){return;}
                if (this->tagged_ents.count(e->tagname) > 0){
                    this->tagged_ents[e->tagname].erase(e->eid);
                }
                e->tagname = ntag;
                if (e->tagname.size() > 0){
                    if (this->tagged_ents.count(e->tagname) == 0){
                        this->tagged_ents[e->tagname] = std::unordered_set<entid_t>();
                    }
                    this->tagged_ents[e->tagname].insert(e->eid);
                }
            }
            void set_tag(const entid_t& eid, const std::string& ntag){
                this->set_tag(this->getent(eid), ntag);
            }
            bool destroy(const entid_t& eid) {
                if (this->_ents_by_id.count(eid) == 0){return false;}
                this->set_tag(eid, "");
                this->_entids_pending_removal.push_back(eid);
                this->_process_deleted_ents_list();
                return true;
            }
            bool destroy(entity* e) {return (e != nullptr ? this->destroy(e->eid) : false);}
            void process_lists() {
                this->_process_deleted_ents_list();
                this->_process_spawned_ents_list();
            }
            void update(const gl45rw::MainloopData& mld) {
                {
                    _EntityLoopLock lock(this);
                    auto pmld = mld;
                    for (int step = 0; step < this->num_logic_steps; step++){
                        pmld.ticdelta = (mld.ticdelta / this->num_logic_steps);
                        pmld.uptime = (mld.uptime - mld.ticdelta) + (pmld.ticdelta * step);
                        for (entity* e : this->_active_ents){
                            if (e->logic != nullptr){e->logic(*e, pmld);}
                        }
                        for (ParticleSystem* ps : this->particle_systems) {
                            ps->logic(pmld);
                        }
                    }

                    this->instance->P_ClearPhysicsGroups();
                    for (entity* e : this->_active_ents){
                        if (e->physics_upload != nullptr){e->physics_upload(*e, mld);}
                    }
                    pmld = mld;
                    for (int step = 0; step < this->num_physics_steps; step++){
                        pmld.ticdelta = (mld.ticdelta / this->num_physics_steps);
                        pmld.uptime = (mld.uptime - mld.ticdelta) + (pmld.ticdelta * step);
                        for (entity* e : this->_active_ents){
                            if (e->physics_step != nullptr){e->physics_step(*e, pmld);}
                        }
                    }
                }
                this->process_lists();
            }
            void draw(const fclock_t curtime=0) {
                this->instance->R_ClearActiveLights();
                this->instance->R_ClearAllRenderBuckets();
                {
                    _EntityLoopLock lock(this);
                    for (entity* e : this->_active_ents){
                        if (e->render != nullptr){e->render(*e);}
                    }
                }
                for (ParticleSystem* ps : this->particle_systems) {
                    ps->render();
                }
                this->instance->R_UpdateLighting();

                this->instance->R_ClearBuffers(true, true);
                this->instance->R_DrawAllRenderBuckets(curtime, SCENE_WORLDMODEL);

                this->instance->R_ClearBuffers(false, true);
                this->instance->R_DrawAllRenderBuckets(curtime, SCENE_VIEWMODEL);

                this->instance->R_ClearAllRenderBuckets();
                this->instance->R_ClearActiveLights();
            }
            int kill_groupid(const uid_t& g){
                int count = 0;
                {_EntityLoopLock _lock(this);
                    for (entity* e : this->_active_ents) {
                        if (e->egroup == g){
                            e->egroup = ~uid_t(0);
                            this->destroy(e);
                            count++;
                        }
                    }
                }
                return count;
            }
            struct spheretest1_isect {
                fvec3_t hitpos, hitnormal, hit_wuv, hitvec;
                fscalar_t hitdist;
                static bool sorted_insert_key(const spheretest1_isect& nu, const spheretest1_isect& cmp){
                    return nu.hitdist < cmp.hitdist;
                }
            };
            bool sphere_test1(vectorlist<spheretest1_isect>& isects, const fvec3_t& centroid, const fscalar_t& radius, const uint32_t mask = uint32_t(-1)){
                isects.clear();

                EngineInstance::physgroup_t* grps[32] = {}; size_t ngrps;
                this->instance->P_GetPhysicsGroupsForMask(ngrps, grps, mask);
                gl45rw::_mathstuff::MollerTrumbore mt;

                for (size_t i = 0; i < ngrps; i++){
                    auto& pgrp = grps[i][0];
                    for (auto& pobj : pgrp){
                        fmat44_t pobjmat(pobj.trs);
                        switch (pobj.pshape){
                            case gl45rw::sgphysshape::trimesh: {
                                for (auto& triangle : pobj.trimesh->data.triangles) {
                                    auto& v0 = pobj.trimesh->data.vertices[triangle[0]];
                                    auto& v1 = pobj.trimesh->data.vertices[triangle[1]];
                                    auto& v2 = pobj.trimesh->data.vertices[triangle[2]];
                                    _mathstuff::tri3d_t gtri(
                                        fvec3_t(pobjmat * fvec4_t(v0.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v1.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v2.co, 1))
                                    );

                                    mt.set_triangle(
                                        gtri.vertices[0],
                                        gtri.vertices[1],
                                        gtri.vertices[2]
                                    );

                                    fvec3_t tnorm = gtri.facenormal();
                                    fvec3_t cpt = gtri.closest_point_on_tri(centroid);
                                    _mathstuff::ray3d_t nray(centroid, glm::normalize(cpt-centroid));
                                    if (mt(nray, radius)){
                                        spheretest1_isect isect = {};
                                        //isect.hitpos = mt.out.hitpoint;
                                        isect.hitpos = cpt;
                                        isect.hitnormal = mt.out.hitnormal;
                                        isect.hit_wuv = mt.out.hit_wuv;
                                        isect.hitvec = glm::normalize(isect.hitpos - centroid);
                                        //isect.hitnormal = (glm::dot(isect.hitvec, mt.out.hitnormal) >= 0 ? -isect.hitvec : isect.hitvec);
                                        isect.hitdist = mt.out.hitdistance;
                                        //sorted_insert<spheretest1_isect>(isects, isect);
                                        isects.push_back(isect);
                                    }

                                }
                                break;
                            }
                            default: ;
                        }
                    }
                }

                return isects.size() > 0;
            }

            struct trace1_result {
                fvec3_t hitpos, hitnormal, hit_wuv;
                fscalar_t hitdist;
                fvec3_t hitnormal_smoothed;
            };
            bool trace1(trace1_result& res, const fvec3_t& a, const fvec3_t& b, const uint32_t mask = uint32_t(-1), const bool front_normals_only=false) {
                EngineInstance::physgroup_t* grps[32] = {}; size_t ngrps;
                this->instance->P_GetPhysicsGroupsForMask(ngrps, grps, mask);
                bool got_atleast_one = false;
                fscalar_t abdist = glm::length(b-a);
                gl45rw::_mathstuff::ray3d_t abray(a, glm::normalize(b-a));
                //gl45rw::_mathstuff::MollerTrumbore mt;
                for (size_t i = 0; i < ngrps; i++){
                    auto& pgrp = grps[i][0];
                    for (auto& pobj : pgrp) {
                        //fvec3_t chpos; fscalar_t chdist;
                        fmat44_t pobjmat(pobj.trs);
                        switch (pobj.pshape){
                            case gl45rw::sgphysshape::trimesh: {
                                for (auto& triangle : pobj.trimesh->data.triangles) {
                                    auto& v0 = pobj.trimesh->data.vertices[triangle[0]];
                                    auto& v1 = pobj.trimesh->data.vertices[triangle[1]];
                                    auto& v2 = pobj.trimesh->data.vertices[triangle[2]];

                                    /*mt.set_triangle(
                                        fvec3_t(pobjmat * fvec4_t(v0.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v1.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v2.co, 1))
                                    );*/
                                    _mathstuff::tri3d_t tri = {
                                        fvec3_t(pobjmat * fvec4_t(v0.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v1.co, 1)),
                                        fvec3_t(pobjmat * fvec4_t(v2.co, 1))
                                    };
                                    _mathstuff::raytrires3d_t rtres;
                                    //if (mt(abray, abdist)){
                                    if (abray.intersect(rtres, tri, abdist)){
                                        //if ((!got_atleast_one) || mt.out.hitdistance < res.hitdist){
                                        if ((!got_atleast_one) || rtres.t <= res.hitdist){
                                            res.hitpos = rtres.pos;
                                            res.hitdist = rtres.t;
                                            res.hit_wuv = rtres.wuv;
                                            res.hitnormal = (front_normals_only ? rtres.ognormal : rtres.normal);

                                            fvec3_t
                                                vn0 = fvec3_t(pobjmat * fvec4_t(v0.normal, 0)),
                                                vn1 = fvec3_t(pobjmat * fvec4_t(v1.normal, 0)),
                                                vn2 = fvec3_t(pobjmat * fvec4_t(v2.normal, 0))
                                            ;
                                            if (!front_normals_only && !rtres.frontal){
                                                vn0 = -vn0;
                                                vn1 = -vn1;
                                                vn2 = -vn2;
                                            }
                                            res.hitnormal_smoothed = glm::normalize(_mathstuff::wsum3(res.hit_wuv, vn0, vn1, vn2));

                                        }
                                        got_atleast_one = true;
                                    }

                                }
                                break;
                            }
                            default: ;
                        }
                    }
                }
                return got_atleast_one;
            }

            struct worldfile_extracmdargs_t {
                Scene* scene = nullptr;
                std::string cmdname_u8;
                std::stringstream* wfss_ptr = nullptr;
            };

            int spawn_worldfile(const std::u32string& path, const uid_t entgroup=1, int (*on_extracmd)(worldfile_extracmdargs_t&)=nullptr){
                std::stringstream wfss;
                djutil::virfs::vfistream istr(this->instance->I_GetMainFS()[0], path, std::ios::binary);
                while (istr){wfss.put(istr.get());}
                istr.close();
                wfss.seekg(0);
                struct espawn_entry {
                    std::string classname;
                    gl45rw::dictionary<std::string, std::string> params;
                };
                std::vector<espawn_entry> entries = {};
                int spawncount = 0;
                while (wfss){
                    std::string cmdname; djutil::ezstr::u8reads(cmdname, wfss);
                    if (cmdname == ""){break;}
                    else if (djutil::ezstr::startswith(cmdname, "//")){
                        char32_t tchr;
                        djutil::ezstr::u8skip_until(tchr, wfss, U"\n\r");
                    }
                    else if (cmdname == "spawn"){
                        std::string classname; djutil::ezstr::u8reads(classname, wfss);
                        entries.emplace_back();
                        auto& entry = entries.back();
                        entry.classname = classname;
                        while (wfss) {
                            std::string param_n, param_v;
                            char32_t param_v_start;
                            djutil::ezstr::u8reads(param_n, wfss);
                            if (djutil::ezstr::startswith(param_n, ";")){break;}
                            else if (djutil::ezstr::startswith(param_n, "//")){
                                char32_t tchr2;
                                djutil::ezstr::u8skip_until(tchr2, wfss, U"\n\r");
                                continue;
                            }
                            else if (djutil::ezstr::u8skipws(wfss)){throw std::runtime_error("Premature EOF in param list!");}
                            auto startpos = wfss.tellg();
                            if (djutil::ezstr::u8getc(param_v_start, wfss) == 0){throw std::runtime_error("Premature EOF in param list!");}
                            wfss.seekg(startpos);

                            if (param_v_start == U'"'){
                                std::u32string param_v_u32;
                                djutil::ezstr::parse_strlit(param_v_u32, wfss);
                                param_v = djutil::ezstr::utf32_to_utf8(param_v_u32);
                            }
                            else {
                                djutil::ezstr::u8reads(param_v, wfss);
                            }
                            entry.params[param_n] = param_v;
                        }

                    }
                    else if (on_extracmd != nullptr){
                        worldfile_extracmdargs_t eca;
                        eca.scene = this;
                        eca.cmdname_u8 = cmdname;
                        eca.wfss_ptr = &wfss;
                        on_extracmd(eca);
                    }
                }
                //int spawncount = 0;
                for (auto& ese : entries){
                    entity* e = this->spawn(ese.classname, ese.params);
                    e->egroup = entgroup;
                    spawncount++;
                }
                this->process_lists();
                return spawncount;
            }

            Scene(EngineInstance* _ei) : _einst(_ei) {}
            ~Scene() {
                this->_currently_iterating_entsbyid = false;
                for (auto& p : this->_ents_by_id) {
                    this->_entids_pending_removal.push_back(p.first);
                }
                this->_process_deleted_ents_list();
                for (auto& ecp : this->entity_classes){
                    delete ecp.second; ecp.second = nullptr;
                }
                this->entity_classes.clear();
                while (this->particle_systems.size() > 0) {
                    this->psys_delete(this->particle_systems.back()->id);
                }
                this->particle_systems.clear();
                this->particle_systems_byid.clear();
            }

    };



    std::vector<EngineInstance*>* _engine_instances = nullptr;
    std::unordered_map<uint32_t, EngineInstance*>* _instances_by_winid = nullptr;

    #ifdef _GL45RW_WINDOW_GLFW3
        std::unordered_map<GLFWwindow*, EngineInstance*>* _instances_by_glfw_window = nullptr;
        void _register_instance_glfw_window(GLFWwindow* gwin, EngineInstance* ei) {
            if (ei == nullptr || gwin == nullptr){return;}
            _instances_by_glfw_window[0][gwin] = ei;
        }
        void _unregister_instance_glfw_window(GLFWwindow* gwin) {
            if (_instances_by_glfw_window->count(gwin) == 0){return;}
            _instances_by_glfw_window->erase(gwin);
        }
    #endif

    void _register_instance_window(const uint32_t wid, EngineInstance* ei)
    {
        if (ei == nullptr){return;}
        _instances_by_winid[0][wid] = ei;
    }
    void _unregister_instance_window(const uint32_t wid)
    {
        if (_instances_by_winid->count(wid) == 0){return;}
        _instances_by_winid->erase(wid);
    }

    EngineInstance* E_GetInstanceForWindow(const uint32_t wid)
    {
        if (_instances_by_winid->count(wid) == 0){return nullptr;}
        return _instances_by_winid[0][wid];
    }
    template <class EI=EngineInstance>
    EI* E_GetEngineInstanceFromID(const uid_t idx) {
        return reinterpret_cast<EI*>(gl45rw::_engine_instances_byid->getd(idx, nullptr));
    }

    void _register_engine_instance(EngineInstance* ei) {
        _engine_instances->push_back(ei);
    }

    template <class I>
    I* new_engine_instance(void* params=nullptr)
    {
        I* ei = new I();
        _engine_instances->push_back(ei);
        ei->_i_register_engine_instance();
		ei->_initializeRandomEngine();
        ei->_onCreate(params);
        return ei;
    }
    void _del_engine_instance(EngineInstance* ei)
    {
        if (ei == nullptr){return;}
        bool foundit = false;
        size_t index = 0;
        /*
        for (; index < _engine_instances->size(); index++)
        {
            if (ei == (*_engine_instances)[index]){foundit = true; break;}
        }
        */
        auto fit = std::find(_engine_instances->begin(), _engine_instances->end(), ei);
        foundit = (fit != _engine_instances->end());
        index = (fit - _engine_instances->begin());
        if (foundit)
        {
            ei->_onDestroy();
            ei->S_SetSoundMode(soundmode_t{.renderer=sndapi::none});
            ei->R_SetVideoMode(videomode_t{.renderer=gfxapi::none, .width=1, .height=1});
            while (ei->assets.size() > 0)
            {
                const uid_t _aid = (ei->assets.begin())->first;
                ei->del_asset(_aid);
            }

            for (auto& p : ei->all_ccmds) {
                if (p.second != nullptr){delete p.second; p.second = nullptr;}
            }
            ei->all_ccmds.clear();

            ei->I_CloseMainFS();
            ei->_i_unregister_engine_instance();
            _engine_instances->erase(_engine_instances->begin() + index);
            delete ei;
        }
    }
    void _remove_engine_instance(EngineInstance* ei) {
        /*
        bool foundit = false;
        auto itr = _engine_instances->begin();
        for (; itr != _engine_instances->end(); itr++)
        {
            if (ei == *itr){foundit = true; break;}
        }
        if (foundit){_engine_instances->erase(itr);}
        */
        auto fit = std::find(_engine_instances->begin(), _engine_instances->end(), ei);
        if (fit != _engine_instances->end()){_engine_instances->erase(fit);}
    }
    template <class EIT, class AIT>
    class AutoAsset {
        private:
            uid_t _eid = 0, _aid = 0;
        public:
            const uid_t &instance_id = _eid, &asset_id = _aid;
            EIT* instance() const {return E_GetEngineInstanceFromID<EIT>(this->_eid);}
            AIT* assetptr() const {
                EIT* ei = this->instance();
                if (ei == nullptr){return nullptr;}
                return (AIT*)(ei->get_asset(this->_aid));
            }

            AutoAsset(EIT* einst, const uid_t& assetid) {
                this->_eid = einst->I_GetInstanceID();
                this->_aid = assetid;
            }

            ~AutoAsset() {
                this->instance()->del_asset(this->_aid); this->_aid = 0;
                this->_eid = 0;
            }
    };

    #ifdef _GL45RW_WINDOW_GLFW3
    void _glfw_window_close_callback(GLFWwindow* gwin) {
        EngineInstance* ei = nullptr;
        try {
            ei = _instances_by_glfw_window->at(gwin);
        } catch (const std::out_of_range& exc){
            return;
        }
        if (ei != nullptr){
            ei->_onWindowCloseEvent();
            glfwSetWindowShouldClose(gwin, GLFW_FALSE);
        }
    }
    #endif

    std::string OPENGL_LIBNAME = "";
    void global_startup()
    {
        if (_globally_initialized){return;}
        #ifndef _GL45RW_HEADLESS
         #if (defined(_GL45RW_WINDOW_SDL2))
         SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);
         SDL_InitSubSystem(SDL_INIT_SENSOR|SDL_INIT_GAMECONTROLLER|SDL_INIT_HAPTIC);
         _sdl2_keystates = SDL_GetKeyboardState(nullptr);
         _gl45rw_UpdateGamepadStates_firstrun = true;
         _gl45rw_pending_gamepad_device_reenum = true;
         #ifdef GL45RW_WITH_OPENGL
          if (SDL_GL_LoadLibrary((OPENGL_LIBNAME.size() > 0 ? OPENGL_LIBNAME.c_str() : nullptr))){
            throw std::runtime_error("FATAL ERROR: couldn't load the specified OpenGL library!");
          }
         #endif
         #elif (defined(_GL45RW_WINDOW_GLFW3))
         if (!glfwInit()){
             throw std::runtime_error("FATAL ERROR: couldn't initialize GLFW3!");
         }

         #endif
        #endif
        #ifdef GL45RW_WITH_LUAJIT
        _lua_engines_bystate = new dictionary<lua_State*, LuaEngineBase*>();
        #endif
        _engine_instances = new std::vector<EngineInstance*>();
        _engine_instances_byid = new dictionary<uid_t, EngineInstance*>();

        _instances_by_winid = new std::unordered_map<uint32_t, EngineInstance*>();
        _input_event_map = new std::unordered_map<std::string, inputeventinfo_t>();

        #ifdef _GL45RW_WINDOW_GLFW3
            _instances_by_glfw_window = new std::unordered_map<GLFWwindow*, EngineInstance*>();
        #endif


        #if (defined(_GL45RW_WINDOW_SDL2) || defined(_GL45RW_HEADLESS))

        #if (!defined(_GL45RW_HEADLESS))
         #define _mapscan(_name, _suffix) _input_event_map[0][_name] = inputeventinfo_t{\
             .strname=_name, .iclass=inputclass::key,\
             .keycode=SDL_SCANCODE_ ## _suffix\
         }\

        #else
         #define _mapscan(_name, _suffix) \
          _input_event_map[0][_name] = inputeventinfo_t{\
              .strname=_name, .iclass=inputclass::key\
          }\

        #endif

        for (char alpha = 'a'; alpha <= 'z'; alpha++)
        {
            inputeventinfo_t iei = {};
            iei.iclass = inputclass::key;
            int _ad = (alpha - 'a');
            std::string _an = " "; _an[0] = alpha;
            iei.strname = _an;
            #ifndef _GL45RW_HEADLESS
             SDL_Scancode _sdl2key = SDL_Scancode(int(SDL_SCANCODE_A) + _ad);
             iei.keycode = _sdl2key;
            #endif
            _input_event_map[0][_an] = iei;
            //std::cout << "[" << _an << "]\n";
        }
        char digits[] = "1234567890";
        for (int i = 0; i < 10; i++)
        {
            inputeventinfo_t iei = {
                .strname=std::string(&digits[i], 1),
                .iclass=inputclass::key
                #ifndef _GL45RW_HEADLESS
                 ,.keycode=SDL_Scancode(int(SDL_SCANCODE_1) + i)
                #endif
            };
            _input_event_map[0][iei.strname] = iei;
        }
        _mapscan("up", UP);
        _mapscan("down", DOWN);
        _mapscan("left", LEFT);
        _mapscan("right", RIGHT);
        _mapscan("escape", ESCAPE);
        _mapscan("space", SPACE);
        _mapscan("tilde", GRAVE);
        _mapscan(";", SEMICOLON);
        _mapscan("[", LEFTBRACKET);
        _mapscan("]", RIGHTBRACKET);
        _mapscan("-", MINUS);
        _mapscan("=", EQUALS);
        _mapscan("backspace", BACKSPACE);
        _mapscan("return", RETURN);
        _mapscan(",", COMMA);
        _mapscan(".", PERIOD);
        _mapscan("backslash", BACKSLASH);
        _mapscan("/", SLASH);
        _mapscan("lshift", LSHIFT);
        _mapscan("rshift", RSHIFT);
        _mapscan("lctrl", LCTRL);
        _mapscan("rctrl", RCTRL);
        _mapscan("lalt", LALT);
        _mapscan("ralt", RALT);
        _mapscan("tab", TAB);
        _mapscan("home", HOME);
        _mapscan("end", END);
        _mapscan("insert", INSERT);
        _mapscan("delete", DELETE);
        _mapscan("pageup", PAGEUP);
        _mapscan("pagedown", PAGEDOWN);
        _mapscan("capslock", CAPSLOCK);
        _mapscan("sysrq", PRINTSCREEN);
        _mapscan("scrlock", SCROLLLOCK);
        _mapscan("pause", PAUSE);
        _mapscan("kp+", KP_PLUS);
        _mapscan("kp-", KP_MINUS);
        _mapscan("kp*", KP_MULTIPLY);
        _mapscan("kp/", KP_DIVIDE);
        _mapscan("kp_enter", KP_ENTER);

        _mapscan("kp0", KP_0);
        _mapscan("kp1", KP_1);
        _mapscan("kp2", KP_2);
        _mapscan("kp3", KP_3);
        _mapscan("kp4", KP_4);
        _mapscan("kp5", KP_5);
        _mapscan("kp6", KP_6);
        _mapscan("kp7", KP_7);
        _mapscan("kp8", KP_8);
        _mapscan("kp9", KP_9);
        _mapscan("kp.", KP_PERIOD);

        #undef _mapscan
        #elif (defined(_GL45RW_WINDOW_GLFW3))

        #define _mapscan(_n, _s) _input_event_map[0][_n] = inputeventinfo_t{\
            .strname=_n, .iclass=inputclass::key,\
            .keycode=GLFW_KEY_ ## _s\
        }\

        for (char alpha = 'a'; alpha <= 'z'; alpha++){
            std::string alpha_s = " "; alpha_s[0] = alpha;
            _input_event_map[0][alpha_s] = inputeventinfo_t{.strname=alpha_s, .iclass=inputclass::key, .keycode=(GLFW_KEY_A + int(alpha - 'a'))};
        }

        for (int digit = 0; digit < 10; digit++){
            std::string digit_s = std::to_string(digit);
            _input_event_map[0][digit_s] = inputeventinfo_t{.strname=digit_s, .iclass=inputclass::key, .keycode=(GLFW_KEY_0 + digit)};
        }

        _mapscan("up", UP);
        _mapscan("down", DOWN);
        _mapscan("left", LEFT);
        _mapscan("right", RIGHT);
        _mapscan("escape", ESCAPE);
        _mapscan("space", SPACE);
        _mapscan("tilde", GRAVE_ACCENT);

        _mapscan(";", SEMICOLON);
        _mapscan("[", LEFT_BRACKET);
        _mapscan("]", RIGHT_BRACKET);
        _mapscan("-", MINUS);
        _mapscan("=", EQUAL);
        _mapscan("backspace", BACKSPACE);
        _mapscan("return", ENTER);
        _mapscan(",", COMMA);
        _mapscan(".", PERIOD);
        _mapscan("backslash", BACKSLASH);
        _mapscan("/", SLASH);
        _mapscan("lshift", LEFT_SHIFT);
        _mapscan("rshift", RIGHT_SHIFT);
        _mapscan("lctrl", LEFT_CONTROL);
        _mapscan("rctrl", RIGHT_CONTROL);
        _mapscan("lalt", LEFT_ALT);
        _mapscan("ralt", RIGHT_ALT);
        _mapscan("tab", TAB);

        _mapscan("insert", INSERT);
        _mapscan("delete", DELETE);
        _mapscan("pageup", PAGE_UP);
        _mapscan("pagedown", PAGE_DOWN);
        _mapscan("capslock", CAPS_LOCK);
        _mapscan("sysrq", PRINT_SCREEN);
        _mapscan("scrlock", SCROLL_LOCK);
        _mapscan("pause", PAUSE);
        _mapscan("kp+", KP_ADD);
        _mapscan("kp-", KP_SUBTRACT);
        _mapscan("kp*", KP_MULTIPLY);
        _mapscan("kp/", KP_DIVIDE);
        _mapscan("kp_enter", KP_ENTER);

        _mapscan("kp0", KP_0);
        _mapscan("kp1", KP_1);
        _mapscan("kp2", KP_2);
        _mapscan("kp3", KP_3);
        _mapscan("kp4", KP_4);
        _mapscan("kp5", KP_5);
        _mapscan("kp6", KP_6);
        _mapscan("kp7", KP_7);
        _mapscan("kp8", KP_8);
        _mapscan("kp9", KP_9);
        _mapscan("kp.", KP_DECIMAL);

        #undef _mapscan
        glfwSetJoystickCallback(_gl45rw_glfw3_joystick_callback);
        #endif

        for (int f = 0; f < 24; f++) //function keys F1 thru F24
        {
            std::string fkname = "F";
            fkname += std::to_string(f+1);

            _input_event_map[0][fkname] = inputeventinfo_t{
                .strname=fkname,
                .iclass=inputclass::key

                #if (defined(_GL45RW_WINDOW_SDL2))
                 ,.keycode=SDL_Scancode(int(SDL_SCANCODE_F1) + f)
                #elif (defined(_GL45RW_WINDOW_GLFW3))
                 ,.keycode=(GLFW_KEY_F1 + f)
                #endif
            };
        }
        const std::string joyaxisdirs = "-+";
        for (int pad = 0; pad < GL45RW_MAX_GAMEPADS; pad++){
            std::string padname = "gp";
            padname += std::to_string(pad);

            for (int button = 0; button < GL45RW_GAMEPAD_MAXBUTTONS; button++){
                const std::string padbtnname = padname + "b" + std::to_string(button);
                _input_event_map[0][padbtnname] = inputeventinfo_t{
                    .strname=padbtnname,
                    .iclass=inputclass::joybutton,
                    #ifndef _GL45RW_HEADLESS
                    #if (defined(_GL45RW_WINDOW_SDL2))
                    .joybtn=(SDL_GameControllerButton)button,
                    #elif (defined(_GL45RW_WINDOW_GLFW3))
                    .joybtn=button,
                    #endif
                    #endif
                    .joyindex=pad
                };
            }
            for (int axis = 0; axis < GL45RW_GAMEPAD_NUMAXES; axis++) {
                for (auto axisdir : joyaxisdirs) {
                    std::string padaxisname = padname + "a" + std::to_string(axis);
                    padaxisname.push_back(axisdir);
                    _input_event_map[0][padaxisname] = inputeventinfo_t{
                        .strname=padaxisname,
                        .iclass=inputclass::joyaxis,
                        #ifndef _GL45RW_HEADLESS
                        #if (defined(_GL45RW_WINDOW_SDL2))
                        .joyaxis=(SDL_GameControllerAxis)axis,
                        #elif (defined(_GL45RW_WINDOW_GLFW3))
                        .joyaxis=axis,
                        #endif
                        #endif
                        .joyindex=pad,
                        .joyaxisfwd=(axisdir == '+')
                    };
                }
            }
        }

        _input_event_map[0]["mmovex-"] = inputeventinfo_t{.strname="mmovex-", .iclass=inputclass::mouseaxis, .mouseaxisno=0, .mouseaxisfwd=false};
        _input_event_map[0]["mmovex+"] = inputeventinfo_t{.strname="mmovex+", .iclass=inputclass::mouseaxis, .mouseaxisno=0, .mouseaxisfwd=true};
        _input_event_map[0]["mmovey-"] = inputeventinfo_t{.strname="mmovey-", .iclass=inputclass::mouseaxis, .mouseaxisno=1, .mouseaxisfwd=false};
        _input_event_map[0]["mmovey+"] = inputeventinfo_t{.strname="mmovey+", .iclass=inputclass::mouseaxis, .mouseaxisno=1, .mouseaxisfwd=true};

        _globally_initialized = true;

    }
    void global_shutdown()
    {
        if (!_globally_initialized){return;}
        while (_engine_instances->size() > 0)
        {
            if (_engine_instances->back() == nullptr){_engine_instances->pop_back(); continue;}
            _del_engine_instance(_engine_instances->back());
        }
        delete _engine_instances; _engine_instances = nullptr;
        delete _engine_instances_byid; _engine_instances_byid = nullptr;
        delete _instances_by_winid; _instances_by_winid = nullptr;
        delete _input_event_map; _input_event_map = nullptr;
        #ifdef _GL45RW_WINDOW_GLFW3
        delete _instances_by_glfw_window; _instances_by_glfw_window = nullptr;
        #endif
        #ifdef GL45RW_WITH_LUAJIT
        delete _lua_engines_bystate; _lua_engines_bystate = nullptr;
        #endif
        #ifndef _GL45RW_HEADLESS
         #if (defined(_GL45RW_WINDOW_SDL2))
         SDL_Quit();
         #elif (defined(_GL45RW_WINDOW_GLFW3))
         glfwTerminate();
         #endif
        #endif
    }

    void global_mainloop(){
        if (!_globally_initialized){return;}
        clockticks_t lastclock = GetClockTicks();
        fclock_t ticdelta, uptime = 0;
        std::vector<EngineInstance*> delete_list = {};
        std::vector<uid_t> tip_deletes;

        while (_engine_instances->size() > 0) {
            clockticks_t curclock = GetClockTicks();
            clockticks_t deltaticks = curclock - lastclock;
            lastclock = curclock;
            ticdelta = GetTimeForTicks(deltaticks);
            uptime += ticdelta;
            const MainloopData mld = {
                .ticdelta=ticdelta,
                .uptime=uptime
            };
            for (auto* ei : *_engine_instances){
				for (auto tip : ei->touch_events_byid){
					if (tip.second.state == EngineInstance::touchstate::up){
						tip_deletes.push_back(tip.first);
					}
				}
				for (uid_t& fid : tip_deletes){
					//std::cout << "delete touch event id=" << fid << ".\n";
					ei->touch_events_byid.erase(fid);
				}
				tip_deletes.clear();
				ei->touch_events.clear();
			}

            #ifndef _GL45RW_HEADLESS
                 #if (defined(_GL45RW_WINDOW_SDL2))
                 SDL_Event evt;
                 //SDL_PumpEvents();
                 while (SDL_PollEvent(&evt))
                 {
                    switch (evt.type)
                    {
                       case SDL_CONTROLLERDEVICEADDED: {
                           _gl45rw_pending_gamepad_device_reenum = true;
                           break;
                       }
                       case SDL_WINDOWEVENT:
                       {
                          EngineInstance* tgi = E_GetInstanceForWindow(evt.window.windowID);
                          if (tgi == nullptr){break;}
                          switch (evt.window.event)
                          {
                             case SDL_WINDOWEVENT_CLOSE:
                             {
                                tgi->_onWindowCloseEvent();
                                break;
                             }
                          }
                          break;
                       }
                       case SDL_MOUSEMOTION: {
                           if (evt.motion.which == SDL_TOUCH_MOUSEID){break;}
                           EngineInstance* tgi = E_GetInstanceForWindow(evt.motion.windowID);
                           if (tgi != nullptr && tgi->E_GetInputGrabbed()){
                               tgi->_rel_mouse_offset += fvec2_t{evt.motion.xrel, evt.motion.yrel};
                           }
                           break;
                       }
                       case SDL_TEXTINPUT: {
                           EngineInstance* tgi = E_GetInstanceForWindow(evt.text.windowID);
                           if (tgi == nullptr){break;}
                           std::string txtu8 = evt.text.text;
                           EngineInstance::textinputevent_t tievt = {
                               ._class=EngineInstance::textinputclass::new_chars,
                               .newchars=djutil::ezstr::utf8_to_utf32(txtu8)
                           };
                           tgi->text_events.push_back(tievt);
                           break;
                       }
                       case SDL_FINGERDOWN: {
                           #ifndef GL45RW_NO_TOUCHSCREEN_CODE
						   //std::cout << "SDL2 finger down: windowID=" << evt.tfinger.windowID << ",fingerId=" << evt.tfinger.fingerId << ",x=" << evt.tfinger.x << ",y=" << evt.tfinger.y << "\n";
						   EngineInstance* tgi = E_GetInstanceForWindow(evt.tfinger.windowID);
						   if (tgi == nullptr){break;}
						   auto& touchevt = tgi->touch_events_byid[evt.tfinger.fingerId] = EngineInstance::touchevent_t{
							   .id=uid_t(evt.tfinger.fingerId),
							   .state=EngineInstance::touchstate::down,
							   .oxy=fvec2_t{evt.tfinger.x, evt.tfinger.y},
							   .cxy=fvec2_t{evt.tfinger.x, evt.tfinger.y},
							   .dxy=fvec2_t{0,0}
						   };
						   tgi->touch_events.push_back(touchevt);
                           #endif
						   break;
					   }
					   case SDL_FINGERMOTION: {
                           #ifndef GL45RW_NO_TOUCHSCREEN_CODE
						   EngineInstance* tgi = E_GetInstanceForWindow(evt.tfinger.windowID);
						   if (tgi == nullptr){break;}
						   EngineInstance::touchevent_t* teptr = nullptr;
						   try {
							   teptr = &(tgi->touch_events_byid[evt.tfinger.fingerId]);
						   }
						   catch (const std::exception& exc){
							   break;
						   }
						   auto& touchevt = *teptr;
						   touchevt.state = EngineInstance::touchstate::motion;
						   touchevt.dxy = fvec2_t{evt.tfinger.dx, evt.tfinger.dy};
						   touchevt.cxy += touchevt.dxy;
						   tgi->touch_events.push_back(touchevt);
						   //std::cout << "SDL2 finger move: windowID=" << evt.tfinger.windowID << ",fingerId=" << evt.tfinger.fingerId << ",x=" << touchevt.cxy.x << ",y=" << touchevt.cxy.y << "\n";
                           #endif
						   break;
					   }
					   case SDL_FINGERUP: {
                           #ifndef GL45RW_NO_TOUCHSCREEN_CODE
						   //std::cout << "SDL2 finger up: windowID=" << evt.tfinger.windowID << ",fingerId=" << evt.tfinger.fingerId << "\n";
						   EngineInstance* tgi = E_GetInstanceForWindow(evt.tfinger.windowID);
						   if (tgi == nullptr){break;}
						   try {
							   auto& touchevt = tgi->touch_events_byid.at(evt.tfinger.fingerId);
							   touchevt.cxy = fvec2_t{evt.tfinger.x, evt.tfinger.y};
							   touchevt.dxy = fvec2_t(0);
							   touchevt.state = EngineInstance::touchstate::up;
							   tgi->touch_events.push_back(touchevt);
						   } catch (const std::exception& exc){;}
                           #endif
						   break;
					   }
                       case SDL_KEYDOWN: {
                           EngineInstance* tgi = E_GetInstanceForWindow(evt.key.windowID);
                           if (tgi == nullptr){break;}
                           switch (evt.key.keysym.scancode) {
                               case SDL_SCANCODE_TAB: {
                                   EngineInstance::textinputevent_t tievt = {
                                       ._class=EngineInstance::textinputclass::tab
                                   };
                                   tgi->text_events.push_back(tievt);
                               	   break;
                               }
                               case SDL_SCANCODE_BACKSPACE: {
                                   EngineInstance::textinputevent_t tievt = {
                                       ._class=EngineInstance::textinputclass::backspace
                                   };
                                   tgi->text_events.push_back(tievt);
                                   break;
                               }
                               case SDL_SCANCODE_RETURN: {
                                   EngineInstance::textinputevent_t tievt = {
                                       ._class=EngineInstance::textinputclass::enter
                                   };
                                   tgi->text_events.push_back(tievt);
                                   break;
                               }
                               case SDL_SCANCODE_UP: {
                                   EngineInstance::textinputevent_t tievt = {
                                       ._class=EngineInstance::textinputclass::hist_up
                                   };
                                   tgi->text_events.push_back(tievt);
                                   break;
                               }
                               case SDL_SCANCODE_DOWN: {
                                   EngineInstance::textinputevent_t tievt = {
                                       ._class=EngineInstance::textinputclass::hist_down
                                   };
                                   tgi->text_events.push_back(tievt);
                                   break;
                               }
                               default: ;
                            }
                            break;
                       }
                       default: break;
                    }
                 }
                 _gl45rw_UpdateGamepadStates();
                 #elif (defined(_GL45RW_WINDOW_GLFW3))
                 glfwPollEvents();
                 _gl45rw_UpdateGamepadStates();
                 /*
                 for (auto* ei : *_engine_instances){
                     if (ei == nullptr){continue;}
                     auto& uwin = ei->uwin;
                     if (uwin.is_closed()){continue;}
                     GLFWwindow* _win = uwin.win;
                     if (glfwWindowShouldClose(_win)){
                         ei->_onWindowCloseEvent();
                         glfwSetWindowShouldClose(_win, GLFW_FALSE);
                     }
                 }
                 */
                 #endif
            #endif

            for (auto* ei : *_engine_instances)
            {
                //std::cout << "DO UPDATE\n";
                if (ei == nullptr || !(ei->_do_update(mld))){delete_list.push_back(ei);}
                if (ei == nullptr){continue;}
                ei->text_events.clear();
                if ((ei->_rel_mouse_updclock += mld.ticdelta)){ei->_rel_mouse_offset = fvec2_t(0);}
                //ei->_rel_mouse_offset = _mathstuff::gmi(ei->_rel_mouse_offset, fvec2_t(0), fvec2_t(0.1f), 1.0f);
            }
            while (delete_list.size() > 0)
            {
                std::cout << "pop from delete list.\n";
                EngineInstance* eid = delete_list.back();
                std::cout << eid << '\n';
                delete_list.pop_back();
                _del_engine_instance(eid);
            }
        }
    }
};

#endif
