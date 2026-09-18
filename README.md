# OpenGL Homeworks

A curated collection of C/OpenGL assignments from CSCI 5529: Computer Graphics. Each project is interactive and uses keyboard controls described in its local README.

## Contents

| Homework | Topic |
| --- | --- |
| [HW 2: Lorenz Attractor](homeworks/hw02-lorenz-attractor/) | Interactive visualization of the Lorenz attractor |
| [HW 3: Saturn Scene](homeworks/hw03-saturn-scene/) | Custom 3D Saturn system with moons and satellite geometry |
| [HW 4: UFO Lighting](homeworks/hw04-ufo-lighting/) | Manually modeled UFO with configurable lighting |
| [HW 5: Multi-Object Lighting](homeworks/hw05-multi-object-lighting/) | Lit UFO, rocket, and hourglass scene with multiple camera modes |
| [HW 6: Textured 3D Scene](homeworks/hw06-textured-3d-scene/) | Textured extension of the multi-object lighting scene |

## Building and running

These assignments are written in C and use OpenGL with GLUT/freeglut. Install a C compiler, `make`, and your platform's OpenGL/GLUT development libraries, then build from an individual homework directory:

```bash
cd homeworks/hw02-lorenz-attractor
make
./hw2
```

The executable name matches the homework number (`hw2` through `hw6`). See each homework's README for controls and assignment-specific details.

## Repository scope

This repository intentionally contains only the completed homework submissions. Course exercises, work-in-progress material, and final projects are excluded.
