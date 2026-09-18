Saturn Scene with Moons and Satellites (OpenGL in C)
----------------------------------------------------

This program visualizes a stylized Saturn-like planet using OpenGL (and yes, as I write this and look at it again, it looks more like a solar system than Saturn).
It features manually generated 3D objects, including Saturn, moons, and a user-defined satellite. The scene supports interactive controls for viewing angles
and toggling various elements. No canned GLUT or GLU objects (like glutSolidSphere) are used for the satellites.

Usage Instructions
------------------

Controls:

  Arrow keys : Rotate the view (left/right/up/down)
  m          : Toggle the moons on/off
  t          : Toggle the satellites on/off
  r          : Toggle automatic scene rotation around a 45° axis
  ESC        : Exit the program

Features
--------

- Saturn is drawn as a custom sphere with a warm-toned gradient based on th and ph angles.
- Moons orbit Saturn in the XZ plane and include orbit ring visuals.
- A satellite (not using glut primitives) orbits Saturn on a tilted axis.
- [NEW] Another satellite: No canned GLUT/GLU shapes are used for this, it's built from original geometry including:
  - A hand-constructed parabolic dish
  - Custom solar panels
  - A rod with a weighted tip
  - Antenna struts and extended poles
- These components are hierarchically combined to approximate the Voyager 1 satellite.
- Scene rotation can be toggled on to spin slowly around a 45° axis.

Notes
-----

- All objects (except Saturn) are manually constructed using quads and triangles.
- No canned GLUT/GLU shapes are used for the satellite to meet assignment requirements.
- Scene rotation adds dynamic visual appeal and helps showcase the 3D nature of the layout.

Time Taken : Approximately 5–6 hours
