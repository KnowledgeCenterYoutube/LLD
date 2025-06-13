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
enum class Coin { PENNY, NICKEL, DIME, QUARTER };
enum class Bill { ONE, FIVE, TEN, TWENTY };
enum class MachineState { IDLE, SELECTING, PAYING, DISPENSING, MAINTENANCE };

// Forward declarations
class Product;
class Payment;
class Inventory;
class Display;

// Product class implementation
class Product {
private:
    string id;
    string name;
    double price;
    string category;
    int quantity;
    chrono::system_clock::time_point expirationDate;
    mutex productMutex;

public:
    Product(const string& name, double price, const string& category)
        : name(name), price(price), category(category), quantity(0) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        id = "PROD" + to_string(dis(gen));
        expirationDate = system_clock::now() + days(30);
    }
    
    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    int getQuantity() const { return quantity; }
    
    void setPrice(double newPrice) {
        lock_guard<mutex> lock(productMutex);
        price = newPrice;
    }
    
    void updateQuantity(int delta) {
        lock_guard<mutex> lock(productMutex);
        quantity += delta;
    }
    
    bool isExpired() const {
        lock_guard<mutex> lock(productMutex);
        return system_clock::now() > expirationDate;
    }
};

// ProductCatalog class implementation
class ProductCatalog {
private:
    unordered_map<string, Product> products;
    unordered_multimap<string, string> productsByCategory;
    mutex catalogMutex;

public:
    void addProduct(const Product& product) {
        lock_guard<mutex> lock(catalogMutex);
        products[product.getId()] = product;
        productsByCategory.insert({product.getCategory(), product.getId()});
    }
    
    void removeProduct(const string& id) {
        lock_guard<mutex> lock(catalogMutex);
        auto it = products.find(id);
        if (it != products.end()) {
            productsByCategory.erase(it->second.getCategory());
            products.erase(it);
        }
    }
    
    Product* getProduct(const string& id) {
        lock_guard<mutex> lock(catalogMutex);
        auto it = products.find(id);
        return it != products.end() ? &it->second : nullptr;
    }
    
    vector<Product> getProductsByCategory(const string& category) {
        lock_guard<mutex> lock(catalogMutex);
        vector<Product> result;
        auto range = productsByCategory.equal_range(category);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(products[it->second]);
        }
        return result;
    }
    
    void updateProductPrice(const string& id, double newPrice) {
        lock_guard<mutex> lock(catalogMutex);
        auto it = products.find(id);
        if (it != products.end()) {
            it->second.setPrice(newPrice);
        }
    }
};

// Payment class implementation
class Payment {
private:
    string id;
    double amount;
    unordered_map<Coin, int> coins;
    unordered_map<Bill, int> bills;
    bool isCompleted;
    mutex paymentMutex;

public:
    Payment(double amount) : amount(amount), isCompleted(false) {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1000, 9999);
        id = "PAY" + to_string(dis(gen));
    }
    
    void addCoin(Coin coin, int count) {
        lock_guard<mutex> lock(paymentMutex);
        coins[coin] += count;
    }
    
    void addBill(Bill bill, int count) {
        lock_guard<mutex> lock(paymentMutex);
        bills[bill] += count;
    }
    
    double getTotalAmount() const {
        lock_guard<mutex> lock(paymentMutex);
        double total = 0.0;
        
        // Calculate coin values
        for (const auto& coin : coins) {
            switch (coin.first) {
                case Coin::PENNY: total += 0.01 * coin.second; break;
                case Coin::NICKEL: total += 0.05 * coin.second; break;
                case Coin::DIME: total += 0.10 * coin.second; break;
                case Coin::QUARTER: total += 0.25 * coin.second; break;
            }
        }
        
        // Calculate bill values
        for (const auto& bill : bills) {
            switch (bill.first) {
                case Bill::ONE: total += 1.00 * bill.second; break;
                case Bill::FIVE: total += 5.00 * bill.second; break;
                case Bill::TEN: total += 10.00 * bill.second; break;
                case Bill::TWENTY: total += 20.00 * bill.second; break;
            }
        }
        
        return total;
    }
    
    bool processPayment() {
        lock_guard<mutex> lock(paymentMutex);
        isCompleted = getTotalAmount() >= amount;
        return isCompleted;
    }
    
    unordered_map<Coin, int> calculateChange(double amount) {
        lock_guard<mutex> lock(paymentMutex);
        unordered_map<Coin, int> change;
        double remaining = getTotalAmount() - amount;
        
        // Calculate change in coins
        while (remaining >= 0.25) {
            change[Coin::QUARTER]++;
            remaining -= 0.25;
        }
        while (remaining >= 0.10) {
            change[Coin::DIME]++;
            remaining -= 0.10;
        }
        while (remaining >= 0.05) {
            change[Coin::NICKEL]++;
            remaining -= 0.05;
        }
        while (remaining >= 0.01) {
            change[Coin::PENNY]++;
            remaining -= 0.01;
        }
        
        return change;
    }
};

// Inventory class implementation
class Inventory {
private:
    unordered_map<string, int> stockLevels;
    unordered_map<string, int> lowStockThresholds;
    mutex inventoryMutex;

public:
    void updateStock(const string& productId, int quantity) {
        lock_guard<mutex> lock(inventoryMutex);
        stockLevels[productId] += quantity;
    }
    
    bool isLowStock(const string& productId) const {
        lock_guard<mutex> lock(inventoryMutex);
        auto it = stockLevels.find(productId);
        auto thresholdIt = lowStockThresholds.find(productId);
        if (it != stockLevels.end() && thresholdIt != lowStockThresholds.end()) {
            return it->second <= thresholdIt->second;
        }
        return false;
    }
    
    int getStockLevel(const string& productId) const {
        lock_guard<mutex> lock(inventoryMutex);
        auto it = stockLevels.find(productId);
        return it != stockLevels.end() ? it->second : 0;
    }
    
    void setLowStockThreshold(const string& productId, int threshold) {
        lock_guard<mutex> lock(inventoryMutex);
        lowStockThresholds[productId] = threshold;
    }
};

// Display class implementation
class Display {
private:
    vector<string> messages;
    mutex displayMutex;

public:
    void showProducts(const vector<Product>& products) {
        lock_guard<mutex> lock(displayMutex);
        cout << "Available Products:" << endl;
        for (const auto& product : products) {
            cout << product.getName() << " - $" << product.getPrice() << endl;
        }
    }
    
    void showPrice(double price) {
        lock_guard<mutex> lock(displayMutex);
        cout << "Price: $" << price << endl;
    }
    
    void showPaymentStatus(const Payment& payment) {
        lock_guard<mutex> lock(displayMutex);
        cout << "Total Amount: $" << payment.getTotalAmount() << endl;
    }
    
    void showChange(const unordered_map<Coin, int>& change) {
        lock_guard<mutex> lock(displayMutex);
        cout << "Change:" << endl;
        for (const auto& coin : change) {
            cout << coin.second << " " << static_cast<int>(coin.first) << " coins" << endl;
        }
    }
    
    void showError(const string& message) {
        lock_guard<mutex> lock(displayMutex);
        cout << "Error: " << message << endl;
    }
    
    void clear() {
        lock_guard<mutex> lock(displayMutex);
        messages.clear();
    }
};

// VendingMachine class implementation (Singleton)
class VendingMachine {
private:
    static VendingMachine* instance;
    static mutex instanceMutex;
    
    ProductCatalog catalog;
    Inventory inventory;
    Display display;
    Payment* currentPayment;
    string selectedProductId;
    MachineState state;
    mutex machineMutex;
    
    VendingMachine() : state(MachineState::IDLE), currentPayment(nullptr) {}
    
public:
    static VendingMachine* getInstance() {
        lock_guard<mutex> lock(instanceMutex);
        if (!instance) {
            instance = new VendingMachine();
        }
        return instance;
    }
    
    void addProduct(const Product& product) {
        lock_guard<mutex> lock(machineMutex);
        catalog.addProduct(product);
        inventory.updateStock(product.getId(), product.getQuantity());
        inventory.setLowStockThreshold(product.getId(), 5);
    }
    
    void selectProduct(const string& productId) {
        lock_guard<mutex> lock(machineMutex);
        if (state != MachineState::IDLE) {
            display.showError("Machine is busy");
            return;
        }
        
        Product* product = catalog.getProduct(productId);
        if (!product) {
            display.showError("Product not found");
            return;
        }
        
        if (inventory.getStockLevel(productId) <= 0) {
            display.showError("Product out of stock");
            return;
        }
        
        selectedProductId = productId;
        state = MachineState::SELECTING;
        display.showPrice(product->getPrice());
    }
    
    void processPayment(const Payment& payment) {
        lock_guard<mutex> lock(machineMutex);
        if (state != MachineState::SELECTING) {
            display.showError("Invalid state for payment");
            return;
        }
        
        Product* product = catalog.getProduct(selectedProductId);
        if (!product) {
            display.showError("Selected product not found");
            return;
        }
        
        if (payment.getTotalAmount() < product->getPrice()) {
            display.showError("Insufficient payment");
            return;
        }
        
        currentPayment = new Payment(payment);
        state = MachineState::PAYING;
        display.showPaymentStatus(*currentPayment);
        
        if (currentPayment->processPayment()) {
            state = MachineState::DISPENSING;
            dispenseProduct();
        }
    }
    
    void dispenseProduct() {
        lock_guard<mutex> lock(machineMutex);
        if (state != MachineState::DISPENSING) {
            display.showError("Invalid state for dispensing");
            return;
        }
        
        Product* product = catalog.getProduct(selectedProductId);
        if (!product) {
            display.showError("Selected product not found");
            return;
        }
        
        inventory.updateStock(selectedProductId, -1);
        cout << "Dispensing " << product->getName() << endl;
        
        if (currentPayment) {
            auto change = currentPayment->calculateChange(product->getPrice());
            display.showChange(change);
            delete currentPayment;
            currentPayment = nullptr;
        }
        
        state = MachineState::IDLE;
        selectedProductId.clear();
    }
    
    void cancelTransaction() {
        lock_guard<mutex> lock(machineMutex);
        if (currentPayment) {
            auto change = currentPayment->calculateChange(0);
            display.showChange(change);
            delete currentPayment;
            currentPayment = nullptr;
        }
        
        state = MachineState::IDLE;
        selectedProductId.clear();
        display.clear();
    }
};

// Initialize static members
VendingMachine* VendingMachine::instance = nullptr;
mutex VendingMachine::instanceMutex;

// Example usage
int main() {
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Add products
    Product chips("Lays Chips", 1.50, "Snacks");
    Product soda("Coca Cola", 1.00, "Beverages");
    machine->addProduct(chips);
    machine->addProduct(soda);
    
    // Select product
    machine->selectProduct(chips.getId());
    
    // Process payment
    Payment payment(2.00);
    payment.addCoin(Coin::QUARTER, 6);
    machine->processPayment(payment);
    
    return 0;
} 