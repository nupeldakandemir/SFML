#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

int main()
{
    
    std::srand(std::time(nullptr));

    sf::Vector2u windowSize(800, 600);

    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Space Shooter Game");
    
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        
        return 1;
    }
        
    sf::Texture rocketTexture;
    if (!rocketTexture.loadFromFile("rocket.bmp")) {
        return 1;
    }
    
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("bullet.bmp")) {
        
        return 1;
    }

    sf::Texture stoneTexture1;
    if (!stoneTexture1.loadFromFile("stone.bmp")) {
        return 1;
    }

    sf::Texture stoneTexture2;
    if (!stoneTexture2.loadFromFile("stone2.bmp")) {
        return 1;
    }

    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("SPACE SHOOTER GAME\n\nWelcome to NUPI'S Universe !\n\nPRESS ENTER TO FIGHT ACROSS THE GALAXY");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White); 
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(windowSize.x / 4, windowSize.y / 3);

    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);

    
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over\nPress Enter to start again");
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(windowSize.x / 4, windowSize.y / 3);

    bool gameStarted = false;

    sf::Sprite rocketSprite(rocketTexture);
    rocketSprite.setPosition(windowSize.x / 2 - rocketTexture.getSize().x / 2, windowSize.y - rocketTexture.getSize().y);

    
    sf::Sprite bulletSprite(bulletTexture);
    bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

    
    std::vector<sf::Sprite> stones;

    int score = 0;
    bool bulletFired = false;
    float bulletSpeed = 1.0f;
    float stoneSpeed = 0.2f;   

    
    sf::Color backgroundColor = sf::Color(0, 0, 0);

    
    bool gameOver = false;

    
    int lives = 3;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (!gameOver)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        gameStarted = true;

                       
                        for (int i = 0; i < 10; ++i) {
                            sf::Texture& texture = (std::rand() % 2 == 0) ? stoneTexture1 : stoneTexture2;
                            sf::Sprite stoneSprite(texture);
                            stoneSprite.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)),
                                -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));

                            
                            float scaleFactor = static_cast<float>(std::rand() % 5 + 5) / 10.0f; 
                            stoneSprite.setScale(scaleFactor, scaleFactor);

                            stones.push_back(stoneSprite);
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Left)
                    {
                       
                        if (rocketSprite.getPosition().x > 0)
                            rocketSprite.move(-10.0f, 0.0f);
                    }
                    else if (event.key.code == sf::Keyboard::Right)
                    {
                        
                        if (rocketSprite.getPosition().x < windowSize.x - rocketTexture.getSize().x)
                            rocketSprite.move(10.0f, 0.0f);
                    }
                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        
                        bulletFired = true;
                        bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);
                    }
                }
                else 
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                       
                        score = 0;
                        lives = 3; 
                        gameOver = false;
                        gameStarted = false;
                        bulletFired = false;
                        stones.clear();
                        welcomeText.setString("SPACE SHOOTER GAME\n\nYou are the evil alien Press Enter to start\n\n Developed by Nupelda Kandemir");
                        gameOverText.setPosition(windowSize.x / 4, windowSize.y / 3);
                    }
                }
            }
        }

        window.clear(backgroundColor);

      
        for (auto& stone : stones) {
            if (gameStarted) 
                stone.move(0.0f, stoneSpeed);
            window.draw(stone);

            if (stone.getPosition().y > static_cast<float>(windowSize.y)) {
                stone.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)), -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));

                
                if (--lives == 0) {
                    gameOver = true;
                    welcomeText.setString("PRESS ENTER TO START AGAIN");
                    gameOverText.setPosition(windowSize.x / 3, windowSize.y / 3);
                }
            }

           
            if (rocketSprite.getGlobalBounds().intersects(stone.getGlobalBounds())) {
                
                --lives;
                
                stone.setPosition(static_cast<float>(std::rand() % static_cast<int>(windowSize.x)), -static_cast<float>(std::rand() % static_cast<int>(windowSize.y)));
            }

           
            if (bulletFired && bulletSprite.getGlobalBounds().intersects(stone.getGlobalBounds())) {
                
                bulletFired = false;
                bulletSprite.setPosition(rocketSprite.getPosition().x + rocketTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, rocketSprite.getPosition().y);

                
                ++score;
            }
        }

        
        if (!gameStarted)
        {
            if (!gameOver)
            {
                window.draw(welcomeText);
            }
            else
            {
                window.draw(gameOverText);
            }
        }
        else 
        {
            
            if (bulletFired)
                bulletSprite.move(0.0f, -bulletSpeed);

            
            bulletSprite.setScale(0.3f, 0.3f);

            
            window.draw(rocketSprite);
            window.draw(bulletSprite);

            sf::Text livesText;
            livesText.setFont(font);
            livesText.setString("Lives: " + std::to_string(lives));
            livesText.setCharacterSize(16);
            livesText.setFillColor(sf::Color::White);
            livesText.setPosition(20, 40);
            window.draw(livesText);

            
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
        }

        window.display();
    }
    return 0;
}
