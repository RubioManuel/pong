# C Pong
A simple pong game written in C using raylib library.
---
## Test it out
To play the game, you must have installed raylib library in your computer with GNU/Linux 
(i don't know if the game works on another system, i only tested it on Linux). Check the
raylib documentation at https://www.raylib.com/ for C.

Once you have raylib set up. Just do
```
git clone https://github.com/rxxbyy/pong
cd ./pong
```
then run the simplest possible build command that gives raylib and run the game
```
gcc -o pong pong.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./pong
```
_NOTE: vertical player's collisions doesn't works too good_
