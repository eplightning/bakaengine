bakaengine
==========

Simple and poorly designed game engine for my CG assignment in C++.

 * OpenGL 3.3 required
 * VS2013 solution included but porting to other build systems and platform shouldn't be too complicated
 * Configuration using ini files
 * Input system with custom bindings support (read from ini files)
 * Basic event support capable of dispatching to static and dynamic objects (dynamic = created in scene)
 * Component based game objects (Strategy pattern)
 * Resource system (simple, no streaming support) - filesystem loader included
 * Unfinished physics system (good enough for example game)
 * Supports all formats supported by assimp (pretty much most of the model formats) and SOIL (png, jpg, bmp, dds, tga)
 * Basic renderer supporting multiple light sources (point or directional)
 * Scene "graph" (flat)
 * Example game

Requirements
------------

 * assimp (models)
 * SOIL (textures)
 * glew (gl extensions)
 * glm (math library)
 * simpleini (ini files)
 * zlib (required by assimp)

