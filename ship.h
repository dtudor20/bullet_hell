#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
extern sf::RenderWindow window;
#define nr_lasers 40 //max nr of lasers

template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

class PLAYER;
class SHIP : public sf::Sprite
{
public:
    sf::Sprite laser[nr_lasers], beam;
    bool exists = true;
    int random_zero_or_one(int);
    SHIP()
    {
        static sf::Texture ship_texture;
        ship_texture.loadFromFile("Textures\\ship.png");
        this->setTexture(ship_texture);
        static sf::Texture laser_texture, beam_texture;
        if (!laser_texture.loadFromFile("Textures\\laser.png"))
        {
            std::cerr << "Failed to load laser texture" << std::endl;
        }
        if (!beam_texture.loadFromFile("Textures\\beam.png"))
        {
            std::cerr << "Failed to load beam texture" << std::endl;
        }
        for (int i = 0; i < nr_lasers; ++i)
        {
            laser[i].setTexture(laser_texture);
            laser[i].setScale(0.5, 0.5);
            laser[i].setPosition(-100, -100);
        }

        this->setScale(0.3, 0.3);
        beam.setTexture(beam_texture);
        beam.setScale(1, 5);
    }
    void update_ship();
    void update_lasers();
    float laser_firerate=0.8f;
    float beam_ontime=2.f;
    float beam_offtime = 2.f;
    float laser_speed = 5.0f;
    float speed = 10.0f;
    int nr=4;//how many direction 2 shoot lasers
private:
    sf::Clock laserClock;
    sf::Clock beamClock;
    void move_ship();
    void shoot_laser(); 
    void shoot_beam();
    void draw_projectiles();
    bool movingRight = true; 
};
extern std::vector<SHIP> ships;