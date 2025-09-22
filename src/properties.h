#pragma once

// -----------------------------
// Constants
// -----------------------------
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// Grid setup
const int CELL_SIZE = 20;               // Each cell is 20x20 pixels
const int COLS = SCREEN_WIDTH / CELL_SIZE; // 30 columns
const int ROWS = SCREEN_HEIGHT / CELL_SIZE; // 30 rows

// Snake starting parameters
const int INITIAL_SNAKE_LENGTH = 3;

// Timing
const double TICK_INTERVAL = 0.12; // seconds per snake move
int _r = 14; int _c = 14;


float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

