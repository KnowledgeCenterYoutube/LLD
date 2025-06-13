#include <iostream>
#include <cassert>
#include <thread>
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

// Test cases
void testBasicParking() {
    cout << "Running basic parking tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    
    // Test parking a car
    Car car("ABC123");
    assertTrue(parkingLot.parkVehicle(&car), "Should be able to park a car");
    assertEqual(9, parkingLot.getAvailableSpots(), "Should have 9 spots available after parking a car");
    
    // Test parking a motorcycle
    Motorcycle bike("XYZ789");
    assertTrue(parkingLot.parkVehicle(&bike), "Should be able to park a motorcycle");
    assertEqual(8, parkingLot.getAvailableSpots(), "Should have 8 spots available after parking a motorcycle");
    
    // Test parking a bus
    Bus bus("DEF456");
    assertTrue(parkingLot.parkVehicle(&bus), "Should be able to park a bus");
    assertEqual(3, parkingLot.getAvailableSpots(), "Should have 3 spots available after parking a bus");
    
    cout << "Basic parking tests passed!" << endl;
}

void testUnparking() {
    cout << "Running unparking tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    
    // Park a car
    Car car("ABC123");
    parkingLot.parkVehicle(&car);
    
    // Test unparking
    assertTrue(parkingLot.unparkVehicle("ABC123"), "Should be able to unpark a car");
    assertEqual(10, parkingLot.getAvailableSpots(), "Should have all spots available after unparking");
    
    // Test unparking non-existent vehicle
    assertFalse(parkingLot.unparkVehicle("XYZ789"), "Should not be able to unpark non-existent vehicle");
    
    cout << "Unparking tests passed!" << endl;
}

void testConcurrentParking() {
    cout << "Running concurrent parking tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    vector<thread> threads;
    vector<Car> cars;
    
    // Create 10 cars
    for (int i = 0; i < 10; i++) {
        cars.emplace_back("CAR" + to_string(i));
    }
    
    // Try to park all cars concurrently
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&parkingLot, &cars, i]() {
            parkingLot.parkVehicle(&cars[i]);
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Verify results
    assertEqual(0, parkingLot.getAvailableSpots(), "All spots should be occupied");
    
    cout << "Concurrent parking tests passed!" << endl;
}

void testBusParking() {
    cout << "Running bus parking tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    
    // Park some cars to fragment the spots
    Car car1("CAR1");
    Car car2("CAR2");
    parkingLot.parkVehicle(&car1);
    parkingLot.parkVehicle(&car2);
    
    // Try to park a bus
    Bus bus("BUS1");
    assertTrue(parkingLot.parkVehicle(&bus), "Should be able to park a bus");
    assertEqual(3, parkingLot.getAvailableSpots(), "Should have 3 spots available after parking a bus");
    
    cout << "Bus parking tests passed!" << endl;
}

void testVehicleTracking() {
    cout << "Running vehicle tracking tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    
    // Park a car
    Car car("ABC123");
    parkingLot.parkVehicle(&car);
    
    // Test finding the vehicle
    auto spot = parkingLot.findVehicle("ABC123");
    assertTrue(spot != nullptr, "Should be able to find parked vehicle");
    assertTrue(spot->getParkedVehicle()->getLicensePlate() == "ABC123", 
              "Found vehicle should have correct license plate");
    
    // Test finding non-existent vehicle
    assertTrue(parkingLot.findVehicle("XYZ789") == nullptr, 
              "Should not find non-existent vehicle");
    
    cout << "Vehicle tracking tests passed!" << endl;
}

void testEdgeCases() {
    cout << "Running edge case tests..." << endl;
    
    ParkingLot parkingLot(1, 1, 10);
    
    // Test parking when lot is full
    vector<Car> cars;
    for (int i = 0; i < 10; i++) {
        cars.emplace_back("CAR" + to_string(i));
        parkingLot.parkVehicle(&cars[i]);
    }
    
    Car extraCar("EXTRA");
    assertFalse(parkingLot.parkVehicle(&extraCar), 
               "Should not be able to park when lot is full");
    
    // Test unparking already unparked vehicle
    assertFalse(parkingLot.unparkVehicle("EXTRA"), 
               "Should not be able to unpark non-existent vehicle");
    
    cout << "Edge case tests passed!" << endl;
}

int main() {
    try {
        testBasicParking();
        testUnparking();
        testConcurrentParking();
        testBusParking();
        testVehicleTracking();
        testEdgeCases();
        
        cout << "All tests passed!" << endl;
        return 0;
    } catch (const exception& e) {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
} 