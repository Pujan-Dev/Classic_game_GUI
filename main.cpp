#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

enum class MenuState
{
    MAIN,
    DEVS
};

void handleButtonHover(sf::RectangleShape &button, sf::Vector2i mousePos)
{
    if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button.setFillColor(sf::Color(72, 128, 250)); // Change color on hover
    }
    else
    {
        button.setFillColor(sf::Color(100, 100, 255)); // Default color
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

void drawRoundedRect(sf::RenderWindow &window, sf::RectangleShape &rect, float radius)
{
    sf::ConvexShape roundedRect;
    roundedRect.setPointCount(12);
    for (int i = 0; i < 12; ++i)
    {
        float angle = i * (3.14159f / 6);
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        roundedRect.setPoint(i, sf::Vector2f(x + rect.getSize().x / 2, y + rect.getSize().y / 2));
    }
    roundedRect.setPosition(rect.getPosition());
    roundedRect.setFillColor(rect.getFillColor());
    window.draw(roundedRect);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Classic Games Menu");

    sf::Font font;
    if (!font.loadFromFile("flappy.ttf"))
    {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    sf::Text title("Classic Games", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);

    std::vector<sf::RectangleShape> buttons;
    std::vector<std::string> menuOptions = {"Pacman", "Snake", "Pong", "Flappy Bird", "Quit"};

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        sf::RectangleShape button(sf::Vector2f(200, 50));
        button.setFillColor(sf::Color(100, 100, 255));
        button.setPosition(window.getSize().x / 2 - button.getSize().x / 2, 200 + i * 60);
        buttons.push_back(button);
    }

    sf::Text devsButton("Devs", font, 20);
    devsButton.setFillColor(sf::Color(255, 165, 0));
    devsButton.setPosition(50, 500);

    sf::Text backButton("Back", font, 30);
    backButton.setPosition(50, 500);

    sf::Text devs("Developers", font, 30);
    devs.setPosition(window.getSize().x / 2 - devs.getGlobalBounds().width / 2, 50);

    std::vector<sf::Text> developerTexts;
    float yOffset = 200;
    std::vector<std::string> developerNames = {"Pujan Neupane", "Sujal Karki", "Prasamsha Adhikari", "Samridhee Ghimire"};
    for (const auto &name : developerNames)
    {
        sf::Text devText(name, font, 20);
        devText.setPosition(window.getSize().x / 2 - devText.getGlobalBounds().width / 2, yOffset);
        developerTexts.push_back(devText);
        yOffset += 40; // Adjust spacing between names
    }

    MenuState menuState = MenuState::MAIN;
    sf::Clock clock;
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
                    for (size_t i = 0; i < buttons.size(); ++i)
                    {
                        if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
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

        window.clear(sf::Color(200, 200, 255)); // Gradient background

        if (menuState == MenuState::MAIN)
        {
            animateTitleOnHover(title, clock, mousePos, window, mouseHeld);

            window.draw(title);

            for (size_t i = 0; i < buttons.size(); ++i)
            {
                handleButtonHover(buttons[i], mousePos);
                drawRoundedRect(window, buttons[i], 15.0f); // Draw rounded buttons
                sf::Text buttonText(menuOptions[i], font, 25);
                buttonText.setFillColor(sf::Color::White);
                buttonText.setPosition(buttons[i].getPosition().x + buttons[i].getSize().x / 2 - buttonText.getGlobalBounds().width / 2,
                                       buttons[i].getPosition().y + buttons[i].getSize().y / 2 - buttonText.getGlobalBounds().height / 2);
                window.draw(buttonText);
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
