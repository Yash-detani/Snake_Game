#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <unistd.h>
#include <termios.h>

using namespace std;

// --- GLOBAL CONSTANTS ---
const int MAX_TAIL = 100;

// Direction Enum
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Helper function to check if a string contains only letters
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

// ==========================================
// 1. TERMINAL CLASS (Handles Linux Console)
// ==========================================
class Terminal {
public:
    // Enables/disables non-blocking raw terminal input mode
    static void setRawMode(bool enable) {
        static struct termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            newt.c_cc[VMIN] = 0;
            newt.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        }
    }

    // Reads key input immediately without pressing Enter
    static char getKeyPress() {
        char key = 0;
        read(STDIN_FILENO, &key, 1);
        return key;
    }

    // Clears the Linux terminal screen
    static void clearScreen() {
        cout << "\033[H\033[J";
    }
};

// ==========================================
// 2. SNAKE CLASS (Encapsulates Snake State)
// ==========================================
class Snake {
private:
    int headX, headY;
    int tailX[MAX_TAIL];
    int tailY[MAX_TAIL];
    int length;
    Direction dir;

public:
    Snake() {
        headX = 0;
        headY = 0;
        length = 0;
        dir = STOP;
    }

    // Initialize/Reset snake position based on custom grid dimensions
    void initPosition(int startX, int startY) {
        headX = startX;
        headY = startY;
        length = 0;
        dir = STOP;
    }

    // Getters
    int getHeadX() const { return headX; }
    int getHeadY() const { return headY; }
    int getLength() const { return length; }
    Direction getDirection() const { return dir; }

    // Setters & Controls
    void setDirection(Direction d) { dir = d; }

    void grow() {
        if (length < MAX_TAIL) {
            length++;
        }
    }

    // Checks if a grid coordinate contains part of the tail
    bool isTailAt(int x, int y) const {
        for (int i = 0; i < length; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                return true;
            }
        }
        return false;
    }

    // Updates body positions and moves the head forward
    void move() {
        if (dir == STOP) return;

        // Shift body array positions
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;

        tailX[0] = headX;
        tailY[0] = headY;

        for (int i = 1; i < length; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        // Move head based on active direction
        if (dir == LEFT)  headX--;
        if (dir == RIGHT) headX++;
        if (dir == UP)    headY--;
        if (dir == DOWN)  headY++;
    }
};

// ==========================================
// 3. FOOD CLASS (Manages Food Generation)
// ==========================================
class Food {
private:
    int x, y;

public:
    Food() { x = 0; y = 0; }

    int getX() const { return x; }
    int getY() const { return y; }

    // Generates new food at a random position inside dynamic grid walls
    void spawn(int gridWidth, int gridHeight) {
        x = rand() % (gridWidth - 2) + 1;
        y = rand() % (gridHeight - 2) + 1;
    }
};

// ==========================================
// 4. GAME ENGINE CLASS (Main Controller)
// ==========================================
class SnakeX {
private:
    Snake snake;
    Food food;
    bool gameOver;
    int score;
    int speedMs;
    int width;
    int height;
    string playerName;

    // Resets the internal state for a fresh game round
    void resetGame() {
        gameOver = false;
        score = 0;
        snake.initPosition(width / 2, height / 2);
        food.spawn(width, height);
    }

public:
    SnakeX() {
        gameOver = false;
        score = 0;
        speedMs = 100000;
        width = 20;
        height = 20;
    }

    // Setup Menu with Validation Loops
    void showMenu() {
        cout << "========================================" << endl;
        cout << "           WELCOME TO SNAKEX            " << endl;
        cout << "========================================" << endl;

        // Validated Player Name Input Loop (Alphabetic Only)
        do {
            cout << "Enter Player Name (letters only): ";
            cin >> playerName;

            if (!isValidName(playerName)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Error] Name cannot contain numbers or special characters!" << endl;
            } else {
                break;
            }
        } while (true);

        // Validated Grid Width Input Loop
        do {
            cout << "\nEnter Grid Width (10 to 80): ";
            cin >> width;
            if (cin.fail() || width < 10 || width > 80) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Error] Invalid input. Width must be a number between 10 and 80." << endl;
            } else {
                break;
            }
        } while (true);

        // Validated Grid Height Input Loop
        do {
            cout << "Enter Grid Height (10 to 40): ";
            cin >> height;
            if (cin.fail() || height < 10 || height > 36) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Error] Invalid input. Height must be a number between 10 and 40." << endl;
            } else {
                break;
            }
        } while (true);

        // Validated Difficulty Input Loop
        int choice;
        do {
            cout << "\nSelect Difficulty Level:" << endl;
            cout << "1. Easy" << endl;
            cout << "2. Medium" << endl;
            cout << "3. Hard" << endl;
            cout << "Choice (1-3): ";
            cin >> choice;

            if (cin.fail() || choice < 1 || choice > 3) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Error] Invalid choice! Select 1, 2, or 3." << endl;
            } else {
                break;
            }
        } while (true);

        if (choice == 1)      speedMs = 150000; // Slower
        else if (choice == 3) speedMs = 50000;  // Faster
        else                  speedMs = 100000; // Normal

        // Flush stdin buffer before launching game raw mode
        cin.ignore(10000, '\n');

        resetGame();
    }

    // Render Grid & UI
    void draw() {
        Terminal::clearScreen();

        cout << "Player: " << playerName << " | Score: " << score 
             << " | Grid: " << width << "x" << height << endl;

        // Top border
        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << endl;

        // Inner grid contents
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "|"; // Left border

                if (i == snake.getHeadY() && j == snake.getHeadX()) {
                    cout << "O"; // Head
                } else if (i == food.getY() && j == food.getX()) {
                    cout << "#"; // Food
                } else if (snake.isTailAt(j, i)) {
                    cout << "o"; // Tail body
                } else {
                    cout << " "; // Empty cell
                }

                if (j == width - 1) cout << "|"; // Right border
            }
            cout << endl;
        }

        // Bottom border
        for (int i = 0; i < width + 2; i++) cout << "-";
        cout << endl;

        if (snake.getDirection() == STOP && !gameOver) {
            cout << "Press W, A, S, or D to start moving!" << endl;
        } else if (!gameOver) {
            cout << "Controls: W (Up) | A (Left) | S (Down) | D (Right) | X (Quit)" << endl;
        }
    }

    // Process Keyboard Inputs
    void handleInput() {
        char key = Terminal::getKeyPress();
        Direction currentDir = snake.getDirection();

        if ((key == 'a' || key == 'A') && currentDir != RIGHT) {
            snake.setDirection(LEFT);
        } else if ((key == 'd' || key == 'D') && currentDir != LEFT) {
            snake.setDirection(RIGHT);
        } else if ((key == 'w' || key == 'W') && currentDir != DOWN) {
            snake.setDirection(UP);
        } else if ((key == 's' || key == 'S') && currentDir != UP) {
            snake.setDirection(DOWN);
        } else if (key == 'x' || key == 'X') {
            gameOver = true;
        }
    }

    // Core Game Physics & Rules
    void updateLogic() {
        if (snake.getDirection() == STOP) return;

        snake.move();

        int headX = snake.getHeadX();
        int headY = snake.getHeadY();

        // Boundary Collision
        if (headX < 0 || headX >= width || headY < 0 || headY >= height) {
            gameOver = true;
        }

        // Self-Collision
        if (snake.isTailAt(headX, headY)) {
            gameOver = true;
        }

        // Food Eating Mechanics
        if (headX == food.getX() && headY == food.getY()) {
            score += 10;
            snake.grow();
            food.spawn(width, height);
        }
    }

    // Main Game Loop with Restart / Quit Option
    void run() {
        showMenu();

        bool keepsPlaying = true;

        while (keepsPlaying) {
            Terminal::setRawMode(true);

            // Active Game Loop
            while (!gameOver) {
                draw();
                handleInput();
                updateLogic();
                usleep(speedMs);
            }

            // Draw final frozen board state
            draw();
            Terminal::setRawMode(false);

            // Display Game Over summary
            cout << "\n========================================" << endl;
            cout << "               GAME OVER                " << endl;
            cout << "========================================" << endl;
            cout << "Player: " << playerName << endl;
            cout << "Final Score: " << score << endl;
            cout << "========================================" << endl;

            // Prompt user to Restart or Quit
            char choice;
            cout << "Do you want to play again? (R to Restart / Q to Quit): ";
            cin >> choice;

            if (choice == 'r' || choice == 'R') {
                cin.ignore(10000, '\n');
                resetGame(); // Reset snake position and score for a new round
            } else {
                keepsPlaying = false; // Break outer loop and terminate program
                cout << "\nThank you for playing SnakeX! Exiting...\n" << endl;
            }
        }
    }
};

// ==========================================
// 5. MAIN FUNCTION
// ==========================================
int main() {
    srand(time(0));

    SnakeX game;
    game.run();

    return 0;
}