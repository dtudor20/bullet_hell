#include "ship.h"
std::vector<SHIP> ships;
int SHIP::random_zero_or_one(int nr) {
    static std::random_device rd;  // Seed for the random number engine
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    static std::uniform_int_distribution<> dis(0, nr); // Distribution that produces 0 or 1

    return dis(gen);
}

void SHIP::move_ship()
{
    // Get the current position of the ship
    sf::Vector2f position = this->getPosition();

    // Get the size of the window
    sf::Vector2u windowSize = window.getSize();
    // Check if the ship is moving right and if it has reached the right edge
    if (movingRight)
    {
        if (position.x + this->getGlobalBounds().width < windowSize.x)
        {
            this->move(speed, 0); // Move right
        }
        else
        {
            movingRight = false; // Change direction to left
        }
    }
    // Check if the ship is moving left and if it has reached the left edge
    else
    {
        if (position.x > 0)
        {
            this->move(-speed, 0); // Move left
        }
        else
        {
            movingRight = true; // Change direction to right
        }
    }
}

void SHIP::update_lasers()
{
    sf::Vector2u windowSize = window.getSize();
    for (int index = 0; index < nr_lasers; index++)
    {
        sf::Vector2f laserPosition = laser[index].getPosition();
        if (laserPosition != sf::Vector2f(-100, -100))
        {
            // Move the laser based on its rotation
            if (laser[index].getRotation() == 135)
            {
                laser[index].move(laser_speed, laser_speed);
            }
            else if (laser[index].getRotation() == 45)
            {
                laser[index].move(-laser_speed, laser_speed);
            }
            else if (laser[index].getRotation() == 225)
            {
                laser[index].move(laser_speed, -laser_speed);
            }
            else if (laser[index].getRotation() == 315)
            {
                laser[index].move(-laser_speed, -laser_speed);
            }

            // Check if the laser is out of the window bounds
            if (laserPosition.x > windowSize.x || laserPosition.x < 0 || laserPosition.y > windowSize.y || laserPosition.y < 0)
            {
                // Reset the laser position
                laser[index].setPosition(-100, -100);
            }
        }
    }
}

void SHIP::shoot_beam()
{
    beam.setPosition(this->getPosition().x+89.5,this->getPosition().y+50);
}
void SHIP::shoot_laser()
{
    for (int index = 0; index < nr_lasers; index++)
    {
        if (laser[index].getPosition() == sf::Vector2f(-100,-100))
        {
            //shoot one laser
            laser[index].setPosition(this->getPosition().x+150,this->getPosition().y+150);
            int dir = random_zero_or_one(nr);
            if (dir == 0)//shoot to the left
            {
                laser[index].setRotation(45);
            }
            else if(dir==1)
            {
                laser[index].setRotation(135);
            }
            else if (dir == 2)
            {
                laser[index].setRotation(225);
            }
            else
            {
                laser[index].setRotation(315);
            }
            break;
        }
    }
}
void SHIP::draw_projectiles()
{
    window.draw(beam);
    for (int index = 0; index < nr_lasers; index++)
    {
        window.draw(laser[index]);
    }
}
void SHIP::update_ship()
{
    // beam toggle time
    if (this->beam.getPosition() == sf::Vector2f(-100, -100))
    {
        if (this->beamClock.getElapsedTime().asSeconds() >= this->beam_offtime)
        {
            this->shoot_beam();
            this->beamClock.restart();
        }
        else if (this->beamClock.getElapsedTime().asSeconds() >= this->beam_offtime - 1.f && std::fmod(this->beamClock.getElapsedTime().asSeconds(), 0.2f) < 0.1f) // make the beam flicker every 0.2s before it is activated
        {
            sf::Sprite aux_beam = beam;
            aux_beam.setPosition(this->getPosition().x + 89.5, this->getPosition().y + 50);
            window.draw(aux_beam);
        }
    }
    else
    {
        if (this->beamClock.getElapsedTime().asSeconds() > this->beam_ontime)
        {
            this->beam.setPosition(-100, -100); // Hide the beam
            this->beamClock.restart();
        }
    }

    // Update beam position if it is visible
    if (this->beam.getPosition() != sf::Vector2f(-100, -100))
    {
        this->shoot_beam();
    }

    // Shooting a laser once a second
    if (this->laserClock.getElapsedTime().asSeconds() >= this->laser_firerate) //laser shotting time
    {
        this->shoot_laser();
        this->laserClock.restart();
    }
    this->draw_projectiles();
    this->move_ship();
    this->update_lasers();
    window.draw(*this);
}