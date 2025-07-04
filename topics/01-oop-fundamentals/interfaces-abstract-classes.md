# Interfaces and Abstract Classes

## Introduction
Interfaces and Abstract Classes are two important mechanisms in Object-Oriented Programming that enable abstraction and polymorphism.

## Abstract Classes
An abstract class is a class that cannot be instantiated and may contain both abstract and concrete methods.

### Abstract Class Diagram
```mermaid
classDiagram
    class AbstractAnimal {
        <<abstract>>
        +string name
        +makeSound()*
        +move()*
        +displayInfo()
    }
    class Dog {
        +makeSound()
        +move()
    }
    class Cat {
        +makeSound()
        +move()
    }
    AbstractAnimal <|-- Dog
    AbstractAnimal <|-- Cat
```

### Abstract Class Example
```cpp
class AbstractAnimal {
protected:
    string name;
    
public:
    AbstractAnimal(const string& name) : name(name) {}
    
    // Pure virtual methods
    virtual void makeSound() = 0;
    virtual void move() = 0;
    
    // Concrete method
    void displayInfo() const {
        cout << "Name: " << name << endl;
    }
    
    virtual ~AbstractAnimal() = default;
};

class Dog : public AbstractAnimal {
public:
    Dog(const string& name) : AbstractAnimal(name) {}
    
    void makeSound() override {
        cout << "Woof!" << endl;
    }
    
    void move() override {
        cout << "Dog is running" << endl;
    }
};
```

## Interfaces
An interface is a contract that defines a set of methods that implementing classes must provide.

### Interface Diagram
```mermaid
classDiagram
    class PaymentProcessor {
        <<interface>>
        +processPayment(double amount)
        +refund(double amount)
        +getTransactionId()
    }
    class CreditCardProcessor {
        +processPayment(double amount)
        +refund(double amount)
        +getTransactionId()
    }
    class PayPalProcessor {
        +processPayment(double amount)
        +refund(double amount)
        +getTransactionId()
    }
    PaymentProcessor <|.. CreditCardProcessor
    PaymentProcessor <|.. PayPalProcessor
```

### Interface Example
```cpp
class PaymentProcessor {
public:
    virtual bool processPayment(double amount) = 0;
    virtual void refund(double amount) = 0;
    virtual string getTransactionId() = 0;
    virtual ~PaymentProcessor() = default;
};

class CreditCardProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        // Implementation
        return true;
    }
    
    void refund(double amount) override {
        // Implementation
    }
    
    string getTransactionId() override {
        return "CC-" + to_string(time(nullptr));
    }
};
```

## Comparison

### Abstract Class vs Interface
```mermaid
graph TD
    A[Abstract Class] --> B[Can have constructors]
    A --> C[Can have instance variables]
    A --> D[Can have concrete methods]
    A --> E[Single inheritance]
    F[Interface] --> G[No constructors]
    F --> H[Only constants]
    F --> I[All methods pure virtual]
    F --> J[Multiple inheritance]
```

## When to Use What?

### Use Abstract Classes When:
1. You want to share code among several related classes
2. You have some common implementation
3. You want to provide a base class with some default behavior
4. You need to declare non-public members

### Use Interfaces When:
1. You want to define a contract
2. You want to specify the behavior of a particular data type
3. You want to separate the definition of a service from its implementation
4. You want to support multiple inheritance

## Best Practices
1. Keep interfaces focused and cohesive
2. Use abstract classes for shared implementation
3. Follow the Interface Segregation Principle
4. Use meaningful names for interfaces and abstract classes
5. Document the purpose and usage of interfaces and abstract classes

## Common Pitfalls
1. Creating too many abstract methods
2. Mixing interface and abstract class responsibilities
3. Not properly documenting the contract
4. Creating interfaces that are too large
5. Using abstract classes when interfaces would be more appropriate

## Practice Problems
1. Create an abstract `Shape` class with concrete implementations
2. Design a `Logger` interface with different logging implementations
3. Implement a `DatabaseConnection` abstract class with specific database implementations

## Interview Questions
1. What is the difference between an abstract class and an interface?
2. When would you use an abstract class over an interface?
3. Can an interface extend another interface?
4. Can an abstract class implement an interface?
5. What is the purpose of pure virtual functions in C++?

## Code Example: Logger Implementation
```cpp
class Logger {
public:
    virtual void log(const string& message) = 0;
    virtual void error(const string& message) = 0;
    virtual void warn(const string& message) = 0;
    virtual ~Logger() = default;
};

class AbstractLogger : public Logger {
protected:
    string formatMessage(const string& level, const string& message) const {
        time_t now = time(nullptr);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0';  // Remove newline
        return "[" + string(dt) + "] " + level + ": " + message;
    }
};

class FileLogger : public AbstractLogger {
private:
    string filePath;
    
public:
    FileLogger(const string& filePath) : filePath(filePath) {}
    
    void log(const string& message) override {
        // Write to file
        cout << formatMessage("INFO", message) << endl;
    }
    
    void error(const string& message) override {
        // Write to file
        cout << formatMessage("ERROR", message) << endl;
    }
    
    void warn(const string& message) override {
        // Write to file
        cout << formatMessage("WARN", message) << endl;
    }
};
```

## Summary
- Abstract classes provide a base implementation and can have both abstract and concrete methods
- Interfaces define a contract that implementing classes must follow
- Abstract classes are used for code reuse and shared implementation
- Interfaces are used for defining behavior and supporting multiple inheritance
- Both are essential tools for creating flexible and maintainable code 