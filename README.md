# Projects 5 & 6: Lights, Camera & Action!

All project handouts can be found [here](https://cs1230.graphics/projects).

## Overview

## Action!
In this project, I extended my code to handle camera movement, completed my illumination model, 
and applied post-processing effects to my scenes using FBOs.

## Design Choices
For lighting, I chose to pass in an int as the uniform to represent the light type since I couldn't pass
in an enum. 0 represents direction, 1 represents spotlight, and 2 represents point light. Distinguishing
between the light types is important since attenuation and falloff only affect certain light types.
In terms of the shaders, I created a postprocess shader to handle the per-pixel (invert) filter and kernel-based (blur) filter.
I have two framebuffers, m_defaultFBO and m_fbo so that I can apply my filters after.
For the camera, I decided to keep a camera as a field. When a user presses W, A, S, D, the space bar, or
control (command for Mac users) or uses the mouse for rotational movement, the camera's position or look
vector can be easily changed without reloading the scene, and the view matrix will be updated. 

## Note
To change the value of the default framebuffer, change the number that m_defaultFBO is bound 
to on line 64 of realtime.cpp.

## Extra Credit
I did not implement any extra credit features.

## Bugs
There are no bugs to my knowledge.

## Lights, Camera
In this project, I designed a real-time scene viewer utilizing concepts such as including parsing, transformations, trimeshes, VAOs, and shaders.

## Design Choices
The utils directory contains code that handles scene parsing, filer reading, and shader loading. The shapes
directory contains code that builds the tesselations for each primitive shape. The Shape.cpp file contains
utility functions that abstracts out some of the logic for making the tesselations. The camera directory contains
the camera class with the necessary fields and methods for getting important camera data, such as the view and projection matricies.

The realtime.cpp file is where the scene is built. Whenever a scene is loaded, a VBO and VAO is made for each primitive
and stored in a hashmap from primitive name -> VBO/VAO id. The hashmaps are kept as fields in the realtime class. Whenever
the settings change, the VBOs and VAOs are remade. In paintGL, several uniforms are created and passed to the vertex and fragment shader.
The inverse transpose of the top 3x3 of the model matrix is stored as a field in realtime to avoid recalculation in
in the vertex shader. At the end, Realtime::finish() is called before the program exits to clear any generated memory to avoid memory leaks.

## Extra Credit
I did not implement any extra credit features.

## Bugs
There are no bugs to my knowledge.
