OpenGL Lighting Assignment: UFO, Rocket, and Hourglass
------------------------------------------------------

This program visualizes a stylized 3D scene using OpenGL in C, showcasing three manually constructed objects:
a classic flying UFO, a rocket ship with boosters, and an hourglass. All objects are built
using custom geometry and manually specified normals to emphasize lighting behavior.

Usage Instructions
------------------

Controls:

  Arrow keys   : Rotate the camera (left/right/up/down) in orbit or FPV modes
  < / >        : Manually rotate the light source around the scene
  [ / ]        : Move the light source vertically
  a / A        : Decrease / Increase ambient light intensity
  d / D        : Decrease / Increase diffuse light intensity
  s / S        : Decrease / Increase specular light intensity
  e / E        : Decrease / Increase emissive light contribution
  n / N        : Decrease / Increase shininess
  m            : Toggle automatic light source movement
  l            : Toggle lighting on/off
  x            : Toggle coordinate axes
  o / O        : Cycle between object views (All, UFO, Rocket, Hourglass)
  p            : Toggle between orthographic and perspective projection
  n / N        : Switch between projection modes (Ortho, Perspective, First-Person)
  0            : Reset camera angles (for orbit views)
  w / a / s / d: Move in first-person mode (forward, strafe left, backward, strafe right)
  ESC          : Exit the program

Features
--------

- Custom-built 3D models with no GLUT or GLU primitives.
- UFO: Dual saucers with slope-adjusted normals and a glowing blue windshield
- Rocket: Cylindrical rocket body with a cone nose, surrounded by four boosters with exhaust cones.
- Hourglass: Two cones joined at the center with circular plates at the ends and a glowing emissive sand core inside.
- Lighting components include ambient, diffuse, specular, and emissive properties.
- Multiple view modes: orthogonal, perspective, and first-person navigation.
- Objects can be viewed individually or together in a combined scene.

Time Taken : Approximately 12 hours