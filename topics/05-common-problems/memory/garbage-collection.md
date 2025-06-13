# Garbage Collection

## Overview
Garbage collection is an automatic memory management mechanism that reclaims memory occupied by objects that are no longer in use. This guide covers different garbage collection algorithms, their characteristics, and best practices for working with garbage-collected languages.

## Types of Garbage Collection

### 1. Reference Counting
```python
# Python's reference counting example
class Node:
    def __init__(self):
        self.next = None

# Objects are automatically collected when reference count reaches 0
node1 = Node()
node2 = Node()
node1.next = node2
node2.next = node1
# Circular reference - potential memory leak
```

### 2. Mark and Sweep
```java
// Java's mark and sweep example
class Node {
    Node next;
}

void process() {
    Node node1 = new Node();
    Node node2 = new Node();
    node1.next = node2;
    node2.next = node1;
    // Objects will be collected by mark and sweep
}
```

### 3. Generational Collection
```java
// Java's generational collection example
class Data {
    byte[] largeArray = new byte[1000000];
}

void process() {
    Data data = new Data();
    // Object likely to be collected in young generation
    data = null;
}
```

## Garbage Collection Algorithms

### 1. Copying Collector
```java
// Example of objects that would be handled by copying collector
class SmallObject {
    int value;
}

void process() {
    SmallObject obj = new SmallObject();
    // Object copied to new space during collection
}
```

### 2. Mark-Compact Collector
```java
// Example of objects that would be compacted
class LargeObject {
    byte[] data = new byte[1000000];
}

void process() {
    LargeObject obj = new LargeObject();
    // Object marked and compacted during collection
}
```

### 3. Incremental Collector
```java
// Example of objects handled by incremental collection
class ComplexObject {
    List<Object> children = new ArrayList<>();
}

void process() {
    ComplexObject obj = new ComplexObject();
    // Object processed incrementally
}
```

## Best Practices

### 1. Object Lifecycle Management
```java
// Good practice
class Resource {
    private boolean isClosed = false;
    
    public void close() {
        if (!isClosed) {
            // Cleanup
            isClosed = true;
        }
    }
    
    protected void finalize() {
        close();
    }
}
```

### 2. Memory Leak Prevention
```java
// Bad practice
class Cache {
    private static Map<String, Object> cache = new HashMap<>();
    
    public void add(String key, Object value) {
        cache.put(key, value);
    }
}

// Good practice
class Cache {
    private static WeakHashMap<String, Object> cache = new WeakHashMap<>();
    
    public void add(String key, Object value) {
        cache.put(key, value);
    }
}
```

### 3. Large Object Handling
```java
// Good practice
class LargeDataProcessor {
    private static final int CHUNK_SIZE = 1024 * 1024;
    
    public void process(byte[] data) {
        for (int i = 0; i < data.length; i += CHUNK_SIZE) {
            int size = Math.min(CHUNK_SIZE, data.length - i);
            processChunk(data, i, size);
        }
    }
}
```

## Common Patterns

### 1. Object Pooling
```java
class ObjectPool<T> {
    private List<T> pool = new ArrayList<>();
    
    public T acquire() {
        if (pool.isEmpty()) {
            return createNew();
        }
        return pool.remove(pool.size() - 1);
    }
    
    public void release(T obj) {
        pool.add(obj);
    }
}
```

### 2. Weak References
```java
class Cache<K, V> {
    private Map<K, WeakReference<V>> cache = new HashMap<>();
    
    public void put(K key, V value) {
        cache.put(key, new WeakReference<>(value));
    }
    
    public V get(K key) {
        WeakReference<V> ref = cache.get(key);
        return ref != null ? ref.get() : null;
    }
}
```

### 3. Finalization
```java
class Resource implements AutoCloseable {
    private boolean isClosed = false;
    
    @Override
    public void close() {
        if (!isClosed) {
            // Cleanup
            isClosed = true;
        }
    }
    
    protected void finalize() {
        close();
    }
}
```

## Common Pitfalls

### 1. Memory Leaks
- Circular references
- Unclosed resources
- Large object retention

### 2. Performance Issues
- Frequent collections
- Large object allocation
- Finalization overhead

### 3. Resource Management
- Unclosed streams
- Unreleased locks
- Unclosed connections

## Tools and Resources

### 1. Profiling Tools
- JVisualVM
- YourKit
- JProfiler

### 2. Monitoring Tools
- GC logs
- Memory analyzers
- Heap dumps

## Practice Problems

### 1. Memory Leak Detection
```java
// Find and fix the memory leak
class EventManager {
    private static List<EventListener> listeners = new ArrayList<>();
    
    public void addListener(EventListener listener) {
        listeners.add(listener);
    }
}
```

### 2. Resource Management
```java
// Implement proper resource management
class FileProcessor {
    private FileInputStream stream;
    
    public void process(String filename) {
        stream = new FileInputStream(filename);
        // Process file
    }
}
```

## Interview Questions

1. Explain different garbage collection algorithms.
2. How does generational garbage collection work?
3. What are weak references and when should they be used?
4. How do you prevent memory leaks in garbage-collected languages?
5. What are the trade-offs between different garbage collection strategies?

## Summary
- Garbage collection automates memory management
- Different algorithms for different scenarios
- Proper object lifecycle management
- Resource cleanup and finalization
- Memory leak prevention
- Performance optimization
- Monitoring and profiling
- Best practices for garbage-collected languages 