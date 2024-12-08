#pragma once
#include "ship.h"
#define nr_bullets 5
class SHIP;
class PLAYER : public sf::Sprite
{
public:
	void movement();
	int update_player();
	void shoot();
	void update_bullets();
	PLAYER()
	{
		this->setPosition(window.getSize().x / 2, window.getSize().y / 2);
		player_texture.loadFromFile("Textures\\player.png");
		heart_texture.loadFromFile("Textures\\heart.png");
		empty_heart_texture.loadFromFile("Textures\\empty_heart.png");
		bullet_texture.loadFromFile("Textures\\bullet.png");
		player_shooting.loadFromFile("Textures\\player_shooting.png");
		for (int index = 0; index < nr_bullets; index++)
		{
			bullet[index].setRotation(270);
			bullet[index].setTexture(bullet_texture);
			bullet[index].setPosition(-100, -100);
		}
		empty_heart.setTexture(empty_heart_texture);
		healthbar.setTexture(heart_texture);
		healthbar.setScale(0.01, 0.01);
		empty_heart.setScale(0.028, 0.028);

		immunity_circle.setRadius(50.f); // Adjust the radius as needed
		immunity_circle.setFillColor(sf::Color(255, 255, 0, 128)); // Yellow with transparency
		immunity_circle.setOrigin(immunity_circle.getRadius(), immunity_circle.getRadius());
		this->setTexture(player_texture);
	}
	int health=5;
	bool immunity = 0;
	float immunity_time = 0.5f;//set how much time immunity will be on for
	float old_immunity_time = 0.5f;
	float bullet_speed = 50.f;
	float bullet_firerate = 0.3f;
	float bullet_damage = 1.f;
	sf::Sprite empty_heart, bullet[nr_bullets];
private:
	sf::Texture heart_texture,player_texture;
	sf::Texture empty_heart_texture, bullet_texture,player_shooting;
	sf::Sprite healthbar;
	float mvmspd = 10.f;
	void update_healthbar();
	int is_hit(std::vector<SHIP>& ships);
	sf::Clock immunity_clock;
	sf::CircleShape immunity_circle;
	sf::Clock shoot_clock;

	friend class GAME;
};
