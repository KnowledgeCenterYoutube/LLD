# Memory Fragmentation

## Overview
Memory fragmentation occurs when free memory is broken into small, non-contiguous blocks, making it difficult to allocate larger chunks of memory even when sufficient total free memory exists. This guide covers the types, causes, and prevention of memory fragmentation.

## Types of Fragmentation

### 1. External Fragmentation
```cpp
// Example of external fragmentation
class MemoryManager {
    private:
        struct Block {
            size_t size;
            bool isFree;
            Block* next;
        };
        
        Block* head;
        
    public:
        // After multiple allocations and deallocations
        // Free memory might look like:
        // [Used][Free][Used][Free][Used][Free]
        // Even with enough total free space, can't allocate
        // a large contiguous block
};
```

### 2. Internal Fragmentation
```cpp
// Example of internal fragmentation
class MemoryAllocator {
    private:
        static const size_t BLOCK_SIZE = 64; // Fixed block size
        
    public:
        void* allocate(size_t size) {
            // Even if user requests 20 bytes,
            // we allocate 64 bytes, wasting 44 bytes
            return new char[BLOCK_SIZE];
        }
};
```

## Causes of Fragmentation

### 1. Variable-Size Allocations
```cpp
// Bad: Frequent variable-size allocations
class DynamicArray {
    private:
        int* data;
        size_t size;
        
    public:
        void add(int value) {
            int* newData = new int[size + 1];
            // Copy old data
            delete[] data;
            data = newData;
            // Frequent reallocations cause fragmentation
        }
};

// Good: Pre-allocate with growth factor
class DynamicArray {
    private:
        int* data;
        size_t size;
        size_t capacity;
        
    public:
        void add(int value) {
            if (size == capacity) {
                capacity *= 2; // Double capacity
                int* newData = new int[capacity];
                // Copy old data
                delete[] data;
                data = newData;
            }
            // Less frequent reallocations
        }
};
```

### 2. Memory Release Patterns
```cpp
// Bad: Scattered deallocations
class ObjectPool {
    private:
        std::vector<Object*> objects;
        
    public:
        void release(Object* obj) {
            // Objects released in random order
            delete obj;
        }
};

// Good: Batch deallocation
class ObjectPool {
    private:
        std::vector<Object*> objects;
        
    public:
        void releaseAll() {
            // Release objects in batch
            for (auto obj : objects) {
                delete obj;
            }
            objects.clear();
        }
};
```

## Prevention Techniques

### 1. Memory Pooling
```cpp
// Memory pool implementation
class MemoryPool {
    private:
        struct Block {
            Block* next;
        };
        
        Block* freeList;
        char* memory;
        size_t blockSize;
        
    public:
        MemoryPool(size_t blockSize, size_t numBlocks) {
            this->blockSize = blockSize;
            memory = new char[blockSize * numBlocks];
            freeList = nullptr;
            
            // Initialize free list
            for (size_t i = 0; i < numBlocks; i++) {
                Block* block = (Block*)(memory + i * blockSize);
                block->next = freeList;
                freeList = block;
            }
        }
        
        void* allocate() {
            if (freeList == nullptr) return nullptr;
            
            Block* block = freeList;
            freeList = block->next;
            return block;
        }
        
        void deallocate(void* ptr) {
            Block* block = (Block*)ptr;
            block->next = freeList;
            freeList = block;
        }
};
```

### 2. Slab Allocation
```cpp
// Slab allocator implementation
class SlabAllocator {
    private:
        struct Slab {
            char* memory;
            size_t used;
            Slab* next;
        };
        
        Slab* slabs;
        size_t objectSize;
        
    public:
        void* allocate() {
            // Find or create slab with free space
            Slab* slab = findFreeSlab();
            if (slab == nullptr) {
                slab = createNewSlab();
            }
            
            // Allocate from slab
            void* ptr = slab->memory + (slab->used * objectSize);
            slab->used++;
            return ptr;
        }
};
```

### 3. Defragmentation
```cpp
// Memory defragmentation
class Defragmenter {
    private:
        struct MemoryBlock {
            void* address;
            size_t size;
            bool isUsed;
        };
        
        std::vector<MemoryBlock> blocks;
        
    public:
        void defragment() {
            // Sort blocks by address
            std::sort(blocks.begin(), blocks.end());
            
            // Compact used blocks
            for (size_t i = 1; i < blocks.size(); i++) {
                if (blocks[i].isUsed) {
                    // Move block to fill gap
                    moveBlock(&blocks[i]);
                }
            }
        }
};
```

## Best Practices

### 1. Allocation Strategies
- Use fixed-size blocks
- Implement memory pools
- Pre-allocate memory
- Use growth factors

### 2. Release Patterns
- Batch deallocations
- Maintain allocation order
- Use smart pointers
- Implement reference counting

### 3. Monitoring
- Track memory usage
- Monitor fragmentation
- Set up alerts
- Regular defragmentation

## Common Patterns

### 1. Object Pool
```cpp
template<typename T>
class ObjectPool {
    private:
        std::vector<T*> freeObjects;
        std::vector<T*> usedObjects;
        
    public:
        T* acquire() {
            if (freeObjects.empty()) {
                return new T();
            }
            
            T* obj = freeObjects.back();
            freeObjects.pop_back();
            usedObjects.push_back(obj);
            return obj;
        }
        
        void release(T* obj) {
            auto it = std::find(usedObjects.begin(), usedObjects.end(), obj);
            if (it != usedObjects.end()) {
                usedObjects.erase(it);
                freeObjects.push_back(obj);
            }
        }
};
```

### 2. Memory Arena
```cpp
class MemoryArena {
    private:
        char* memory;
        size_t size;
        size_t used;
        
    public:
        MemoryArena(size_t size) {
            memory = new char[size];
            this->size = size;
            used = 0;
        }
        
        void* allocate(size_t size) {
            if (used + size > this->size) {
                return nullptr;
            }
            
            void* ptr = memory + used;
            used += size;
            return ptr;
        }
        
        void reset() {
            used = 0;
        }
};
```

## Common Pitfalls

### 1. Allocation Issues
- Frequent small allocations
- Variable-size allocations
- Scattered deallocations
- Memory leaks

### 2. Design Problems
- Poor memory layout
- Inefficient data structures
- Lack of pooling
- No defragmentation

### 3. Performance Impact
- Allocation overhead
- Cache misses
- Page faults
- System calls

## Tools and Resources

### 1. Analysis Tools
- Memory profilers
- Fragmentation analyzers
- Performance monitors
- Debugging tools

### 2. Libraries
- Memory allocators
- Object pools
- Smart pointers
- Memory management utilities

## Practice Problems

### 1. Memory Pool
```cpp
// Implement a memory pool
class MemoryPool {
    private:
        char* memory;
        size_t blockSize;
        size_t numBlocks;
        
    public:
        void* allocate() {
            // Implement allocation
        }
        
        void deallocate(void* ptr) {
            // Implement deallocation
        }
};
```

### 2. Defragmenter
```cpp
// Implement a memory defragmenter
class Defragmenter {
    private:
        struct Block {
            void* address;
            size_t size;
            bool isUsed;
        };
        
    public:
        void defragment() {
            // Implement defragmentation
        }
};
```

## Interview Questions

1. What is memory fragmentation and how does it occur?
2. Explain different types of memory fragmentation.
3. How do you prevent memory fragmentation?
4. What are the trade-offs between different memory allocation strategies?
5. How do you handle memory fragmentation in your applications?

## Summary
- Memory fragmentation affects memory utilization
- Use appropriate allocation strategies
- Implement memory pools and arenas
- Follow best practices for memory management
- Monitor and defragment when necessary
- Consider performance implications
- Use appropriate tools
- Regular maintenance 