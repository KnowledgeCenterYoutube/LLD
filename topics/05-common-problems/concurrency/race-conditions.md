# Race Conditions

## Overview
Race conditions occur when two or more threads access shared data simultaneously, and the final outcome depends on the timing of thread execution. This guide covers the causes, detection, and prevention of race conditions in concurrent programming.

## Causes of Race Conditions

### 1. Shared Resource Access
```java
// Bad: Race condition in counter
class Counter {
    private int count = 0;
    
    public void increment() {
        count++; // Not thread-safe
    }
    
    public int getCount() {
        return count;
    }
}

// Good: Thread-safe counter
class Counter {
    private AtomicInteger count = new AtomicInteger(0);
    
    public void increment() {
        count.incrementAndGet();
    }
    
    public int getCount() {
        return count.get();
    }
}
```

### 2. Check-Then-Act Pattern
```java
// Bad: Race condition in singleton
class Singleton {
    private static Singleton instance;
    
    public static Singleton getInstance() {
        if (instance == null) { // Race condition here
            instance = new Singleton();
        }
        return instance;
    }
}

// Good: Thread-safe singleton
class Singleton {
    private static volatile Singleton instance;
    private static final Object lock = new Object();
    
    public static Singleton getInstance() {
        if (instance == null) {
            synchronized (lock) {
                if (instance == null) {
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```

### 3. Compound Operations
```java
// Bad: Race condition in list operations
class ListManager {
    private List<String> items = new ArrayList<>();
    
    public void addIfAbsent(String item) {
        if (!items.contains(item)) { // Race condition
            items.add(item);
        }
    }
}

// Good: Thread-safe list operations
class ListManager {
    private List<String> items = Collections.synchronizedList(new ArrayList<>());
    
    public void addIfAbsent(String item) {
        synchronized (items) {
            if (!items.contains(item)) {
                items.add(item);
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
- Thread analyzers
- Race condition detectors
- Runtime monitoring

### 3. Testing
- Stress testing
- Concurrency testing
- Unit testing with multiple threads

## Prevention Techniques

### 1. Synchronization
```java
// Using synchronized methods
class BankAccount {
    private double balance;
    
    public synchronized void deposit(double amount) {
        balance += amount;
    }
    
    public synchronized void withdraw(double amount) {
        balance -= amount;
    }
}

// Using synchronized blocks
class BankAccount {
    private double balance;
    private final Object lock = new Object();
    
    public void deposit(double amount) {
        synchronized (lock) {
            balance += amount;
        }
    }
}
```

### 2. Atomic Operations
```java
// Using atomic variables
class Counter {
    private AtomicInteger count = new AtomicInteger(0);
    
    public void increment() {
        count.incrementAndGet();
    }
    
    public void add(int value) {
        count.addAndGet(value);
    }
}
```

### 3. Immutable Objects
```java
// Using immutable objects
@Immutable
class Transaction {
    private final String id;
    private final double amount;
    private final LocalDateTime timestamp;
    
    public Transaction(String id, double amount) {
        this.id = id;
        this.amount = amount;
        this.timestamp = LocalDateTime.now();
    }
}
```

## Best Practices

### 1. Thread Safety
- Use thread-safe collections
- Implement proper synchronization
- Avoid shared mutable state

### 2. Design Patterns
- Immutable objects
- Thread-local storage
- Producer-consumer pattern

### 3. Testing
- Write concurrent tests
- Use stress testing
- Monitor thread behavior

## Common Patterns

### 1. Producer-Consumer
```java
class ProducerConsumer {
    private BlockingQueue<Item> queue = new LinkedBlockingQueue<>();
    
    public void produce(Item item) throws InterruptedException {
        queue.put(item);
    }
    
    public Item consume() throws InterruptedException {
        return queue.take();
    }
}
```

### 2. Read-Write Lock
```java
class DataStore {
    private final ReadWriteLock lock = new ReentrantReadWriteLock();
    private Map<String, String> data = new HashMap<>();
    
    public String read(String key) {
        lock.readLock().lock();
        try {
            return data.get(key);
        } finally {
            lock.readLock().unlock();
        }
    }
    
    public void write(String key, String value) {
        lock.writeLock().lock();
        try {
            data.put(key, value);
        } finally {
            lock.writeLock().unlock();
        }
    }
}
```

### 3. Thread Pool
```java
class TaskProcessor {
    private ExecutorService executor = Executors.newFixedThreadPool(4);
    
    public void processTasks(List<Task> tasks) {
        for (Task task : tasks) {
            executor.submit(() -> task.execute());
        }
    }
}
```

## Common Pitfalls

### 1. Synchronization Issues
- Missing synchronization
- Over-synchronization
- Deadlocks

### 2. Visibility Problems
- Stale data
- Memory consistency
- Cache coherency

### 3. Performance Impact
- Lock contention
- Thread overhead
- Resource consumption

## Tools and Resources

### 1. Analysis Tools
- Thread analyzers
- Race condition detectors
- Performance profilers

### 2. Libraries
- Concurrent collections
- Atomic variables
- Synchronization utilities

## Practice Problems

### 1. Thread-Safe Counter
```java
// Implement a thread-safe counter
class Counter {
    private int count = 0;
    
    public void increment() {
        // Implement thread-safe increment
    }
    
    public int getCount() {
        // Implement thread-safe get
    }
}
```

### 2. Producer-Consumer
```java
// Implement a thread-safe producer-consumer queue
class Queue<T> {
    private List<T> items = new ArrayList<>();
    
    public void produce(T item) {
        // Implement thread-safe produce
    }
    
    public T consume() {
        // Implement thread-safe consume
    }
}
```

## Interview Questions

1. What is a race condition and how can it be prevented?
2. Explain different synchronization mechanisms.
3. How do you ensure thread safety in your code?
4. What are atomic operations and when should they be used?
5. How do you test for race conditions?

## Summary
- Race conditions occur in concurrent access to shared resources
- Use proper synchronization mechanisms
- Implement thread-safe designs
- Follow best practices for concurrency
- Use appropriate tools for detection
- Regular testing and monitoring
- Performance considerations
- Error prevention 