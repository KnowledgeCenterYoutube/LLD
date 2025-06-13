# Parking Lot System - Problem Statement

## Overview
Design a parking lot system that can manage different types of vehicles and parking spots. The system should handle parking and unparking operations efficiently while maintaining proper organization and tracking of vehicles.

## Requirements

### Functional Requirements
1. The parking lot should have multiple levels
2. Each level should have multiple rows of spots
3. The system should support different types of vehicles:
   - Motorcycle
   - Car
   - Bus
4. Different types of parking spots:
   - Motorcycle spots
   - Compact spots (can fit cars)
   - Large spots (can fit buses)
5. The system should:
   - Park a vehicle in the first available spot
   - Unpark a vehicle
   - Find a vehicle by its license plate
   - Get the number of available spots
   - Get the number of total spots

### Non-functional Requirements
1. The system should be thread-safe
2. The system should handle concurrent parking and unparking operations
3. The system should be scalable to add new types of vehicles and spots
4. The system should be maintainable and extensible
5. The system should have proper error handling

## Constraints
1. A motorcycle can park in any spot
2. A car can park in compact or large spots
3. A bus can only park in large spots
4. A bus requires 5 consecutive large spots
5. Each level has a limited number of spots

## Example Usage
```cpp
// Create a parking lot with 3 levels
ParkingLot parkingLot(3);

// Park a car
Car car("ABC123");
bool parked = parkingLot.parkVehicle(car);

// Park a bus
Bus bus("XYZ789");
bool busParked = parkingLot.parkVehicle(bus);

// Unpark a vehicle
bool unparked = parkingLot.unparkVehicle("ABC123");

// Get available spots
int availableSpots = parkingLot.getAvailableSpots();
```

## Design Considerations
1. How will you handle different types of vehicles and spots?
2. How will you ensure thread safety?
3. How will you handle the case when a bus needs 5 consecutive spots?
4. How will you track parked vehicles?
5. How will you handle edge cases (e.g., parking lot full, invalid vehicle type)?

## Evaluation Criteria
1. Object-oriented design
2. Code organization and readability
3. Error handling
4. Thread safety
5. Extensibility
6. Test coverage
7. Documentation
8. Performance considerations 