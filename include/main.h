#pragma once

namespace mkb2
{

/*
 * Single function representing all initialization code in the game prior to the game loop.
 *
 * Call this once before calling tick() in a loop.
 *
 * In the original game, initialization is spread between the DOL file and the mainloop REL file
 * prior to the game loop.
 */
void init();

/*
 * Single function representing an iteration of the game loop
 * as originally written in the mainloop REL prolog function.
 *
 * Call this in a loop in your code after calling init().
 */
void tick();

}