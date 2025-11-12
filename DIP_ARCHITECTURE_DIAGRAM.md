# DIP Architecture Diagram

## Before DIP (Tight Coupling)
```
┌──────────────────┐
│      Ball        │
│  (Core Logic)    │
└────────┬─────────┘
         │ depends on (concrete)
         ↓
┌──────────────────┐
│ ClassicalMechanics│
│   (Physics Lib)  │
└──────────────────┘

Problem: Core depends on concrete implementation
```

## After DIP (Loose Coupling)
```
                         ┌─────────────────────┐
                         │      main.cpp       │
                         │ (Composition Root)  │
                         │   - Creates         │
                         │     Factory         │
                         └──────────┬──────────┘
                                    │
                                    │ injects
                                    ↓
┌────────────────────────────────────────────────────┐
│                   Ball (Core Logic)                 │
│   - Uses IPhysicsEngineFactory                      │
│   - Creates physics via factory                     │
└────────────────────┬───────────────────────────────┘
                     │
                     │ depends on (abstraction)
                     ↓
┌────────────────────────────────────────────────────┐
│            IPhysicsEngine (Interface)               │
│   + getJumpDistance()                               │
│   + getVelocity(t)                                  │
│   + getPosition(t)                                  │
│   + getTimeToGetDestination()                       │
└────────────────────┬───────────────────────────────┘
                     ↑
                     │ implements
                     │
┌────────────────────┴───────────────────────────────┐
│         ClassicalMechanicsAdapter (Adapter)         │
│   - Wraps ClassicalMechanics                        │
│   - Implements IPhysicsEngine                       │
└────────────────────┬───────────────────────────────┘
                     │
                     │ uses
                     ↓
┌────────────────────────────────────────────────────┐
│         ClassicalMechanics (Physics Library)        │
│   - Concrete implementation                         │
│   - External dependency                             │
└────────────────────────────────────────────────────┘

Benefits:
✓ Core logic depends on abstraction
✓ Easy to swap implementations
✓ Testable (can mock physics)
✓ Follows Clean Architecture
```

## Factory Pattern
```
┌──────────────────────────────────────────────┐
│    IPhysicsEngineFactory (Interface)         │
│  + createPhysicsEngine(params)               │
└──────────────────┬───────────────────────────┘
                   ↑
                   │ implements
                   │
┌──────────────────┴───────────────────────────┐
│      ClassicalMechanicsFactory               │
│  + createPhysicsEngine(params)               │
│    → returns ClassicalMechanicsAdapter       │
└──────────────────────────────────────────────┘

Factory created in main.cpp and injected through:
main → Window → Controller → Scene → MapGenerator → Ball
```

## Key Principles Applied

1. **Dependency Inversion Principle (DIP)**
   - High-level modules (Ball) don't depend on low-level modules (ClassicalMechanics)
   - Both depend on abstractions (IPhysicsEngine)

2. **Dependency Injection**
   - Dependencies are provided from outside (via constructor)
   - main.cpp is the composition root

3. **Adapter Pattern**
   - ClassicalMechanicsAdapter wraps existing ClassicalMechanics
   - Adapts it to IPhysicsEngine interface

4. **Abstract Factory Pattern**
   - IPhysicsEngineFactory defines creation interface
   - ClassicalMechanicsFactory provides concrete creation logic

5. **Clean Architecture**
   - Dependencies point inward (toward domain)
   - Core is independent of frameworks/libraries

