#include "player.h"

sf::RenderWindow window(sf::VideoMode(1920, 1080), "Bullet Hell v0");
void PLAYER::movement()
{
	sf::Vector2f position = this->getPosition();
	float new_x = position.x;
	float new_y = position.y;

	// Diagonal movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		new_x += mvmspd;
		new_y += mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		new_x -= mvmspd;
		new_y += mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		new_x += mvmspd;
		new_y -= mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		new_x -= mvmspd;
		new_y -= mvmspd;
	}
	// Normal movement
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		new_x += mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		new_x -= mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		new_y -= mvmspd;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		new_y += mvmspd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->shoot_clock.getElapsedTime().asSeconds() >= bullet_firerate)
	{
		shoot();
		shoot_clock.restart();
	}

	// Boundary checks
	if (new_x < 0)
		new_x = 0;
	else if (new_x + this->getGlobalBounds().width > window.getSize().x)
		new_x = window.getSize().x - this->getGlobalBounds().width;

	if (new_y < 0)
		new_y = 0;
	else if (new_y + this->getGlobalBounds().height > window.getSize().y)
		new_y = window.getSize().y - this->getGlobalBounds().height;

	this->setPosition(new_x, new_y);
}

void PLAYER::update_healthbar()
{
	for (int index = 0; index < this->health; index++)
	{
		healthbar.setPosition(this->getPosition().x+25*index-35,this->getPosition().y-20);
		window.draw(healthbar);
	}
	for (int index = this->health; index < 5; index++)
	{
		empty_heart.setPosition(this->getPosition().x + 25 * index - 35, this->getPosition().y - 17.5);
		window.draw(empty_heart);
	}
}
int PLAYER::is_hit(std::vector<SHIP>& ships)
{
	if (immunity && immunity_clock.getElapsedTime().asSeconds() < immunity_time) {
		return 0; // Player is immune
	}
	for (auto& ship : ships)
	{
		for (int index = 0; index < nr_lasers; index++)
		{
			sf::FloatRect laserBounds = ship.laser[index].getGlobalBounds();
			laserBounds.width -= 50;
			laserBounds.height -= 50;
			if (this->getGlobalBounds().intersects(laserBounds))
			{
				health--;
				immunity = 1;
				immunity_clock.restart();
				// Reset the laser position
				ship.laser[index].setPosition(-100, -100);
			}
		}

		sf::FloatRect playerBounds = this->getGlobalBounds();
		sf::FloatRect beamBounds = ship.beam.getGlobalBounds();
		beamBounds.width -= 10;
		beamBounds.height -= 10;
		if (playerBounds.intersects(beamBounds))
		{
			health--;
			immunity = 1;
			immunity_clock.restart();
			//ship.beam.setPosition(-100, -100);
		}

		sf::FloatRect shipRect(ship.getGlobalBounds());
		shipRect.width -= 89;
		shipRect.height -= 100;
		
		if (this->getGlobalBounds().intersects(shipRect) && ship.exists)
		{
			health--;
			immunity = 1;
			immunity_clock.restart();
		}
	}
	if (this->health == 0)
	{
		return 1;//you lost
	}
}
void PLAYER::shoot()
{
	for (int i = 0; i < nr_bullets; ++i)
	{
		if (bullet[i].getPosition().y==-100)
		{
			this->setTexture(player_shooting);
			bullet[i].setPosition(this->getPosition().x + this->getGlobalBounds().width / 2-61, this->getPosition().y+40);
			break;
		}
	}
}
void PLAYER::update_bullets()
{
	for (int i = 0; i < nr_bullets; ++i)
	{
		if (bullet[i].getPosition().y != -100)
		{
			bullet[i].move(0, -bullet_speed);

			if (bullet[i].getPosition().y < 0)
			{
				bullet[i].setPosition(-100, -100); // Reset the bullet position
			}

			window.draw(bullet[i]);
		}

	}
}
int PLAYER::update_player()
{
	if (this->is_hit(ships) == 1)
	{
		return 1;//player lost
	}
	this->update_healthbar();
	this->update_bullets();
	if (this->shoot_clock.getElapsedTime() > sf::seconds(bullet_firerate))
	{
		this->setTexture(player_texture);
	}
	window.draw(*this);

	if (immunity && immunity_clock.getElapsedTime().asSeconds() < immunity_time) {
		immunity_circle.setPosition(this->getPosition().x + this->getGlobalBounds().width / 2, this->getPosition().y + this->getGlobalBounds().height / 2);
		window.draw(immunity_circle);
	}
	else {
		immunity_time = old_immunity_time;
		immunity = false; // Reset immunity
	}
}


