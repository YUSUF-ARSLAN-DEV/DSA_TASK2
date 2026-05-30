#include <iostream>
#include <string>
#include "OrderManagement.cpp"
#include "task1main.cpp"
#include "task 3.cpp"
#include "Robot_CIRCUAL_QUEUE.cpp"
#include "item_search_management.cpp"
#include "task4.cpp"
#include "task5.cpp"
using namespace std;

int taskIdCounter = 0;

void runTask2() {
    AssignmentQueue aq;
    aq.addRobot(101);
    aq.addRobot(102);
    aq.addRobot(103);

    int choice;
    do {
        cout << "\n========== ROBOT ASSIGNMENT MODULE ==========\n";
        cout << "1. Add Robot\n";
        cout << "2. Assign Task to Next Available Robot\n";
        cout << "3. Display All Robots\n";
        cout << "0. Exit to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) {
            static int nextId = 104;
            aq.addRobot(nextId++);
            cout << "Robot " << (nextId - 1) << " added.\n";
        } else if (choice == 2) {
            Task* t = new Task;
            t->id = ++taskIdCounter;
            cout << "Enter task description: ";
            getline(cin, t->taskDescription);
            t->next = nullptr;
            aq.assignTask(t);
        } else if (choice == 3) {
            if (aq.totalNumberOfRobots == 0) {
                cout << "No robots in the system.\n";
            } else {
                aq.displayAllRobots();
            }
        }
    } while (choice != 0);
}

int main() {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "   WAREHOUSE ROBOT NAVIGATION SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Order Management Module\n";
        cout << "2. Robot Assignment Module\n";
        cout << "3. Robot Navigation & Path Tracking\n";
        cout << "4. Item Search & Management\n";
        cout << "5. Warehouse Layout & Navigation\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) task1Main();
        else if (choice == 2) runTask2();
        else if (choice == 3) task3Main();
        else if (choice == 4) task4Main();
        else if (choice == 5) task5Main();
        else if (choice == 0) cout << "System exited.\n";
        else cout << "Invalid choice.\n";
    } while (choice != 0);
    return 0;
}
