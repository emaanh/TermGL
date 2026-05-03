# TermGL

A terminal rendering library for real-time 2D/3D graphics.

The goal is to build a small software rendering pipeline from scratch: frame timing, buffers, primitives, projection, depth, and ASCII/ANSI output.

## Goals

- Render real-time graphics directly in the terminal
- Support simple 2D primitives and 3D scenes
- Build the rendering pipeline without relying on a windowing or graphics API
- Keep the code understandable and easy to modify

## Current Status

Right now this is mostly a sandbox for terminal animation, ASCII shading, and renderer experiments.

## Planned

- Framebuffer abstraction
- Line and triangle drawing
- Depth buffer
- 3D transforms and projection
- Mesh demos
- ANSI color output
- A small draw-call style API

## Build

```sh
g++ main.cpp -O2 -std=c++20 -o termgl
./termgl
