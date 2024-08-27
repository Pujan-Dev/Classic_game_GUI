#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

class FloppyBirdGame
{
public:
    FloppyBirdGame()
    {
        // Initialize window
        window.create(VideoMode(1000, 600), "Floppy Bird");
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        srand(time(0));

        // Load resources
        loadResources();

        // Initialize game variables
        initGame();
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
    RenderWindow window;
    SoundBuffer chingBuffer, hopBuffer, dishkBuffer;
    Sound ching, hop, dishk;
    Texture flappy[3], pipe, background, gameover;
    Sprite flappySprite;
    vector<Sprite> pipes;

    enum class GameState
    {
        waiting,
        started,
        gameover
    };

    struct Game
    {
        int score = 0;
        int highscore = 0;
        int frames = 0;
        GameState gameState = GameState::waiting;
        Sprite background[3];
        Sprite gameover;
        Text pressC;
        Text scoreText;
        Text highscoreText;
        Font font;
    } game;

    float birdVelocity = 0.0f; // Velocity of the bird
    const float flapVelocity = -10.0f; // Velocity applied when flapping
    const float gravity = 0.5f; // Gravity affecting the bird

    bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
    {
        return (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2);
    }

    void loadResources()
    {
        // Load sounds
        chingBuffer.loadFromFile("Flappy_bird/audio/score.wav");
        hopBuffer.loadFromFile("Flappy_bird/audio/flap.wav");
        dishkBuffer.loadFromFile("Flappy_bird/audio/crash.wav");
        ching.setBuffer(chingBuffer);
        hop.setBuffer(hopBuffer);
        dishk.setBuffer(dishkBuffer);

        // Load textures
        background.loadFromFile("Flappy_bird/images/background.png");
        pipe.loadFromFile("Flappy_bird/images/pipe.png");
        gameover.loadFromFile("Flappy_bird/images/gameover.png");
        flappy[0].loadFromFile("Flappy_bird/images/flappy1.png");
        flappy[1].loadFromFile("Flappy_bird/images/flappy2.png");
        flappy[2].loadFromFile("Flappy_bird/images/flappy3.png");

        // Load font
        game.font.loadFromFile("Flappy_bird/fonts/flappy.ttf");
    }

    void initGame()
    {
        flappySprite.setTexture(flappy[1]);
        flappySprite.setPosition(250, 300);
        flappySprite.setScale(1.5, 1.5);

        for (int i = 0; i < 3; ++i)
        {
            game.background[i].setTexture(background);
            game.background[i].setScale(1.15625, 1.171875);
        }
        game.background[1].setPosition(333, 0);
        game.background[2].setPosition(666, 0);

        game.gameover.setTexture(gameover);
        game.gameover.setOrigin(192 / 2, 42 / 2);
        game.gameover.setPosition(500, 125);
        game.gameover.setScale(2, 2);

        game.pressC.setString("Press C to continue playing \n Press Q to quit");
        game.pressC.setFont(game.font);
        game.pressC.setFillColor(Color::White);
        game.pressC.setCharacterSize(50);
        game.pressC.setOrigin(game.pressC.getLocalBounds().width / 2, 0);
        game.pressC.setPosition(500, 250);

        game.scoreText.setFont(game.font);
        game.scoreText.setFillColor(Color::White);
        game.scoreText.setCharacterSize(75);
        game.scoreText.setPosition(30, 0);

        game.highscoreText.setFont(game.font);
        game.highscoreText.setFillColor(Color::White);
        game.highscoreText.setCharacterSize(75);
        game.highscoreText.setPosition(30, 80);
    }

    void handleEvents()
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    if (game.gameState == GameState::waiting)
                    {
                        game.gameState = GameState::started;
                    }
                    if (game.gameState == GameState::started)
                    {
                        birdVelocity = flapVelocity; // Apply flap velocity
                        hop.play();
                    }
                }
                else if (event.key.code == Keyboard::C && game.gameState == GameState::gameover)
                {
                    game.gameState = GameState::waiting;
                    flappySprite.setPosition(250, 300);
                    birdVelocity = 0; // Reset bird velocity
                    game.score = 0;
                    pipes.clear();
                }
                else if (event.key.code == Keyboard::Q && game.gameState == GameState::gameover)
                {
                    window.close();
                }
            }
        }
    }

    void update()
    {
        flappySprite.setTexture(flappy[1]);
        game.scoreText.setString(to_string(game.score));
        game.highscoreText.setString("HI " + to_string(game.highscore));

        float fx = flappySprite.getPosition().x;
        float fy = flappySprite.getPosition().y;
        float fw = 34 * flappySprite.getScale().x;
        float fh = 24 * flappySprite.getScale().y;

        if (game.gameState == GameState::waiting || game.gameState == GameState::started)
        {
            if (game.frames % 6 == 0)
            {
                // Frame update logic
            }
        }

        if (game.gameState == GameState::started)
        {
            birdVelocity += gravity; // Apply gravity to velocity
            flappySprite.move(0, birdVelocity); // Move the bird

            // Boundary check
            if (fy < 0)
            {
                flappySprite.setPosition(250, 0);
                birdVelocity = 0;
            }
            else if (fy > 600)
            {
                flappySprite.setPosition(250, 600);
                birdVelocity = 0;
                game.gameState = GameState::gameover;
                dishk.play();
            }
        }

        for (auto itr = pipes.begin(); itr != pipes.end();)
        {
            if (game.gameState == GameState::started && itr->getPosition().x == 250)
            {
                game.score++;
                ching.play();
                if (game.score > game.highscore)
                {
                    game.highscore = game.score;
                }
                itr = pipes.erase(itr); // Remove the pipe after scoring
            }
            else
            {
                ++itr;
            }
        }

        if (game.gameState == GameState::started && game.frames % 150 == 0)
        {
            int r = rand() % 275 + 75;
            int gap = 150;

            Sprite pipeL;
            pipeL.setTexture(pipe);
            pipeL.setPosition(1000, r + gap);
            pipeL.setScale(2, 2);

            Sprite pipeU;
            pipeU.setTexture(pipe);
            pipeU.setPosition(1000, r);
            pipeU.setScale(2, -2);

            pipes.push_back(pipeL);
            pipes.push_back(pipeU);
        }

        if (game.gameState == GameState::started)
        {
            for (auto &pipe : pipes)
            {
                pipe.move(-3, 0);
            }
        }

        if (game.frames % 100 == 0)
        {
            auto startitr = pipes.begin();
            auto enditr = pipes.begin();
            for (; enditr != pipes.end(); ++enditr)
            {
                if (enditr->getPosition().x > -104)
                {
                    break;
                }
            }
            pipes.erase(startitr, enditr);
        }

        if (game.gameState == GameState::started)
        {
            for (auto &pipe : pipes)
            {
                float px, py, pw, ph;
                if (pipe.getScale().y > 0)
                {
                    px = pipe.getPosition().x;
                    py = pipe.getPosition().y;
                    pw = 52 * pipe.getScale().x;
                    ph = 320 * pipe.getScale().y;
                }
                else
                {
                    pw = 52 * pipe.getScale().x;
                    ph = -320 * pipe.getScale().y;
                    px = pipe.getPosition().x;
                    py = pipe.getPosition().y - ph;
                }
                if (collides(fx, fy, fw, fh, px, py, pw, ph))
                {
                    game.gameState = GameState::gameover;
                    dishk.play();
                }
            }
        }

        game.frames++;
    }

    void render()
    {
        window.clear();
        window.draw(game.background[0]);
        window.draw(game.background[1]);
        window.draw(game.background[2]);
        window.draw(flappySprite);
        for (const auto &pipe : pipes)
        {
            window.draw(pipe);
        }
        window.draw(game.scoreText);
        window.draw(game.highscoreText);

        if (game.gameState == GameState::gameover)
        {
            window.draw(game.gameover);
            if (game.frames % 60 < 30)
            {
                window.draw(game.pressC);
            }
        }
        window.display();
    }
};
