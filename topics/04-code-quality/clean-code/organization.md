# Code Organization

## Introduction
Code organization is crucial for maintaining a clean, maintainable, and scalable codebase. This guide covers best practices for structuring code, organizing files, and managing dependencies effectively.

## Principles

### 1. Separation of Concerns
- Group related functionality
- Minimize dependencies
- Clear boundaries
- Single responsibility

### 2. Modularity
- Independent components
- Well-defined interfaces
- Loose coupling
- High cohesion

### 3. Maintainability
- Easy to understand
- Easy to modify
- Easy to test
- Easy to extend

## Project Structure

### 1. Directory Organization
```
project/
├── src/                    # Source code
│   ├── core/              # Core functionality
│   ├── features/          # Feature modules
│   ├── utils/             # Utility functions
│   └── tests/             # Test files
├── include/               # Header files
├── docs/                  # Documentation
├── build/                 # Build artifacts
└── scripts/              # Build and utility scripts
```

### 2. Module Organization
```
feature/
├── models/               # Data models
├── services/            # Business logic
├── repositories/        # Data access
├── controllers/         # Request handling
└── utils/              # Feature-specific utilities
```

### 3. File Organization
```cpp
// Good
// user.h
#pragma once

#include <string>
#include <vector>

namespace user {

class User {
public:
    // Public interface
private:
    // Private implementation
};

} // namespace user

// Bad
// user.h
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
// ... many more includes

class User {
    // Everything mixed together
};
```

## Code Organization Patterns

### 1. Layer Pattern
```cpp
// Presentation Layer
class UserController {
public:
    void handleRequest(const Request& request) {
        // Validate request
        // Call service
        // Format response
    }
};

// Service Layer
class UserService {
public:
    User createUser(const UserData& data) {
        // Business logic
        // Call repository
    }
};

// Repository Layer
class UserRepository {
public:
    void saveUser(const User& user) {
        // Database operations
    }
};
```

### 2. Feature Organization
```cpp
// Good
namespace auth {
    class AuthenticationService;
    class UserRepository;
    class PasswordHasher;
}

namespace payment {
    class PaymentProcessor;
    class TransactionRepository;
    class PaymentValidator;
}

// Bad
class AuthenticationService;
class PaymentProcessor;
class UserRepository;
class TransactionRepository;
class PasswordHasher;
class PaymentValidator;
```

### 3. Interface Segregation
```cpp
// Good
class IUserReader {
public:
    virtual User findById(int id) = 0;
    virtual vector<User> findAll() = 0;
};

class IUserWriter {
public:
    virtual void save(const User& user) = 0;
    virtual void delete(int id) = 0;
};

class UserRepository : public IUserReader, public IUserWriter {
    // Implementation
};

// Bad
class IUserRepository {
public:
    virtual User findById(int id) = 0;
    virtual vector<User> findAll() = 0;
    virtual void save(const User& user) = 0;
    virtual void delete(int id) = 0;
    virtual void validate(const User& user) = 0;
    virtual void notify(const User& user) = 0;
};
```

## Best Practices

### 1. Include Organization
```cpp
// Good
// Standard library includes
#include <string>
#include <vector>
#include <memory>

// Third-party includes
#include <boost/optional.hpp>
#include <gtest/gtest.h>

// Project includes
#include "core/types.h"
#include "utils/logger.h"

// Bad
#include "utils/logger.h"
#include <string>
#include "core/types.h"
#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <boost/optional.hpp>
```

### 2. Class Organization
```cpp
// Good
class User {
public:
    // Constants
    static const int MAX_NAME_LENGTH = 100;
    
    // Constructors
    User();
    explicit User(const string& name);
    
    // Public interface
    string getName() const;
    void setName(const string& name);
    
protected:
    // Protected interface
    
private:
    // Private implementation
    string m_name;
    void validateName(const string& name);
};

// Bad
class User {
    string m_name;
    static const int MAX_NAME_LENGTH = 100;
    
    void validateName(const string& name);
    
public:
    User();
    explicit User(const string& name);
    string getName() const;
    void setName(const string& name);
};
```

### 3. Function Organization
```cpp
// Good
class OrderProcessor {
public:
    // Public interface
    void processOrder(const Order& order);
    
private:
    // Helper functions
    void validateOrder(const Order& order);
    void calculateTotal(Order& order);
    void applyDiscounts(Order& order);
    void saveOrder(const Order& order);
    
    // Utility functions
    double calculateTax(double amount);
    bool isEligibleForDiscount(const Order& order);
};

// Bad
class OrderProcessor {
public:
    void processOrder(const Order& order);
    void validateOrder(const Order& order);
    void calculateTotal(Order& order);
    void applyDiscounts(Order& order);
    void saveOrder(const Order& order);
    double calculateTax(double amount);
    bool isEligibleForDiscount(const Order& order);
};
```

## Language-Specific Considerations

### C++
```cpp
// Good
// Separate interface and implementation
// user.h
class User {
public:
    virtual ~User() = default;
    virtual string getName() const = 0;
};

// user.cpp
class UserImpl : public User {
public:
    string getName() const override;
};

// Bad
// Everything in one file
class User {
    string m_name;
public:
    string getName() const { return m_name; }
};
```

### Java
```java
// Good
// Package organization
package com.company.project.feature.user;

public class User {
    // Class implementation
}

// Bad
// No package organization
public class User {
    // Class implementation
}
```

### Python
```python
# Good
# Module organization
from typing import List, Optional

class User:
    def __init__(self, name: str) -> None:
        self._name = name
    
    def get_name(self) -> str:
        return self._name

# Bad
# No type hints
# No clear organization
class User:
    def __init__(self, name):
        self.name = name
    
    def get_name(self):
        return self.name
```

## Common Pitfalls

### 1. Circular Dependencies
```cpp
// Bad
// user.h
#include "order.h"
class User { /* ... */ };

// order.h
#include "user.h"
class Order { /* ... */ };

// Good
// user.h
class Order;  // Forward declaration
class User { /* ... */ };

// order.h
class User;  // Forward declaration
class Order { /* ... */ };
```

### 2. Large Files
```cpp
// Bad
// user.h (1000+ lines)
class User {
    // Many methods and properties
};

// Good
// user.h
class User {
    // Core interface
};

// user_impl.h
class UserImpl : public User {
    // Implementation details
};
```

### 3. Mixed Responsibilities
```cpp
// Bad
class UserManager {
    void processOrder();
    void sendEmail();
    void updateDatabase();
    void validateInput();
};

// Good
class UserManager {
    void updateUser();
};

class OrderProcessor {
    void processOrder();
};

class EmailService {
    void sendEmail();
};
```

## Practice Problems

1. Reorganize a large class into smaller, focused classes
2. Refactor a codebase to follow proper layering
3. Implement proper interface segregation
4. Fix circular dependencies
5. Organize a new feature module

## Interview Questions

1. How do you organize a large codebase?
2. What are the benefits of proper code organization?
3. How do you handle dependencies between modules?
4. When should you split a class or module?
5. How do you maintain code organization over time?

## Summary
- Follow clear organizational principles
- Use appropriate patterns and structures
- Maintain consistent style
- Regular refactoring and cleanup
- Document organization decisions
- Review and improve continuously 