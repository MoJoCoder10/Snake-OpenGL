// -----------------------------
// Includes
// -----------------------------
#include <GLFW/glfw3.h>
#include <vector>
#include <deque>
#include <cstdlib>   // For random
#include <ctime>     // For srand
#include <iostream>


#include "properties.h"

// -----------------------------
// Enums
// -----------------------------
enum class TileType {
    EMPTY,
    SNAKE,
    FOOD,
    WALL
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

// -----------------------------
// Structs
// -----------------------------
struct Position {
    int col;
    int row;
};

struct Tile {
    Position pos;     // Row and column on the grid
    TileType type;    // Empty, Snake, Food, or Wall
};

// -----------------------------
// Game State
// -----------------------------
struct GameState {
    std::deque<Tile> snake{ { Tile{14, 14, TileType::SNAKE}}};          // Snake body positions, head = front
    Tile grid[COLS][ROWS]; // 2D grid of tiles
    Direction currentDir = Direction::NONE;               // Current movement direction
    Position food{ 6, 6 };                        // Current food position
    bool gameOver;                        // True if game ended
    int score;                            // Current score
};

struct RGB {
    float r, g, b;
};


int PlaceFood();
void drawSquare(Tile tile);
GameState gameState;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moveSquare();
RGB typeToColor(TileType type);
GLFWwindow* window;

int main() {
    srand(time(0));  // Use current time as seed

    PlaceFood();
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Make the window unresizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a GLFW window
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    // Make the window's context current
    glfwMakeContextCurrent(window);
    double lastMoveTime = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color to black

        // Run moveSquare() every 0.5 seconds
        double currentTime = glfwGetTime();
        if (currentTime - lastMoveTime >= TICK_INTERVAL) {
            moveSquare();
            lastMoveTime = currentTime;
        }



        for (const Tile& tile : gameState.snake) {
            if (gameState.grid[tile.pos.row][tile.pos.col].type == TileType::FOOD) {
                gameState.grid[tile.pos.row][tile.pos.col].type = TileType::EMPTY;
                _r = rand() % ROWS;
                _c = rand() % COLS;
                gameState.grid[_r][_c].type = TileType::FOOD;
            }
            drawSquare(tile);
        }
        drawSquare(Tile{ gameState.food, TileType::FOOD });


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glfwTerminate();
    return 0;
}


int PlaceFood() {
    Tile tile;
    int r = rand() % ROWS;
    int c = rand() % COLS;
    tile = gameState.grid[r][c];

    return 0;
}

void drawSquare(Tile tile) {
    int row = tile.pos.row;
    int col = tile.pos.col;
    float x1 = col * CELL_SIZE;
    float y1 = row * CELL_SIZE;
    float x2 = x1 + CELL_SIZE;
    float y2 = y1 + CELL_SIZE;

    // Convert to NDC
    float x1_ndc = (x1 / (SCREEN_WIDTH / 2)) - 1.0f;
    float y1_ndc = 1.0f - (y1 / (SCREEN_HEIGHT / 2));
    float x2_ndc = (x2 / (SCREEN_WIDTH / 2)) - 1.0f;
    float y2_ndc = 1.0f - (y2 / (SCREEN_HEIGHT / 2));

    float r = typeToColor(tile.type).r;
    float g = typeToColor(tile.type).g;
    float b = typeToColor(tile.type).b;
    glColor3f(r, g, b);

    // Draw the square
    glBegin(GL_QUADS);
    glVertex2f(x1_ndc, y1_ndc); // Top-left
    glVertex2f(x2_ndc, y1_ndc); // Top-right
    glVertex2f(x2_ndc, y2_ndc); // Bottom-right
    glVertex2f(x1_ndc, y2_ndc); // Bottom-left
    glEnd();
}

// Correct GLFW key callback signature
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {  // Only respond when key is first pressed
        switch (key) {
        case GLFW_KEY_UP:
            if (gameState.currentDir != Direction::DOWN) {
                gameState.currentDir = Direction::UP;

            }
            break;
        case GLFW_KEY_DOWN:
            if (gameState.currentDir != Direction::UP) {
                gameState.currentDir = Direction::DOWN;

            }
            break;
        case GLFW_KEY_LEFT:
            if (gameState.currentDir != Direction::RIGHT) {
                gameState.currentDir = Direction::LEFT;

            }
            break;
        case GLFW_KEY_RIGHT:
            if (gameState.currentDir != Direction::LEFT) {
                gameState.currentDir = Direction::RIGHT;

            }
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true); // Close window on ESC
            break;
        }
    }
}

void moveSquare() {
    Tile head = gameState.snake.front();
    switch (gameState.currentDir) {
    case Direction::UP:
        head.pos.row--;
        break;
    case Direction::DOWN:
        head.pos.row++;
        break;
    case Direction::LEFT:
        head.pos.col--;
        break;
    case Direction::RIGHT:
        head.pos.col++;
        break;
    }

    if (gameState.currentDir == Direction::NONE) {
        return;
    }
    if (head.pos.col == gameState.food.col && head.pos.row == gameState.food.row) {
        gameState.snake.push_front(head);
        _r = rand() % ROWS;
        _c = rand() % COLS;
        for (const Tile& segment : gameState.snake) {
            if (segment.pos.col == _c && segment.pos.row == _r) {
                _r = rand() % ROWS;
                _c = rand() % COLS;
            }
        }
        gameState.food = { _r, _c };
    } else {
        for (const Tile& segment : gameState.snake) {
            if (segment.pos.col == head.pos.col && segment.pos.row == head.pos.row) {
                gameState.gameOver = true;
                std::cout << "Game Over! You ran into yourself." << std::endl;
                glfwSetWindowShouldClose(window, true);
                return;
            }
        }
    }
    if (head.pos.row < 0 || head.pos.row >= ROWS || head.pos.col < 0 || head.pos.col >= COLS) {
        gameState.gameOver = true;
        std::cout << "Game Over! You hit a wall." << std::endl;
        glfwSetWindowShouldClose(window, true);
        return;
    }
    gameState.snake.push_front(head);
    gameState.snake.pop_back();


}

RGB typeToColor(TileType type) {
    switch (type) {
    case TileType::EMPTY:
        return { 0.0f, 0.0f, 0.0f }; // Black
    case TileType::SNAKE:
        return { 0.0f, 1.0f, 0.0f }; // Green
    case TileType::FOOD:
        return { 1.0f, 0.0f, 0.0f }; // Red
    case TileType::WALL:
        return { 0.5f, 0.5f, 0.5f }; // Gray
    default:
        return { 1.0f, 1.0f, 1.0f }; // White for unknown types
    }
}