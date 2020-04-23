# JUMPER BALL GAME

This project was started in October 2019, there is no a beautiful visual rendering for now.
The core of the project (physics,maps) is currently under development.


## Install requirements

- **CMake >= 3.2** to generate the solution : [Download CMake](https://cmake.org/download/)


**Specifically for Windows** 


- **Microsoft Visual Studio 2017/2019** or **MinGW**


## Generation of the solution

- Checkout this repository's master branch:

  ```sh
  git clone https://github.com/zephyrol/JumperBall.git
  ```

**Specifically for Windows** 

- Run Cmake-gui once, select the repo root as a source directory, `"repoRoot"/build/` as the build directory. Configure, select the Visual Studio 2019, Visual Studio 2017 or MinGW Makesfiles generetor 
- Generate


**Specifically for Unix/Linux** 

- Go to the repo root directory and create a `build` directory
  ```
  mkdir build
  ```

- Go to the `build` dir
  ```
  cd build
  ```

- run cmake specifying the parent directory containing sources
  ```
  cmake ..
  ```


## Compilation


**Specifically for Windows** 

If you use **Visual Studio**:

- Open the generated Visual Studio solution (`build/JumperBallApplication.sln`)
- Build the `ALL_BUILD` target, and then the `INSTALL` target
- Select the JumperBallApplication as startup project and run it


If you use a **MinGW Makefile**:

- Go to your build directory with the terminal (use cmd or cygwin)
- Build and install the project
  ```
  cmake --build . --target install 
  ```

- Run the project
  ```
  JumperBallApplication.exe
  ```


**Specifically for Unix/Linux** 

- Build the solution
  ```
  make -j 4
  ```

- Install the project
  ```
  make install
  ```

- Run the project
  ```
  ./JumperBallApplication
  ```

## How to play

- Use the arrows to move and Space (or Enter ) to jump.

