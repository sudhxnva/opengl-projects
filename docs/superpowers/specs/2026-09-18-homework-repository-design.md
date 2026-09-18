# OpenGL Homework Repository Design

## Purpose

Create a polished public GitHub repository containing only the five labeled CSCI 5529 OpenGL homework submissions. Course exercises and final-project directories remain local and are excluded from the repository.

## Repository Layout

```text
.
├── README.md
├── .gitignore
└── homeworks/
    ├── hw02-lorenz-attractor/
    ├── hw03-saturn-scene/
    ├── hw04-ufo-lighting/
    ├── hw05-multi-object-lighting/
    └── hw06-textured-3d-scene/
```

Each homework directory will include its source code, headers, Makefile, documentation, and runtime BMP textures when applicable. Existing per-homework readmes will remain with their submission content. The root README will summarize the collection and explain build prerequisites and usage.

## Inclusion and Exclusion Rules

Include only the material for these submissions:

| Source directory | Repository directory | Topic |
| --- | --- | --- |
| `week_3/hw2` | `homeworks/hw02-lorenz-attractor` | Lorenz attractor visualization |
| `week_4/hw_3` | `homeworks/hw03-saturn-scene` | Saturn scene and custom satellite |
| `week_5/hw_4` | `homeworks/hw04-ufo-lighting` | UFO lighting |
| `week_6/hw_5` | `homeworks/hw05-multi-object-lighting` | Lit UFO, rocket, and hourglass |
| `week_8/hw_6` | `homeworks/hw06-textured-3d-scene` | Textured 3D scene |

Exclude executables, object files, static libraries, ZIP submission archives, macOS metadata, and other generated build output. Exclude all course exercises (`week_0`, `week_1`, and exercise folders), projects (`project`, `project_v2`, `project_v3`, `steamtrain`), temporary material, virtual environments, and their nested Git repositories.

## Git Publishing

Initialize Git at the course-folder root. Configure `origin` as `git@github.com:sudhxnva/opengl-projects.git`, use branch `master`, commit only the curated repository contents, verify the staged and committed tree, then push the initial branch.

## Verification

Before the push, verify that tracked files are limited to the approved layout; generated artifacts and excluded directories are absent. Validate every retained Makefile with a dry-run, and check the README links and documented directories.
