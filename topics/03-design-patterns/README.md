# Design Patterns

## Overview
Design patterns are reusable solutions to common problems in software design. They represent best practices evolved over time and provide a shared vocabulary for developers to communicate effectively about software design.

## Categories

### Creational Patterns
Creational patterns deal with object creation mechanisms, trying to create objects in a manner suitable to the situation.

- [Abstract Factory](creational/abstract-factory.md) - Creates families of related objects
- [Builder](creational/builder.md) - Constructs complex objects step by step
- [Factory Method](creational/factory-method.md) - Creates objects without specifying exact class
- [Prototype](creational/prototype.md) - Creates new objects by cloning existing ones
- [Singleton](creational/singleton.md) - Ensures a class has only one instance
- [Creational Patterns Summary](creational/summary.md)

### Structural Patterns
Structural patterns deal with object composition, creating relationships between objects to form larger structures.

- [Adapter](structural/adapter.md) - Makes incompatible interfaces work together
- [Bridge](structural/bridge.md) - Separates abstraction from implementation
- [Composite](structural/composite.md) - Composes objects into tree structures
- [Decorator](structural/decorator.md) - Adds responsibilities to objects dynamically
- [Facade](structural/facade.md) - Provides a simplified interface to a complex subsystem
- [Flyweight](structural/flyweight.md) - Shares objects to reduce memory usage
- [Proxy](structural/proxy.md) - Controls access to another object
- [Structural Patterns Summary](structural/summary.md)

### Behavioral Patterns
Behavioral patterns deal with object interaction and responsibility distribution.

- [Chain of Responsibility](behavioral/chain-of-responsibility.md) - Passes requests along a chain of handlers
- [Command](behavioral/command.md) - Encapsulates a request as an object
- [Interpreter](behavioral/interpreter.md) - Implements a specialized language
- [Iterator](behavioral/iterator.md) - Accesses elements of a collection sequentially
- [Mediator](behavioral/mediator.md) - Defines simplified communication between classes
- [Memento](behavioral/memento.md) - Captures and restores object state
- [Observer](behavioral/observer.md) - Defines one-to-many dependency between objects
- [State](behavioral/state.md) - Allows an object to alter its behavior when its state changes
- [Strategy](behavioral/strategy.md) - Defines a family of algorithms
- [Template Method](behavioral/template-method.md) - Defines the skeleton of an algorithm
- [Visitor](behavioral/visitor.md) - Separates an algorithm from an object structure
- [Behavioral Patterns Summary](behavioral/summary.md)

## Best Practices
1. Choose patterns based on the problem, not the other way around
2. Understand the pattern's intent and applicability
3. Consider the trade-offs and alternatives
4. Keep implementations simple and focused
5. Document pattern usage and relationships

## Common Pitfalls
1. Over-engineering with patterns
2. Using patterns where simple solutions suffice
3. Not understanding pattern relationships
4. Implementing patterns incorrectly
5. Ignoring maintenance implications

## Resources
- [Design Patterns: Elements of Reusable Object-Oriented Software](https://www.amazon.com/Design-Patterns-Elements-Reusable-Object-Oriented/dp/0201633612) (Gang of Four)
- [Head First Design Patterns](https://www.amazon.com/Head-First-Design-Patterns-Brain-Friendly/dp/149207800X)
- [Refactoring Guru](https://refactoring.guru/design-patterns)
- [Source Making](https://sourcemaking.com/design_patterns) 