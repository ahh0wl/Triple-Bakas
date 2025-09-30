#include "game.h"
#include "draw.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

void Game::drawNotes(sf::RenderWindow& window, int elapsedMs, float noteSpeed) {
    for (auto& nota : notes) {
        if (nota.hit) continue;

        float y = TARGET_Y - (nota.time - elapsedMs) * noteSpeed;

        if (y > -80.f && y < WINDOW_Y + 80.f) {
            draw_notes(window, nota, y);
        }
    }
}

void Game::updateNotes(int elapsedMs) {
    for (auto& nota : notes) {
        if (!nota.hit && elapsedMs > nota.time + 100) {
            nota.hit = true;
            missCount++;
            cout << "[MISS] Nota persa: " << nota.key 
                 << " al tempo " << nota.time << "ms\n";
        }
    }
}

void Game::checkInput(int elapsedMs) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) handleHit('a', elapsedMs);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) handleHit('s', elapsedMs);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) handleHit('d', elapsedMs);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) handleHit('f', elapsedMs);
}

void Game::handleHit(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit || nota.key != key) continue;

        int delta = abs(elapsedMs - nota.time);

        if (delta <= 30) {
            nota.hit = true;
            hitCount++;
            cout << "[PERFECT] " << key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
            return;
        } else if (delta <= 50){
            nota.hit = true;
            hitCount++;
            cout << "[OK] " << key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
        }
    }
}

void Game::run() {
    if (notes.empty()) {
        cout << "Nessuna nota caricata!\n";
        return;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Triple-Bakas");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const float noteSpeed = 0.5f; // pixel per millisecondo

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        int elapsedMs = clock.getElapsedTime().asMilliseconds();

        //update
        updateNotes(elapsedMs);   // miss automatici
        checkInput(elapsedMs);    // gestione input

        //disegno
        window.clear(sf::Color::Black);
        draw_board(window, TARGET_Y);
        drawNotes(window, elapsedMs, noteSpeed);
        window.display();
    }

    cout << "\n--- RISULTATO ---\n";
    cout << "Hit: " << hitCount << "\n";
    cout << "Miss: " << missCount << "\n";
}
