# Parking Lot System - Solution Approach

## Design Overview
The parking lot system will be designed using object-oriented principles, design patterns, and thread-safe mechanisms. The solution will be modular, extensible, and maintainable.

## Class Structure

### 1. Vehicle Hierarchy
```cpp
// Base class for all vehicles
class Vehicle {
protected:
    string licensePlate;
    VehicleType type;
    int spotsNeeded;

public:
    Vehicle(string licensePlate, VehicleType type, int spotsNeeded);
    virtual ~Vehicle() = default;
    
    string getLicensePlate() const { return licensePlate; }
    VehicleType getType() const { return type; }
    int getSpotsNeeded() const { return spotsNeeded; }
};

// Concrete vehicle classes
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
```

### 2. Parking Spot Hierarchy
```cpp
// Base class for parking spots
class ParkingSpot {
protected:
    int level;
    int row;
    int spotNumber;
    VehicleType spotType;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(int level, int row, int spotNumber, VehicleType spotType);
    virtual ~ParkingSpot() = default;
    
    bool isAvailable() const { return parkedVehicle == nullptr; }
    bool canFitVehicle(Vehicle* vehicle) const;
    bool park(Vehicle* vehicle);
    bool unpark();
    
    int getLevel() const { return level; }
    int getRow() const { return row; }
    int getSpotNumber() const { return spotNumber; }
    VehicleType getSpotType() const { return spotType; }
};

// Concrete spot classes
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
```

### 3. Level Class
```cpp
class Level {
private:
    int levelNumber;
    vector<ParkingSpot*> spots;
    mutex levelMutex;

public:
    Level(int levelNumber, int rows, int spotsPerRow);
    ~Level();
    
    bool parkVehicle(Vehicle* vehicle);
    bool unparkVehicle(string licensePlate);
    int getAvailableSpots() const;
    int getTotalSpots() const { return spots.size(); }
};
```

### 4. Parking Lot Class
```cpp
class ParkingLot {
private:
    vector<Level*> levels;
    unordered_map<string, ParkingSpot*> vehicleLocation;
    mutex parkingLotMutex;

public:
    ParkingLot(int numLevels, int rowsPerLevel, int spotsPerRow);
    ~ParkingLot();
    
    bool parkVehicle(Vehicle* vehicle);
    bool unparkVehicle(string licensePlate);
    int getAvailableSpots() const;
    int getTotalSpots() const;
    ParkingSpot* findVehicle(string licensePlate);
};
```

## Design Patterns Used

### 1. Factory Pattern
- Used for creating different types of vehicles and parking spots
- Encapsulates object creation logic
- Makes the system extensible for new vehicle/spot types

### 2. Strategy Pattern
- Used for different parking strategies
- Allows different algorithms for finding parking spots
- Makes the system flexible for different parking rules

### 3. Observer Pattern
- Used for notifying about parking spot status changes
- Helps in monitoring and logging
- Useful for UI updates

## Thread Safety

### 1. Mutex Locks
- Level class has its own mutex for spot management
- ParkingLot class has a mutex for vehicle tracking
- Prevents race conditions in concurrent operations

### 2. Atomic Operations
- Used for spot counting
- Ensures thread-safe increment/decrement
- Prevents count inconsistencies

## Error Handling

### 1. Custom Exceptions
```cpp
class ParkingLotException : public runtime_error {
public:
    explicit ParkingLotException(const string& message) 
        : runtime_error(message) {}
};

class InvalidVehicleException : public ParkingLotException {
public:
    explicit InvalidVehicleException(const string& message) 
        : ParkingLotException(message) {}
};

class ParkingLotFullException : public ParkingLotException {
public:
    explicit ParkingLotFullException(const string& message) 
        : ParkingLotException(message) {}
};
```

### 2. Error States
- Invalid vehicle type
- Parking lot full
- Vehicle not found
- Invalid spot type
- Concurrent modification

## Testing Strategy

### 1. Unit Tests
- Vehicle creation and properties
- Spot availability and parking
- Level management
- Parking lot operations

### 2. Integration Tests
- End-to-end parking flow
- Concurrent operations
- Error handling
- Edge cases

### 3. Performance Tests
- Concurrent parking/unparking
- Large number of vehicles
- Memory usage
- Response time

## Extensibility

### 1. New Vehicle Types
- Add new vehicle class
- Implement required methods
- Update factory

### 2. New Spot Types
- Add new spot class
- Implement required methods
- Update factory

### 3. New Features
- Payment system
- Reservation system
- Monitoring system
- Reporting system

## Performance Considerations

### 1. Data Structures
- Hash map for vehicle tracking
- Vector for spot management
- Efficient spot finding algorithm

### 2. Memory Management
- Smart pointers for resource management
- RAII principles
- Proper cleanup

### 3. Concurrency
- Minimal locking
- Atomic operations
- Efficient thread synchronization 