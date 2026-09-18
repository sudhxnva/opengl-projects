UFO Lighting Assignment (OpenGL in C)
-------------------------------------

This program renders a stylized UFO composed of manually constructed geometric primitives using OpenGL in C.
The UFO consists of two saucers joined back-to-back, topped with a glowing blue semi-spherical windshield.
 A yellow emissive beam projects downward from the bottom of the UFO using a capped frustum.

Usage Instructions
------------------

Controls:

  Arrow keys  : Rotate the camera view (left/right/up/down)
  < / >       : Manually rotate the light source around the UFO
  [ / ]       : Adjust the vertical position of the light
  a/A         : Decrease/Increase ambient lighting
  d/D         : Decrease/Increase diffuse lighting
  s/S         : Decrease/Increase specular lighting
  e/E         : Decrease/Increase emissive strength
  n/N         : Decrease/Increase shininess
  p           : Toggle between orthographic and perspective projection
  l           : Toggle lighting on/off
  m           : Toggle automatic light movement
  0           : Reset camera angles
  x           : Toggle coordinate axes
  ESC         : Exit the program

Features
--------

- The UFO is constructed without canned GLUT/GLU primitives, using custom geometry and manually calculated normals.
- Normals are slope-adjusted to provide proper lighting reflections on both the top and bottom saucers.
- A downward-facing beam is created using a capped frustum with emissive yellow glow.
- A moving point light source orbits the scene to highlight dynamic lighting interaction.
- Supports both smooth and flat shading models.

Time Taken : Approximately 10 hours
