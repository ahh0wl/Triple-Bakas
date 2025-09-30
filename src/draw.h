#include "game.h"
#include <SFML/Graphics.hpp>
#ifndef DRAW_H
# define DRAW_H

void draw_board(sf::RenderWindow& window, float targetY);
void draw_notes(sf::RenderWindow& window, Nota nota, float y);

#endif