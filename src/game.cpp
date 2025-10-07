#include "game.h"
#include "draw.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#define PERFECT 30
#define OK 50
#define LATE 100

using namespace std;

void Game::loadBeatmap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire la beatmap " << filename << "\n";
        return;
    }

    int time;
    int lasts;
    char key;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        if (iss >> time >> key){
            if (iss >> lasts)
                notes.emplace_back(time, key, lasts);
            else
                notes.emplace_back(time, key, 0);
        }
    }
    file.close();
}

void Game::drawNotes(sf::RenderWindow& window, int elapsedMs, float noteSpeed) {
    for (auto& nota : notes) {
        if (nota.hit) continue;

        float y = TARGET_Y - (nota.time - elapsedMs) * noteSpeed;
        if (y > 0.f && y < WINDOW_Y + 80.f) {
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

void Game::checkHit(Nota& nota, int elapsedMs, int notaTime)
{
    int delta = abs(elapsedMs - notaTime);
        if (delta <= PERFECT) {
            nota.hit = true;
            hitCount++;
            cout << "[PERFECT] " << nota.key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
            return;
        } else if (delta <= OK){
            nota.hit = true;
            hitCount++;
            cout << "[OK] " << nota.key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
        } else if (delta <= LATE) {
            nota.hit = true;
            missCount++;
            cout << "[TROPPO PRESTO] " << nota.key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
        }
        else {
            nota.hit = true;
            missCount++;
            cout << "[TROPPO TARDI] " << nota.key << " al tempo " << elapsedMs << "ms (delta " << delta << ")\n";
        }
}

void Game::handleHit(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit || nota.key != key) continue;
            checkHit(nota, elapsedMs, nota.time);

        /*sf::Event lastEvent;
        if (nota.lasts){
            while ((window.pollEvent(lastEvent) == sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && elapsedMs <= nota.time + nota.lasts){
                if (window.pollEvent(lastEvent) == sf::Keyboard::isKeyReleased(sf::Keyboard::A))
                checkHit(nota, elapsedMs, nota.time + nota.lasts)
            }
        }*/
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
