## Raycasting Game
Overview

This is a raycasting-based game written in C using the SDL2 library. The game implements a basic first-person 3D environment, where the player can move, strafe, and rotate their view, much like classic retro-style games.
Controls

    Move Forward: W
    Move Backward: S
    Strafe Right: D
    Strafe Left: A
    Rotate Right: Right Arrow
    Rotate Left: Left Arrow
    Toggle Minimap: M

How to Run

To run the game, you need to have GCC and SDL2 installed on your system. Follow the steps below to compile and run the game:

    Clone the repository or download the source files.

    Open a terminal and navigate to the src directory.

    Compile the game with the following command:

    bash

gcc *.c -lm -lSDL2 -lSDL2_image -o raycasting_game

Run the compiled game:

bash

    ./raycasting_game map.txt

License

This project is licensed under the MIT License. See the LICENSE file for more details.


MIT License

Copyright (c) 2024 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

How to Contribute

Contributions are welcome! If you'd like to help improve the game, please follow these steps:

    Fork the repository: Click on the fork button at the top right of the repository page to create your own version of the project.

    Clone the repository: Clone your forked repository to your local machine.


git clone https://github.com/dalhatmd/the_maze.git

Create a new branch: Create a branch for your new feature or bug fix.


git checkout -b feature-name

Make your changes: Work on the changes or fixes in your local environment.

Commit and push: Commit your changes and push them to your forked repository.


    git add .
    git commit -m "Add description of your changes"
    git push origin feature-name

    Create a pull request: Open a pull request to the main repository with a description of your changes and why they should be merged.

Make sure your contributions follow the coding standards and guidelines provided in the repository.

Thank you for checking out the project, and feel free to reach out if you have any questions!
