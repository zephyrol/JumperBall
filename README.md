# GRAVITY GLOBE GAME

**Gravity globe** (formerly Jumper Ball) is a 3D video game.
It works on **Windows**, **Unix/Linux** and **macOS**.


## Install requirements

- **Conan >= 2** to build third party libraries : [Install Conan](https://docs.conan.io/2/installation.html)

- **CMake >= 3.20** to generate the solution : [Install CMake](https://cmake.org/download/)

## Generation of the solution

- Download the project as zip file or checkout this repository's master branch:

  ```sh
  git clone https://github.com/zephyrol/JumperBall.git
  ```

- In the root project folder, run **Conan**:

  ```
  conan install . --output-folder=build --build=missing -s build_type=Release
  ```

  If you wish, you can switch the **build_type** to **Debug** ```build_type=Debug```

- The dependencies are installed now. You can run **CMake**:
  ```
  cmake -G YOUR_GENERATOR --preset conan-release
  ```

  or if you have set your conan profile on **Debug**, run
  ```
  cmake -G YOUR_GENERATOR --preset conan-debug
  ```

Replace **YOUR_GENERATOR** by the generator you would like to use. Here is a list of examples:

- **"Visual Studio 15 2017"** or **"Visual Studio 16 2019"** on **Windows**
- "**Unix Makefiles**" on **Linux**
- "**Unix Makefiles**", **Xcode** on **macOS**

### Commands examples: 
- Windows: 
```
cmake -G "Visual Studio 15 2017" --preset conan-release
```

- macOS:
```
cmake -G Xcode --preset conan-release
```

- Linux:

On Linux (and on macOS), **Unix Makefiles** are usually used by default, you don't need to specify it:
```
cmake --preset conan-release
```

## Compilation of the solution

- Go to the build directories.

If you use **Visual Studio**:

- Open the generated Visual Studio solution
- Build the `ALL_BUILD` target, and then the `INSTALL` target
- Select `GravityGlobe` as startup project and run it

Don't forget to set the build to **Release** or **Debug** depending on your initial choice.

If you use **Unix Makefiles**:

- Build and install the project using:

  ```
  make -j 8 install
  ```

- Run the project
  ```
  ./GravityGlobe
  ```


If you use **Xcode**:
- Open the **Xcode** solution
- Build the `ALL_BUILD` target, and then the `install` target
- Select the **GravityGlobe** project and run it


Don't forget to set the build to **Release** or **Debug** depending on your initial choice.


## How to play

### Keyboard
- Use the **arrows**, **HJKL** to move and **Space** or **Enter** to jump.

### Mouse 
- **Click** on the screen to Jump, **press and swipe up** to go ahead, **press and swipe left or right** to turn.
