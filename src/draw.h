#include "game.h"
#include <SFML/Graphics.hpp>
#ifndef DRAW_H
# define DRAW_H

void draw_board(sf::RenderWindow& window, sf::Font& font, float targetY);
void draw_circle(sf::RenderWindow& window, Nota nota, float y);
void draw_trail(sf::RenderWindow& window, Nota nota, float yhead, float ytail);
int getIfromKey(char c);

#endif