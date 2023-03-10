#include <screen.h>
#include <unistd.h>
#include <iostream>
#include <ios>

void Chip8Screen::create()
{
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width*pixelWidth, height*pixelWidth), "MPSChip8");
    window->setFramerateLimit(60);
    window->clear();
}

void Chip8Screen::clear()
{
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            grid[x][y] = 0;
        }
    }
    draw();
}

uint8_t Chip8Screen::flip(uint8_t x, uint8_t y)
{
    if (x >= 0 && x < width && y >= 0 && y <= height) {
        uint8_t wasOn = grid[x][y];
        grid[x][y] ^= 1;
        return wasOn;
    }
    return 0;
}

void Chip8Screen::draw()
{
    window->clear();
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            if (grid[x][y]) {
                auto p = createPixel(x, y);
                window->draw(p);
            }
        }
    }
    window->display();
}

void Chip8Screen::testCase()
{
    create();
    for (size_t i = 0; i < 3; ++i) {
        for (size_t x = 0; x < width; ++x) {
            for (size_t y = 0; y < height; ++y) {
                flip(x, y);
                draw();
            }
        }
    }

}

bool Chip8Screen::checkKey(uint8_t key)
{
    bool pressed = sf::Keyboard::isKeyPressed(keys[key]);
    std::cout << (int) key << " is pressed: " << pressed << "\n";
    return pressed;
}

int Chip8Screen::anyPress()
{
    std::cout << "any press\n";
    window->pollEvent(event);
    if (event.type == sf::Event::KeyReleased) {
        auto it = std::find(keys.begin(), keys.end(), event.key.code);
        if (it != keys.end()) {
            return std::distance(keys.begin(), it);
        }
    }
    return -1;
}

sf::RectangleShape Chip8Screen::createPixel(uint8_t x, uint8_t y)
{
    auto rec = sf::RectangleShape(sf::Vector2f(pixelWidth, pixelWidth));
    rec.setFillColor(sf::Color::White);
    rec.setOutlineColor(sf::Color::Red);
    rec.setOutlineThickness(2);
    rec.setPosition(sf::Vector2f(x*pixelWidth, y*pixelWidth));
    return rec;
}
