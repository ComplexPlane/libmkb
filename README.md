# libmkb

A functionally-matching, embeddable Super Monkey Ball 2 decompilation.

## Goals

* **Readability.** Decompile Super Monkey Ball 2's physics, camera, and controls code to a clean, well-commented source that is enlightening to anyone seeking to understand _exactly_ how this game works.
* **Accuracy.** Be functionally-identical to the original game (aside from floating-point roundoff errors which are [unfortunately unavoidable](https://randomascii.wordpress.com/2013/07/16/floating-point-determinism/)). Closely follow the original source code so that `libmkb` can serve as an accurate reference.
* **Embeddability.** Work as a C++ library, allowing real gameplay with a graphical frontend. Compiling `libmkb` for the browser via [WebAssembly](https://webassembly.org/) with [three.js](https://threejs.org/) for graphics is my goal for an initial frontend.

## Non-Goals

* **Perfect Matching.** An alternate approach is to decompile the code such that it may be recompiled by the original compiler to perfectly match the original game's binary. However, this takes orders of magnitude more time and effort than a "functionally-matching" decompilation, and may stifle readability and embeddability.
* **Completeness.** At the moment I think the most salient parts of the game to focus on are physics, camera, and controls, meanwhile graphics, menus, and character animation for example are less important initially. If you're motivated to work on these however, feel free!
