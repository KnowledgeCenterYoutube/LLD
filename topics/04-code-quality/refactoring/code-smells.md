# Code Smells and Refactoring

## Introduction
Code smells are indicators of potential problems in code that may lead to maintenance issues. This guide covers common code smells, their impact, and refactoring techniques to address them.

## Common Code Smells

### 1. Long Method
```cpp
// Bad
void processOrder(Order& order) {
    // Validate order
    if (order.getItems().empty()) {
        throw std::invalid_argument("Order cannot be empty");
    }
    if (order.getTotal() < 0) {
        throw std::invalid_argument("Order total cannot be negative");
    }
    
    // Calculate discounts
    double discount = 0;
    if (order.getCustomer().isPremium()) {
        discount = order.getTotal() * 0.1;
    }
    if (order.getItems().size() > 5) {
        discount += order.getTotal() * 0.05;
    }
    
    // Apply discounts
    order.setDiscount(discount);
    
    // Save to database
    Database::getInstance().saveOrder(order);
    
    // Send notifications
    NotificationService::getInstance().sendOrderConfirmation(order);
}

// Good
void processOrder(Order& order) {
    validateOrder(order);
    applyDiscounts(order);
    saveOrder(order);
    sendNotifications(order);
}

void validateOrder(const Order& order) {
    if (order.getItems().empty()) {
        throw std::invalid_argument("Order cannot be empty");
    }
    if (order.getTotal() < 0) {
        throw std::invalid_argument("Order total cannot be negative");
    }
}

void applyDiscounts(Order& order) {
    double discount = calculateCustomerDiscount(order);
    discount += calculateBulkDiscount(order);
    order.setDiscount(discount);
}
```

### 2. Large Class
```cpp
// Bad
class UserManager {
    // User management
    void createUser();
    void updateUser();
    void deleteUser();
    void findUser();
    
    // Authentication
    void login();
    void logout();
    void resetPassword();
    
    // Profile management
    void updateProfile();
    void uploadAvatar();
    void updatePreferences();
    
    // Notifications
    void sendWelcomeEmail();
    void sendPasswordResetEmail();
    void sendProfileUpdateNotification();
};

// Good
class UserManager {
    void createUser();
    void updateUser();
    void deleteUser();
    void findUser();
};

class AuthenticationService {
    void login();
    void logout();
    void resetPassword();
};

class ProfileManager {
    void updateProfile();
    void uploadAvatar();
    void updatePreferences();
};

class NotificationService {
    void sendWelcomeEmail();
    void sendPasswordResetEmail();
    void sendProfileUpdateNotification();
};
```

### 3. Duplicate Code
```cpp
// Bad
class OrderProcessor {
    void processRegularOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        applyTax(order);
        saveOrder(order);
    }
    
    void processPremiumOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        applyTax(order);
        applyPremiumDiscount(order);
        saveOrder(order);
    }
};

// Good
class OrderProcessor {
    void processOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        applyTax(order);
        if (order.isPremium()) {
            applyPremiumDiscount(order);
        }
        saveOrder(order);
    }
};
```

### 4. Primitive Obsession
```cpp
// Bad
class Order {
    string customerName;
    string customerEmail;
    string customerPhone;
    string shippingAddress;
    string billingAddress;
    double total;
    string currency;
};

// Good
class Order {
    Customer customer;
    Address shippingAddress;
    Address billingAddress;
    Money total;
};
```

### 5. Data Clumps
```cpp
// Bad
class User {
    string firstName;
    string lastName;
    string street;
    string city;
    string state;
    string zipCode;
    string country;
};

// Good
class User {
    Name name;
    Address address;
};
```

## Refactoring Techniques

### 1. Extract Method
```cpp
// Before
void printOrderSummary(Order& order) {
    cout << "Order #" << order.getId() << endl;
    cout << "Customer: " << order.getCustomerName() << endl;
    cout << "Items:" << endl;
    for (const auto& item : order.getItems()) {
        cout << "- " << item.getName() << ": $" << item.getPrice() << endl;
    }
    cout << "Total: $" << order.getTotal() << endl;
}

// After
void printOrderSummary(Order& order) {
    printOrderHeader(order);
    printOrderItems(order);
    printOrderTotal(order);
}

void printOrderHeader(Order& order) {
    cout << "Order #" << order.getId() << endl;
    cout << "Customer: " << order.getCustomerName() << endl;
}
```

### 2. Move Method
```cpp
// Before
class Order {
    double calculateTax() {
        return total * taxRate;
    }
};

// After
class TaxCalculator {
    static double calculateTax(double amount, double rate) {
        return amount * rate;
    }
};
```

### 3. Replace Conditional with Polymorphism
```cpp
// Before
class OrderProcessor {
    void processOrder(Order& order) {
        if (order.getType() == "regular") {
            processRegularOrder(order);
        } else if (order.getType() == "premium") {
            processPremiumOrder(order);
        } else if (order.getType() == "international") {
            processInternationalOrder(order);
        }
    }
};

// After
class OrderProcessor {
    virtual void processOrder(Order& order) = 0;
};

class RegularOrderProcessor : public OrderProcessor {
    void processOrder(Order& order) override {
        // Process regular order
    }
};

class PremiumOrderProcessor : public OrderProcessor {
    void processOrder(Order& order) override {
        // Process premium order
    }
};
```

### 4. Introduce Parameter Object
```cpp
// Before
void createUser(string firstName, string lastName, string email, 
               string phone, string street, string city, 
               string state, string zipCode) {
    // Create user
}

// After
void createUser(const UserData& userData) {
    // Create user
}
```

### 5. Replace Magic Numbers with Constants
```cpp
// Before
double calculateDiscount(double total) {
    if (total > 1000) {
        return total * 0.1;
    }
    return total * 0.05;
}

// After
const double PREMIUM_DISCOUNT_RATE = 0.1;
const double STANDARD_DISCOUNT_RATE = 0.05;
const double PREMIUM_THRESHOLD = 1000.0;

double calculateDiscount(double total) {
    if (total > PREMIUM_THRESHOLD) {
        return total * PREMIUM_DISCOUNT_RATE;
    }
    return total * STANDARD_DISCOUNT_RATE;
}
```

## Best Practices

### 1. When to Refactor
- When adding new features
- When fixing bugs
- During code reviews
- When improving performance
- When improving maintainability

### 2. Refactoring Process
1. Identify code smells
2. Write tests
3. Make small changes
4. Run tests
5. Commit changes
6. Repeat

### 3. Testing During Refactoring
```cpp
// Before refactoring
TEST(OrderProcessor, ProcessRegularOrder) {
    Order order;
    // Setup order
    processor.processOrder(order);
    EXPECT_EQ(order.getTotal(), expectedTotal);
}

// After refactoring
TEST(OrderProcessor, ProcessRegularOrder) {
    Order order;
    // Setup order
    processor.processOrder(order);
    EXPECT_EQ(order.getTotal(), expectedTotal);
    // Same test should still pass
}
```

## Common Pitfalls

### 1. Over-Refactoring
```cpp
// Bad
class OrderProcessor {
    void processOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        applyTax(order);
        applyDiscount(order);
        saveOrder(order);
        sendNotification(order);
    }
    
    void validateOrder(Order& order) {
        if (order.getItems().empty()) {
            throw std::invalid_argument("Order cannot be empty");
        }
    }
};

// Good
class OrderProcessor {
    void processOrder(Order& order) {
        if (order.getItems().empty()) {
            throw std::invalid_argument("Order cannot be empty");
        }
        calculateTotal(order);
        applyTax(order);
        applyDiscount(order);
        saveOrder(order);
        sendNotification(order);
    }
};
```

### 2. Breaking Existing Functionality
```cpp
// Bad
void processOrder(Order& order) {
    // Removed validation
    calculateTotal(order);
    saveOrder(order);
}

// Good
void processOrder(Order& order) {
    validateOrder(order);  // Keep existing validation
    calculateTotal(order);
    saveOrder(order);
}
```

### 3. Incomplete Refactoring
```cpp
// Bad
class OrderProcessor {
    void processOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        // Missing tax calculation
        saveOrder(order);
    }
};

// Good
class OrderProcessor {
    void processOrder(Order& order) {
        validateOrder(order);
        calculateTotal(order);
        applyTax(order);
        saveOrder(order);
    }
};
```

## Practice Problems

1. Refactor a long method into smaller, focused methods
2. Split a large class into smaller, cohesive classes
3. Remove duplicate code using inheritance or composition
4. Replace primitive types with proper domain objects
5. Apply the Replace Conditional with Polymorphism pattern

## Interview Questions

1. What are code smells and why are they important?
2. How do you decide when to refactor code?
3. What are your favorite refactoring techniques?
4. How do you ensure refactoring doesn't break existing functionality?
5. How do you handle refactoring in a team environment?

## Summary
- Identify and address code smells early
- Use appropriate refactoring techniques
- Maintain test coverage
- Make small, incremental changes
- Document refactoring decisions
- Regular code reviews and improvements 