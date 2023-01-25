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
    uint8_t flip(uint8_t x, uint8_t y);
    void draw();

    void testCase();

    bool checkKey(uint8_t key);

private:
    static const size_t width {64};
    static const size_t height {32};
    const size_t pixelWidth {10};

    bool grid[width][height] {0};

    std::unique_ptr<sf::RenderWindow> window;
    sf::Event event;

    sf::RectangleShape createPixel(uint8_t x, uint8_t y);

};

#endif