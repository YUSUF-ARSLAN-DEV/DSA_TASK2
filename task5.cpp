#include <iostream>
#include <string>
using namespace std;

struct WarehouseNode {
    string name;
    string type;
    string locationCode;
    WarehouseNode* firstChild;
    WarehouseNode* nextSibling;
    WarehouseNode* parent;

    WarehouseNode(string n, string t, string code)
        : name(n), type(t), locationCode(code),
          firstChild(nullptr), nextSibling(nullptr), parent(nullptr) {}
};

struct QueueCell {
    WarehouseNode* node;
    QueueCell* next;

    QueueCell(WarehouseNode* n) : node(n), next(nullptr) {}
};

class LayoutQueue {
private:
    QueueCell* frontCell;
    QueueCell* rearCell;

public:
    LayoutQueue() : frontCell(nullptr), rearCell(nullptr) {}

    ~LayoutQueue() {
        while (!isEmpty()) dequeue();
    }

    void enqueue(WarehouseNode* node) {
        QueueCell* cell = new QueueCell(node);
        if (rearCell == nullptr) {
            frontCell = rearCell = cell;
        } else {
            rearCell->next = cell;
            rearCell = cell;
        }
    }

    WarehouseNode* dequeue() {
        if (isEmpty()) return nullptr;
        QueueCell* temp = frontCell;
        WarehouseNode* data = temp->node;
        frontCell = frontCell->next;
        if (frontCell == nullptr) rearCell = nullptr;
        delete temp;
        return data;
    }

    bool isEmpty() const {
        return frontCell == nullptr;
    }
};

class WarehouseLayout {
private:
    WarehouseNode* root;
    int nodeCount;

    void addChild(WarehouseNode* parentNode, WarehouseNode* childNode) {
        childNode->parent = parentNode;
        if (parentNode->firstChild == nullptr) {
            parentNode->firstChild = childNode;
        } else {
            WarehouseNode* sibling = parentNode->firstChild;
            while (sibling->nextSibling != nullptr) {
                sibling = sibling->nextSibling;
            }
            sibling->nextSibling = childNode;
        }
        nodeCount++;
    }

    void initializeLayout() {
        root = new WarehouseNode("Warehouse", "Warehouse", "WH");
        nodeCount = 1;

        WarehouseNode* zoneA = new WarehouseNode("Zone A", "Zone", "ZA");
        WarehouseNode* zoneB = new WarehouseNode("Zone B", "Zone", "ZB");
        WarehouseNode* zoneC = new WarehouseNode("Zone C", "Zone", "ZC");
        addChild(root, zoneA);
        addChild(root, zoneB);
        addChild(root, zoneC);

        WarehouseNode* aisleA1 = new WarehouseNode("Aisle A1", "Aisle", "ZA-A1");
        WarehouseNode* aisleA2 = new WarehouseNode("Aisle A2", "Aisle", "ZA-A2");
        addChild(zoneA, aisleA1);
        addChild(zoneA, aisleA2);

        WarehouseNode* aisleB1 = new WarehouseNode("Aisle B1", "Aisle", "ZB-B1");
        WarehouseNode* aisleB2 = new WarehouseNode("Aisle B2", "Aisle", "ZB-B2");
        addChild(zoneB, aisleB1);
        addChild(zoneB, aisleB2);

        WarehouseNode* aisleC1 = new WarehouseNode("Aisle C1", "Aisle", "ZC-C1");
        addChild(zoneC, aisleC1);

        addChild(aisleA1, new WarehouseNode("Shelf A1-S1", "Shelf", "ZA-A1-S1"));
        addChild(aisleA1, new WarehouseNode("Shelf A1-S2", "Shelf", "ZA-A1-S2"));
        addChild(aisleA1, new WarehouseNode("Shelf A1-S3", "Shelf", "ZA-A1-S3"));

        addChild(aisleA2, new WarehouseNode("Shelf A2-S1", "Shelf", "ZA-A2-S1"));
        addChild(aisleA2, new WarehouseNode("Shelf A2-S2", "Shelf", "ZA-A2-S2"));

        addChild(aisleB1, new WarehouseNode("Shelf B1-S1", "Shelf", "ZB-B1-S1"));
        addChild(aisleB1, new WarehouseNode("Shelf B1-S2", "Shelf", "ZB-B1-S2"));

        addChild(aisleB2, new WarehouseNode("Shelf B2-S1", "Shelf", "ZB-B2-S1"));
        addChild(aisleB2, new WarehouseNode("Shelf B2-S2", "Shelf", "ZB-B2-S2"));
        addChild(aisleB2, new WarehouseNode("Shelf B2-S3", "Shelf", "ZB-B2-S3"));

        addChild(aisleC1, new WarehouseNode("Shelf C1-S1", "Shelf", "ZC-C1-S1"));
        addChild(aisleC1, new WarehouseNode("Shelf C1-S2", "Shelf", "ZC-C1-S2"));
    }

    void destroyTree(WarehouseNode* node) {
        if (node == nullptr) return;
        destroyTree(node->firstChild);
        destroyTree(node->nextSibling);
        delete node;
    }

    void printIndented(WarehouseNode* node, int depth) const {
        if (node == nullptr) return;
        for (int i = 0; i < depth; i++) cout << "    ";
        cout << "[" << node->type << "]  " << node->name
             << "  (" << node->locationCode << ")" << endl;
        printIndented(node->firstChild, depth + 1);
        printIndented(node->nextSibling, depth);
    }

    WarehouseNode* searchByCode(WarehouseNode* node, const string& code) const {
        if (node == nullptr) return nullptr;
        if (node->locationCode == code) return node;
        WarehouseNode* result = searchByCode(node->firstChild, code);
        if (result != nullptr) return result;
        return searchByCode(node->nextSibling, code);
    }

    int getNodeDepth(WarehouseNode* node) const {
        int depth = 0;
        while (node != nullptr) {
            depth++;
            node = node->parent;
        }
        return depth;
    }

    WarehouseNode* findLCA(WarehouseNode* a, WarehouseNode* b) const {
        int depthA = getNodeDepth(a);
        int depthB = getNodeDepth(b);
        while (depthA > depthB) { a = a->parent; depthA--; }
        while (depthB > depthA) { b = b->parent; depthB--; }
        while (a != b) { a = a->parent; b = b->parent; }
        return a;
    }

    void findAndDisplayPath(WarehouseNode* from, WarehouseNode* to) const {
        if (from == to) {
            cout << "\nSource and destination are the same location.\n";
            return;
        }

        WarehouseNode* lca = findLCA(from, to);

        int upSteps = getNodeDepth(from) - getNodeDepth(lca) + 1;
        int downSteps = getNodeDepth(to) - getNodeDepth(lca);

        WarehouseNode** upPath = new WarehouseNode*[upSteps];
        WarehouseNode* temp = from;
        for (int i = 0; i < upSteps; i++) {
            upPath[i] = temp;
            temp = temp->parent;
        }

        WarehouseNode** downPath = new WarehouseNode*[downSteps];
        temp = to;
        for (int i = 0; i < downSteps; i++) {
            downPath[i] = temp;
            temp = temp->parent;
        }

        cout << "\nRoute: ";
        for (int i = 0; i < upSteps; i++) {
            cout << upPath[i]->locationCode;
            if (i < upSteps - 1 || downSteps > 0) cout << " -> ";
        }
        for (int i = downSteps - 1; i >= 0; i--) {
            cout << downPath[i]->locationCode;
            if (i > 0) cout << " -> ";
        }

        cout << "\nTotal Steps: " << (upSteps - 1 + downSteps) << endl;

        delete[] upPath;
        delete[] downPath;
    }

    void collectShelves(WarehouseNode* node) const {
        if (node == nullptr) return;
        if (node->type == "Shelf") {
            cout << "  " << node->name << "  [" << node->locationCode << "]" << endl;
        }
        collectShelves(node->firstChild);
        collectShelves(node->nextSibling);
    }

    void displayAvailableLocations() const {
        cout << "\n--- Available Location Codes ---\n";
        LayoutQueue queue;
        queue.enqueue(root);
        while (!queue.isEmpty()) {
            WarehouseNode* current = queue.dequeue();
            cout << "  " << current->locationCode
                 << "  (" << current->name << ")\n";
            WarehouseNode* child = current->firstChild;
            while (child != nullptr) {
                queue.enqueue(child);
                child = child->nextSibling;
            }
        }
        cout << "--------------------------------\n";
    }

public:
    WarehouseLayout() : root(nullptr), nodeCount(0) {
        initializeLayout();
    }

    ~WarehouseLayout() {
        destroyTree(root);
    }

    void displayTreeStructure() const {
        cout << "\n========== WAREHOUSE LAYOUT (TREE VIEW) ==========\n";
        printIndented(root, 0);
        cout << "===================================================\n";
        cout << "Total Locations: " << nodeCount << endl;
    }

    void displayBFSTraversal() const {
        cout << "\n========== WAREHOUSE BFS LEVEL TRAVERSAL ==========\n";
        if (root == nullptr) {
            cout << "Warehouse layout is empty.\n";
            return;
        }

        LayoutQueue bfsQueue;
        bfsQueue.enqueue(root);
        int currentLevel = 0;
        int nodesAtCurrentLevel = 1;
        int nodesAtNextLevel = 0;

        while (!bfsQueue.isEmpty()) {
            if (nodesAtCurrentLevel == 0) {
                currentLevel++;
                nodesAtCurrentLevel = nodesAtNextLevel;
                nodesAtNextLevel = 0;
            }

            WarehouseNode* current = bfsQueue.dequeue();
            nodesAtCurrentLevel--;

            cout << "  Level " << currentLevel << "  |  "
                 << current->type << "  |  "
                 << current->name << "  ["
                 << current->locationCode << "]\n";

            WarehouseNode* child = current->firstChild;
            while (child != nullptr) {
                bfsQueue.enqueue(child);
                nodesAtNextLevel++;
                child = child->nextSibling;
            }
        }
        cout << "====================================================\n";
    }

    void findPath(const string& fromCode, const string& toCode) const {
        WarehouseNode* fromNode = searchByCode(root, fromCode);
        WarehouseNode* toNode = searchByCode(root, toCode);

        if (fromNode == nullptr) {
            cout << "\nLocation \"" << fromCode << "\" not found.\n";
            return;
        }
        if (toNode == nullptr) {
            cout << "\nLocation \"" << toCode << "\" not found.\n";
            return;
        }

        cout << "\nFrom : " << fromNode->name << "  [" << fromNode->locationCode << "]";
        cout << "\nTo   : " << toNode->name << "  [" << toNode->locationCode << "]";
        findAndDisplayPath(fromNode, toNode);
    }

    void searchLocation(const string& code) const {
        WarehouseNode* result = searchByCode(root, code);
        if (result == nullptr) {
            cout << "\nLocation \"" << code << "\" not found.\n";
            return;
        }
        cout << "\n--- Location Found ---\n";
        cout << "Name          : " << result->name << endl;
        cout << "Type          : " << result->type << endl;
        cout << "Location Code : " << result->locationCode << endl;
        if (result->parent != nullptr) {
            cout << "Parent        : " << result->parent->name
                 << "  [" << result->parent->locationCode << "]\n";
        }
        if (result->firstChild != nullptr) {
            cout << "Children      : ";
            WarehouseNode* child = result->firstChild;
            while (child != nullptr) {
                cout << child->locationCode;
                if (child->nextSibling != nullptr) cout << ", ";
                child = child->nextSibling;
            }
            cout << endl;
        }
    }

    void displayAllShelves() const {
        cout << "\n========== ALL SHELVES ==========\n";
        collectShelves(root);
        cout << "=================================\n";
    }

    void showAvailableLocations() const {
        displayAvailableLocations();
    }
};

int main() {
    WarehouseLayout warehouse;
    int choice = -1;
    string fromCode, toCode, searchCode;

    do {
        cout << "\n============================================\n";
        cout << "   WAREHOUSE LAYOUT AND NAVIGATION MODULE\n";
        cout << "============================================\n";
        cout << "1. Display Warehouse Tree Structure\n";
        cout << "2. Display BFS Level-Order Traversal\n";
        cout << "3. Find Route Between Two Locations\n";
        cout << "4. Search Location by Code\n";
        cout << "5. Display All Shelves\n";
        cout << "6. Display All Location Codes\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        } else {
            cin.ignore(1000, '\n');
        }

        if (choice == 1) {
            warehouse.displayTreeStructure();
        } else if (choice == 2) {
            warehouse.displayBFSTraversal();
        } else if (choice == 3) {
            warehouse.showAvailableLocations();
            cout << "Enter source location code: ";
            getline(cin, fromCode);
            cout << "Enter destination location code: ";
            getline(cin, toCode);
            warehouse.findPath(fromCode, toCode);
        } else if (choice == 4) {
            cout << "Enter location code to search: ";
            getline(cin, searchCode);
            warehouse.searchLocation(searchCode);
        } else if (choice == 5) {
            warehouse.displayAllShelves();
        } else if (choice == 6) {
            warehouse.showAvailableLocations();
        } else if (choice == 0) {
            cout << "Exiting warehouse navigation module.\n";
        } else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
