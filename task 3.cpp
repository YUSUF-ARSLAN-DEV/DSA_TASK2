#include <iostream>
#include <cstring>

// ------------------------------------------------------------
// Movement Directions
// ------------------------------------------------------------

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    NONE
};

Direction inverseDirection(Direction dir) {
    switch (dir) {
        case FORWARD:  return BACKWARD;
        case BACKWARD: return FORWARD;
        case LEFT:     return RIGHT;
        case RIGHT:    return LEFT;
        default:       return NONE;
    }
}

const char* directionToString(Direction dir) {
    switch (dir) {
        case FORWARD:  return "FORWARD";
        case BACKWARD: return "BACKWARD";
        case LEFT:     return "LEFT";
        case RIGHT:    return "RIGHT";
        default:       return "NONE";
    }
}

// ------------------------------------------------------------
// MovementStep - Represents a single movement from one
// position to another in the direction given.
// ------------------------------------------------------------

struct MovementStep {
    Direction dir;
    int fromX;
    int fromY;
    int toX;
    int toY;
    int stepNumber;

    MovementStep()
        : dir(NONE), fromX(0), fromY(0),
          toX(0), toY(0), stepNumber(0) {}

    MovementStep(Direction d, int fx, int fy, int tx, int ty, int sn)
        : dir(d), fromX(fx), fromY(fy),
          toX(tx), toY(ty), stepNumber(sn) {}
};

// ------------------------------------------------------------
// Self-Implemented Stack (dynamic array-based)
// No STL containers used per assignment requirements.
// ------------------------------------------------------------

template <typename T>
class Stack {
private:
    T* data;
    int capacity;
    int topIndex;

    void resize() {
        int newCapacity = (capacity == 0) ? 4 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < topIndex; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Stack() : data(nullptr), capacity(0), topIndex(0) {}

    Stack(const Stack& other)
        : data(nullptr), capacity(0), topIndex(0) {
        if (other.topIndex > 0) {
            capacity = other.capacity;
            data = new T[capacity];
            topIndex = other.topIndex;
            for (int i = 0; i < topIndex; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            topIndex = 0;
            if (other.topIndex > 0) {
                capacity = other.capacity;
                data = new T[capacity];
                topIndex = other.topIndex;
                for (int i = 0; i < topIndex; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& item) {
        if (topIndex >= capacity) {
            resize();
        }
        data[topIndex++] = item;
    }

    T pop() {
        if (isEmpty()) {
            return T();
        }
        return data[--topIndex];
    }

    T top() const {
        if (isEmpty()) {
            return T();
        }
        return data[topIndex - 1];
    }

    bool isEmpty() const {
        return topIndex == 0;
    }

    int size() const {
        return topIndex;
    }
};

// ------------------------------------------------------------
// NavigationLog - Stores and displays forward/reverse paths
// ------------------------------------------------------------

class NavigationLog {
private:
    Stack<MovementStep> forwardPath;
    Stack<MovementStep> reversePath;
    int totalSteps;

    void displayStack(const Stack<MovementStep>& s,
                      const char* header) const {
        std::cout << "\n" << header << "\n";
        if (s.isEmpty()) {
            std::cout << "  (no steps recorded)\n";
            return;
        }
        Stack<MovementStep> temp = s;
        int count = temp.size();
        MovementStep* arr = new MovementStep[count];
        for (int i = count - 1; i >= 0; --i) {
            arr[i] = temp.pop();
        }
        for (int i = 0; i < count; ++i) {
            std::cout << "  Step " << arr[i].stepNumber << ": "
                      << directionToString(arr[i].dir)
                      << " (" << arr[i].fromX << "," << arr[i].fromY
                      << ") -> (" << arr[i].toX << ","
                      << arr[i].toY << ")\n";
        }
        delete[] arr;
    }

public:
    NavigationLog() : totalSteps(0) {}

    void recordForward(const MovementStep& step) {
        forwardPath.push(step);
        totalSteps++;
    }

    void recordReverse(const MovementStep& step) {
        reversePath.push(step);
        totalSteps++;
    }

    void displayForward() const {
        displayStack(forwardPath, "--- Forward Movement Path ---");
    }

    void displayReverse() const {
        displayStack(reversePath, "--- Reverse Path (Return) ---");
    }

    void displayComplete() const {
        std::cout << "\n========== COMPLETE NAVIGATION LOG ==========\n";
        displayForward();
        displayReverse();
        std::cout   << "\nTotal movements recorded: " << totalSteps
                    << " (" << forwardPath.size() << " forward, "
                    << reversePath.size() << " reverse)\n";
        std::cout << "==============================================\n";
    }

    int getTotalSteps()  const { return totalSteps; }
    int getForwardCount() const { return forwardPath.size(); }
    int getReverseCount() const { return reversePath.size(); }
};

// ------------------------------------------------------------
// RobotNavigator - Main controller for robot movement
// ------------------------------------------------------------

class RobotNavigator {
private:
    int currentX;
    int currentY;
    int startX;
    int startY;
    int stepCounter;
    Stack<MovementStep> pathStack;
    NavigationLog log;
    bool returnedToStart;

public:
    RobotNavigator(int sx = 0, int sy = 0)
        : currentX(sx), currentY(sy),
          startX(sx), startY(sy),
          stepCounter(0), returnedToStart(false) {}

    void move(Direction dir) {
        if (returnedToStart) {
            std::cout << "Error: Robot already returned. Cannot move.\n";
            return;
        }

        stepCounter++;
        int prevX = currentX;
        int prevY = currentY;

        switch (dir) {
            case FORWARD:  currentY++; break;
            case BACKWARD: currentY--; break;
            case LEFT:     currentX--; break;
            case RIGHT:    currentX++; break;
            default:
                std::cout << "Error: Invalid direction.\n";
                stepCounter--;
                return;
        }

        MovementStep step(dir, prevX, prevY, currentX, currentY, stepCounter);
        pathStack.push(step);
        log.recordForward(step);

        std::cout << "  Step " << stepCounter << ": "
                  << directionToString(dir)
                  << " -> (" << currentX << "," << currentY << ")\n";
    }

    void moveToLocation(int targetX, int targetY) {
        std::cout << "\n>> Navigating to (" << targetX
                  << ", " << targetY << ")...\n";
        while (currentX != targetX || currentY != targetY) {
            if (currentX < targetX) {
                move(RIGHT);
            } else if (currentX > targetX) {
                move(LEFT);
            } else if (currentY < targetY) {
                move(FORWARD);
            } else if (currentY > targetY) {
                move(BACKWARD);
            }
        }
        std::cout << ">> Arrived at destination (" << targetX
                  << ", " << targetY << ").\n";
    }

    void backtrack() {
        if (pathStack.isEmpty()) {
            std::cout << "Cannot backtrack: no steps recorded.\n";
            return;
        }

        MovementStep lastStep = pathStack.pop();
        currentX = lastStep.fromX;
        currentY = lastStep.fromY;

        std::cout << "\n>> Obstacle! Backtracked step "
                  << lastStep.stepNumber
                  << ", returned to (" << currentX << ","
                  << currentY << ").\n";
    }

    void returnToStart() {
        if (returnedToStart) {
            std::cout << "Robot has already returned.\n";
            return;
        }

        std::cout << "\n--- Returning to Start ---\n";

        int reverseCount = 0;

        while (!pathStack.isEmpty()) {
            MovementStep fs = pathStack.pop();
            reverseCount++;

            Direction rd = inverseDirection(fs.dir);

            int newX = currentX;
            int newY = currentY;
            switch (rd) {
                case FORWARD:  newY++; break;
                case BACKWARD: newY--; break;
                case LEFT:     newX--; break;
                case RIGHT:    newX++; break;
                default: break;
            }

            MovementStep rs(rd, currentX, currentY, newX, newY, reverseCount);
            log.recordReverse(rs);

            currentX = newX;
            currentY = newY;

            std::cout << "  Reverse step " << reverseCount << ": "
                      << directionToString(rd)
                      << " (" << rs.fromX << "," << rs.fromY
                      << ") -> (" << rs.toX << "," << rs.toY << ")\n";
        }

        returnedToStart = true;

        if (currentX == startX && currentY == startY) {
            std::cout << "\n>> Successfully returned to start ("
                      << startX << ", " << startY << ").\n";
        } else {
            std::cout << "\n>> Returned to (" << currentX << ","
                      << currentY << ") but start is ("
                      << startX << "," << startY << ").\n";
        }
    }

    void displayStatus() const {
        std::cout << "\n--- Robot Status ---\n";
        std::cout << "  Start:    (" << startX << ", " << startY << ")\n";
        std::cout << "  Current:  (" << currentX << ", " << currentY << ")\n";
        std::cout << "  Steps:    " << stepCounter << "\n";
        std::cout << "  Returned: " << (returnedToStart ? "Yes" : "No") << "\n";
    }

    void displayNavigationLog() const {
        log.displayComplete();
    }

    // Integration interface for other modules
    int getCurrentX()  const { return currentX; }
    int getCurrentY()  const { return currentY; }
    int getStartX()    const { return startX; }
    int getStartY()    const { return startY; }
    int getStepCount() const { return stepCounter; }
    bool isAtLocation(int tx, int ty) const {
        return currentX == tx && currentY == ty;
    }
    bool hasReturned()    const { return returnedToStart; }
    bool isAtStart() const {
        return currentX == startX && currentY == startY;
    }
};

// ------------------------------------------------------------
// Demo / Test
// ------------------------------------------------------------

int task3Main() {
    std::cout << "========== ROBOT NAVIGATION AND PATH TRACKING ==========\n";
    std::cout << "TASK 3  |  Data Structure: Self-implemented Stack (LIFO)\n";
    std::cout << "=======================================================\n";

    RobotNavigator robot(0, 0);

    std::cout << "\n>> Scenario: Robot dispatched to pick item at (2, 3)\n\n";
    robot.moveToLocation(2, 3);

    robot.displayStatus();

    std::cout << "\n>> Obstacle detected at (2, 3)! Backtracking and rerouting...\n";
    robot.backtrack();

    std::cout << "\n>> Taking alternate route to (2, 3)...\n";
    robot.move(FORWARD);
    robot.move(RIGHT);
    robot.move(FORWARD);

    robot.displayStatus();

    std::cout << "\n>> Task complete. Robot returning to start...\n";
    robot.returnToStart();

    robot.displayStatus();
    robot.displayNavigationLog();

    return 0;
}
