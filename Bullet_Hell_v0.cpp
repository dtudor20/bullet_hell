#include "game.h"

int main()
{
    
    window.setFramerateLimit(60);
    sf::Sprite background;
    sf::Texture background_texture;
    background_texture.loadFromFile("Textures\\background2.png");
    player.setScale(1.5f, 1.5f);
    background.setTexture(background_texture);
    ships.emplace_back();
    player.setPosition(window.getSize().x/2 , window.getSize().y / 2);
    ships[0].laser_firerate = 0.3f;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color::Green);
        window.draw(background);
        player.movement();
        game.play();
        window.display();
    }
    return 0;
}
