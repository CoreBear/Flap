# Flap
Passion Project

Console-based, multi-player, Snake clone. Previously a Flappy Bird clone, but the movement doesn't look right in the console. Should've thought about this earlier.

This project is for learning new techniques and showcasing my knowledge of: architecture; concurrency/parallelism; game logic, and networking.



Notes:
- Ran into an issue where the CollisionRender thread was running it's update before the Scene thread, because the Scene thread released it first. This cannot happen, because the scene needs to update first. Fixed it by adding another wait on the CollisionRender. May need a better solution, but for now, everything seems fine



Thoughts:
- Yes, raw pointers
- I really like concurrency/parallelism and system architecture
- Sending collision notification was an issue, but it's now working fine