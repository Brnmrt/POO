# Desert Caravans: A C++ Object-Oriented Simulation & Strategy Game

[](https://isocpp.org/)
[](https://cmake.org/)
[](LICENSE.md)

Welcome to Desert Caravans, a sophisticated turn-based strategy game developed in C++. This project is a deep dive into object-oriented design, modern C++ practices, and complex system simulation. Players are tasked with managing a fleet of caravans, navigating a treacherous desert, engaging in strategic combat, and building a mercantile empire.

This repository is not just a game; it's a comprehensive showcase of software engineering skills, from architectural design to memory management and the implementation of classic design patterns.

## 🚀 Project Vision

The goal of Desert Caravans was to create a highly modular and extensible simulation engine. The game world is dynamic, with emergent behaviors arising from the interaction of various systems. The project emphasizes clean code, robust design, and the practical application of advanced C++ concepts, making it an ideal portfolio piece for demonstrating technical expertise.

## ✨ Features

This simulation offers a rich and challenging gameplay experience, built upon a powerful set of technical features:

#### Core Gameplay

  * **Dynamic, Spherical World**: Explore a grid-based map where movement wraps around the edges, creating a seamless and continuous world.
  * **Diverse Caravan Types**: Command specialized units with unique strengths and weaknesses:
      * **Commercial Caravans**: Masters of trade with vast cargo holds.
      * **Military Caravans**: Formidable in combat, designed for protecting your assets and hunting enemies.
      * **Secret Caravans**: High-risk, high-reward units with unique, covert abilities like targeted destruction and stealth transport.
  * **Strategic Combat**: Engage in tactical, turn-based combat with rogue barbarian forces. Victory and defeat have lasting consequences on your resources and crew.
  * **Resource Management**: Juggle essential resources like water, crew, and currency to keep your caravans operational and your enterprise profitable.

#### Dynamic World Systems

  * **Player-Triggered Events**: Manipulate the game world by creating sandstorms that can devastate friend and foe alike.
  * **Procedural Item Spawning**: Discover powerful, magic-infused items that appear dynamically across the desert, each with a unique effect—from boons like the "Arca do Tesouro" (Treasure Chest) to curses like the "Caixa de Pandora" (Pandora's Box).
  * **Autonomous AI**: Face off against barbarian caravans that operate autonomously, hunting player caravans and adding a constant layer of threat and unpredictability.

#### Technical Features

  * **Command-Driven Interface**: A robust command-line interface allows for precise control over every aspect of the simulation, powered by a flexible Command design pattern.
  * **Custom Console Buffer**: A custom-built, in-memory screen buffer ensures clean, flicker-free rendering of the game state directly to the console, showcasing low-level control over output streams.
  * **Visual State Management**: Save, load, and manage multiple "snapshots" of the game's visual state, allowing for strategic analysis and replay of key moments.

## 🔧 Technical Deep Dive

This project is a testament to solid software architecture and modern C++ development practices.

#### Architectural Design

The system is built on a foundation of key OOP principles and design patterns:

  * **Modular & Decoupled Architecture**: The core logic is split into several independent modules:
      * `Simulador`: The main engine that orchestrates the game loop, state transitions, and event handling.
      * `GestorComandos`: A dedicated command processor that decouples command parsing and execution from the core simulation logic, implementing the **Command Pattern**.
      * `Mapa`: Manages the game world, including terrain, cities, and the placement of dynamic entities.
  * **Inheritance & Polymorphism**: A deep inheritance hierarchy allows for code reuse and specialization.
      * The `Caravana` base class defines common attributes, while derived classes like `CaravanaMilitar` and `CaravanaComercial` implement specialized behaviors through virtual functions (`move`, `consumirRecursos`).
      * Similarly, `Item` and `Pixel` hierarchies enable polymorphic handling of various game objects.

#### C++ Language & Features

  * **C++20 Standard**: The project is built using modern C++20 features, ensuring type safety, performance, and clean syntax.
  * **Smart Pointers for Memory Management**: `std::unique_ptr` and `std::shared_ptr` are used extensively to manage dynamic memory, completely eliminating manual `new`/`delete` and preventing memory leaks. This is a cornerstone of modern, safe C++ development.
  * **STL & Standard Library**: Intelligent use of the C++ Standard Library, including containers (`std::vector`, `std::map`), algorithms, and I/O streams, demonstrates a strong grasp of the available tools.

## 🛠️ Getting Started

Follow these steps to compile and run the simulation.

#### Prerequisites

  * A C++ compiler that supports the C++20 standard (e.g., GCC 10+, Clang 12+).
  * CMake (version 3.16 or higher).

#### Compilation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/desert-caravans.git
    cd desert-caravans
    ```
2.  **Configure and build with CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

#### Running the Simulation

1.  **Launch the executable:**
    ```bash
    ./ProjetoPOO2024
    ```
2.  **Start a new game:** The simulation begins in a setup phase. Load a map configuration to start.
    ```
    config map_config.txt
    ```
3.  **Play\!** Once the map is loaded, you can start issuing commands. To automate testing or gameplay, you can execute a script of commands:
    ```
    exec comandos_simulador.txt
    ```

## 📂 Project Structure

A brief overview of the key source files:

  * `main.cpp`: Entry point of the application.
  * `Simulador.h/.cpp`: Contains the core simulation logic.
  * `GestorComandos.h/.cpp`: Manages command parsing and execution.
  * `Comando.h/.cpp`: Defines the base `Comando` class and its derivatives.
  * `Mapa.h/.cpp`: Manages the game world grid and its contents.
  * `Caravana.h/.cpp`: Base and derived classes for all caravan types.
  * `Item.h/.cpp`: Base and derived classes for all discoverable items.
  * `Buffer.h/.cpp`: Implementation of the custom console rendering buffer.

## 🔮 Future Enhancements

The modular design of this project opens the door to many exciting future possibilities:

  * **GUI Implementation**: Migrating from the console to a graphical framework like SFML or Qt to create a visually rich user experience.
  * **Advanced Economic Model**: Introducing a dynamic economy where prices fluctuate based on supply, demand, and in-game events.
  * **Multiplayer Functionality**: Extending the simulation to support multiple players competing or cooperating in the same world.
  * **Procedural Map Generation**: Implementing algorithms to generate unique and varied desert maps for each playthrough.
