#include <iostream>
#include <string>
#include "OrderManagement.h"
#include "OrderManagement.cpp"
#include "Robot_CIRCUAL_QUEUE.cpp"
#include "item_manager.h"
#include "item_search_management.cpp"
#include "task5.cpp"
#include "task 3.cpp"
using namespace std;

struct WorkflowState {
    OrderManagement om;
    AssignmentQueue aq;
    ItemManager im;
    WarehouseLayout wl;

    string currentOrderId;
    string currentItemId;
    int currentItemNum;
    string currentLocation;

    int taskIdCounter;
    int nextRobotId;

    WorkflowState() : om(3), taskIdCounter(0), nextRobotId(106) {
        aq.firstBot = nullptr;
        aq.currentToBeAssigned = nullptr;
        for (int i = 1; i <= 5; i++) aq.addRobot(100 + i);
        im.addItem(205, "Wireless Mouse", "ZA-A1-S1");
        im.addItem(110, "Mechanical Keyboard", "ZA-A2-S1");
        im.addItem(330, "27-inch Monitor", "ZB-B1-S1");
        im.addItem(450, "Desk Lamp", "ZB-B2-S1");
        im.addItem(120, "USB-C Hub", "ZC-C1-S1");
    }
};

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

bool proceed(const string& msg) {
    cout << "\n" << msg << " (y/n): ";
    string ans; getline(cin, ans);
    return ans == "y" || ans == "Y";
}

void orderManagement(WorkflowState& state);
void robotAssignment(WorkflowState& state);
void itemSearch(WorkflowState& state, bool backendMode);
void warehouseLayout(WorkflowState& state, bool backendMode);
void robotNavigation(WorkflowState& state, bool backendMode = false);

void orderManagement(WorkflowState& state) {
    int choice;
    Order nextOrder;

    do {
        cout << "\n--- WAREHOUSE ORDER MANAGEMENT ---\n";
        cout << "1. Receive Next Order From Dataset\n";
        cout << "2. Send Next Pending Order For Processing\n";
        cout << "3. Mark Current Order As Completed\n";
        cout << "4. Display Pending Orders\n";
        cout << "5. Display Current Order\n";
        cout << "6. Display Completed Orders\n";
        cout << "7. Display Summary\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; }
        else cin.ignore(1000, '\n');

        if (choice == 1) {
            if (state.om.receiveNextOrder())
                cout << ">> Next order received and added to pending queue.\n";
            else
                cout << ">> Could not receive order.\n";
        }
        else if (choice == 2) {
            if (state.om.processNextOrder(nextOrder)) {
                state.currentOrderId = nextOrder.orderId;
                state.currentItemId = nextOrder.itemId;
                state.currentItemNum = stoi(nextOrder.itemId.substr(1));
                state.currentLocation.clear();
                cout << ">> Order " << nextOrder.orderId << " is now being processed.\n"
                     << "   Customer: " << nextOrder.customerId << "\n"
                     << "   Item: " << nextOrder.itemId << " (Qty: " << nextOrder.quantity << ")\n";
                if (proceed("Proceed to Robot Assignment?")) {
                    robotAssignment(state);
                    return;
                }
            } else {
                cout << ">> No pending order to process.\n";
            }
        }
        else if (choice == 3) {
            if (state.om.completeCurrentOrder())
                cout << ">> Current order marked as completed.\n";
            else
                cout << ">> No current order to complete.\n";
        }
        else if (choice == 4) state.om.displayPendingOrders();
        else if (choice == 5) state.om.displayCurrentOrder();
        else if (choice == 6) state.om.displayCompletedOrders();
        else if (choice == 7) state.om.displaySummary();
        else if (choice != 0) cout << "Invalid choice.\n";

    } while (choice != 0);
}

void robotAssignment(WorkflowState& state) {
    if (state.currentItemId.empty()) {
        cout << "\nNo order has been processed yet.\n";
        cout << "Please use Order Management (option 1) to receive and process an order first.\n";
        cout << "Press Enter to return to main menu...";
        cin.get();
        return;
    }

    int choice;
    do {
        cout << "\n--- ROBOT ASSIGNMENT MODULE ---\n";
        cout << "\nCurrent Robots:\n";
        state.aq.displayAllRobots();
        cout << "\n1. Add Robot\n";
        cout << "2. Assign Task to Next Available Robot\n";
        cout << "3. Display All Robots\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; }
        else cin.ignore(1000, '\n');

        if (choice == 2) {
            Task* task = new Task;
            task->id = ++state.taskIdCounter;
            task->taskDescription = "Collect " + state.currentItemId;
            task->next = nullptr;
            state.aq.assignTask(task);
            cout << ">> Task assigned: " << task->taskDescription << "\n";

            cout << "\n--- ITEM SEARCH (Auto) ---\n";
            Item found;
            if (state.im.searchItemById(state.currentItemNum, found)) {
                state.currentLocation = found.location;
                cout << "Item ID: " << found.itemId << "\n";
                cout << "Item Name: " << found.itemName << "\n";
                cout << "Location: " << state.currentLocation << "\n";

                cout << "\n--- ROUTE PLANNING (Auto) ---\n";
                cout << "Item is at " << state.currentLocation << ".\n";
                cout << "Planning route from WH to " << state.currentLocation << "...\n";
                state.wl.findPath("WH", state.currentLocation);

                if (proceed("Proceed to Robot Navigation?")) {
                    robotNavigation(state, true);
                    return;
                }
            } else {
                cout << "Item ID " << state.currentItemNum << " not found in catalog.\n";
            }
        }
        else if (choice == 1) {
            state.aq.addRobot(state.nextRobotId++);
            cout << ">> Robot " << (state.nextRobotId - 1) << " added.\n";
        }
        else if (choice == 3) {
            state.aq.displayAllRobots();
        }
        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void itemSearch(WorkflowState& state, bool backendMode) {
    if (state.currentItemNum == 0 && backendMode) {
        cout << "\nNo item to search for.\n";
        cout << "Please process an order and assign a robot first.\n";
        return;
    }

    if (backendMode) {
        cout << "\n--- ITEM SEARCH (Auto) ---\n";
        Item found;
        if (state.im.searchItemById(state.currentItemNum, found)) {
            state.currentLocation = found.location;
            cout << "Item ID: " << found.itemId << "\n";
            cout << "Item Name: " << found.itemName << "\n";
            cout << "Location: " << state.currentLocation << "\n";
        } else {
            cout << "Item ID " << state.currentItemNum << " not found in catalog.\n";
        }
        return;
    }

    int choice;
    do {
        cout << "\n--- ITEM SEARCH & MANAGEMENT ---\n";
        cout << "1. Add Item\n";
        cout << "2. Search Item by ID\n";
        cout << "3. Search Item by Name\n";
        cout << "4. Update Item\n";
        cout << "5. Delete Item\n";
        cout << "6. Display All Items\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; }
        else cin.ignore(1000, '\n');

        if (choice == 1) {
            int id;
            cout << "Enter item ID: "; cin >> id; cin.ignore(1000, '\n');
            string name, loc;
            cout << "Enter item name: "; getline(cin, name);
            cout << "Enter item location: "; getline(cin, loc);
            if (state.im.addItem(id, name, loc))
                cout << "Item added.\n";
            else
                cout << "Item ID already exists.\n";
        }
        else if (choice == 2) {
            int id;
            cout << "Enter item ID to search: "; cin >> id; cin.ignore(1000, '\n');
            Item found;
            if (state.im.searchItemById(id, found)) {
                cout << "Item ID: " << found.itemId << "\n";
                cout << "Item Name: " << found.itemName << "\n";
                cout << "Location: " << found.location << "\n";
            } else {
                cout << "Item not found.\n";
            }
        }
        else if (choice == 3) {
            cout << "Enter item name: ";
            string name; getline(cin, name);
            if (!state.im.searchItemByName(name))
                cout << "No items found.\n";
        }
        else if (choice == 4) {
            int id;
            cout << "Enter item ID to update: "; cin >> id; cin.ignore(1000, '\n');
            string nn, nl;
            cout << "Enter new name: "; getline(cin, nn);
            cout << "Enter new location: "; getline(cin, nl);
            if (state.im.updateItem(id, nn, nl))
                cout << "Item updated.\n";
            else
                cout << "Item not found.\n";
        }
        else if (choice == 5) {
            int id;
            cout << "Enter item ID to delete: "; cin >> id; cin.ignore(1000, '\n');
            if (state.im.deleteItem(id))
                cout << "Item deleted.\n";
            else
                cout << "Item not found.\n";
        }
        else if (choice == 6) {
            state.im.displayAllItems();
        }
        else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void warehouseLayout(WorkflowState& state, bool backendMode) {
    if (backendMode) {
        if (state.currentLocation.empty()) {
            cout << "\nNo item location known yet. Cannot plan route.\n";
            return;
        }
        cout << "\n--- ROUTE PLANNING (Auto) ---\n";
        cout << "Item is at " << state.currentLocation << ".\n";
        cout << "Planning route from WH to " << state.currentLocation << "...\n";
        state.wl.findPath("WH", state.currentLocation);
        return;
    }

    int choice;
    do {
        cout << "\n--- WAREHOUSE LAYOUT & NAVIGATION ---\n";
        cout << "1. Display Warehouse Tree Structure\n";
        cout << "2. Display BFS Level-Order Traversal\n";
        cout << "3. Find Route Between Two Locations\n";
        cout << "4. Search Location by Code\n";
        cout << "5. Display All Shelves\n";
        cout << "6. Display All Location Codes\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); choice = -1; }
        else cin.ignore(1000, '\n');

        if (choice == 1) state.wl.displayTreeStructure();
        else if (choice == 2) state.wl.displayBFSTraversal();
        else if (choice == 3) {
            state.wl.showAvailableLocations();
            string from, to;
            cout << "Enter source location code: "; getline(cin, from);
            cout << "Enter destination location code: "; getline(cin, to);
            state.wl.findPath(from, to);
        }
        else if (choice == 4) {
            cout << "Enter location code to search: ";
            string code; getline(cin, code);
            state.wl.searchLocation(code);
        }
        else if (choice == 5) state.wl.displayAllShelves();
        else if (choice == 6) state.wl.showAvailableLocations();
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0);
}

void robotNavigation(WorkflowState& state, bool backendMode) {
    if (!backendMode && state.currentLocation.empty()) {
        cout << "\nNo destination known.\n";
        cout << "Please process an order through the workflow first.\n";
        cout << "Press Enter to return to main menu...";
        cin.get();
        return;
    }

    int tx = locationToGridX(state.currentLocation);
    int ty = locationToGridY(state.currentLocation);

    cout << "\n--- ROBOT NAVIGATION & PATH TRACKING ---\n";
    cout << "Task: Collect " << state.currentItemId
         << " from " << state.currentLocation << "\n";
    cout << "Destination coordinates: (" << tx << ", " << ty << ")\n";

    RobotNavigator robot(0, 0);
    robot.moveToLocation(tx, ty);
    robot.returnToStart();
    robot.displayStatus();
    robot.displayNavigationLog();

    if (proceed("Return to Order Management to complete the order?"))
        orderManagement(state);
}

int main() {
    WorkflowState state;
    int choice;

    do {
        cout << "\n============================================\n";
        cout << "   WAREHOUSE ROBOT NAVIGATION SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Order Management\n";
        cout << "2. Robot Assignment\n";
        cout << "3. Robot Navigation & Path Tracking\n";
        cout << "4. Item Search & Management\n";
        cout << "5. Warehouse Layout & Navigation\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) orderManagement(state);
        else if (choice == 2) robotAssignment(state);
        else if (choice == 3) robotNavigation(state, false);
        else if (choice == 4) itemSearch(state, false);
        else if (choice == 5) warehouseLayout(state, false);
        else if (choice == 0) cout << "System exited.\n";
        else cout << "Invalid choice.\n";
    } while (choice != 0);

    return 0;
}
