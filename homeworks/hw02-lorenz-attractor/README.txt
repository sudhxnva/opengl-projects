Lorenz Attractor Visualization (OpenGL in C)
--------------------------------------------

This program visualizes the Lorenz Attractor using OpenGL. It allows interactive control of
viewing angles, system parameters, and includes an animated ball tracing the attractor path.

Usage Instructions
------------------

Controls:

  Arrow keys : Rotate the view (left/right/up/down)
  b          : Toggle the animated ball
  c          : Toggle attractor color (white ↔ gradient)
  s / S      : Decrease / Increase the 's' parameter
  r / R      : Decrease / Increase the 'r' parameter
  q / Q      : Decrease / Increase the 'b' parameter

Each time a parameter is changed, the attractor is regenerated.

Notes
-----

- The attractor is visualized as a continuous line (GL_LINE_STRIP).
- The animated ball moves along the attractor in a loop.
- The view uses an orthographic projection that adjusts with window size.
- The title bar shows the current values of s, r, and b (optional enhancement).
- Interesting Note: The ball speeds up when moving through the outer loops of the attractor and slows down in the inner loops.

Time Taken : Approximately 3 hours
