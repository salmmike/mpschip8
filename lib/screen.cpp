#include <screen.h>

void Chip8Screen::create()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 320), "MPSChip8");
    window->setFramerateLimit(60);

}

void Chip8Screen::set(uint8_t x, uint8_t y)
{
}
