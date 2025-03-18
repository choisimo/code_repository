#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Player 클래스 정의
class Player {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Player(float startX, float startY) {
        shape.setSize({ 50.0f, 50.0f });
        shape.setPosition(startX, startY);
        shape.setFillColor(sf::Color::Blue);
        speed = 5.0f;
    }

    void move(const sf::Vector2f& direction) {
        shape.move(direction * speed);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }
};

// Obstacle 클래스 정의
class Obstacle {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Obstacle(float startX, float startY) {
        shape.setSize({ 50.0f, 50.0f });
        shape.setPosition(startX, startY);
        shape.setFillColor(sf::Color::Red);
        speed = 3.0f;
    }

    void update() {
        shape.move(0, speed);
        if (shape.getPosition().y > 600) {
            shape.setPosition(rand() % 400, -50); // 화면 위에서 다시 시작
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getBounds() {
        return shape.getGlobalBounds();
    }
};

// Game 클래스 정의
class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Obstacle> obstacles;

public:
    Game() : window(sf::VideoMode(400, 600), "Simple GUI Game"), player(175.0f, 500.0f) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < 5; ++i) {
            obstacles.emplace_back(rand() % 400, -50 * i);
        }
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    void update() {
        // 플레이어 이동
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move({ -1.0f, 0.0f });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move({ 1.0f, 0.0f });
        }

        // 장애물 업데이트
        for (auto& obstacle : obstacles) {
            obstacle.update();
            if (player.getBounds().intersects(obstacle.getBounds())) {
                window.close(); // 충돌 시 게임 종료
            }
        }
    }

    void render() {
        window.clear();
        player.draw(window);
        for (auto& obstacle : obstacles) {
            obstacle.draw(window);
        }
        window.display();
    }
};
