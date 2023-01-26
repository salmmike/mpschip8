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
        /*
            1, 2, 3, C
            4, 5, 6, D
            7, 8, 9, E
            A, 0, B, F

            1,  2,  3, 12
            4,  5,  6, 13
            7,  8,  9, 14
            10, 0, 11, 15

            1, 2, 3, 4,
            Q, W, E, R
            A, S, D, F
            Z, X, C, V
        */

        sf::Keyboard::Key::X,       // 0
        sf::Keyboard::Key::Num1,    // 1
        sf::Keyboard::Key::Num2,    // 2
        sf::Keyboard::Key::Num3,    // 3
        sf::Keyboard::Key::Q,       // 4
        sf::Keyboard::Key::W,       // 5
        sf::Keyboard::Key::E,       // 6
        sf::Keyboard::Key::A,       // 7
        sf::Keyboard::Key::S,       // 8
        sf::Keyboard::Key::D,       // 9
        sf::Keyboard::Key::Z,       // 10
        sf::Keyboard::Key::C,       // 11
        sf::Keyboard::Key::Num4,    // 12
        sf::Keyboard::Key::R,       // 13
        sf::Keyboard::Key::F,       // 14
        sf::Keyboard::Key::V        // 15
    };

};

#endif