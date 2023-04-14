# Flap
Passion Project

Console-based, multi-player, Snake clone. Previously a Flappy Bird clone, but the movement doesn't look right in the console. Should've thought about this earlier.

This project is for learning new techniques and showcasing my knowledge of: architecture; concurrency/parallelism; game logic, and networking.



Notes:
- Ran into an issue where the CollisionRender thread was running it's update before the Scene thread, because the Scene thread released it first. This cannot happen, because the scene needs to update first. Fixed it by adding another wait on the CollisionRender. May need a better solution, but for now, everything seems fine
- Implemented my own Doubly-Linked List and replaced all usages of std::list
- Pivoting and going down a different path :(
     - Collision and buffer filling will happen on the game thread
		- Too much synchronization with this current implementation and it's starting not to make any sense
     - Input will be on its own thread
     - Rendering will be on its own thread
		- A console project won't be able to take advantage of the speed of a separate render thread :(, but it could be used for key-frame interpolation :)

Thoughts:
- No work-tickets, since I'm working alone. I'm bouncing around, because I've been creating a backlong on Notepad++ and handling them as I build out the structure of the game.
- Yes, raw pointers
- I really like concurrency/parallelism and system architecture
- Sending collision notification was an issue, but it's now working fine
- I like C#'s comfort in certain aspects, like the fact that I can pass around member function pointers, without the object itself. If I have to pass the object to use the function pointer, what's the point of the function pointer?!