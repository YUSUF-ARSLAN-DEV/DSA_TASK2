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

int locationToGridX(const string& loc) {
    if (loc.find("ZA") != string::npos) return 2;
    if (loc.find("ZB") != string::npos) return 5;
    if (loc.find("ZC") != string::npos) return 8;
    return 0;
}

int locationToGridY(const string& loc) {
    if (loc.find("S1") != string::npos) return 1;
    if (loc.find("S2") != string::npos) return 2;
    if (loc.find("S3") != string::npos) return 3;
    return 0;
}

void runTask2() {
    AssignmentQueue aq;
    aq.firstBot = nullptr;
    aq.currentToBeAssigned = nullptr;
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

void runFullWorkflow() {
    cout << "\n========== FULL SYSTEM WORKFLOW ==========\n";

    OrderManagement om(5);
    AssignmentQueue aq;
    aq.firstBot = nullptr;
    aq.currentToBeAssigned = nullptr;
    ItemManager im;
    WarehouseLayout wl;

    aq.addRobot(101);
    aq.addRobot(102);
    aq.addRobot(103);

    // Seed items matching order dataset
    im.addItem(205, "Wireless Mouse", "ZA-A1-S1");
    im.addItem(110, "Mechanical Keyboard", "ZA-A2-S1");
    im.addItem(330, "27-inch Monitor", "ZB-B1-S1");
    im.addItem(450, "Desk Lamp", "ZB-B2-S1");
    im.addItem(120, "USB-C Hub", "ZC-C1-S1");

    cout << "\nSeeded items and robots. Starting order processing...\n";

    for (int orderNum = 0; orderNum < 5; orderNum++) {
        cout << "\n--------------------------------------------\n";
        cout << ">>> CYCLE " << (orderNum + 1) << " <<<\n";
        cout << "--------------------------------------------\n";

        // 1. Receive order from dataset
        if (!om.receiveNextOrder()) {
            cout << "Error: Could not receive order.\n";
            break;
        }

        // 2. Process order (dequeue from pending)
        Order current;
        if (!om.processNextOrder(current)) {
            cout << "Error: Could not process order.\n";
            break;
        }

        cout << "\n[1] ORDER RECEIVED & PROCESSING\n";
        cout << "  Order ID : " << current.orderId << "\n";
        cout << "  Customer : " << current.customerId << "\n";
        cout << "  Item ID  : " << current.itemId << "\n";
        cout << "  Quantity : " << current.quantity << "\n";

        // 3. Lookup item in ItemManager
        int itemNum = stoi(current.itemId.substr(1));
        Item found;
        string location;

        cout << "\n[2] ITEM LOCATION LOOKUP\n";
        if (im.searchItemById(itemNum, found)) {
            location = found.location;
            cout << "  Item : " << found.itemName << "\n";
            cout << "  Location : " << location << "\n";
        } else {
            cout << "  Warning: Item ID " << itemNum << " not found in catalog.\n";
        }

        // 4. Plan route via WarehouseLayout
        cout << "\n[3] ROUTE PLANNING\n";
        if (!location.empty()) {
            wl.findPath("WH", location);
        }

        // 5. Assign robot
        cout << "\n[4] ROBOT ASSIGNMENT\n";
        Task* task = new Task;
        task->id = ++taskIdCounter;
        task->taskDescription = current.itemId + " at " + location;
        task->next = nullptr;
        aq.assignTask(task);

        // 6. Navigate robot to location
        cout << "\n[5] ROBOT NAVIGATION\n";
        if (!location.empty()) {
            int tx = locationToGridX(location);
            int ty = locationToGridY(location);

            RobotNavigator robot(0, 0);
            robot.moveToLocation(tx, ty);
            robot.returnToStart();
            robot.displayStatus();
        }

        // 7. Complete order
        om.completeCurrentOrder();
        cout << "\n[6] ORDER COMPLETED\n";
        cout << "  " << current.orderId << " marked as Completed.\n";
    }

    cout << "\n========== WORKFLOW SUMMARY ==========\n";
    om.displaySummary();
    cout << "======================================\n";
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
        cout << "6. Full System Workflow\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) task1Main();
        else if (choice == 2) runTask2();
        else if (choice == 3) task3Main();
        else if (choice == 4) task4Main();
        else if (choice == 5) task5Main();
        else if (choice == 6) runFullWorkflow();
        else if (choice == 0) cout << "System exited.\n";
        else cout << "Invalid choice.\n";
    } while (choice != 0);
    return 0;
}
