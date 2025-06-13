# Functions and Methods

## Introduction
Functions are the building blocks of clean code. Well-written functions are essential for creating maintainable, readable, and testable code. This guide covers best practices for writing clean functions and methods.

## Principles

### 1. Single Responsibility
- Each function should do one thing
- Functions should be small and focused
- Avoid side effects
- Clear purpose and intent

### 2. Readability
- Clear and descriptive names
- Logical organization
- Proper abstraction level
- Self-documenting code

### 3. Maintainability
- Easy to modify
- Easy to test
- Easy to understand
- Easy to reuse

## Function Structure

### 1. Size and Complexity
```cpp
// Good
double calculateDiscount(const Order& order) {
    if (order.isEmpty()) {
        return 0.0;
    }
    return order.getTotal() * DISCOUNT_RATE;
}

// Bad
double calculateDiscount(const Order& order) {
    double total = 0.0;
    for (const auto& item : order.getItems()) {
        total += item.getPrice() * item.getQuantity();
        if (item.isOnSale()) {
            total -= item.getDiscount();
        }
    }
    if (order.getCustomer().isPremium()) {
        total *= 0.9;
    }
    if (order.getDate().isHoliday()) {
        total *= 0.95;
    }
    return total;
}
```

### 2. Parameters
```cpp
// Good
void updateUserProfile(const string& userId, const UserProfile& profile);

// Bad
void updateUserProfile(const string& userId, const string& name, 
                      const string& email, const string& phone,
                      const string& address, const string& city,
                      const string& state, const string& zip);
```

### 3. Return Values
```cpp
// Good
bool isValidEmail(const string& email) {
    return regex_match(email, EMAIL_PATTERN);
}

// Bad
void validateEmail(const string& email, bool& isValid) {
    isValid = regex_match(email, EMAIL_PATTERN);
}
```

## Best Practices

### 1. Function Names
- Use verb-noun format
- Be descriptive and specific
- Avoid abbreviations
- Match domain language

```cpp
// Good
void calculateTotalPrice();
bool isValidEmail();
User findUserById();

// Bad
void calc();           // Unclear
bool check();          // Vague
User get();            // Ambiguous
```

### 2. Parameter Order
- Most important first
- Related parameters together
- Optional parameters last
- Consistent ordering

```cpp
// Good
void updateUser(int userId, const UserData& data);
void sendEmail(const string& to, const string& subject, const string& body);

// Bad
void updateUser(const UserData& data, int userId);  // Inconsistent
void sendEmail(const string& body, const string& to, const string& subject);  // Unclear order
```

### 3. Error Handling
```cpp
// Good
Result<User> findUser(int userId) {
    if (userId <= 0) {
        return Result<User>::error("Invalid user ID");
    }
    // ... find user logic
}

// Bad
User findUser(int userId) {
    if (userId <= 0) {
        throw std::invalid_argument("Invalid user ID");
    }
    // ... find user logic
}
```

## Common Patterns

### 1. Command Functions
```cpp
// Good
void saveUser(const User& user);
void deleteUser(int userId);
void updateUserStatus(int userId, UserStatus status);

// Bad
void userOperation(const User& user, OperationType type);  // Too generic
```

### 2. Query Functions
```cpp
// Good
bool isUserActive(int userId);
int getUserAge(int userId);
string getUserEmail(int userId);

// Bad
User getUser(int userId);  // Returns too much information
```

### 3. Transformation Functions
```cpp
// Good
string formatDate(const Date& date);
vector<int> filterEvenNumbers(const vector<int>& numbers);
string convertToUpperCase(const string& text);

// Bad
void processData(vector<int>& numbers);  // Modifies input
```

## Language-Specific Considerations

### C++
```cpp
// Good
class UserManager {
public:
    // Const correctness
    string getName() const;
    
    // Reference parameters
    void updateUser(const User& user);
    
    // Return by value or reference
    const vector<Order>& getOrders() const;
    
    // Exception safety
    void saveUser(const User& user) noexcept;
};

// Bad
class UserManager {
public:
    // Missing const
    string getName();
    
    // Value parameters
    void updateUser(User user);
    
    // Unnecessary pointer
    vector<Order>* getOrders();
    
    // Unsafe exceptions
    void saveUser(const User& user);
};
```

### Java
```java
// Good
public class UserManager {
    // Immutable return
    public List<Order> getOrders() {
        return Collections.unmodifiableList(orders);
    }
    
    // Optional parameters
    public void updateUser(User user, boolean notify) {
        // ...
    }
    
    // Builder pattern
    public UserBuilder newUser() {
        return new UserBuilder();
    }
}

// Bad
public class UserManager {
    // Mutable return
    public List<Order> getOrders() {
        return orders;  // Exposes internal state
    }
    
    // Too many parameters
    public void updateUser(User user, boolean notify, boolean log, boolean validate) {
        // ...
    }
}
```

### Python
```python
# Good
class UserManager:
    def get_orders(self) -> List[Order]:
        return self._orders.copy()  # Return copy
    
    def update_user(self, user: User, notify: bool = False) -> None:
        # Type hints
        pass
    
    def find_user(self, user_id: int) -> Optional[User]:
        # Optional return
        pass

# Bad
class UserManager:
    def get_orders(self):
        return self._orders  # Exposes internal state
    
    def update_user(self, user, notify, log, validate):
        # Too many parameters
        pass
    
    def find_user(self, user_id):
        # No type hints
        pass
```

## Common Pitfalls

### 1. Too Many Parameters
- Hard to understand
- Hard to maintain
- Hard to test
- Hard to use

### 2. Side Effects
- Unpredictable behavior
- Hard to test
- Hard to debug
- Hard to maintain

### 3. Long Functions
- Hard to understand
- Hard to test
- Hard to maintain
- Hard to reuse

## Practice Problems

1. Refactor a long function into smaller ones
2. Improve parameter handling in a complex function
3. Add proper error handling to a function
4. Convert a function with side effects to a pure function
5. Improve function naming and documentation

## Interview Questions

1. What makes a good function?
2. How do you handle functions with many parameters?
3. What are the benefits of pure functions?
4. How do you test complex functions?
5. When should you split a function?

## Summary
- Functions should be small and focused
- Clear naming and purpose
- Proper parameter handling
- Effective error handling
- Regular review and refactoring 