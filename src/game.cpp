#include "game.h"
#include "draw.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
using namespace std;

#define PERFECT 30
#define OK 50
#define LATE 100

void Game::loadBeatmap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire la beatmap " << filename << endl;
        return;
    }

    int time, lasts;
    char key;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        if (iss >> time >> key) {
            if (!(iss >> lasts)) lasts = 0;
            notes.emplace_back(time, key, lasts);
        }
    }
    file.close();
}

void Game::drawnotes(sf::RenderWindow& window, int elapsedMs, float noteSpeed) {
    for (auto& nota : notes) {
        if (nota.hit && !nota.hold) continue;

        float yhead = TARGET_Y - (nota.time - elapsedMs) * noteSpeed;
        float ytail;
        if (nota.lasts)
            ytail = TARGET_Y - ((nota.time + nota.lasts) - elapsedMs) * noteSpeed;
        else
            ytail = yhead;
        if (yhead > 0.f || ytail < WINDOW_Y + 80.f) {
            if (!nota.lasts) 
            {
                draw_circle(window, nota, yhead);
            }
            else
            {
                draw_trail(window, nota, yhead, ytail);
                draw_circle(window, nota, yhead);
                draw_circle(window, nota, ytail);
            }
        }
    }
}

void Game::updateNotes(int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit) continue;
        if (nota.hold) continue;

        // se è passata senza premere
        if (elapsedMs > nota.time + nota.lasts + 200 && !nota.hold) {
            nota.hit = true;
            missCount++;
            cout << "[MISS] " << nota.key << " non colpita" << endl;
        }
    }
}

void Game::handlePress(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit) continue;
        if (nota.key != key) continue;

        int delta = elapsedMs - nota.time;

        // dentro la finestra di hit
        if (abs (delta) <= 200)
        {
            nota.missed = true;
            if (abs(delta) <= 50) {
                if (nota.lasts) {
                    nota.hold = true;
                    cout << "[HOLD START] " << key << " premuto a " << elapsedMs << "ms" << endl;
                } else {
                    nota.hit = true;
                    hitCount++;
                    cout << "[HIT] " << key << " a " << elapsedMs << "ms" << endl;
                }
                break;
            }
        }
    }
}

void Game::handleRelease(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (!nota.hold) continue;        // non è in hold
        if (nota.key != key) continue;
        if (nota.hit) continue;          // già completata

        int releaseTime = nota.time + nota.lasts;
        int delta = elapsedMs - releaseTime;

        if (abs(delta) <= PERFECT) {
            hitCount++;
            cout << "[HOLD PERFECT] " << key << " rilasciato perfettamente (" << elapsedMs 
                 << "ms, delta " << delta << ")" << endl;
        }
        else if (abs(delta) <= OK) {
            hitCount++;
            cout << "[HOLD OK] " << key << " rilasciato quasi giusto (" << elapsedMs 
                 << "ms, delta " << delta << ")" << endl;
        }
        else if (delta < -OK) {
            missCount++;
            cout << "[HOLD EARLY] " << key << " rilasciato troppo presto (" << elapsedMs 
                 << "ms, delta " << delta << ")" << endl;
        }
        else if (delta > OK) {
            missCount++;
            cout << "[HOLD LATE] " << key << " rilasciato troppo tardi (" << elapsedMs 
                 << "ms, delta " << delta << ")" << endl;
        }
        nota.hit = true;
        nota.hold = false;
        break;
    }
}

void Game::check(char key, sf::Keyboard::Key sfKey, int elapsedMs)
{
    bool pressed = sf::Keyboard::isKeyPressed(sfKey);
    int i = getIfromKey(key);
    if (pressed) {
        // appena premuto
        keyHeld[i] = true;
        handlePress(key, elapsedMs);
    } else if (!pressed && keyHeld[i]) {
        keyHeld[i] = false;
        handleRelease(key, elapsedMs);
    }

}

void Game::checkPress(int elapsedMs) {
    check('a', sf::Keyboard::A, elapsedMs);
    check('s', sf::Keyboard::S, elapsedMs);
    check('d', sf::Keyboard::D, elapsedMs);
    check('f', sf::Keyboard::F, elapsedMs);
}



void Game::run() {
    if (notes.empty()) {
        cout << "Nessuna nota caricata!" << endl;
        return;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Triple-Bakas");
    window.setFramerateLimit(60);

    sf::Clock clock;
    const float noteSpeed = 0.5f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        int elapsedMs = clock.getElapsedTime().asMilliseconds();

        updateNotes(elapsedMs);
        checkPress(elapsedMs);

        window.clear(sf::Color::Black);
        draw_board(window, TARGET_Y);
        drawnotes(window, elapsedMs, noteSpeed);
        window.display();
    }

    cout << "\n--- RISULTATO ---\n";
    cout << "Hit: " << hitCount << "\n";
    cout << "Miss: " << missCount << "\n";
}
