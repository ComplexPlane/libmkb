# libmkb

A functionally-matching, embeddable Super Monkey Ball 2 decompilation.

## Goals

* Decompile Super Monkey Ball 2's physics, camera, and controls code to a clean, well-commented source which closely follows the original source and is enlightening to anyone seeking to understand _exactly_ how this game works.
* Be functionally-identical to the original game (aside from floating-point roundoff errors which are unfortunately unavoidable).
* Work as a C++ library, allowing real gameplay with a graphical frontend. Compiling `libmkb` for the browser via [WebAssembly](https://webassembly.org/) alongside a [three.js](https://threejs.org/) graphical frontend is my goal for an initial frontend.

## Non-Goals

* Perfectly match the original game: another approach is to decompile the code such that it may be recompiled by the original compiler to perfectly match the original game's binary. However, this takes orders of magnitude more time and effort than a "functionally-matching" decompilation, and may stifle readability and embeddability.
* Decompile the entire game: at the moment I think the most salient components to focus on are physics, camera, and controls, and things like graphics, menus, and character animation are less important. If you feel motivated to work on these though, feel free!
