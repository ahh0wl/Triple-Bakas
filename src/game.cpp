#include "game.h"
#include "draw.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iostream>
using namespace std;

#define PERFECT 20
#define OK 40
#define LATE 50

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
        // le note PERFETTE, OK già risolte e non di tipo hold non vengono più ridisegnate
        if ((nota.hit == HitResult::PERFECT_HIT || nota.hit == HitResult::OK_HIT) && !nota.lasts)
            continue;

        float yhead = TARGET_Y - (nota.time - elapsedMs) * noteSpeed;
        float ytail;
        if (nota.lasts)
            ytail = TARGET_Y - ((nota.time + nota.lasts) - elapsedMs) * noteSpeed;
        else
            ytail = yhead;

        // disegna solo se la nota è visibile nel range schermo ±100px
        if (yhead > -100.f && ytail < WINDOW_Y + 100.f) {
            // disegna anche le note "mancate" per un po' dopo il MISS
            if (nota.hit == NOT_HIT || nota.hold || nota.hit == LATE_HIT || nota.hit == EARLY_HIT) {
                if (!nota.lasts) {
                    // nota singola
                    draw_circle(window, nota, yhead);
                } else {
                    // nota lunga
                    draw_trail(window, nota, yhead, ytail, sf::Color(56, 255, 226, 150));
                    draw_circle(window, nota, yhead);
                    draw_circle(window, nota, ytail);
                    if (nota.hold) {
                        draw_trail(window, nota, yhead, TARGET_Y, sf::Color(56, 255, 56, 150));
                        draw_circle(window, nota, TARGET_Y);
                    }
                }
            }
        }
    }
}

void Game::draw_timing(int elapsedMs, sf::RenderWindow& window) {
    for (auto& nota : notes) {
        if (nota.hit == NOT_HIT)
            continue;
        if (nota.hit == EARLY_HIT && elapsedMs < nota.time + 500)
            timing_result(window, nota, font, "EARLY!", sf::Color (255, 255, 0, 255));
        else if (nota.hit == OK_HIT && elapsedMs < nota.time + nota.lasts + 500)
            timing_result(window, nota, font, "OK!", sf::Color (26, 167, 255, 255));
        else if (nota.hit == PERFECT_HIT && elapsedMs < nota.time + nota.lasts + 500)
            timing_result(window, nota, font, "PERFECT!", sf::Color (204, 0, 204, 255));
        else if (nota.hit == LATE_HIT && elapsedMs < nota.time + nota.lasts + 500)
            timing_result(window, nota, font, "MISS!", sf::Color (255, 0, 0, 255));
    }
}

void Game::updateNotes(int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit != NOT_HIT && nota.hold == false)
            continue;
        
        // se è passata senza premere o tieni troppo premuto
        if ((elapsedMs > nota.time + nota.lasts + LATE)) {
            nota.hold = false;
            nota.hit = LATE_HIT;
            missCount++;
            cout << "[MISS-LATE HOLD] " << nota.key << " non colpita" << endl;
        }
    }
}

void Game::handlePress(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (nota.hit != NOT_HIT) continue;
        if (nota.key != key) continue;

        int delta = elapsedMs - nota.time;
        if (abs(delta) > 200) continue;

        if (nota.hit == HitResult::NOT_HIT)
        {
            if (delta < -LATE) {
                nota.hit = HitResult::EARLY_HIT;
                cout << "[EARLY] " << key << " premuto troppo presto (" << delta << "ms)" << endl;
                missCount++;
            } else if (abs(delta) <= PERFECT && abs(delta) >= -PERFECT) {
                nota.hit = HitResult::PERFECT_HIT;
                if (!nota.lasts)
                    hitCount++;
                cout << "[PERFECT] " << key << " a " << delta << "ms" << endl;
            } else if (abs(delta) <= OK && abs(delta) >= -OK) {
                nota.hit = HitResult::OK_HIT;
                if (!nota.lasts)
                    hitCount++;
                cout << "[OK] " << key << " a " << delta << "ms" << endl;
            } else {
                nota.hit = HitResult::LATE_HIT;
                missCount++;
                cout << "[LATE] " << key << " premuto tardi (" << delta << "ms)" << endl;
            }
        }
        if (nota.lasts && (nota.hit == HitResult::OK_HIT || nota.hit == HitResult::PERFECT_HIT)) {
            nota.hold = true;
            cout << "[HOLD START] " << key << endl;
        }
        break;
    }
}

void Game::handleRelease(char key, int elapsedMs) {
    for (auto& nota : notes) {
        if (!nota.hold) continue;        // non è in hold
        if (nota.key != key) continue;
        
        int releaseTime = nota.time + nota.lasts;
        int delta = elapsedMs - releaseTime;
        if (!nota.missed())
        {
            if (abs(delta) <= PERFECT) {
                hitCount++;
                nota.hit = HitResult::PERFECT_HIT;
                cout << "[HOLD PERFECT] " << key << " rilasciato perfettamente (" << elapsedMs 
                     << "ms, delta " << delta << ")" << endl;
            }
            else if (abs(delta) <= OK) {
                hitCount++;
                nota.hit = HitResult::OK_HIT;
                cout << "[HOLD OK] " << key << " rilasciato quasi giusto (" << elapsedMs 
                     << "ms, delta " << delta << ")" << endl;
            }
            else if (delta < -OK) {
                missCount++;
                nota.hit = HitResult::EARLY_HIT;
                nota.hold = false;
                cout << "[HOLD EARLY] " << key << " rilasciato troppo presto (" << elapsedMs 
                     << "ms, delta " << delta << ")" << endl;
            }
            else if (delta > OK) {
                missCount++;
                nota.hit = HitResult::LATE_HIT;
                cout << "[HOLD LATE] " << key << " rilasciato troppo tardi (" << elapsedMs 
                     << "ms, delta " << delta << ")" << endl;
            }
        }
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        int elapsedMs = clock.getElapsedTime().asMilliseconds();

        checkPress(elapsedMs);
        updateNotes(elapsedMs);

        window.clear(sf::Color::Black);
        draw_board(window, font, TARGET_Y);
        drawnotes(window, elapsedMs, NOTE_SPEED);
        draw_timing(elapsedMs, window);
        window.display();
    }

    cout << "\n--- RISULTATO ---\n";
    cout << "Hit: " << hitCount << "\n";
    cout << "Miss: " << missCount << "\n";
}
