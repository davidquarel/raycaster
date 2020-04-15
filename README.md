# raycaster

Raycaster in C by David Quarel, Jon Connor

![Picture of Raycaster](img/demo.gif)

![Texture Mapping](img/demo_texture.png)

# Dependencies
```
Ubuntu:
	sudo apt-get install make gcc libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev
Arch:
	pacman -S sdl2 sdl2_gfx sdl2_ttf sdl2_image 
```

# How to run
Compile with `make`, then run `./bin/ray`.

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
* P - toggle show game status
* KEYPAD_8 - move minimap up
* KEYPAD_5 - move minimap down
* KEYPAD_4 - move minimap left
* KEYPAD_6 - move minimap right

# TODO Now
* ~~Algorithm to cast rays is inefficient, and takes little 0.01 steps
forward till it hits a wall. Use Bresenhams line algorithm and some
maths to make it fast.~~ DONE
* New faster raycasting algorithm seems to still have some minor
distortion after correcting for fisheye effect. Investigate cause and fix.
* Make movement adjust player acceleration rather than velocity, and add friction
 so player movement feels more natural as compared to abrupt movement.
* ~~Game only updates where you hit WASD or LEFT/RIGHT. Eventually want
it to run at 30/60fps, and you hold WASD to constantly move in that direction.~~
DONE
* Add mouselook.
* ~~Learn how to and then implement texture mapping.~~ ~~Grey walls are boring~~ ~~Stripey
walls are less boring.~~ ~~Done, but it needs to be more efficient.~~ DONE
* ~~Don't draw walls outside view area.~~ Done? Needs to be verified.
* ~~Add the ability for the minimap to draw the rays that have been cast~~ DONE
* ~~Add collisions with walls~~ DONE
* Change the format for texture mapping rather than reinventing the wheel.
* Change the format of the map to have different kinds of walls that are
texture mapped in different ways.
* Adjust collision behaviour so you walk along a wall rather than
stopping immediately when you touch a wall.

# TODO Someday
* Add something for the player to interact with (items, monsters)
* Texture map the sky and the floor.
* Make some interesting maps, refactor to allow different levels to
be loaded from a file.
* Add some music/SFX from open domain material.
* Work out how to load lots of textures sensibly as .pngs
* Add support for sprites to draw the player and things in the world.
* Add multiplayer mode (split screen? multiple SDL windows?)
* Add network support for multiplayer over LAN
