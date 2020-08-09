# OpenGLGraphics

## Overview

The following project can read vertex and fragment GLSL shader code from a file and compile it using OpenGL. Meshes can also be defined by vertex and index data, and easily drawn to a window using OpenGL. The scene can be explored with a viewport camera.

![Alt Text](https://i.gyazo.com/51d7f8945ffbf3278d45fd6ef02d7624.png)

## Libraries

- OpenGL Extension Wrangler Library (GLEW) is used to determine which OpenGL extensions are supported on the target platform.
- OpenGL FrameWork (GLFW) is used for creating windows, contexts, surfaces and recieving input and events. It supports Multiple windows, monitors as well as inputs such as keyboard, mouse, gamepad and joystick.
- OpenGL Mathematics (GLM) is used for any linear algebra that is needed such as matrix and vector math.


## Future Plans

The intent is to experiment with 3D rendering, and implement basic features such as image/texture loading, texture mapping, lighting (direction, point and spot) using the Phong lighting model, model importing, shadow mapping and skyboxes.
