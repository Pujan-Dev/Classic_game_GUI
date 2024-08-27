

# 🎮 Classic Games Menu

Welcome to the **Classic Games Menu**! This project is a graphical user interface that allows you to access and play a collection of classic games. It features a stylish menu with hover effects and animations, making it easy and fun to navigate between games and view developer information.

## Features ✨

- **Interactive Main Menu**: 
  - Play classic games like Pacman, Snake, Pong, and Flappy Bird. 🎉
  - Hover effects for menu items to enhance user experience. 🎨
  - Animated title that responds to user interactions. 🌀

- **Developer Section**:
  - View details about the developers behind the project. 👥
  - Navigate back to the main menu easily. 🔙

- **Sound Effects**:
  - Includes sound effects for game interactions, such as button clicks and game-specific sounds. 🔊

- **Old and classic games**
  - classic titles like Pacman , Snake , Pong and Flappy bird.
## Games 🎮

### Pacman
Navigate Pacman through a maze, eating pellets and avoiding ghosts. The game mimics the classic arcade experience with grid-like movement. 🟡👻

### Snake
Control a snake to eat food and grow longer while avoiding collisions with walls and itself. The game features simple, intuitive controls and a growing challenge. 🐍🍎

### Pong
A classic Pong game where you control paddles to hit the ball back and forth. Play against a simple AI opponent or challenge a friend. 🏓🎯

### Flappy Bird
Navigate a bird through a series of pipes by tapping to flap. The game tests your timing and reflexes as you try to achieve the highest score. 🐦🌳

## Requirements 📋

- SFML (Simple and Fast Multimedia Library)
- C++ compiler

## Installation ⚙️

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/Pujan-Dev/Classic_game_GUI
    cd Classic_game_GUI
    ```

2. **Install SFML**:
    - Follow the [SFML installation guide](https://www.sfml-dev.org/download.php) for your platform.

3. **Compile the Project**:
    - Ensure you have the SFML libraries linked correctly in your build system.
    - Compile the project using a command like:
        ```bash
        g++ -o game main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
        ```

## Usage 🚀

1. **Run the Application**:
    ```bash
    ./game
    ```

2. **Navigate the Menu**:
    - Use your mouse to hover over and click on menu items. 🖱️
    - Click on "Pacman", "Snake", "Pong", or "Flappy Bird" to start the corresponding game. 🎮
    - Click on "Devs" to view developer information. 👤
    - Use the "Back" button to return to the main menu. 🔙

## File Structure 📂

- **main.cpp**: The main source file containing the menu logic and game integration.
- **flappy.ttf**: Font used for text rendering.
- **code/pacman.cpp**: Contains the logic for the Pacman game.
- **snake/src/snake.cpp**: Contains the logic for the Snake game.
- **Pong/src/PongGame.cpp**: Contains the logic for the Pong game.
- **Flappy_bird/main.cpp**: Contains the logic for the Flappy Bird game.
- **res/**: Directory containing sound effects and images used in the games.
  - **ball.png**: Image for the ball in Pong.
  - **numbers/**: Directory containing digit images (0.png to 9.png).
  - **paddle.wav**: Sound effect for paddle interactions in Pong.
  - **score.png**: Image for displaying the score in Pong.
  - **score.wav**: Sound effect for scoring in Pong.
  - **stick_1.png**, **stick_2.png**: Images for paddles in Pong.
  - **wall.wav**: Sound effect for wall interactions in Pong.

## Code Overview 🧩

- **handleButtonHover**: Changes the button color when the mouse hovers over it, providing visual feedback. 🎨
- **animateTitleOnHover**: Animates the title based on mouse interaction to make it more engaging. ✨
- **setTextProperties**: Configures the appearance of text elements including font, size, color, and outline. 🖋️

## Troubleshooting 🛠️

- **SFML Issues**: Ensure that SFML is correctly installed and linked with your project.
- **Font File**: Make sure the font file `flappy.ttf` is present in the working directory.
- **Sound Files**: Verify that sound files in the `res/` directory are correctly named and located.
- 
## Developer Section 👥
The Classic Games Menu is developed by a talented team of individuals who contributed their skills and creativity to bring this project to life. Here are the details about the developers:

**Pujan Neupane**: As the project lead and primary developer, Pujan brings a deep passion for game development and extensive experience with SFML and C++. He spearheaded the development of Pacman and made significant contributions to Pong and Flappy Bird. 📧 Email

**Sujal Karki**: Sujal played a crucial role in designing the game logic and user interface. His expertise in C++ and SFML was pivotal in developing the Snake game. 🧑‍💻

**Prasamsha Adhikari**: Prasamsha is the talented developer behind Pong. Her dedication to creating engaging gameplay and seamless interactions was essential to the game’s success. 🎵

**Samridhee Ghimire**: Samridhee developed Flappy Bird, adding creative and dynamic elements to enhance the game’s visual appeal and user experience. 🎨


## Contributing 🤝

Contributions are welcome! If you have suggestions or improvements, please fork the repository and submit a pull request.

## Contact 📫

For any questions or issues, please contact:

- **Pujan Neupane**: [Email](mailto:pujanneupaneop@gmail.com)

