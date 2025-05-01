#include <iostream>
#include <conio.h>w
#include <windows.h>
#include <vector>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 25;

class Snake
{
private:
    COORD position;
    int snakeLen;
    char direction;
    vector<COORD> tail;

public:
    Snake(COORD startPos);

    void changeDirection(char newDirection);
    void move();
    bool eatFruit(COORD fruitPos);
    void incLen();
    bool hitWall();

    COORD getPosition();
    vector<COORD> getTail();
};

Snake::Snake(COORD startPos)
{
    position = startPos;
    direction = 'd';
    snakeLen = 1;

    tail.push_back(position);
}

void Snake::changeDirection(char newDirection) {
    direction = newDirection;
}

void Snake::move() {
    switch (direction)
    {
    case 'w': position.Y -= 1; break;
    case 's': position.Y += 1; break;
    case 'a': position.X -= 1; break;
    case 'd': position.X += 1; break;
    }

    tail.push_back(position);
    if (tail.size() > snakeLen) tail.erase(tail.begin());
}

bool Snake::eatFruit(COORD fruitPos) {
    if ((position.X == fruitPos.X) && (position.Y == fruitPos.Y)) {
        return true;
    }
    else {
        return false;
    }
}

void Snake::incLen() {
    snakeLen++;
}

bool Snake::hitWall() {
    if (position.X <= 0 || position.X >= WIDTH - 1 ||
        position.Y <= 0 || position.Y >= HEIGHT - 1) {
        return true;
    }

    for (int i = 0; i < tail.size() - 1; ++i) {
        if (position.X == tail[i].X && position.Y == tail[i].Y) {
            return true;
        }
    }

    return false;
}

COORD Snake::getPosition() {
    return position;
}

vector<COORD> Snake::getTail() {
    return tail;
}

class Fruit
{
private:
    COORD fruitPos;

public:
    void spawnFruit();
    COORD getFruitPos();
};

void Fruit::spawnFruit(){
    fruitPos.X = (rand() % WIDTH - 2) + 1;
    fruitPos.Y = (rand() % HEIGHT - 2) + 1;
}

COORD Fruit::getFruitPos() {
    return fruitPos;
}

Snake snake({WIDTH / 2, HEIGHT / 2 });
Fruit fruit;

int score = 0;

void board()
{
    COORD snakePos = snake.getPosition();
    COORD fruitPos = fruit.getFruitPos();

    vector<COORD> tail = snake.getTail();

    for (int i = 0; i < HEIGHT; i++) {
        cout << "\t\t#";
        for (int j = 0; j < WIDTH - 2; j++) {
            if (i == 0 || i == HEIGHT - 1) {
                cout << '#';
            }
            else if ((i == snakePos.Y) && (j + 1 == snakePos.X)) {
                cout << '0';
            }
            else if ((i == fruitPos.Y) && (j + 1 == fruitPos.X)) {
                cout << '@';
            }
            else {
                bool isTail = false;
                for (int k = 0; k < tail.size() - 1; k++)
                {
                    if (i == tail[k].Y && j + 1 == tail[k].X)
                    {
                        cout << 'o';
                        isTail = true;
                        break;
                    }
                }

                if (!isTail) cout << ' ';
            }
        }
        cout << "#\n";
    }

    cout << "\n\nSCORE : " << score;
}

int main() {
    srand(time(NULL));

    fruit.spawnFruit();

    bool isAlive = true;

    while (isAlive)
    {
        board();

        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                snake.changeDirection('w');
                break;
            case 'a':
                snake.changeDirection('a');
                break;
            case 's':
                snake.changeDirection('s');
                break;
            case 'd':
                snake.changeDirection('d');
                break;
            }
        }

        if (snake.hitWall()) {
            isAlive = false;
        }

        if (snake.eatFruit(fruit.getFruitPos())) {
            fruit.spawnFruit();
            snake.incLen();
            score += 10;
        }

        snake.move();

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
    }

    return 0;
}