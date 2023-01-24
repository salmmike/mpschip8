#ifndef MPS_CHIP8_SCREEN_H
#define MPS_CHIP8_SCREEN_H

#include <SFML/Graphics.hpp>
#include <memory>

class Chip8Screen
{
public:
    Chip8Screen() {};
    ~Chip8Screen() {};

    void create();
    void clear();
    void draw();
    void set(uint8_t x, uint8_t y);

private:
    std::unique_ptr<sf::RenderWindow> window;
    sf::Event event;
};

#endif