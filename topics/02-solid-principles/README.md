# SOLID Principles

## Overview
SOLID is an acronym for five design principles in object-oriented programming that help make software designs more understandable, flexible, and maintainable. This guide covers each principle in detail with practical examples.

## Principles

### 1. Single Responsibility Principle (SRP)
- A class should have only one reason to change
- Each class should have a single responsibility
- Separation of concerns
- High cohesion
- Low coupling

```java
// Bad: Multiple responsibilities
class UserManager {
    public void createUser() { /* ... */ }
    public void deleteUser() { /* ... */ }
    public void sendEmail() { /* ... */ }
    public void generateReport() { /* ... */ }
}

// Good: Single responsibility
class UserManager {
    public void createUser() { /* ... */ }
    public void deleteUser() { /* ... */ }
}

class EmailService {
    public void sendEmail() { /* ... */ }
}

class ReportGenerator {
    public void generateReport() { /* ... */ }
}
```

### 2. Open/Closed Principle (OCP)
- Software entities should be open for extension but closed for modification
- Use abstraction and polymorphism
- Design for extension
- Avoid modifying existing code
- Use inheritance and interfaces

```java
// Bad: Not open for extension
class Rectangle {
    public double width;
    public double height;
}

class AreaCalculator {
    public double calculateArea(Rectangle rectangle) {
        return rectangle.width * rectangle.height;
    }
}

// Good: Open for extension
interface Shape {
    double calculateArea();
}

class Rectangle implements Shape {
    private double width;
    private double height;
    
    @Override
    public double calculateArea() {
        return width * height;
    }
}

class Circle implements Shape {
    private double radius;
    
    @Override
    public double calculateArea() {
        return Math.PI * radius * radius;
    }
}
```

### 3. Liskov Substitution Principle (LSP)
- Subtypes must be substitutable for their base types
- Maintain behavioral compatibility
- Preserve invariants
- Respect contracts
- Follow the "is-a" relationship

```java
// Bad: Violates LSP
class Bird {
    public void fly() { /* ... */ }
}

class Penguin extends Bird {
    @Override
    public void fly() {
        throw new UnsupportedOperationException();
    }
}

// Good: Follows LSP
interface FlyingBird {
    void fly();
}

class Sparrow implements FlyingBird {
    @Override
    public void fly() { /* ... */ }
}

class Penguin implements Bird {
    // No fly method
}
```

### 4. Interface Segregation Principle (ISP)
- Clients should not be forced to depend on interfaces they do not use
- Keep interfaces small and focused
- Avoid fat interfaces
- Split large interfaces
- Use role interfaces

```java
// Bad: Fat interface
interface Worker {
    void work();
    void eat();
    void sleep();
}

// Good: Segregated interfaces
interface Workable {
    void work();
}

interface Eatable {
    void eat();
}

interface Sleepable {
    void sleep();
}
```

### 5. Dependency Inversion Principle (DIP)
- High-level modules should not depend on low-level modules
- Both should depend on abstractions
- Use dependency injection
- Program to interfaces
- Invert dependencies

```java
// Bad: High-level depends on low-level
class UserService {
    private MySQLDatabase database;
    
    public UserService() {
        this.database = new MySQLDatabase();
    }
}

// Good: Both depend on abstraction
interface Database {
    void save(User user);
    User findById(String id);
}

class UserService {
    private Database database;
    
    public UserService(Database database) {
        this.database = database;
    }
}
```

## Best Practices

### 1. Applying SOLID
- Identify responsibilities
- Design for extension
- Use abstractions
- Keep interfaces focused
- Invert dependencies

### 2. Implementation
- Use design patterns
- Apply dependency injection
- Write unit tests
- Document decisions
- Review code

### 3. Maintenance
- Regular refactoring
- Code reviews
- Testing
- Documentation
- Monitoring

## Common Patterns

### 1. Dependency Injection
```java
class OrderProcessor {
    private final PaymentProcessor paymentProcessor;
    private final ShippingService shippingService;
    
    public OrderProcessor(
        PaymentProcessor paymentProcessor,
        ShippingService shippingService
    ) {
        this.paymentProcessor = paymentProcessor;
        this.shippingService = shippingService;
    }
}
```

### 2. Strategy Pattern
```java
interface SortingStrategy {
    void sort(List<Integer> list);
}

class QuickSort implements SortingStrategy {
    @Override
    public void sort(List<Integer> list) {
        // Quick sort implementation
    }
}

class MergeSort implements SortingStrategy {
    @Override
    public void sort(List<Integer> list) {
        // Merge sort implementation
    }
}
```

## Common Pitfalls

### 1. SRP Violations
- Too many responsibilities
- Mixed concerns
- Unclear boundaries
- Tight coupling
- Poor cohesion

### 2. OCP Violations
- Direct modifications
- Lack of abstraction
- Rigid design
- Poor extensibility
- Code duplication

### 3. LSP Violations
- Broken contracts
- Incompatible behavior
- Unsupported operations
- Violated invariants
- Type mismatches

### 4. ISP Violations
- Fat interfaces
- Unused methods
- Forced dependencies
- Interface pollution
- Client coupling

### 5. DIP Violations
- Direct dependencies
- Concrete implementations
- Tight coupling
- Poor testability
- Rigid design

## Tools and Resources

### 1. Development Tools
- Static analysis tools
- Code quality checkers
- Testing frameworks
- Documentation generators
- Design tools

### 2. Learning Resources
- Design patterns
- Best practices
- Code examples
- Online tutorials
- Books

## Practice Problems

### 1. Class Design
```java
// Design a payment system following SOLID principles
interface PaymentProcessor {
    void processPayment(Payment payment);
}

interface PaymentValidator {
    boolean validate(Payment payment);
}

class Payment {
    // Implement payment class
}
```

### 2. System Design
```java
// Design a notification system following SOLID principles
interface NotificationService {
    void send(Message message);
}

interface MessageFormatter {
    String format(Message message);
}

class Message {
    // Implement message class
}
```

## Interview Questions

1. What are the SOLID principles and why are they important?
2. How do you apply the Single Responsibility Principle in practice?
3. Explain the Open/Closed Principle with an example.
4. What is the Liskov Substitution Principle and how do you ensure it?
5. How do you implement the Dependency Inversion Principle?

## Summary
- Follow SOLID principles
- Design for maintainability
- Use appropriate patterns
- Avoid common pitfalls
- Regular refactoring
- Continuous improvement
- Code review
- Documentation 