# raycaster

Raycaster in C by David Quarel

![Picture of Raycaster](img/demo.gif)

# Dependencies
```
sudo apt-get install libsdl2-dev 
```

# How to run
Run the makefile with `make`.

Run `./bin/ray`

# How to play
* W - Move Forward
* S - Move backward
* A - Strafe left
* D - Strafe right
* LEFT - Rotate left
* RIGHT - Rotate right
* Q - decrease FOV
* E - increase FOV
* M - toggle minimap
* K - increase minimap size
* L - decrease minimap size

# TODO
* Algorithm to cast rays is inefficient, and takes little 0.01 steps
forward till it hits a wall. Use Bresenhams line algorithm and some
maths to make it fast.
* Game only updates where you hit WASD or LEFT/RIGHT. Eventually want
it to run at 30/60fps, and you hold WASD to constantly move in that direction,
and look using the mouse.
* Learn how to and then implement texture mapping. ~~Grey walls are boring~~ Stripey
walls are less boring.
* ~~Add the ability for the minimap to draw the rays that have been cast~~ DONE
* Add collisions with walls
* Add something for the player to interact with (items, monsters)
