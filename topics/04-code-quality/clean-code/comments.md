# Comments and Documentation

## Introduction
Good comments and documentation are essential for maintaining code quality and helping other developers understand your code. This guide covers best practices for writing effective comments and documentation that add value rather than noise.

## Principles

### 1. Why Comment?
- Explain "why" not "what"
- Document complex logic
- Clarify non-obvious behavior
- Provide context and intent

### 2. When to Comment
- Public APIs and interfaces
- Complex algorithms
- Business rules
- Non-obvious decisions
- Workarounds and hacks

### 3. When Not to Comment
- Obvious code
- Redundant explanations
- Outdated information
- Commented-out code

## Types of Comments

### 1. Documentation Comments
```cpp
/**
 * Calculates the total price including tax and discounts.
 * 
 * @param order The order to calculate price for
 * @param taxRate The applicable tax rate (0.0 to 1.0)
 * @return The total price including tax
 * @throws InvalidOrderException if order is empty
 */
double calculateTotalPrice(const Order& order, double taxRate);
```

### 2. Implementation Comments
```cpp
// Good
// Using binary search for O(log n) performance
int findIndex(const vector<int>& numbers, int target) {
    // ...
}

// Bad
// Loop through numbers
for (int i = 0; i < numbers.size(); i++) {
    // ...
}
```

### 3. TODO Comments
```cpp
// TODO: Implement caching for better performance
// FIXME: Handle edge case when input is empty
// HACK: Temporary workaround for API limitation
// NOTE: This will be deprecated in v2.0
```

## Best Practices

### 1. Keep Comments Up-to-Date
```cpp
// Good
// Current implementation uses quick sort
void sort(vector<int>& numbers) {
    // ...
}

// Bad
// Uses bubble sort (outdated comment)
void sort(vector<int>& numbers) {
    // Actually uses quick sort
    // ...
}
```

### 2. Use Clear Language
```cpp
// Good
// Calculate compound interest using the formula: A = P(1 + r/n)^(nt)
double calculateCompoundInterest(double principal, double rate, int years) {
    // ...
}

// Bad
// Do the math thing
double calculateCompoundInterest(double p, double r, int t) {
    // ...
}
```

### 3. Document Assumptions
```cpp
// Good
// Assumes input array is sorted
// Assumes all elements are positive
int findFirstDuplicate(const vector<int>& numbers) {
    // ...
}

// Bad
int findFirstDuplicate(const vector<int>& numbers) {
    // No documentation of assumptions
    // ...
}
```

## Language-Specific Documentation

### C++
```cpp
/**
 * @brief A class representing a user in the system
 * 
 * This class handles user authentication and profile management.
 * It is thread-safe and can be used in a multi-threaded environment.
 */
class User {
public:
    /**
     * @brief Creates a new user with the given credentials
     * 
     * @param username The user's login name
     * @param email The user's email address
     * @throws InvalidCredentialsException if username or email is invalid
     */
    User(const string& username, const string& email);
    
    /**
     * @brief Authenticates the user with the given password
     * 
     * @param password The password to verify
     * @return true if authentication succeeds
     * @throws AuthenticationException if authentication fails
     */
    bool authenticate(const string& password);
};
```

### Java
```java
/**
 * Represents a user in the system.
 * This class is thread-safe and can be used in a multi-threaded environment.
 */
public class User {
    /**
     * Creates a new user with the given credentials.
     *
     * @param username the user's login name
     * @param email the user's email address
     * @throws InvalidCredentialsException if username or email is invalid
     */
    public User(String username, String email) {
        // ...
    }
    
    /**
     * Authenticates the user with the given password.
     *
     * @param password the password to verify
     * @return true if authentication succeeds
     * @throws AuthenticationException if authentication fails
     */
    public boolean authenticate(String password) {
        // ...
    }
}
```

### Python
```python
class User:
    """
    Represents a user in the system.
    
    This class is thread-safe and can be used in a multi-threaded environment.
    
    Attributes:
        username (str): The user's login name
        email (str): The user's email address
    """
    
    def __init__(self, username: str, email: str) -> None:
        """
        Creates a new user with the given credentials.
        
        Args:
            username: The user's login name
            email: The user's email address
            
        Raises:
            InvalidCredentialsException: If username or email is invalid
        """
        pass
    
    def authenticate(self, password: str) -> bool:
        """
        Authenticates the user with the given password.
        
        Args:
            password: The password to verify
            
        Returns:
            bool: True if authentication succeeds
            
        Raises:
            AuthenticationException: If authentication fails
        """
        pass
```

## Common Pitfalls

### 1. Redundant Comments
```cpp
// Bad
// Increment counter by 1
counter++;

// Good
// Track number of failed login attempts
failedLoginAttempts++;
```

### 2. Misleading Comments
```cpp
// Bad
// Sort array in ascending order
void sort(vector<int>& numbers) {
    // Actually sorts in descending order
    // ...
}

// Good
// Sort array in descending order
void sort(vector<int>& numbers) {
    // ...
}
```

### 3. Commented-Out Code
```cpp
// Bad
// void oldImplementation() {
//     // lots of old code
// }

// Good
// Removed old implementation in favor of new algorithm
// See git history for details
```

## Documentation Tools

### 1. Documentation Generators
- Doxygen (C++)
- Javadoc (Java)
- Sphinx (Python)
- JSDoc (JavaScript)

### 2. Code Review Tools
- GitHub
- GitLab
- Bitbucket
- Gerrit

### 3. IDE Support
- IntelliJ IDEA
- Visual Studio
- Eclipse
- VS Code

## Practice Problems

1. Add documentation to an undocumented class
2. Fix misleading comments in a codebase
3. Convert implementation comments to documentation comments
4. Remove redundant comments
5. Update outdated documentation

## Interview Questions

1. When should you add comments to code?
2. What makes a good comment?
3. How do you maintain documentation?
4. What tools do you use for documentation?
5. How do you handle outdated comments?

## Summary
- Comments should explain "why" not "what"
- Keep comments up-to-date
- Use clear and consistent language
- Document public APIs thoroughly
- Regular review and maintenance 