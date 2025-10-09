#include "draw.h"
void draw_board(sf::RenderWindow& window, float targetY)
{
    // linea target
    sf::RectangleShape line(sf::Vector2f(800.f, 5.f));
    line.setFillColor(sf::Color::White);
    line.setPosition(0.f, targetY);
    window.draw(line);

    // colonne verticali
    for (int i = 1; i < 4; i++) { 
        sf::RectangleShape vline(sf::Vector2f(5.f, 600.f));
        vline.setFillColor(sf::Color::White);
        vline.setPosition(i * 195.f, 0.f);
        window.draw(vline);
    }
}

float getXfromKey(char c)
{
    switch (c) {
        case 'a': return 100; break;
        case 's': return 300; break;
        case 'd': return 500; break;
        case 'f': return 700; break;
        default: return 10; break;
    }
}

void draw_trail(sf::RenderWindow& window, Nota nota, float yhead, float ytail)
{
    sf::RectangleShape line(sf::Vector2f(80., ytail - yhead));
    line.setFillColor(sf::Color(56, 255, 226, 150));
    line.setOrigin(40, 0);
    line.setOutlineThickness(3.f);
    line.setOutlineColor(sf::Color::White);
    line.setPosition(getXfromKey(nota.key), yhead);
    window.draw(line);
}

void draw_circle(sf::RenderWindow& window, Nota nota, float y)
{
    // cerchio principale
    sf::CircleShape note(40.f);
    note.setFillColor(sf::Color(56, 255, 226, 255));
    note.setOrigin(note.getRadius(), note.getRadius());
    note.setOutlineThickness(3.f);
    note.setOutlineColor(sf::Color::White);
    note.setPosition(getXfromKey(nota.key), y);
 
    // cerchio centrale
    sf::CircleShape center(5.f);
    center.setFillColor(sf::Color::White);
    center.setOrigin(center.getRadius(), center.getRadius());
    center.setPosition(note.getPosition());
    window.draw(note);
    window.draw(center);
}