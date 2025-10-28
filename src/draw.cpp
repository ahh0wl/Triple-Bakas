#include "draw.h"
void draw_board(sf::RenderWindow& window, sf::Font& font, float targetY)
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
        vline.setPosition(i * 200.f, 0.f);
        window.draw(vline);
    }
    for (int i = 0; i < 4; i++){
        //cerchi target
        sf::CircleShape target(42.f);
        target.setOrigin(target.getRadius(), target.getRadius());
        target.setOutlineThickness(5.f);
        target.setFillColor(sf::Color(0, 0, 0, 255));
        target.setOutlineColor(sf::Color::White);
        target.setPosition(100 + i * 200 , TARGET_Y);
        window.draw(target);
        //lettere
        sf::Text text;
        text.setFont(font);
        switch (i){
            case 0:
                text.setString("A");
            break;
            case 1:
                text.setString("S");
            break;
            case 2:
                text.setString("D");
            break;
            case 3:
                text.setString("F");
            break;
            default:
                text.setString("Z");
            break;
        }   
        text.setCharacterSize(30);                
        text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
        text.setPosition(100. + i * 200, targetY);
        window.draw(text);
    }

}

int getIfromKey(char c)
{
    switch (c) {
        case 'a': return 0; break;
        case 's': return 1; break;
        case 'd': return 2; break;
        case 'f': return 3; break;
        default: return 4; break;
    }
}
void draw_timing(sf::RenderWindow& window, Nota nota, sf::Font& font, const string& str, sf::Color color)
{
    sf::Text text;
    text.setFont(font);
    int i = getIfromKey(nota.key);
    text.setString(str);
    text.setCharacterSize(30);                
    text.setFillColor(color);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
    text.setPosition(150. +  i * 200, TARGET_Y - 50);
    window.draw(text);
}

void draw_trail(sf::RenderWindow& window, Nota nota, float yhead, float ytail, sf::Color color)
{
    sf::RectangleShape line(sf::Vector2f(80., ytail - yhead));
    if (nota.missed())
        line.setFillColor(sf::Color(255, 0, 0, 150));
    else
        line.setFillColor(color);
    line.setOrigin(40, 0);
    line.setOutlineThickness(3.f);
    line.setOutlineColor(sf::Color::White);
    line.setPosition(100 + getIfromKey(nota.key) * 200, yhead);
    window.draw(line);
}

void draw_circle(sf::RenderWindow& window, Nota nota, float y)
{
    // cerchio principale
    sf::CircleShape note(40.f);
    if (!nota.missed())
        note.setFillColor(sf::Color(56, 255, 226, 255));
    else
        note.setFillColor(sf::Color(255, 0, 0, 255));
    note.setOrigin(note.getRadius(), note.getRadius());
    note.setOutlineThickness(3.f);
    note.setOutlineColor(sf::Color::White);
    note.setPosition(100 + getIfromKey(nota.key) * 200, y);
 
    // cerchio centrale
    sf::CircleShape center(5.f);
    center.setFillColor(sf::Color::White);
    center.setOrigin(center.getRadius(), center.getRadius());
    center.setPosition(note.getPosition());
    window.draw(note);
    window.draw(center);
}