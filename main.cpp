#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int width = 800;
int height = 600;
int size = 20;
int score = 0;
int speed = 10;

sf::Vector2i direction(1, 0);
sf::Vector2i food;
vector<sf::Vector2i> snake;

sf::SoundBuffer eatBuffer, dieBuffer;
sf::Sound eatSound, dieSound;

void spawnFood() {
    food.x = rand() % (width / size);
    food.y = rand() % (height / size);
}

void resetGame() {
    snake.clear();
    snake.push_back(sf::Vector2i(10, 10));
    direction = sf::Vector2i(1, 0);
    score = 0;
    speed = 10;
    spawnFood();
}

bool checkCollision(sf::Vector2i pos) {
    for (int i = 1; i < snake.size(); i++) {
        if (snake[i] == pos) return true;
    }
    return false;
}

int main() {
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(width, height), "Snake Game");
    window.setFramerateLimit(60);

    if (!eatBuffer.loadFromFile("food.wav") || !dieBuffer.loadFromFile("gameover.wav")) {
        cout << "Sound files missing!" << endl;
        return -1;
    }

    eatSound.setBuffer(eatBuffer);
    dieSound.setBuffer(dieBuffer);

    sf::RectangleShape block(sf::Vector2f(size - 2, size - 2));
    sf::Clock clock;
    float timer = 0, delay = 0.1;

    resetGame();

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction.y != 1)
            direction = sf::Vector2i(0, -1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction.y != -1)
            direction = sf::Vector2i(0, 1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction.x != 1)
            direction = sf::Vector2i(-1, 0);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction.x != -1)
            direction = sf::Vector2i(1, 0);

        if (timer > delay) {
            timer = 0;
            sf::Vector2i head = snake[0] + direction;

            if (head.x < 0 || head.y < 0 || head.x >= width / size || head.y >= height / size || checkCollision(head)) {
                dieSound.play();
                resetGame();
                continue;
            }

            snake.insert(snake.begin(), head);

            if (head == food) {
                eatSound.play();
                score++;
                spawnFood();
                if (delay > 0.03) delay -= 0.005;
            } else {
                snake.pop_back();
            }
        }

        window.clear(sf::Color::Black);

        // Draw snake
        for (auto& part : snake) {
            block.setPosition(part.x * size, part.y * size);
            block.setFillColor(sf::Color::Green);
            window.draw(block);
        }

        // Draw food
        block.setPosition(food.x * size, food.y * size);
        block.setFillColor(sf::Color::Red);
        window.draw(block);

        window.display();
    }

    return 0;
}
