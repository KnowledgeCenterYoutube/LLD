# Resource Management

## Overview
Resource management is crucial for ensuring proper allocation, usage, and deallocation of system resources. This guide covers various types of resources, their lifecycle management, and best practices for handling them effectively.

## Types of Resources

### 1. Memory Resources
```cpp
// Bad
void processData() {
    int* data = new int[1000];
    // Process data
    // Missing delete
}

// Good
void processData() {
    std::unique_ptr<int[]> data = std::make_unique<int[]>(1000);
    // Process data
    // Automatically cleaned up
}
```

### 2. File Resources
```cpp
// Bad
void processFile() {
    FILE* file = fopen("data.txt", "r");
    // Process file
    // Missing fclose
}

// Good
void processFile() {
    std::ifstream file("data.txt");
    // Process file
    // Automatically closed
}
```

### 3. Network Resources
```cpp
// Bad
void handleConnection() {
    Socket* socket = new Socket();
    // Use socket
    // Missing cleanup
}

// Good
void handleConnection() {
    std::unique_ptr<Socket> socket = std::make_unique<Socket>();
    // Use socket
    // Automatically cleaned up
}
```

## Resource Lifecycle

### 1. Acquisition
```cpp
class Resource {
    std::unique_ptr<Data> data;
public:
    Resource() : data(std::make_unique<Data>()) {}
    // Resource acquired in constructor
};
```

### 2. Usage
```cpp
class Resource {
    std::unique_ptr<Data> data;
public:
    void use() {
        if (!data) {
            throw std::runtime_error("Resource not acquired");
        }
        // Use resource
    }
};
```

### 3. Release
```cpp
class Resource {
    std::unique_ptr<Data> data;
public:
    ~Resource() {
        // Resource automatically released
    }
};
```

## Best Practices

### 1. RAII Pattern
```cpp
class FileHandler {
    std::ifstream file;
public:
    FileHandler(const std::string& filename) 
        : file(filename) {
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }
    // File automatically closed in destructor
};
```

### 2. Resource Pooling
```cpp
class ConnectionPool {
    std::vector<std::unique_ptr<Connection>> connections;
public:
    std::unique_ptr<Connection> acquire() {
        if (connections.empty()) {
            return std::make_unique<Connection>();
        }
        auto conn = std::move(connections.back());
        connections.pop_back();
        return conn;
    }
    
    void release(std::unique_ptr<Connection> conn) {
        connections.push_back(std::move(conn));
    }
};
```

### 3. Error Handling
```cpp
class ResourceManager {
    std::unique_ptr<Resource> resource;
public:
    void initialize() {
        try {
            resource = std::make_unique<Resource>();
        } catch (const std::exception& e) {
            // Handle initialization error
            throw;
        }
    }
};
```

## Common Patterns

### 1. Factory Pattern
```cpp
class ResourceFactory {
public:
    static std::unique_ptr<Resource> create() {
        return std::make_unique<Resource>();
    }
};
```

### 2. Singleton Pattern
```cpp
class ResourceManager {
    static std::unique_ptr<ResourceManager> instance;
public:
    static ResourceManager& getInstance() {
        if (!instance) {
            instance = std::make_unique<ResourceManager>();
        }
        return *instance;
    }
};
```

### 3. Observer Pattern
```cpp
class ResourceObserver {
public:
    virtual void onResourceAcquired() = 0;
    virtual void onResourceReleased() = 0;
};

class Resource {
    std::vector<ResourceObserver*> observers;
public:
    void addObserver(ResourceObserver* observer) {
        observers.push_back(observer);
    }
};
```

## Common Pitfalls

### 1. Resource Leaks
- Forgetting to release resources
- Exception safety issues
- Circular references

### 2. Resource Contention
- Deadlocks
- Starvation
- Race conditions

### 3. Resource Exhaustion
- Memory leaks
- File handle leaks
- Connection leaks

## Tools and Resources

### 1. Development Tools
- Memory profilers
- Resource monitors
- Static analyzers

### 2. Libraries
- Smart pointers
- RAII wrappers
- Resource management frameworks

## Practice Problems

### 1. Basic Resource Management
```cpp
// Implement proper resource management
class DatabaseConnection {
    Connection* conn;
public:
    DatabaseConnection() {
        conn = new Connection();
    }
    void query() {
        // Execute query
    }
};
```

### 2. Resource Pooling
```cpp
// Implement a resource pool
class ThreadPool {
    std::vector<std::thread> threads;
public:
    void addThread() {
        // Add thread to pool
    }
    void removeThread() {
        // Remove thread from pool
    }
};
```

## Interview Questions

1. What is RAII and how does it help with resource management?
2. Explain different types of resources and how to manage them.
3. How do you handle resource cleanup in case of exceptions?
4. What patterns do you use for resource management?
5. How do you prevent resource leaks in your code?

## Summary
- Resources must be properly acquired, used, and released
- Use RAII for automatic resource management
- Implement proper error handling
- Follow resource management patterns
- Use appropriate tools for monitoring
- Regular testing and maintenance
- Resource pooling when appropriate
- Exception safety 