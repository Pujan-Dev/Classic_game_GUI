#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

// Include the game headers
#include "code/pacman.cpp"     // Include the Pacman game logic
#include "snake/src/main.cpp" // Include the Snake game logic
#include "Pong/src/PongGame.cpp"
#include "Flappy_bird/main.cpp"

enum class MenuState
{
    MAIN,
    DEVS
};
void drawDynamicGradientBackground(sf::RenderWindow &window, sf::Clock &clock)
{
    // Create a vertex array with 4 vertices
    sf::VertexArray gradient(sf::TrianglesStrip, 4);

    // Calculate time elapsed to create a dynamic effect
    float time = clock.getElapsedTime().asSeconds();

    // Reduce the speed of the gradient effect by dividing the time variable
    float speedFactor = 0.2f; // Lower value means slower gradient change
    float adjustedTime = time * speedFactor;

    // Generate dynamic gray-blue colors based on adjusted time
    sf::Color topColor(
        static_cast<sf::Uint8>(50 + 50 * std::sin(adjustedTime * 0.5f)), // Gray-blue R
        static_cast<sf::Uint8>(60 + 50 * std::cos(adjustedTime * 0.5f)), // Gray-blue G
        static_cast<sf::Uint8>(70 + 50 * std::sin(adjustedTime * 0.7f))  // Gray-blue B
    );

    sf::Color bottomColor(
        static_cast<sf::Uint8>(50 + 50 * std::cos(adjustedTime * 0.5f)), // Gray-blue R
        static_cast<sf::Uint8>(60 + 50 * std::sin(adjustedTime * 0.5f)), // Gray-blue G
        static_cast<sf::Uint8>(70 + 50 * std::cos(adjustedTime * 0.7f))  // Gray-blue B
    );

    // Set the vertices with positions and colors
    gradient[0].position = sf::Vector2f(0, 0);
    gradient[0].color = topColor;

    gradient[1].position = sf::Vector2f(window.getSize().x, 0);
    gradient[1].color = topColor;

    gradient[2].position = sf::Vector2f(0, window.getSize().y);
    gradient[2].color = bottomColor;

    gradient[3].position = sf::Vector2f(window.getSize().x, window.getSize().y);
    gradient[3].color = bottomColor;

    // Draw the gradient
    window.draw(gradient);
}


void handleButtonHover(sf::Text &button, sf::Vector2i mousePos)
{
    if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button.setFillColor(sf::Color(72, 128, 250));
    }
    else
    {
        button.setFillColor(sf::Color::White);
    }
}

void animateTitleOnHover(sf::Text &title, sf::Clock &clock, sf::Vector2i mousePos, sf::RenderWindow &window, bool mouseHeld)
{
    float time = clock.getElapsedTime().asSeconds();
    if (!title.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        if (mouseHeld)
        {
            float tilt = std::sin(time * 8) * 1.0f;
            title.setRotation(tilt);
            title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);
        }
        else
        {
            float wobble = std::sin(time * 4) * 2.0f;
            title.setRotation(0);
            title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50 + wobble);
        }
    }
    else
    {
        title.setRotation(0);
        title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);
    }
}

void setTextProperties(sf::Text &text, sf::Font &font, const std::string &str, unsigned int size, sf::Color color, float outlineThickness, sf::Color outlineColor)
{
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setOutlineThickness(outlineThickness);
    text.setOutlineColor(outlineColor);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Classic Games Menu");
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("flappy.ttf"))
    {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    sf::Text title;
    setTextProperties(title, font, "Classic Games", 60, sf::Color::Yellow, 2.0f, sf::Color::Black);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);

    std::vector<sf::Text> menu;
    std::vector<std::string> menuOptions = {"Pacman", "Snake", "Pong", "Flappy Bird", "Quit"};

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        sf::Text item;
        setTextProperties(item, font, menuOptions[i], 30, sf::Color::White, 2.0f, sf::Color::Black);
        item.setPosition(window.getSize().x / 2 - item.getGlobalBounds().width / 2, 200 + i * 60);
        menu.push_back(item);
    }

    sf::Text devsButton;
    setTextProperties(devsButton, font, "Devs", 30, sf::Color(255, 165, 0), 2.0f, sf::Color::Black);
    devsButton.setPosition(50, 500);

    sf::Text backButton;
    setTextProperties(backButton, font, "Back", 30, sf::Color::Red, 2.0f, sf::Color::Black);
    backButton.setPosition(50, 500);

    sf::Text devs;
    setTextProperties(devs, font, "Developers", 40, sf::Color::Yellow, 2.0f, sf::Color::Black);
    devs.setPosition(window.getSize().x / 2 - devs.getGlobalBounds().width / 2, 50);

    std::vector<sf::Text> developerTexts;
    float yOffset = 200;
    std::vector<std::string> developerNames = {"Pujan Neupane", "Sujal Karki", "Prasamsha Adhikari", "Samridhee Ghimire"};
    for (const auto &name : developerNames)
    {
        sf::Text devText;
        setTextProperties(devText, font, name, 30, sf::Color::White, 2.0f, sf::Color::Black);
        devText.setPosition(window.getSize().x / 2 - devText.getGlobalBounds().width / 2, yOffset);
        developerTexts.push_back(devText);
        yOffset += 50; // Adjust spacing between names
    }

    MenuState menuState = MenuState::MAIN;
    bool mouseHeld = false;

    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseHeld = true;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseHeld = false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (menuState == MenuState::MAIN)
                {
                    for (size_t i = 0; i < menu.size(); ++i)
                    {
                        if (menu[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            if (i == 0)
                            {
                                window.close();
                                PacmanGame game;
                                game.run();
                            }
                            if (i == 1)
                            {
                                window.close();
                                SnakeGame game;
                                game.run();
                            }
                            if (i == 2)
                            {
                                window.close();
                                Game game;
                                game.run();
                            }
                            if (i == 3)
                            {
                                window.close();
                                FloppyBirdGame FLAPPY_BIRD;
                                FLAPPY_BIRD.run();
                            }
                            else if (i == 4)
                            {
                                window.close();
                            }
                        }
                    }

                    if (devsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuState = MenuState::DEVS;
                    }
                }
                else if (menuState == MenuState::DEVS)
                {
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuState = MenuState::MAIN;
                    }
                }
            }
        }

        window.clear();

        // Draw dynamic gradient background
        drawDynamicGradientBackground(window, clock);

        if (menuState == MenuState::MAIN)
        {
            animateTitleOnHover(title, clock, mousePos, window, mouseHeld);

            window.draw(title);

            for (auto &item : menu)
            {
                handleButtonHover(item, mousePos);
                window.draw(item);
            }

            handleButtonHover(devsButton, mousePos);
            window.draw(devsButton);
        }
        else if (menuState == MenuState::DEVS)
        {
            devs.setFillColor(sf::Color::Yellow);
            backButton.setFillColor(sf::Color::Red);
            animateTitleOnHover(devs, clock, mousePos, window, mouseHeld);

            window.draw(devs);
            for (const auto &text : developerTexts)
            {
                window.draw(text);
            }
            window.draw(backButton);
        }

        window.display();
    }

    return 0;
}
