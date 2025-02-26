# Copy Tool Net

This project contains a client and server application for copying files over a network.

## Prerequisites

- C++ compiler (e.g., g++, clang++)
- CMake
- Boost libraries

## Building the Project

1. Clone the repository:
    ```sh
    git clone https://github.com/asharypau/copy-tool-net.git
    cd copy-tool-net
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Override CLIENT_STORAGE_PATH variable

4. Build the project:
    ```sh
    cmake --build . --config Release
    ```

## Running the Server

1. Run
    ```sh
    CopyToolNet.exe port=8080 host=127.0.0.1 app_type=1
    ```

## Running the Client

1. Run
    ```sh
    CopyToolNet.exe port=8080 host=127.0.0.1 app_type=2
    ```
