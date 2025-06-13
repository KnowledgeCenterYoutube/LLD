# Deadlocks

## Overview
Deadlocks occur when two or more threads are blocked forever, each waiting for a resource held by the other. This guide covers the causes, detection, and prevention of deadlocks in concurrent programming.

## Causes of Deadlocks

### 1. Nested Locks
```java
// Bad: Potential deadlock
class BankAccount {
    private final Object lock1 = new Object();
    private final Object lock2 = new Object();
    
    public void transfer(BankAccount other, double amount) {
        synchronized (lock1) {
            synchronized (other.lock1) {
                // Transfer money
            }
        }
    }
}

// Good: Consistent lock ordering
class BankAccount {
    private final Object lock = new Object();
    
    public void transfer(BankAccount other, double amount) {
        BankAccount first = this;
        BankAccount second = other;
        
        if (System.identityHashCode(first) > System.identityHashCode(second)) {
            BankAccount temp = first;
            first = second;
            second = temp;
        }
        
        synchronized (first.lock) {
            synchronized (second.lock) {
                // Transfer money
            }
        }
    }
}
```

### 2. Resource Acquisition
```java
// Bad: Potential deadlock
class ResourceManager {
    private final Object resource1 = new Object();
    private final Object resource2 = new Object();
    
    public void process1() {
        synchronized (resource1) {
            synchronized (resource2) {
                // Process
            }
        }
    }
    
    public void process2() {
        synchronized (resource2) {
            synchronized (resource1) {
                // Process
            }
        }
    }
}

// Good: Single lock
class ResourceManager {
    private final Object lock = new Object();
    
    public void process1() {
        synchronized (lock) {
            // Process
        }
    }
    
    public void process2() {
        synchronized (lock) {
            // Process
        }
    }
}
```

### 3. Circular Wait
```java
// Bad: Circular wait
class Philosopher implements Runnable {
    private final Object leftFork;
    private final Object rightFork;
    
    public void run() {
        while (true) {
            synchronized (leftFork) {
                synchronized (rightFork) {
                    // Eat
                }
            }
        }
    }
}

// Good: Resource ordering
class Philosopher implements Runnable {
    private final Object[] forks;
    private final int id;
    
    public void run() {
        while (true) {
            int left = id;
            int right = (id + 1) % forks.length;
            
            if (left > right) {
                int temp = left;
                left = right;
                right = temp;
            }
            
            synchronized (forks[left]) {
                synchronized (forks[right]) {
                    // Eat
                }
            }
        }
    }
}
```

## Detection Methods

### 1. Static Analysis
- Use tools like FindBugs, PMD
- Enable compiler warnings
- Code review for common patterns

### 2. Dynamic Analysis
- Thread dumps
- Deadlock detectors
- Runtime monitoring

### 3. Testing
- Stress testing
- Concurrency testing
- Deadlock detection tools

## Prevention Techniques

### 1. Lock Ordering
```java
class Transaction {
    private static final Object globalLock = new Object();
    
    public void transfer(Account from, Account to, double amount) {
        Account first = from;
        Account second = to;
        
        if (System.identityHashCode(first) > System.identityHashCode(second)) {
            Account temp = first;
            first = second;
            second = temp;
        }
        
        synchronized (first) {
            synchronized (second) {
                // Transfer
            }
        }
    }
}
```

### 2. Timeout Mechanism
```java
class ResourceManager {
    private final Lock lock = new ReentrantLock();
    
    public boolean acquireResource(long timeout, TimeUnit unit) {
        try {
            return lock.tryLock(timeout, unit);
        } catch (InterruptedException e) {
            return false;
        }
    }
    
    public void releaseResource() {
        lock.unlock();
    }
}
```

### 3. Resource Hierarchy
```java
class ResourceHierarchy {
    private static final int LEVELS = 3;
    private final Lock[] locks = new Lock[LEVELS];
    
    public ResourceHierarchy() {
        for (int i = 0; i < LEVELS; i++) {
            locks[i] = new ReentrantLock();
        }
    }
    
    public void acquire(int level) {
        for (int i = 0; i <= level; i++) {
            locks[i].lock();
        }
    }
    
    public void release(int level) {
        for (int i = level; i >= 0; i--) {
            locks[i].unlock();
        }
    }
}
```

## Best Practices

### 1. Lock Management
- Consistent lock ordering
- Minimal lock scope
- Timeout mechanisms

### 2. Resource Management
- Resource hierarchy
- Single lock when possible
- Avoid nested locks

### 3. Design Patterns
- Resource ordering
- Timeout patterns
- Hierarchical locking

## Common Patterns

### 1. Try-Lock Pattern
```java
class ResourceManager {
    private final Lock lock = new ReentrantLock();
    
    public boolean tryOperation() {
        if (lock.tryLock()) {
            try {
                // Perform operation
                return true;
            } finally {
                lock.unlock();
            }
        }
        return false;
    }
}
```

### 2. Hierarchical Locking
```java
class HierarchicalLock {
    private final Lock[] locks;
    
    public void acquire(int level) {
        for (int i = 0; i <= level; i++) {
            locks[i].lock();
        }
    }
    
    public void release(int level) {
        for (int i = level; i >= 0; i--) {
            locks[i].unlock();
        }
    }
}
```

### 3. Resource Pool
```java
class ResourcePool {
    private final Semaphore semaphore;
    private final List<Resource> resources;
    
    public Resource acquire() throws InterruptedException {
        semaphore.acquire();
        synchronized (resources) {
            return resources.remove(0);
        }
    }
    
    public void release(Resource resource) {
        synchronized (resources) {
            resources.add(resource);
        }
        semaphore.release();
    }
}
```

## Common Pitfalls

### 1. Lock Ordering
- Inconsistent lock ordering
- Nested locks
- Circular dependencies

### 2. Resource Management
- Resource leaks
- Lock timeouts
- Starvation

### 3. Design Issues
- Over-synchronization
- Complex locking
- Poor resource hierarchy

## Tools and Resources

### 1. Analysis Tools
- Thread analyzers
- Deadlock detectors
- Performance profilers

### 2. Libraries
- Lock implementations
- Resource management
- Concurrency utilities

## Practice Problems

### 1. Bank Transfer
```java
// Implement deadlock-free bank transfer
class BankAccount {
    private double balance;
    
    public void transfer(BankAccount other, double amount) {
        // Implement transfer
    }
}
```

### 2. Resource Manager
```java
// Implement deadlock-free resource manager
class ResourceManager {
    private List<Resource> resources;
    
    public void acquireResources(List<Resource> required) {
        // Implement resource acquisition
    }
}
```

## Interview Questions

1. What is a deadlock and how can it be prevented?
2. Explain different deadlock prevention techniques.
3. How do you handle resource acquisition in concurrent code?
4. What are the trade-offs between different locking strategies?
5. How do you test for deadlocks?

## Summary
- Deadlocks occur in circular wait situations
- Use consistent lock ordering
- Implement timeout mechanisms
- Follow resource hierarchy
- Use appropriate tools for detection
- Regular testing and monitoring
- Performance considerations
- Error prevention 