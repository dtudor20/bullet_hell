#pragma once
#include "player.h"
extern PLAYER player;
class PLAYER;
class GAME
{
public:
	GAME()
	{
		hp_bar_inner_texture.loadFromFile("Textures\\hp_bar_inner.png");
		hp_bar_outer_texture.loadFromFile("Textures\\hp_bar_outer.png");
		hp_bar_inner.setTexture(hp_bar_inner_texture);
		hp_bar_outer.setTexture(hp_bar_outer_texture);
		heart_texture.loadFromFile("Textures\\heart.png");
		shield_texture.loadFromFile("Textures\\shield.png");
		hp_bar_inner.setScale(1.5, 1.5);
		hp_bar_outer.setScale(1.5, 1.5);
	}

	float drop_life = 5.f;
	//float drop_speed = 10.f;
	float drop_rate = 5.f; // once every x sec
	int health = 100;
	int max_health = 100; // Define max_health here
	int update_health();
	void is_hit();
	void spawn_heart();
	void spawn_shield();
	void update_power_ups();
	void play();
private:
	sf::Clock drop_clock;
	sf::Texture hp_bar_inner_texture, hp_bar_outer_texture;
	sf::Sprite hp_bar_inner, hp_bar_outer;
	sf::Texture heart_texture, shield_texture;

	struct PowerUp
	{
		sf::Sprite sprite;
		sf::Clock clock;
	};

	std::vector<PowerUp> hearts;
	std::vector<PowerUp> shields;
};

extern GAME game;
