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

void draw_notes(sf::RenderWindow& window, Nota nota, float y)
{
    // cerchio principale
    sf::CircleShape note(40.f);
    note.setFillColor(sf::Color(56, 255, 226, 200));
    note.setOrigin(note.getRadius(), note.getRadius());
    note.setOutlineThickness(3.f);
    note.setOutlineColor(sf::Color::White);

    switch (nota.key) {
        case 'a': note.setPosition(100.f, y); break;
        case 's': note.setPosition(300.f, y); break;
        case 'd': note.setPosition(500.f, y); break;
        case 'f': note.setPosition(700.f, y); break;
        default: note.setPosition(10.f, y); break;
    }
    // cerchio centrale
    sf::CircleShape center(5.f);
    center.setFillColor(sf::Color::White);
    center.setOrigin(center.getRadius(), center.getRadius());
    center.setPosition(note.getPosition());
    window.draw(note);
    window.draw(center);
}