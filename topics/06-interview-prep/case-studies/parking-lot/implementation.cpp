#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <stdexcept>

using namespace std;

// Enums
enum class VehicleType {
    MOTORCYCLE,
    CAR,
    BUS
};

// Forward declarations
class Vehicle;
class ParkingSpot;
class Level;
class ParkingLot;

// Vehicle class implementation
class Vehicle {
protected:
    string licensePlate;
    VehicleType type;
    int spotsNeeded;

public:
    Vehicle(string licensePlate, VehicleType type, int spotsNeeded)
        : licensePlate(licensePlate), type(type), spotsNeeded(spotsNeeded) {}
    
    virtual ~Vehicle() = default;
    
    string getLicensePlate() const { return licensePlate; }
    VehicleType getType() const { return type; }
    int getSpotsNeeded() const { return spotsNeeded; }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(string licensePlate) 
        : Vehicle(licensePlate, VehicleType::MOTORCYCLE, 1) {}
};

class Car : public Vehicle {
public:
    Car(string licensePlate) 
        : Vehicle(licensePlate, VehicleType::CAR, 1) {}
};

class Bus : public Vehicle {
public:
    Bus(string licensePlate) 
        : Vehicle(licensePlate, VehicleType::BUS, 5) {}
};

// ParkingSpot class implementation
class ParkingSpot {
protected:
    int level;
    int row;
    int spotNumber;
    VehicleType spotType;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(int level, int row, int spotNumber, VehicleType spotType)
        : level(level), row(row), spotNumber(spotNumber), spotType(spotType),
          parkedVehicle(nullptr) {}
    
    virtual ~ParkingSpot() = default;
    
    bool isAvailable() const { return parkedVehicle == nullptr; }
    
    bool canFitVehicle(Vehicle* vehicle) const {
        if (!isAvailable()) return false;
        
        // Check if the spot type is appropriate for the vehicle
        if (vehicle->getType() == VehicleType::MOTORCYCLE) return true;
        if (vehicle->getType() == VehicleType::CAR) 
            return spotType == VehicleType::CAR || spotType == VehicleType::BUS;
        if (vehicle->getType() == VehicleType::BUS) 
            return spotType == VehicleType::BUS;
            
        return false;
    }
    
    bool park(Vehicle* vehicle) {
        if (!canFitVehicle(vehicle)) return false;
        parkedVehicle = vehicle;
        return true;
    }
    
    bool unpark() {
        if (!isAvailable()) {
            parkedVehicle = nullptr;
            return true;
        }
        return false;
    }
    
    int getLevel() const { return level; }
    int getRow() const { return row; }
    int getSpotNumber() const { return spotNumber; }
    VehicleType getSpotType() const { return spotType; }
    Vehicle* getParkedVehicle() const { return parkedVehicle; }
};

class MotorcycleSpot : public ParkingSpot {
public:
    MotorcycleSpot(int level, int row, int spotNumber)
        : ParkingSpot(level, row, spotNumber, VehicleType::MOTORCYCLE) {}
};

class CompactSpot : public ParkingSpot {
public:
    CompactSpot(int level, int row, int spotNumber)
        : ParkingSpot(level, row, spotNumber, VehicleType::CAR) {}
};

class LargeSpot : public ParkingSpot {
public:
    LargeSpot(int level, int row, int spotNumber)
        : ParkingSpot(level, row, spotNumber, VehicleType::BUS) {}
};

// Level class implementation
class Level {
private:
    int levelNumber;
    vector<unique_ptr<ParkingSpot>> spots;
    mutex levelMutex;

public:
    Level(int levelNumber, int rows, int spotsPerRow) : levelNumber(levelNumber) {
        // Create spots for each row
        for (int row = 0; row < rows; row++) {
            // First spot in each row is for motorcycles
            spots.push_back(make_unique<MotorcycleSpot>(levelNumber, row, 0));
            
            // Next 2 spots are compact spots
            for (int spot = 1; spot <= 2; spot++) {
                spots.push_back(make_unique<CompactSpot>(levelNumber, row, spot));
            }
            
            // Remaining spots are large spots
            for (int spot = 3; spot < spotsPerRow; spot++) {
                spots.push_back(make_unique<LargeSpot>(levelNumber, row, spot));
            }
        }
    }
    
    bool parkVehicle(Vehicle* vehicle) {
        lock_guard<mutex> lock(levelMutex);
        
        // For buses, we need to find 5 consecutive large spots
        if (vehicle->getType() == VehicleType::BUS) {
            return parkBus(vehicle);
        }
        
        // For other vehicles, find the first available spot
        for (auto& spot : spots) {
            if (spot->canFitVehicle(vehicle)) {
                return spot->park(vehicle);
            }
        }
        
        return false;
    }
    
    bool unparkVehicle(string licensePlate) {
        lock_guard<mutex> lock(levelMutex);
        
        for (auto& spot : spots) {
            if (!spot->isAvailable() && 
                spot->getParkedVehicle()->getLicensePlate() == licensePlate) {
                return spot->unpark();
            }
        }
        
        return false;
    }
    
    int getAvailableSpots() const {
        int count = 0;
        for (const auto& spot : spots) {
            if (spot->isAvailable()) count++;
        }
        return count;
    }
    
    int getTotalSpots() const { return spots.size(); }
    
private:
    bool parkBus(Vehicle* bus) {
        // Find 5 consecutive large spots
        for (size_t i = 0; i <= spots.size() - 5; i++) {
            bool canPark = true;
            for (size_t j = 0; j < 5; j++) {
                if (!spots[i + j]->canFitVehicle(bus)) {
                    canPark = false;
                    break;
                }
            }
            
            if (canPark) {
                // Park the bus in all 5 spots
                for (size_t j = 0; j < 5; j++) {
                    spots[i + j]->park(bus);
                }
                return true;
            }
        }
        
        return false;
    }
};

// ParkingLot class implementation
class ParkingLot {
private:
    vector<unique_ptr<Level>> levels;
    unordered_map<string, ParkingSpot*> vehicleLocation;
    mutex parkingLotMutex;

public:
    ParkingLot(int numLevels, int rowsPerLevel, int spotsPerRow) {
        for (int i = 0; i < numLevels; i++) {
            levels.push_back(make_unique<Level>(i, rowsPerLevel, spotsPerRow));
        }
    }
    
    bool parkVehicle(Vehicle* vehicle) {
        lock_guard<mutex> lock(parkingLotMutex);
        
        // Try to park in each level
        for (auto& level : levels) {
            if (level->parkVehicle(vehicle)) {
                // Find the spot where the vehicle was parked
                for (int i = 0; i < level->getTotalSpots(); i++) {
                    auto spot = level->getSpot(i);
                    if (!spot->isAvailable() && 
                        spot->getParkedVehicle()->getLicensePlate() == vehicle->getLicensePlate()) {
                        vehicleLocation[vehicle->getLicensePlate()] = spot;
                        break;
                    }
                }
                return true;
            }
        }
        
        return false;
    }
    
    bool unparkVehicle(string licensePlate) {
        lock_guard<mutex> lock(parkingLotMutex);
        
        auto it = vehicleLocation.find(licensePlate);
        if (it == vehicleLocation.end()) return false;
        
        ParkingSpot* spot = it->second;
        bool success = spot->unpark();
        if (success) {
            vehicleLocation.erase(it);
        }
        return success;
    }
    
    int getAvailableSpots() const {
        int count = 0;
        for (const auto& level : levels) {
            count += level->getAvailableSpots();
        }
        return count;
    }
    
    int getTotalSpots() const {
        int count = 0;
        for (const auto& level : levels) {
            count += level->getTotalSpots();
        }
        return count;
    }
    
    ParkingSpot* findVehicle(string licensePlate) {
        auto it = vehicleLocation.find(licensePlate);
        return it != vehicleLocation.end() ? it->second : nullptr;
    }
};

// Example usage
int main() {
    // Create a parking lot with 3 levels, 5 rows per level, and 10 spots per row
    ParkingLot parkingLot(3, 5, 10);
    
    // Create some vehicles
    Car car1("ABC123");
    Car car2("DEF456");
    Bus bus1("GHI789");
    Motorcycle bike1("JKL012");
    
    // Park vehicles
    cout << "Parking car1: " << (parkingLot.parkVehicle(&car1) ? "Success" : "Failed") << endl;
    cout << "Parking car2: " << (parkingLot.parkVehicle(&car2) ? "Success" : "Failed") << endl;
    cout << "Parking bus1: " << (parkingLot.parkVehicle(&bus1) ? "Success" : "Failed") << endl;
    cout << "Parking bike1: " << (parkingLot.parkVehicle(&bike1) ? "Success" : "Failed") << endl;
    
    // Get available spots
    cout << "Available spots: " << parkingLot.getAvailableSpots() << endl;
    
    // Unpark a vehicle
    cout << "Unparking car1: " << (parkingLot.unparkVehicle("ABC123") ? "Success" : "Failed") << endl;
    
    // Get available spots again
    cout << "Available spots: " << parkingLot.getAvailableSpots() << endl;
    
    return 0;
} 