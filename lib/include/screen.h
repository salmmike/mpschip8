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
    int anyPress();

private:
    static const size_t width {64};
    static const size_t height {32};
    const size_t pixelWidth {10};

    bool grid[width][height] {0};

    std::unique_ptr<sf::RenderWindow> window;
    sf::Event event;

    sf::RectangleShape createPixel(uint8_t x, uint8_t y);

    const std::vector<sf::Keyboard::Key> keys
    {
        sf::Keyboard::Key::Num1,
        sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
        sf::Keyboard::Key::Num4, sf::Keyboard::Key::Q,
        sf::Keyboard::Key::W,    sf::Keyboard::Key::E,
        sf::Keyboard::Key::R, sf::Keyboard::Key::A,
        sf::Keyboard::Key::S, sf::Keyboard::Key::D,
        sf::Keyboard::Key::F, sf::Keyboard::Key::Z,
        sf::Keyboard::Key::X, sf::Keyboard::Key::C
    };

};

#endif