# Naming Conventions

## Introduction
Good naming is crucial for writing clean, maintainable code. Names should be clear, meaningful, and consistent throughout the codebase. This guide covers best practices for naming variables, functions, classes, and other code elements.

## Principles

### 1. Clarity
- Names should be self-explanatory
- Avoid abbreviations unless widely known
- Use domain-specific terminology
- Make names pronounceable

### 2. Consistency
- Follow language conventions
- Use consistent patterns
- Maintain style across codebase
- Stick to project standards

### 3. Meaning
- Names should reveal intent
- Avoid misleading names
- Use descriptive terms
- Avoid single-letter names (except for loops)

## Naming Patterns

### Variables and Constants
```cpp
// Good
const int MAX_RETRY_COUNT = 3;
string userFullName;
bool isActive;
vector<int> primeNumbers;

// Bad
const int mrc = 3;  // Unclear abbreviation
string n;          // Too short
bool a;            // Meaningless
vector<int> v;     // Unclear purpose
```

### Functions and Methods
```cpp
// Good
void calculateTotalPrice();
bool isValidEmail(const string& email);
User findUserById(int userId);

// Bad
void calc();           // Unclear abbreviation
bool check(string s);  // Unclear parameters
User get(int id);      // Unclear purpose
```

### Classes and Interfaces
```cpp
// Good
class CustomerRepository;
interface PaymentProcessor;
struct Point2D;

// Bad
class CR;              // Unclear abbreviation
interface PP;          // Unclear purpose
struct P;              // Too short
```

## Common Patterns

### 1. Boolean Names
- Use is/has/should prefix
- Make it a question
- Be positive

```cpp
// Good
bool isActive;
bool hasPermission;
bool shouldValidate;

// Bad
bool active;           // Not clear it's boolean
bool notValid;         // Negative is confusing
bool flag;             // Unclear purpose
```

### 2. Collection Names
- Use plural form
- Be specific about type
- Indicate purpose

```cpp
// Good
vector<User> activeUsers;
map<string, int> wordFrequency;
set<int> uniqueIds;

// Bad
vector<User> user;     // Should be plural
map<string, int> m;    // Unclear purpose
set<int> s;            // Too short
```

### 3. Constants
- Use UPPER_SNAKE_CASE
- Be descriptive
- Group related constants

```cpp
// Good
const int MAX_CONNECTION_POOL_SIZE = 10;
const double PI = 3.14159;
const string DEFAULT_ENCODING = "UTF-8";

// Bad
const int max = 10;    // Not clear it's a constant
const double p = 3.14159;  // Unclear purpose
const string enc = "UTF-8";  // Unclear abbreviation
```

## Language-Specific Conventions

### C++
- Use camelCase for variables and functions
- Use PascalCase for classes and structs
- Use UPPER_SNAKE_CASE for constants
- Prefix member variables with m_
- Prefix private members with underscore

```cpp
class UserManager {
private:
    string m_name;
    int m_age;
    bool m_isActive;
    
public:
    void setAge(int age);
    bool isValid() const;
    static const int MAX_AGE = 120;
};
```

### Java
- Use camelCase for variables and methods
- Use PascalCase for classes and interfaces
- Use UPPER_SNAKE_CASE for constants
- Prefix interfaces with I

```java
public class UserManager {
    private String name;
    private int age;
    private boolean isActive;
    
    public void setAge(int age) { }
    public boolean isValid() { }
    public static final int MAX_AGE = 120;
}
```

### Python
- Use snake_case for variables and functions
- Use PascalCase for classes
- Use UPPER_SNAKE_CASE for constants
- Prefix private members with underscore

```python
class UserManager:
    def __init__(self):
        self._name = ""
        self._age = 0
        self._is_active = False
    
    def set_age(self, age):
        pass
    
    def is_valid(self):
        pass
    
    MAX_AGE = 120
```

## Best Practices

### 1. Length
- Make names as long as necessary
- But no longer than needed
- Balance clarity and brevity
- Consider scope

### 2. Context
- Consider the surrounding code
- Use consistent terminology
- Match domain language
- Be aware of conventions

### 3. Evolution
- Refactor names when needed
- Update documentation
- Maintain consistency
- Consider impact

## Common Pitfalls

### 1. Over-Abbreviation
- Using unclear shortcuts
- Assuming common knowledge
- Mixing styles
- Inconsistent abbreviations

### 2. Misleading Names
- Names that don't match behavior
- Inconsistent with type
- Wrong domain terminology
- Outdated names

### 3. Inconsistent Style
- Mixing naming conventions
- Inconsistent casing
- Varying patterns
- Unclear standards

## Practice Problems

1. Refactor variable names in a complex function
2. Rename a class to better reflect its purpose
3. Standardize naming across a codebase
4. Create clear names for a new feature
5. Fix misleading names in legacy code

## Interview Questions

1. What makes a good variable name?
2. How do you handle naming in a team?
3. When should you use abbreviations?
4. How do you maintain naming consistency?
5. What are your naming conventions?

## Summary
- Names should be clear and meaningful
- Follow consistent patterns
- Use appropriate conventions
- Consider context and scope
- Regular review and refactoring 