# Thread Safety

## Overview
Thread safety ensures that a program's behavior is correct when multiple threads access shared data simultaneously. This guide covers the principles, techniques, and best practices for writing thread-safe code.

## Thread Safety Principles

### 1. Immutability
```java
// Thread-safe immutable class
@Immutable
class Point {
    private final int x;
    private final int y;
    
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
    
    public int getX() { return x; }
    public int getY() { return y; }
    
    public Point move(int dx, int dy) {
        return new Point(x + dx, y + dy);
    }
}
```

### 2. Thread Confinement
```java
// Thread-local storage
class UserContext {
    private static final ThreadLocal<User> currentUser = new ThreadLocal<>();
    
    public static void setUser(User user) {
        currentUser.set(user);
    }
    
    public static User getUser() {
        return currentUser.get();
    }
    
    public static void clear() {
        currentUser.remove();
    }
}
```

### 3. Synchronization
```java
// Thread-safe counter
class Counter {
    private final AtomicInteger count = new AtomicInteger(0);
    
    public void increment() {
        count.incrementAndGet();
    }
    
    public int getCount() {
        return count.get();
    }
}
```

## Implementation Techniques

### 1. Atomic Operations
```java
// Using atomic operations
class BankAccount {
    private final AtomicLong balance = new AtomicLong(0);
    
    public void deposit(long amount) {
        balance.addAndGet(amount);
    }
    
    public boolean withdraw(long amount) {
        while (true) {
            long current = balance.get();
            if (current < amount) {
                return false;
            }
            if (balance.compareAndSet(current, current - amount)) {
                return true;
            }
        }
    }
}
```

### 2. Locking
```java
// Using explicit locks
class DataStore {
    private final ReadWriteLock lock = new ReentrantReadWriteLock();
    private Map<String, String> data = new HashMap<>();
    
    public String get(String key) {
        lock.readLock().lock();
        try {
            return data.get(key);
        } finally {
            lock.readLock().unlock();
        }
    }
    
    public void put(String key, String value) {
        lock.writeLock().lock();
        try {
            data.put(key, value);
        } finally {
            lock.writeLock().unlock();
        }
    }
}
```

### 3. Concurrent Collections
```java
// Using concurrent collections
class Cache {
    private final ConcurrentHashMap<String, Object> cache = new ConcurrentHashMap<>();
    
    public Object get(String key) {
        return cache.get(key);
    }
    
    public void put(String key, Object value) {
        cache.put(key, value);
    }
    
    public Object computeIfAbsent(String key, Function<String, Object> mappingFunction) {
        return cache.computeIfAbsent(key, mappingFunction);
    }
}
```

## Best Practices

### 1. State Management
- Minimize shared state
- Use immutable objects
- Thread confinement

### 2. Synchronization
- Use appropriate locks
- Minimize lock scope
- Avoid nested locks

### 3. Design Patterns
- Thread-safe collections
- Atomic operations
- Concurrent utilities

## Common Patterns

### 1. Double-Checked Locking
```java
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

### 2. Producer-Consumer
```java
class ProducerConsumer {
    private final BlockingQueue<Item> queue = new LinkedBlockingQueue<>();
    
    public void produce(Item item) throws InterruptedException {
        queue.put(item);
    }
    
    public Item consume() throws InterruptedException {
        return queue.take();
    }
}
```

### 3. Thread Pool
```java
class TaskProcessor {
    private final ExecutorService executor = Executors.newFixedThreadPool(4);
    
    public void processTasks(List<Task> tasks) {
        for (Task task : tasks) {
            executor.submit(() -> task.execute());
        }
    }
}
```

## Common Pitfalls

### 1. Visibility Issues
- Stale data
- Memory consistency
- Cache coherency

### 2. Synchronization Problems
- Race conditions
- Deadlocks
- Livelocks

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

### 1. Thread-Safe Stack
```java
// Implement a thread-safe stack
class Stack<T> {
    private List<T> elements = new ArrayList<>();
    
    public void push(T element) {
        // Implement thread-safe push
    }
    
    public T pop() {
        // Implement thread-safe pop
    }
}
```

### 2. Concurrent Cache
```java
// Implement a thread-safe cache
class Cache<K, V> {
    private Map<K, V> data = new HashMap<>();
    
    public V get(K key) {
        // Implement thread-safe get
    }
    
    public void put(K key, V value) {
        // Implement thread-safe put
    }
}
```

## Interview Questions

1. What is thread safety and how do you achieve it?
2. Explain different synchronization mechanisms.
3. How do you handle shared state in concurrent code?
4. What are the trade-offs between different thread safety approaches?
5. How do you test thread-safe code?

## Summary
- Thread safety ensures correct behavior in concurrent execution
- Use appropriate synchronization mechanisms
- Follow thread safety principles
- Implement best practices
- Use appropriate tools
- Regular testing and monitoring
- Performance considerations
- Error prevention 