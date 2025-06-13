# Error Handling

## Introduction
Error handling is a critical aspect of writing robust and maintainable code. This guide covers best practices for handling errors, exceptions, and edge cases in a clean and effective manner.

## Principles

### 1. Fail Fast
- Validate inputs early
- Check preconditions
- Handle errors at the appropriate level
- Provide clear error messages

### 2. Be Explicit
- Use specific error types
- Include relevant context
- Document error conditions
- Make error handling visible

### 3. Maintain Consistency
- Follow language conventions
- Use consistent patterns
- Handle similar errors similarly
- Document error handling strategy

## Error Handling Patterns

### 1. Result Pattern
```cpp
// Good
class Result<T> {
public:
    static Result<T> success(T value) {
        return Result<T>(value);
    }
    
    static Result<T> error(const string& message) {
        return Result<T>(message);
    }
    
    bool isSuccess() const { return m_error.empty(); }
    T getValue() const { return m_value; }
    string getError() const { return m_error; }
    
private:
    T m_value;
    string m_error;
};

// Usage
Result<User> findUser(int userId) {
    if (userId <= 0) {
        return Result<User>::error("Invalid user ID");
    }
    // ... find user logic
    return Result<User>::success(user);
}
```

### 2. Exception Handling
```cpp
// Good
class UserNotFoundException : public std::runtime_error {
public:
    explicit UserNotFoundException(int userId)
        : std::runtime_error("User not found: " + std::to_string(userId)) {}
};

User findUser(int userId) {
    if (userId <= 0) {
        throw std::invalid_argument("Invalid user ID");
    }
    // ... find user logic
    if (!user) {
        throw UserNotFoundException(userId);
    }
    return user;
}

// Usage
try {
    User user = findUser(userId);
    // Use user
} catch (const UserNotFoundException& e) {
    // Handle specific error
} catch (const std::exception& e) {
    // Handle other errors
}
```

### 3. Optional Pattern
```cpp
// Good
std::optional<User> findUser(int userId) {
    if (userId <= 0) {
        return std::nullopt;
    }
    // ... find user logic
    return user;
}

// Usage
if (auto user = findUser(userId)) {
    // Use user
} else {
    // Handle not found case
}
```

## Best Practices

### 1. Input Validation
```cpp
// Good
void processOrder(const Order& order) {
    if (order.isEmpty()) {
        throw std::invalid_argument("Order cannot be empty");
    }
    if (order.getTotal() < 0) {
        throw std::invalid_argument("Order total cannot be negative");
    }
    // Process order
}

// Bad
void processOrder(const Order& order) {
    // No validation
    // Process order
}
```

### 2. Error Context
```cpp
// Good
class DatabaseError : public std::runtime_error {
public:
    DatabaseError(const string& operation, const string& details)
        : std::runtime_error("Database error during " + operation + ": " + details) {}
};

// Bad
throw std::runtime_error("Database error");  // No context
```

### 3. Error Recovery
```cpp
// Good
class FileProcessor {
public:
    void processFile(const string& filename) {
        try {
            // Try primary method
            processFileInternal(filename);
        } catch (const FileSystemError& e) {
            // Try backup method
            processFileBackup(filename);
        }
    }
};
```

## Language-Specific Considerations

### C++
```cpp
// Good
class UserManager {
public:
    // Use noexcept for functions that won't throw
    void updateUser(const User& user) noexcept {
        // ...
    }
    
    // Document exceptions that can be thrown
    /**
     * @throws UserNotFoundException if user doesn't exist
     * @throws DatabaseError if database operation fails
     */
    User findUser(int userId) {
        // ...
    }
    
    // Use RAII for resource management
    class DatabaseConnection {
    public:
        DatabaseConnection() {
            // Acquire resource
        }
        ~DatabaseConnection() {
            // Release resource
        }
    };
};

// Bad
class UserManager {
public:
    // No exception specification
    void updateUser(const User& user) {
        // ...
    }
    
    // No documentation of exceptions
    User findUser(int userId) {
        // ...
    }
};
```

### Java
```java
// Good
public class UserManager {
    // Document checked exceptions
    /**
     * @throws UserNotFoundException if user doesn't exist
     * @throws DatabaseException if database operation fails
     */
    public User findUser(int userId) throws UserNotFoundException, DatabaseException {
        // ...
    }
    
    // Use try-with-resources
    public void processFile(String filename) {
        try (FileInputStream fis = new FileInputStream(filename)) {
            // Process file
        } catch (IOException e) {
            // Handle error
        }
    }
}

// Bad
public class UserManager {
    // No exception documentation
    public User findUser(int userId) {
        // ...
    }
    
    // No resource cleanup
    public void processFile(String filename) {
        FileInputStream fis = new FileInputStream(filename);
        // Process file
    }
}
```

### Python
```python
# Good
class UserManager:
    def find_user(self, user_id: int) -> User:
        """
        Find a user by ID.
        
        Args:
            user_id: The user's ID
            
        Returns:
            User: The found user
            
        Raises:
            UserNotFoundError: If user doesn't exist
            DatabaseError: If database operation fails
        """
        if user_id <= 0:
            raise ValueError("Invalid user ID")
        # ...
    
    def process_file(self, filename: str) -> None:
        with open(filename) as f:
            # Process file
            pass

# Bad
class UserManager:
    def find_user(self, user_id):
        # No type hints
        # No documentation
        # No error handling
        pass
    
    def process_file(self, filename):
        f = open(filename)
        # No resource cleanup
        pass
```

## Common Pitfalls

### 1. Swallowing Exceptions
```cpp
// Bad
try {
    processData();
} catch (...) {
    // Swallow all exceptions
}

// Good
try {
    processData();
} catch (const std::exception& e) {
    logger.error("Failed to process data: " + string(e.what()));
    // Handle error appropriately
}
```

### 2. Generic Error Messages
```cpp
// Bad
throw std::runtime_error("Error occurred");

// Good
throw DatabaseError("Failed to connect to database", "Connection timeout");
```

### 3. Resource Leaks
```cpp
// Bad
void processFile() {
    File* file = new File("data.txt");
    // Process file
    // Forgot to delete file
}

// Good
void processFile() {
    std::unique_ptr<File> file = std::make_unique<File>("data.txt");
    // Process file
    // File automatically deleted
}
```

## Practice Problems

1. Implement a robust file processing system with error handling
2. Create a custom exception hierarchy for a specific domain
3. Add proper error handling to an existing codebase
4. Implement error recovery mechanisms
5. Convert error codes to exceptions

## Interview Questions

1. What are the different ways to handle errors in your language?
2. How do you decide between using exceptions and error codes?
3. What makes a good error message?
4. How do you handle errors in a multi-threaded environment?
5. How do you test error handling code?

## Summary
- Fail fast and explicitly
- Provide clear error messages
- Use appropriate error handling patterns
- Handle resources properly
- Document error conditions
- Regular review and testing 