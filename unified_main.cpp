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

void runGuidedWorkflow() {
    cout << "\n============= WAREHOUSE WORKFLOW ==============\n";

    OrderManagement om(5);
    AssignmentQueue aq;
    aq.firstBot = nullptr;
    aq.currentToBeAssigned = nullptr;
    ItemManager im;
    WarehouseLayout wl;

    for (int i = 1; i <= 5; i++) aq.addRobot(100 + i);

    im.addItem(205, "Wireless Mouse", "ZA-A1-S1");
    im.addItem(110, "Mechanical Keyboard", "ZA-A2-S1");
    im.addItem(330, "27-inch Monitor", "ZB-B1-S1");
    im.addItem(450, "Desk Lamp", "ZB-B2-S1");
    im.addItem(120, "USB-C Hub", "ZC-C1-S1");

    int localTaskId = 0;

    int nextRobotId = 106;

    for (int cycle = 0; cycle < 5; cycle++) {
        cout << "\n#############################################\n";
        cout << "#            ORDER CYCLE " << (cycle + 1) << " OF 5            #\n";
        cout << "#############################################\n";

        Order current;
        string location;

        // ==== STEP 1: Task 1 — Receive Order ====
        cout << "\n--- WAREHOUSE ORDER MANAGEMENT ---\n";
        while (true) {
            cout << "\n1. Receive Next Order From Dataset\n";
            cout << "2. Send Next Pending Order For Processing\n";
            cout << "3. Mark Current Order As Completed\n";
            cout << "4. Display Pending Orders\n";
            cout << "5. Display Current Order\n";
            cout << "6. Display Completed Orders\n";
            cout << "7. Display Summary\n";
            cout << "Enter choice: ";
            int ch; cin >> ch; cin.ignore(1000, '\n');
            if (ch == 1) {
                if (om.receiveNextOrder()) {
                    cout << ">> Next order received and added to pending queue.\n";
                    break;
                } else cout << ">> Could not receive order.\n";
            } else if (ch == 4) om.displayPendingOrders();
            else if (ch == 5) om.displayCurrentOrder();
            else if (ch == 6) om.displayCompletedOrders();
            else if (ch == 7) om.displaySummary();
            else cout << "Invalid option for this step.\n";
        }
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 2: Task 1 — Process Order ====
        cout << "\n--- WAREHOUSE ORDER MANAGEMENT ---\n";
        while (true) {
            cout << "\n1. Receive Next Order From Dataset\n";
            cout << "2. Send Next Pending Order For Processing\n";
            cout << "3. Mark Current Order As Completed\n";
            cout << "4. Display Pending Orders\n";
            cout << "5. Display Current Order\n";
            cout << "6. Display Completed Orders\n";
            cout << "7. Display Summary\n";
            cout << "Enter choice: ";
            int ch; cin >> ch; cin.ignore(1000, '\n');
            if (ch == 2) {
                if (om.processNextOrder(current))
                    cout << ">> Order " << current.orderId << " is now being processed.\n"
                         << "   Customer: " << current.customerId << "\n"
                         << "   Item: " << current.itemId << " (Qty: " << current.quantity << ")\n";
                break;
            } else if (ch == 1) {
                if (om.receiveNextOrder()) cout << ">> Next order received.\n";
                else cout << ">> Could not receive order.\n";
            } else if (ch == 4) om.displayPendingOrders();
            else if (ch == 5) om.displayCurrentOrder();
            else if (ch == 6) om.displayCompletedOrders();
            else if (ch == 7) om.displaySummary();
            else cout << "Invalid option for this step.\n";
        }
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 3: Task 4 — Item Lookup ====
        cout << "\n--- ITEM SEARCH & MANAGEMENT ---\n";
        { int itemNum = stoi(current.itemId.substr(1));
          Item found;
          if (im.searchItemById(itemNum, found)) {
              location = found.location;
              cout << "Item ID " << itemNum << ": " << found.itemName << "\n";
              cout << "Located at: " << location << "\n";
          } else {
              cout << "Item " << itemNum << " not found in catalog.\n";
          } }
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 4: Task 5 — Route Planning ====
        cout << "\n--- WAREHOUSE LAYOUT & NAVIGATION ---\n";
        if (!location.empty()) wl.findPath("WH", location);
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 5: Task 2 — Assign Robot ====
        cout << "\n--- ROBOT ASSIGNMENT MODULE ---\n";
        cout << "\nCurrent Robots:\n";
        aq.displayAllRobots();
        while (true) {
            cout << "\n1. Add Robot\n";
            cout << "2. Assign Task to Next Available Robot\n";
            cout << "3. Display All Robots\n";
            cout << "Enter choice: ";
            int ch; cin >> ch; cin.ignore(1000, '\n');
            if (ch == 2) {
                Task* task = new Task;
                task->id = ++localTaskId;
                task->taskDescription = current.itemId + " at " + location;
                task->next = nullptr;
                aq.assignTask(task);
                break;
            } else if (ch == 1) {
                aq.addRobot(nextRobotId++);
                cout << "Robot " << (nextRobotId - 1) << " added.\n";
            } else if (ch == 3) {
                aq.displayAllRobots();
            } else {
                cout << "Invalid choice.\n";
            }
        }
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 6: Task 3 — Navigation ====
        cout << "\n--- ROBOT NAVIGATION & PATH TRACKING ---\n";
        if (!location.empty()) {
            int tx = locationToGridX(location);
            int ty = locationToGridY(location);
            cout << "\nTask: Collect " << current.itemId << " from " << location << "\n";
            cout << "Destination coordinates: (" << tx << ", " << ty << ")\n";
            RobotNavigator robot(0, 0);
            robot.moveToLocation(tx, ty);
            robot.returnToStart();
            robot.displayStatus();
        }
        cout << "\nPress Enter to continue..."; cin.get();

        // ==== STEP 7: Task 1 — Complete Order ====
        cout << "\n--- WAREHOUSE ORDER MANAGEMENT ---\n";
        while (true) {
            cout << "\n1. Receive Next Order From Dataset\n";
            cout << "2. Send Next Pending Order For Processing\n";
            cout << "3. Mark Current Order As Completed\n";
            cout << "4. Display Pending Orders\n";
            cout << "5. Display Current Order\n";
            cout << "6. Display Completed Orders\n";
            cout << "7. Display Summary\n";
            cout << "Enter choice: ";
            int ch; cin >> ch; cin.ignore(1000, '\n');
            if (ch == 3) {
                if (om.completeCurrentOrder())
                    cout << ">> Order " << current.orderId << " marked as Completed!\n";
                break;
            } else if (ch == 1) {
                if (om.receiveNextOrder()) cout << ">> Next order received.\n";
                else cout << ">> Could not receive order.\n";
            } else if (ch == 2) {
                Order temp;
                if (om.processNextOrder(temp))
                    cout << ">> Order " << temp.orderId << " is now being processed.\n";
            } else if (ch == 4) om.displayPendingOrders();
            else if (ch == 5) om.displayCurrentOrder();
            else if (ch == 6) om.displayCompletedOrders();
            else if (ch == 7) om.displaySummary();
            else cout << "Invalid option for this step.\n";
        }
    }

    cout << "\n========== ALL ORDERS COMPLETE ==========\n";
    om.displaySummary();
    cout << "========================================\n";
    cout << "\nPress Enter to return to main menu...";
    cin.get();
}

int main() {
    int choice;
    do {
        cout << "\n============================================\n";
        cout << "   WAREHOUSE ROBOT NAVIGATION SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Start\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) runGuidedWorkflow();
        else if (choice == 0) cout << "System exited.\n";
        else cout << "Invalid choice.\n";
    } while (choice != 0);
    return 0;
}
