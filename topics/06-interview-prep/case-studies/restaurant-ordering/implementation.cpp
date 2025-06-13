#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

// Enums
enum class OrderStatus { PENDING, CONFIRMED, PREPARING, READY, DELIVERED, CANCELLED };
enum class PaymentMethod { CASH, CREDIT_CARD, DEBIT_CARD, MOBILE_PAYMENT };
enum class ItemStatus { PENDING, PREPARING, READY, DELIVERED };

// Forward declarations
class MenuItem;
class Order;
class Payment;
class Table;

// MenuItem class implementation
class MenuItem {
private:
    string id;
    string name;
    double price;
    string category;
    bool available;
    map<string, string> translations;
    mutex itemMutex;

public:
    MenuItem(const string& name, double price, const string& category)
        : name(name), price(price), category(category), available(true) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        id = "ITEM" + to_string(dis(gen));
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    bool isAvailable() const { return available; }
    
    void setPrice(double newPrice) {
        lock_guard<mutex> lock(itemMutex);
        price = newPrice;
    }
    
    void setAvailability(bool available) {
        lock_guard<mutex> lock(itemMutex);
        this->available = available;
    }
    
    void addTranslation(const string& language, const string& translation) {
        lock_guard<mutex> lock(itemMutex);
        translations[language] = translation;
    }
    
    string getTranslation(const string& language) const {
        lock_guard<mutex> lock(itemMutex);
        auto it = translations.find(language);
        return it != translations.end() ? it->second : name;
    }
};

// Menu class implementation
class Menu {
private:
    unordered_map<string, MenuItem> items;
    unordered_multimap<string, string> itemsByCategory;
    mutex menuMutex;

public:
    void addItem(const MenuItem& item) {
        lock_guard<mutex> lock(menuMutex);
        items[item.getId()] = item;
        itemsByCategory.insert({item.getCategory(), item.getId()});
    }
    
    void removeItem(const string& id) {
        lock_guard<mutex> lock(menuMutex);
        auto it = items.find(id);
        if (it != items.end()) {
            itemsByCategory.erase(it->second.getCategory());
            items.erase(it);
        }
    }
    
    MenuItem* getItem(const string& id) {
        lock_guard<mutex> lock(menuMutex);
        auto it = items.find(id);
        return it != items.end() ? &it->second : nullptr;
    }
    
    vector<MenuItem> getItemsByCategory(const string& category) {
        lock_guard<mutex> lock(menuMutex);
        vector<MenuItem> result;
        auto range = itemsByCategory.equal_range(category);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(items[it->second]);
        }
        return result;
    }
    
    void updateItemPrice(const string& id, double newPrice) {
        lock_guard<mutex> lock(menuMutex);
        auto it = items.find(id);
        if (it != items.end()) {
            it->second.setPrice(newPrice);
        }
    }
};

// OrderItem class implementation
class OrderItem {
private:
    string menuItemId;
    int quantity;
    vector<string> specialInstructions;
    double price;

public:
    OrderItem(const string& menuItemId, int quantity, double price)
        : menuItemId(menuItemId), quantity(quantity), price(price) {}
    
    void addSpecialInstruction(const string& instruction) {
        specialInstructions.push_back(instruction);
    }
    
    double getSubtotal() const {
        return price * quantity;
    }
    
    string getMenuItemId() const { return menuItemId; }
    int getQuantity() const { return quantity; }
    const vector<string>& getSpecialInstructions() const { return specialInstructions; }
};

// Order class implementation
class Order {
private:
    string id;
    int tableNumber;
    vector<OrderItem> items;
    OrderStatus status;
    chrono::system_clock::time_point orderTime;
    mutex orderMutex;

public:
    Order(int tableNumber) : tableNumber(tableNumber), status(OrderStatus::PENDING) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        id = "ORDER" + to_string(dis(gen));
        orderTime = system_clock::now();
    }
    
    void addItem(const MenuItem& item, int quantity) {
        lock_guard<mutex> lock(orderMutex);
        if (items.size() >= 20) {
            throw runtime_error("Order size limit exceeded");
        }
        items.emplace_back(item.getId(), quantity, item.getPrice());
    }
    
    void removeItem(const string& menuItemId) {
        lock_guard<mutex> lock(orderMutex);
        items.erase(
            remove_if(items.begin(), items.end(),
                [&](const OrderItem& item) { return item.getMenuItemId() == menuItemId; }),
            items.end()
        );
    }
    
    void updateItemQuantity(const string& menuItemId, int quantity) {
        lock_guard<mutex> lock(orderMutex);
        for (auto& item : items) {
            if (item.getMenuItemId() == menuItemId) {
                item = OrderItem(menuItemId, quantity, item.getPrice());
                break;
            }
        }
    }
    
    void updateStatus(OrderStatus newStatus) {
        lock_guard<mutex> lock(orderMutex);
        status = newStatus;
    }
    
    double getTotal() const {
        lock_guard<mutex> lock(orderMutex);
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getSubtotal();
        }
        return total;
    }
    
    bool canAddMoreItems() const {
        lock_guard<mutex> lock(orderMutex);
        return items.size() < 20;
    }
    
    string getId() const { return id; }
    int getTableNumber() const { return tableNumber; }
    OrderStatus getStatus() const { return status; }
    const vector<OrderItem>& getItems() const { return items; }
};

// Payment class implementation
class Payment {
private:
    string id;
    double amount;
    vector<pair<PaymentMethod, double>> paymentMethods;
    bool isCompleted;
    mutex paymentMutex;

public:
    Payment(double amount) : amount(amount), isCompleted(false) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        id = "PAY" + to_string(dis(gen));
    }
    
    void addPaymentMethod(PaymentMethod method, double amount) {
        lock_guard<mutex> lock(paymentMutex);
        if (paymentMethods.size() >= 4) {
            throw runtime_error("Maximum split payment ways exceeded");
        }
        paymentMethods.emplace_back(method, amount);
    }
    
    bool processPayment() {
        lock_guard<mutex> lock(paymentMutex);
        double totalPaid = 0.0;
        for (const auto& payment : paymentMethods) {
            totalPaid += payment.second;
        }
        isCompleted = totalPaid >= amount;
        return isCompleted;
    }
    
    bool canSplitPayment() const {
        lock_guard<mutex> lock(paymentMutex);
        return paymentMethods.size() < 4;
    }
    
    double getRemainingAmount() const {
        lock_guard<mutex> lock(paymentMutex);
        double totalPaid = 0.0;
        for (const auto& payment : paymentMethods) {
            totalPaid += payment.second;
        }
        return max(0.0, amount - totalPaid);
    }
    
    void generateReceipt() {
        lock_guard<mutex> lock(paymentMutex);
        cout << "Receipt for Payment " << id << endl;
        cout << "Total Amount: $" << amount << endl;
        cout << "Payment Methods:" << endl;
        for (const auto& payment : paymentMethods) {
            cout << "- " << static_cast<int>(payment.first) << ": $" << payment.second << endl;
        }
        cout << "Status: " << (isCompleted ? "Completed" : "Pending") << endl;
    }
};

// Table class implementation
class Table {
private:
    int number;
    int capacity;
    bool isOccupied;
    chrono::system_clock::time_point reservationTime;
    mutex tableMutex;

public:
    Table(int number, int capacity)
        : number(number), capacity(capacity), isOccupied(false) {}
    
    bool isAvailable() const {
        lock_guard<mutex> lock(tableMutex);
        return !isOccupied;
    }
    
    void reserve(const chrono::system_clock::time_point& time) {
        lock_guard<mutex> lock(tableMutex);
        if (isOccupied) {
            throw runtime_error("Table is already occupied");
        }
        reservationTime = time;
        isOccupied = true;
    }
    
    void release() {
        lock_guard<mutex> lock(tableMutex);
        isOccupied = false;
    }
    
    bool canAccommodate(int people) const {
        lock_guard<mutex> lock(tableMutex);
        return people <= capacity;
    }
    
    int getNumber() const { return number; }
    int getCapacity() const { return capacity; }
};

// RestaurantSystem class implementation (Singleton)
class RestaurantSystem {
private:
    static RestaurantSystem* instance;
    static mutex instanceMutex;
    
    Menu menu;
    unordered_map<string, Order> orders;
    unordered_map<int, Table> tables;
    mutex systemMutex;
    
    RestaurantSystem() = default;
    
public:
    static RestaurantSystem* getInstance() {
        lock_guard<mutex> lock(instanceMutex);
        if (!instance) {
            instance = new RestaurantSystem();
        }
        return instance;
    }
    
    string addMenuItem(const string& name, double price, const string& category) {
        MenuItem item(name, price, category);
        menu.addItem(item);
        return item.getId();
    }
    
    string createOrder(int tableNumber) {
        lock_guard<mutex> lock(systemMutex);
        Order order(tableNumber);
        string id = order.getId();
        orders[id] = order;
        return id;
    }
    
    void addItemToOrder(const string& orderId, const string& menuItemId, int quantity) {
        lock_guard<mutex> lock(systemMutex);
        auto orderIt = orders.find(orderId);
        if (orderIt == orders.end()) {
            throw runtime_error("Order not found");
        }
        
        MenuItem* item = menu.getItem(menuItemId);
        if (!item) {
            throw runtime_error("Menu item not found");
        }
        
        orderIt->second.addItem(*item, quantity);
    }
    
    void updateOrderStatus(const string& orderId, OrderStatus status) {
        lock_guard<mutex> lock(systemMutex);
        auto it = orders.find(orderId);
        if (it != orders.end()) {
            it->second.updateStatus(status);
        }
    }
    
    bool reserveTable(int tableNumber, const chrono::system_clock::time_point& time) {
        lock_guard<mutex> lock(systemMutex);
        auto it = tables.find(tableNumber);
        if (it != tables.end() && it->second.isAvailable()) {
            it->second.reserve(time);
            return true;
        }
        return false;
    }
    
    void releaseTable(int tableNumber) {
        lock_guard<mutex> lock(systemMutex);
        auto it = tables.find(tableNumber);
        if (it != tables.end()) {
            it->second.release();
        }
    }
    
    vector<MenuItem> searchMenuItems(const string& query) {
        return menu.getItemsByCategory(query);
    }
};

// Initialize static members
RestaurantSystem* RestaurantSystem::instance = nullptr;
mutex RestaurantSystem::instanceMutex;

// Example usage
int main() {
    RestaurantSystem* restaurant = RestaurantSystem::getInstance();
    
    // Add menu items
    string pizzaId = restaurant->addMenuItem("Margherita Pizza", 12.99, "Pizza");
    string pastaId = restaurant->addMenuItem("Spaghetti Carbonara", 14.99, "Pasta");
    
    // Create an order
    string orderId = restaurant->createOrder(1);
    restaurant->addItemToOrder(orderId, pizzaId, 2);
    restaurant->addItemToOrder(orderId, pastaId, 1);
    
    // Update order status
    restaurant->updateOrderStatus(orderId, OrderStatus::PREPARING);
    
    // Reserve a table
    auto now = system_clock::now();
    restaurant->reserveTable(1, now + hours(2));
    
    // Search menu items
    vector<MenuItem> pizzaItems = restaurant->searchMenuItems("Pizza");
    cout << "Found " << pizzaItems.size() << " pizza items" << endl;
    
    return 0;
} 