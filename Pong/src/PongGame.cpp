#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

const float windowWidth = 800.0f;
const float windowHeight = 600.0f;
const float paddleWidth = 20.0f;
const float paddleHeight = 100.0f;
const float ballRadius = 10.0f;
const float paddleSpeed = 10.0f; // Increased speed for responsive control
const float ballSpeed = 5.0f;

enum class GameState
{
    Playing,
    GameOver
};

class Paddle
{
public:
    Paddle(float x, float y)
    {
        shape.setSize({paddleWidth, paddleHeight});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(6.0f);
    }

    void moveUp()
    {
        if (shape.getPosition().y > 0)
        {
            shape.move(0.0f, -paddleSpeed);
        }
    }

    void moveDown()
    {
        if (shape.getPosition().y < windowHeight - paddleHeight)
        {
            shape.move(0.0f, paddleSpeed);
        }
    }

    sf::FloatRect getBounds()
    {
        return shape.getGlobalBounds();
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

private:
    sf::RectangleShape shape;
};
class Ball
{
public:
    Ball(float x, float y)
    {
        shape.setRadius(ballRadius);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2.0f);
        velocity = {-ballSpeed, -ballSpeed};
    }

    void update()
    {
        shape.move(velocity);

        // Check for collisions with the top and bottom walls
        if (shape.getPosition().y <= 0 || shape.getPosition().y >= windowHeight - 2 * ballRadius)
        {
            velocity.y = -velocity.y;
            wallSound.play();
        }
    }

    void reset()
    {
        shape.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
        velocity = {-ballSpeed, -ballSpeed};
    }

    void bounceX()
    {
        velocity.x = -velocity.x;
        paddleSound.play();
    }

    float getY()
    {
        return shape.getPosition().y;
    }

    float getX()
    {
        return shape.getPosition().x;
    }

    sf::FloatRect getBounds()
    {
        return shape.getGlobalBounds();
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

    void setSounds(sf::Sound &paddle, sf::Sound &wall)
    {
        paddleSound = paddle;
        wallSound = wall;
    }

    void setPosition(float x, float y)
    {
        shape.setPosition(x, y);
    }

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Sound paddleSound;
    sf::Sound wallSound;
};

class Score
{
public:
    Score()
    {
        font.loadFromFile("Pong/res/arial.ttf");
        leftScoreText.setFont(font);
        rightScoreText.setFont(font);
        leftScoreText.setCharacterSize(50);
        rightScoreText.setCharacterSize(50);
        leftScoreText.setPosition(200, 20);
        rightScoreText.setPosition(windowWidth - 250, 20);

        leftScore = 0;
        rightScore = 0;
        leftScoreText.setString(std::to_string(leftScore));
        rightScoreText.setString(std::to_string(rightScore));
    }

    void updateScore(bool leftScored)
    {
        if (leftScored)
        {
            leftScore++;
        }
        else
        {
            rightScore++;
        }
        leftScoreText.setString(std::to_string(leftScore));
        rightScoreText.setString(std::to_string(rightScore));
    }

    int getLeftScore() const
    {
        return leftScore;
    }

    int getRightScore() const
    {
        return rightScore;
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(leftScoreText);
        window.draw(rightScoreText);
    }

private:
    sf::Font font;
    sf::Text leftScoreText;
    sf::Text rightScoreText;
    int leftScore;
    int rightScore;
};

class Game
{
public:
    Game() : window(sf::VideoMode(windowWidth, windowHeight), "Pong Game"),
             playerPaddle(50.0f, windowHeight / 2 - paddleHeight / 2),
             aiPaddle(windowWidth - 50.0f - paddleWidth, windowHeight / 2 - paddleHeight / 2),
             ball(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius),
             score(), gameState(GameState::Playing)
    {
        window.setFramerateLimit(60); // Set frame rate limit to 60 FPS

        paddleSoundBuffer.loadFromFile("Pong/res/paddle.wav");
        wallSoundBuffer.loadFromFile("Pong/res/wall.wav");
        scoreSoundBuffer.loadFromFile("Pong/res/score.wav");

        paddleSound.setBuffer(paddleSoundBuffer);
        wallSound.setBuffer(wallSoundBuffer);
        scoreSound.setBuffer(scoreSoundBuffer);

        ball.setSounds(paddleSound, wallSound);

        gameOverFont.loadFromFile("Pong/res/arial.ttf");

        gameOverText.setFont(gameOverFont);
        gameOverText.setCharacterSize(50);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setPosition(windowWidth / 2 - 150, windowHeight / 2 - 50);

        restartButton.setSize({200, 50});
        restartButton.setFillColor(sf::Color::Green);
        restartButton.setPosition(windowWidth / 2 - 100, windowHeight / 2 + 50);

        quitButton.setSize({200, 50});
        quitButton.setFillColor(sf::Color::Red);
        quitButton.setPosition(windowWidth / 2 - 100, windowHeight / 2 + 120);

        restartText.setFont(gameOverFont);
        restartText.setCharacterSize(25);
        restartText.setFillColor(sf::Color::White);
        restartText.setString("Restart");
        restartText.setPosition(windowWidth / 2 - 50, windowHeight / 2 + 60);

        quitText.setFont(gameOverFont);
        quitText.setCharacterSize(25);
        quitText.setFillColor(sf::Color::White);
        quitText.setString("Quit");
        quitText.setPosition(windowWidth / 2 - 35, windowHeight / 2 + 130);

        // Initialize center hollow circle
        centerCircle.setRadius(25.0f); // Adjust the radius as needed
        centerCircle.setFillColor(sf::Color(0, 0, 0, 0.5)); // Fully transparent fill color
        centerCircle.setOutlineColor(sf::Color::White); // Outline color
        centerCircle.setOutlineThickness(3.0f); // Thickness of the outline
        centerCircle.setPosition(windowWidth / 2 - centerCircle.getRadius(), windowHeight / 2 - centerCircle.getRadius());
    }

    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    Paddle playerPaddle;
    Paddle aiPaddle;
    Ball ball;
    Score score;
    sf::SoundBuffer paddleSoundBuffer;
    sf::SoundBuffer wallSoundBuffer;
    sf::SoundBuffer scoreSoundBuffer;
    sf::Sound paddleSound;
    sf::Sound wallSound;
    sf::Sound scoreSound;
    GameState gameState;

    sf::Font gameOverFont;
    sf::Text gameOverText;
    sf::RectangleShape restartButton;
    sf::RectangleShape quitButton;
    sf::Text restartText;
    sf::Text quitText;

    sf::CircleShape centerCircle; // Hollow circle for visual effect

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (gameState == GameState::Playing)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                playerPaddle.moveUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                playerPaddle.moveDown();
            }
        }
        else if (gameState == GameState::GameOver)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (restartButton.getGlobalBounds().contains(mousePos))
                {
                    resetGame();
                }
                else if (quitButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                }
            }
        }
    }
   void update()
{
    if (gameState == GameState::Playing)
    {
        ball.update();

        // AI movement with reduced speed
        float aiPaddleSpeed = paddleSpeed * 0.3f; // Adjust AI paddle speed here (slower)
        float aiPaddleCenterY = aiPaddle.getBounds().top + aiPaddle.getBounds().height / 2;
        float ballCenterY = ball.getY() + ballRadius;

        // Move AI paddle towards the ball with the adjusted speed
        if (ballCenterY < aiPaddleCenterY - aiPaddleSpeed) // Move up
        {
            if (aiPaddle.getBounds().top > 0)
            {
                aiPaddle.moveUp();
            }
        }
        else if (ballCenterY > aiPaddleCenterY + aiPaddleSpeed) // Move down
        {
            if (aiPaddle.getBounds().top < windowHeight - paddleHeight)
            {
                aiPaddle.moveDown();
            }
        }

        // Collision and scoring logic
        if (ball.getBounds().intersects(playerPaddle.getBounds()))
        {
            ball.bounceX();
            ball.setPosition(playerPaddle.getBounds().left + playerPaddle.getBounds().width, ball.getY());
        }

        if (ball.getBounds().intersects(aiPaddle.getBounds()))
        {
            ball.bounceX();
            ball.setPosition(aiPaddle.getBounds().left - ballRadius * 2, ball.getY());
        }

        if (ball.getX() < 0)
        {
            score.updateScore(false);
            ball.reset();
            scoreSound.play();
            checkGameOver();
        }
        else if (ball.getX() > windowWidth)
        {
            score.updateScore(true);
            ball.reset();
            scoreSound.play();
            checkGameOver();
        }
    }
}

    void render()
    {
        window.clear(sf::Color(56, 56, 56));

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(windowWidth / 2, 0), sf::Color::White),
            sf::Vertex(sf::Vector2f(windowWidth / 2, windowHeight), sf::Color::White)
        };
        window.draw(line, 8, sf::Lines);

        if (gameState == GameState::Playing)
        {
            playerPaddle.draw(window);
            aiPaddle.draw(window);
            ball.draw(window);
            score.draw(window);
            window.draw(centerCircle); // Draw the center hollow circle
        }
        else if (gameState == GameState::GameOver)
        {
            window.draw(gameOverText);
            window.draw(restartButton);
            window.draw(restartText);
            window.draw(quitButton);
            window.draw(quitText);
        }

        window.display();
    }

    void checkGameOver()
    {
        if (score.getLeftScore() >= 10 || score.getRightScore() >= 10)
        {
            gameState = GameState::GameOver;
            gameOverText.setString(score.getLeftScore() >= 10 ? "You Win!" : "AI Wins!");
        }
    }

    void resetGame()
    {
        gameState = GameState::Playing;
        score = Score(); // Reset the score
        playerPaddle = Paddle(50.0f, windowHeight / 2 - paddleHeight / 2);
        aiPaddle = Paddle(windowWidth - 50.0f - paddleWidth, windowHeight / 2 - paddleHeight / 2);
        ball.reset();
    }
};

// int main()
// {
//     Game game;
//     game.run();
//     return 0;
// }
