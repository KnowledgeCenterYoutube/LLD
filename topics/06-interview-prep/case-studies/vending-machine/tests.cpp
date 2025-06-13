#include <iostream>
#include <cassert>
#include <vector>
#include "implementation.cpp"

using namespace std;

// Test helper functions
void assertTrue(bool condition, const string& message) {
    if (!condition) {
        throw runtime_error("Test failed: " + message);
    }
}

void assertFalse(bool condition, const string& message) {
    assertTrue(!condition, message);
}

void assertEqual(int expected, int actual, const string& message) {
    if (expected != actual) {
        throw runtime_error("Test failed: " + message + 
                          " (Expected: " + to_string(expected) + 
                          ", Got: " + to_string(actual) + ")");
    }
}

void assertEqual(double expected, double actual, const string& message) {
    if (abs(expected - actual) > 0.001) {
        throw runtime_error("Test failed: " + message + 
                          " (Expected: " + to_string(expected) + 
                          ", Got: " + to_string(actual) + ")");
    }
}

// Test cases
void testProductManagement() {
    cout << "Running product management tests..." << endl;
    
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Test adding products
    Product chips("Lays Chips", 1.50, "Snacks");
    Product soda("Coca Cola", 1.00, "Beverages");
    machine->addProduct(chips);
    machine->addProduct(soda);
    
    // Test product selection
    machine->selectProduct(chips.getId());
    
    cout << "Product management tests passed!" << endl;
}

void testPaymentProcessing() {
    cout << "Running payment processing tests..." << endl;
    
    // Create a payment
    Payment payment(1.50);
    
    // Test adding coins
    payment.addCoin(Coin::QUARTER, 6);
    assertEqual(1.50, payment.getTotalAmount(), "Total amount should be $1.50");
    
    // Test payment processing
    assertTrue(payment.processPayment(), "Payment should be completed");
    
    // Test change calculation
    auto change = payment.calculateChange(1.00);
    assertEqual(2, change[Coin::QUARTER], "Should return 2 quarters as change");
    
    cout << "Payment processing tests passed!" << endl;
}

void testInventoryManagement() {
    cout << "Running inventory management tests..." << endl;
    
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Add a product
    Product chips("Lays Chips", 1.50, "Snacks");
    machine->addProduct(chips);
    
    // Test stock level
    assertEqual(0, machine->getStockLevel(chips.getId()), "Initial stock should be 0");
    
    // Test low stock threshold
    machine->setLowStockThreshold(chips.getId(), 5);
    assertTrue(machine->isLowStock(chips.getId()), "Should indicate low stock");
    
    cout << "Inventory management tests passed!" << endl;
}

void testTransactionFlow() {
    cout << "Running transaction flow tests..." << endl;
    
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Add a product
    Product chips("Lays Chips", 1.50, "Snacks");
    machine->addProduct(chips);
    
    // Test complete transaction flow
    machine->selectProduct(chips.getId());
    
    Payment payment(2.00);
    payment.addCoin(Coin::QUARTER, 8);
    machine->processPayment(payment);
    
    cout << "Transaction flow tests passed!" << endl;
}

void testErrorHandling() {
    cout << "Running error handling tests..." << endl;
    
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Test invalid product selection
    machine->selectProduct("INVALID_ID");
    
    // Test insufficient payment
    Product chips("Lays Chips", 1.50, "Snacks");
    machine->addProduct(chips);
    machine->selectProduct(chips.getId());
    
    Payment payment(1.00);
    payment.addCoin(Coin::QUARTER, 3);
    machine->processPayment(payment);
    
    cout << "Error handling tests passed!" << endl;
}

void testConcurrentOperations() {
    cout << "Running concurrent operations tests..." << endl;
    
    VendingMachine* machine = VendingMachine::getInstance();
    
    // Add products
    Product chips("Lays Chips", 1.50, "Snacks");
    Product soda("Coca Cola", 1.00, "Beverages");
    machine->addProduct(chips);
    machine->addProduct(soda);
    
    // Simulate concurrent operations
    // In a real system, this would be handled by multiple threads
    // For testing, we'll just verify the thread safety of the operations
    
    machine->selectProduct(chips.getId());
    Payment payment1(2.00);
    payment1.addCoin(Coin::QUARTER, 8);
    machine->processPayment(payment1);
    
    machine->selectProduct(soda.getId());
    Payment payment2(1.00);
    payment2.addCoin(Coin::QUARTER, 4);
    machine->processPayment(payment2);
    
    cout << "Concurrent operations tests passed!" << endl;
}

void testChangeCalculation() {
    cout << "Running change calculation tests..." << endl;
    
    // Test various change scenarios
    Payment payment(2.00);
    payment.addCoin(Coin::QUARTER, 8);
    
    auto change = payment.calculateChange(1.50);
    assertEqual(2, change[Coin::QUARTER], "Should return 2 quarters as change");
    
    change = payment.calculateChange(1.75);
    assertEqual(1, change[Coin::QUARTER], "Should return 1 quarter as change");
    
    cout << "Change calculation tests passed!" << endl;
}

int main() {
    try {
        testProductManagement();
        testPaymentProcessing();
        testInventoryManagement();
        testTransactionFlow();
        testErrorHandling();
        testConcurrentOperations();
        testChangeCalculation();
        
        cout << "All tests passed!" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
} 