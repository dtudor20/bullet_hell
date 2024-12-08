#include "game.h"
PLAYER player;
GAME game;
sf::Clock flickerClock;


void GAME::is_hit()
{
	for (int index = 0; index < nr_bullets; index++)
	{
		if (player.bullet[index].getGlobalBounds().intersects(ships[0].getGlobalBounds()))
		{
            health -= player.bullet_damage;
            if (drop_clock.getElapsedTime().asSeconds() >= drop_rate)
            {
                if (ships[0].random_zero_or_one(1)==1)
                {
                    spawn_heart();
                }
                else
                    spawn_shield();
            }
            player.bullet[index].setPosition(-100, -100);
		}
	}
}
int GAME::update_health()
{
    // Draw the outer health bar
    window.draw(hp_bar_outer);

    // Calculate the width of the inner health bar based on the current health
    float healthPercentage = static_cast<float>(health) / max_health; // Assuming max health is 100
    sf::IntRect innerRect = hp_bar_inner.getTextureRect();
    innerRect.width = static_cast<int>(hp_bar_inner.getTexture()->getSize().x * healthPercentage);
    hp_bar_inner.setTextureRect(innerRect);

    // Position the inner health bar correctly within the outer health bar
    hp_bar_inner.setPosition(hp_bar_outer.getPosition().x+55, hp_bar_outer.getPosition().y+18);

    // Draw the inner health bar
    if (health == 0)
    {
        return 1; //level defeated
    }
    window.draw(hp_bar_inner);
}

void GAME::spawn_heart()
{
    PowerUp heart;
    heart.sprite.setTexture(heart_texture);
    heart.sprite.setScale(0.02, 0.02);
    heart.sprite.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
    if (drop_clock.getElapsedTime().asSeconds() > drop_rate)
    {
        hearts.push_back(heart);
        drop_clock.restart();
    }
}

void GAME::spawn_shield()
{
    PowerUp shield;
    shield.sprite.setTexture(shield_texture);
    shield.sprite.setScale(0.04, 0.04);
    shield.sprite.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
    if (drop_clock.getElapsedTime().asSeconds() > drop_rate)
    {
        shields.push_back(shield);
        drop_clock.restart();
    }
}

void GAME::update_power_ups()
{
    // Check for collisions between player and hearts
    for (auto it = hearts.begin(); it != hearts.end();)
    {
        if (player.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
        {
            player.health++;
            it = hearts.erase(it); // Remove the heart
        }
        else if (it->clock.getElapsedTime().asSeconds() > drop_life)
        {
            it = hearts.erase(it); // Remove the heart if its lifetime is over
        }
        else
        {
            // Flicker effect towards the end of lifetime
            if (drop_life - it->clock.getElapsedTime().asSeconds() < 1.0f)
            {
                if (static_cast<int>(it->clock.getElapsedTime().asMilliseconds() / 100) % 2 == 0)
                {
                    it->sprite.setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
                }
                else
                {
                    it->sprite.setColor(sf::Color(255, 255, 255, 255)); // Opaque
                }
            }
            window.draw(it->sprite);
            ++it;
        }
    }
    // Check for collisions between player and shields
    for (auto it = shields.begin(); it != shields.end();)
    {
        if (player.getGlobalBounds().intersects(it->sprite.getGlobalBounds()))
        {
            player.immunity = true;
            // Grant immunity to the player
            player.immunity_time = 5.f;//setting immunity for 5 secs after power up
            player.immunity_clock.restart(); // Restart the immunity clock
            it = shields.erase(it); // Remove the shield
        }
        else if (it->clock.getElapsedTime().asSeconds() > drop_life)
        {
            it = shields.erase(it); // Remove the shield if its lifetime is over
        }
        else
        {
            // Flicker effect towards the end of lifetime
            if (drop_life - it->clock.getElapsedTime().asSeconds() < 1.0f)
            {
                if (static_cast<int>(it->clock.getElapsedTime().asMilliseconds() / 100) % 2 == 0)
                {
                    it->sprite.setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
                }
                else
                {
                    it->sprite.setColor(sf::Color(255, 255, 255, 255)); // Opaque
                }
            }
            window.draw(it->sprite);
            ++it;
        }
    }
}
void GAME::play()
{

    for (auto& ship : ships)
    {
        ship.update_ship();
    }
    game.update_power_ups();
    if (player.update_player() == 1)
    {
        // player lost
        sf::RectangleShape transparentRedScreen(sf::Vector2f(window.getSize().x, window.getSize().y));
        transparentRedScreen.setFillColor(sf::Color(255, 0, 0, 128)); // Red color with 50% transparency
        window.draw(transparentRedScreen, sf::BlendAlpha); // Enable blending mode
        window.display();
        sf::sleep(sf::seconds(10));
        window.close();
    }
    game.is_hit();
    if (game.update_health() == 1)
    {
        // player lost
        sf::RectangleShape transparentRedScreen(sf::Vector2f(window.getSize().x, window.getSize().y));
        transparentRedScreen.setFillColor(sf::Color(0, 255, 0, 128)); // Red color with 50% transparency
        window.draw(transparentRedScreen, sf::BlendAlpha); // Enable blending mode
        window.display();
        sf::sleep(sf::seconds(10));
        window.close();
    }

    if (health > max_health/2 )
    {
        //first stage

    }
    else if (health > max_health / 4)
    {
        //second stage
        if (ships.size() == 1)
        {
            ships.emplace_back();
            flickerClock.restart();
            ships[1].exists = false;
            ships[1].move(400, 400);
            ships[1].beam_ontime = 0.f;
            ships[1].speed = 10.f;
        }
       if (flickerClock.getElapsedTime().asSeconds() < 5.0f)
        {
            if (static_cast<int>(flickerClock.getElapsedTime().asMilliseconds() / 100) % 2 == 0)
            {
                ships[1].setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
            }
            else
            {
                ships[1].setColor(sf::Color(255, 255, 255, 255)); // Opaque
            }
            
        }
        else
        {
            ships[1].exists = true;
            ships[1].beam_ontime = 2.f;
            ships[0].speed = max(ships[0].random_zero_or_one(20), ships[0].random_zero_or_one(20));
            ships[1].speed = max(ships[0].random_zero_or_one(20), ships[0].random_zero_or_one(20));
        }
        
    }
    else
    {
        //third stage
        if (ships.size() == 2)
        {
            ships.emplace_back();
            flickerClock.restart();
            ships[2].exists = false;
            ships[2].move(400, 800);
            ships[2].beam_ontime = 0.f;
            ships[2].speed = 15.f;
            ships[1].speed = 15.f;
            ships[2].laser_firerate = 0.5f;
        }
        if (flickerClock.getElapsedTime().asSeconds() < 5.0f)
        {
            if (static_cast<int>(flickerClock.getElapsedTime().asMilliseconds() / 100) % 2 == 0)
            {
                ships[2].setColor(sf::Color(255, 255, 255, 128)); // Semi-transparent
            }
            else
            {
                ships[2].setColor(sf::Color(255, 255, 255, 255)); // Opaque
            }

        }
        else
        {
            ships[2].exists = true;
            ships[2].beam_ontime = 2.f;
            ships[1].speed = max(ships[0].random_zero_or_one(30), ships[0].random_zero_or_one(30));
            ships[2].speed = max(ships[0].random_zero_or_one(30), ships[0].random_zero_or_one(30));
            ships[0].speed = max(ships[0].random_zero_or_one(30), ships[0].random_zero_or_one(30));
        }
    }
}