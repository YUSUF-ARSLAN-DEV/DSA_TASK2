#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <string>
using namespace std;

struct Item
{
    int itemId;
    string itemName;
    string location;
};

class ItemManager
{
private:
    struct Node{
        Item data;
        Node* left;
        Node* right;

        Node(const Item& item) : data(item), left(nullptr), right(nullptr) {}
    };

    Node* root;

    bool insert(Node*& current, const Item& item);
    Node* searchById(Node* current, int itemId) const;
    bool searchByNameAndDisplay(Node* current, const string& name) const;
    bool update(Node* current, int itemId, const string& newName, const string& newLocation);
    Node* deleteNode(Node* current, int itemId, bool& deleted);
    Node* findMin(Node* current) const;
    void displayInOrder(Node* current) const;
    void destroy(Node* current);

public:
    ItemManager();
    ~ItemManager();

    bool addItem(int itemId, const string& itemName, const string& location);
    bool searchItemById(int itemId, Item& foundItem) const;
    bool searchItemByName(const string& name) const;
    bool updateItem(int itemId, const string& newName, const string& newLocation);
    bool deleteItem(int itemId);
    void displayAllItems() const;
};

#endif