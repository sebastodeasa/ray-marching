# RayMarching

## About

This is a simple ray marching renderer implemented in C++, using SFML for window management, input handling, and drawing the final rendered images to the window. The rendering logic itself is implemented from scratch, and marches a ray through a scene full of 3D objects for each pixel independently.

The renderer supports the simple 3D scene objects of spheres, boxes, and planes, in addition to a 3D Mandelbulb fractal object.

Currently, all objects must be hardcoded into the scene within the 'main()' function in RayMarching.cpp.


This is a personal project that I started alongside my college C++ and programming courses in 2022. Since graduating in 2025, I have decided to come back to this project and continue refactoring/expanding the codebase, both as a way to keep my C++ skills sharp and to continue to have fun.

## Gallery

![alt text](videos/raymarching1.mp4 "raymarching1.mp4")

A video of the ray marcher rendering a scene with a box and two spheres, one of which is animated to move up and down.

![alt text](videos/raymarching2.mp4 "raymarching2.mp4")

A video of the ray marcher rendering a scene with the same box and two spheres; however, it is now set to march rays using the maximum distance from the ray to the scene objects rather than the minimum, enabling boolean rendering. The spheres are both set to have negative distance functions, while the box has the usual positive distance function, resulting in the spheres being subtracted from the box's geometry.

![alt text](videos/raymarching3.mp4 "raymarching3.mp4")

A video of the ray marcher rendering a scene with a mandelbulb, which is having its parameters dynamically updated over time.



## Controls

The camera is moved via the WASD keys, and the mouse is used to look around. Hold down left ctrl to zoom the camera's FOV.

If the mandelbulb is in the scene and the corresponding code is uncommented, the arrow keys can be used to adjust the parameters of the mandelbulb function:
- Up/down to increase/decrease the "power" of the mandelbulb
- Left/right to increase/decrease the number of iterations in the mandelbulb function calculation.