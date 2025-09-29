#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>

using namespace std;

void Game::loadBeatmap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire la beatmap " << filename << "\n";
        return;
    }

    int time;
    char key;
    while (file >> time >> key) {
        notes.emplace_back(time, key);
    }
    file.close();
}

bool checkKey(Note& note) {
    if (note.key == 'a') return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    if (note.key == 's') return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    if (note.key == 'd') return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    if (note.key == 'f') return sf::Keyboard::isKeyPressed(sf::Keyboard::F);
    return false;
}

void Game::run() {
    if (notes.empty()) {
        cout << "Nessuna nota caricata!\n";
        return;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Rhythm Game");
    window.setFramerateLimit(60);

    float targetY = 500.f; // linea target

    int hitCount = 0;
    int missCount = 0;

    sf::Clock clock;

    const float noteSpeed = 0.5f; // pixel per millisecondo, regola per adattare la velocità

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int elapsedMs = clock.getElapsedTime().asMilliseconds();

        window.clear(sf::Color::Black);

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

        // disegna note
        for (auto& note : notes) {
            if (note.hit) continue;

            // calcola posizione Y in base al tempo
            float y = targetY - (note.time - elapsedMs) * noteSpeed;

            // ancora fuori dallo schermo sopra
            if (y < -80.f) continue;

            // già passata la finestra, considerala persa
            if (y > 600.f) {
                note.hit = true;
                missCount++;
                cout << "[MISS] Nota persa: " << note.key << " al tempo " << note.time << "ms\n";
                continue;
            }

            // cerchio principale
            sf::CircleShape nota(40.f);
            nota.setFillColor(sf::Color(56, 255, 226, 200));
            nota.setOrigin(nota.getRadius(), nota.getRadius());
            nota.setOutlineThickness(3.f);
            nota.setOutlineColor(sf::Color::White);

            switch (note.key) {
                case 'a': nota.setPosition(100.f, y); break;
                case 's': nota.setPosition(300.f, y); break;
                case 'd': nota.setPosition(500.f, y); break;
                case 'f': nota.setPosition(700.f, y); break;
                default: nota.setPosition(500.f, y); break;
            }

            // pallino centrale
            sf::CircleShape center(5.f);
            center.setFillColor(sf::Color::White);
            center.setOrigin(center.getRadius(), center.getRadius());
            center.setPosition(nota.getPosition());

            window.draw(nota);
            window.draw(center);

            // check HIT basato sul tempo (non sulla posizione)
            if (elapsedMs >= note.time - 50 && elapsedMs <= note.time + 50) {
                if (checkKey(note)) {
                    note.hit = true;
                    hitCount++;
                    cout << "[HIT] " << note.key << " al tempo " << elapsedMs << "ms\n";
                }
            }
        }

        window.display();
    }

    cout << "\n--- RISULTATO ---\n";
    cout << "Hit: " << hitCount << "\n";
    cout << "Miss: " << missCount << "\n";
}
