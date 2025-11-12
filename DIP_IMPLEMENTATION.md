# Dependency Inversion Principle (DIP) Applied to ClassicalMechanics

## Overview
This document describes how the Dependency Inversion Principle (DIP) has been applied to the ClassicalMechanics physics engine in the JumperBall project.

## Problem
Previously, the core game logic (Ball, MapGenerator, etc.) directly depended on the concrete `ClassicalMechanics` implementation from the `entities/physics` module. This created a tight coupling and violated the DIP.

## Solution Architecture

### 1. Abstraction Layer (Domain - Core Classes)
Created interface definitions in `coreClasses/physics/`:

- **IPhysicsEngine.h**: Abstract interface defining physics calculations contract
  - `getJumpDistance()`: Returns jump distance
  - `getVelocity(t)`: Returns velocity at time t
  - `getPosition(t)`: Returns position at time t
  - `getTimeToGetDestination()`: Returns time to destination
  - `timesShock()`: Sets shock times
  - `addShockFromPosition()`: Adds shock from position

- **IPhysicsEngineFactory.h**: Abstract factory for creating physics engine instances
  - Two factory methods for different physics configurations

### 2. Adapter Layer (Infrastructure)
Created adapters in `adapters/`:

- **ClassicalMechanicsAdapter.h/.cpp**: Wraps `ClassicalMechanics` to implement `IPhysicsEngine`
  - Translates between interface types and concrete implementation
  - Converts `ClassicalMechanics::physics2DVector` to `IPhysicsEngine::Vector2D`

- **ClassicalMechanicsFactory.h/.cpp**: Concrete factory implementing `IPhysicsEngineFactory`
  - Creates `ClassicalMechanicsAdapter` instances
  - Handles C++11 compatibility (uses `new` instead of `std::make_unique`)

### 3. Dependency Injection Chain
The factory is created in `main.cpp` and injected through the application layers:

```
main.cpp 
  └─> creates ClassicalMechanicsFactory
      └─> passes to Window
          └─> passes to Controller
              └─> passes to Scene
                  └─> passes to MapGenerator
                      └─> passes to Ball
```

### 4. Updated Classes

#### Ball (coreClasses/scene/Ball.h/.cpp)
- **Constructor**: Now accepts `CstIPhysicsEngineFactory_sptr`
- **Members**: Changed from `ClassicalMechanics` to `IPhysicsEngine_uptr`
  - `_mechanicsPatternJumping`
  - `_mechanicsPatternLongJumping`
  - `_mechanicsPatternFalling`
- **Methods**: Return `IPhysicsEngine&` instead of `ClassicalMechanics&`

#### MapGenerator (coreClasses/scene/MapGenerator.h/.cpp)
- **loadMap()**: Accepts `CstIPhysicsEngineFactory_sptr` parameter
- **uncompressMap()**: Accepts factory and passes it to Ball constructor

#### Scene (graphicsClasses/scene/Scene.h/.cpp)
- **Constructor**: Accepts `CstIPhysicsEngineFactory_sptr` parameter
- Passes factory to MapGenerator

#### Controller (graphicsClasses/controller/Controller.h/.cpp)
- **Constructor**: Accepts `CstIPhysicsEngineFactory_sptr` parameter
- **Member**: Stores `_physicsFactory` for creating new scenes
- Passes factory to Scene constructors

#### Window (application/Window.h/.cpp)
- **Constructor**: Accepts `CstIPhysicsEngineFactory_sptr` parameter
- Passes factory to Controller

#### main.cpp (application/main.cpp)
- Creates `ClassicalMechanicsFactory` instance
- Injects factory into Window constructor
- **This is the composition root where dependencies are wired**

## Benefits

1. **Loose Coupling**: Core game logic no longer depends on concrete physics implementation
2. **Testability**: Easy to create mock physics engines for testing
3. **Flexibility**: Can switch physics implementations without changing core code
4. **Clean Architecture**: Dependencies point inward (from infrastructure to domain)
5. **Single Responsibility**: Each layer has a clear responsibility

## Dependency Flow

```
┌─────────────────────────────────────────────────────┐
│              Application Layer (main.cpp)            │
│  - Creates concrete factory                          │
│  - Composition root                                  │
└──────────────────────┬──────────────────────────────┘
                       │ injects
                       ↓
┌─────────────────────────────────────────────────────┐
│         Presentation Layer (Window, Controller)      │
│  - Receives factory via constructor                  │
│  - Passes to domain                                  │
└──────────────────────┬──────────────────────────────┘
                       │ injects
                       ↓
┌─────────────────────────────────────────────────────┐
│      Domain Layer (Ball, MapGenerator, Scene)        │
│  - Depends on IPhysicsEngine interface               │
│  - Uses factory to create physics instances          │
│  - No knowledge of concrete implementation           │
└──────────────────────┬──────────────────────────────┘
                       │ depends on (abstraction)
                       ↓
┌─────────────────────────────────────────────────────┐
│          Domain Interfaces (IPhysicsEngine)          │
│  - Abstract interfaces                               │
│  - No dependencies                                   │
└──────────────────────┬──────────────────────────────┘
                       ↑ implements
                       │
┌─────────────────────────────────────────────────────┐
│   Infrastructure (ClassicalMechanicsAdapter)         │
│  - Implements interfaces                             │
│  - Wraps concrete ClassicalMechanics                 │
└──────────────────────┬──────────────────────────────┘
                       │ uses
                       ↓
┌─────────────────────────────────────────────────────┐
│    External Library (ClassicalMechanics)             │
│  - Concrete physics implementation                   │
│  - entities/physics module                           │
└─────────────────────────────────────────────────────┘
```

## Files Added

- `coreClasses/physics/IPhysicsEngine.h`
- `coreClasses/physics/IPhysicsEngineFactory.h`
- `adapters/ClassicalMechanicsAdapter.h`
- `adapters/ClassicalMechanicsAdapter.cpp`
- `adapters/ClassicalMechanicsFactory.h`
- `adapters/ClassicalMechanicsFactory.cpp`

## Files Modified

- `coreClasses/scene/Ball.h`
- `coreClasses/scene/Ball.cpp`
- `coreClasses/scene/MapGenerator.h`
- `coreClasses/scene/MapGenerator.cpp`
- `graphicsClasses/scene/Scene.h`
- `graphicsClasses/scene/Scene.cpp`
- `graphicsClasses/controller/Controller.h`
- `graphicsClasses/controller/Controller.cpp`
- `application/Window.h`
- `application/Window.cpp`
- `application/main.cpp`
- `CMakeLists.txt`

## Compilation

The project compiles successfully with the DIP implementation:
```bash
cmake .
make -j4
```

All changes are backward compatible with C++11 standard used by the project.

