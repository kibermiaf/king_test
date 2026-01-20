# Modern C++ Messaging Service Refactor

A modular, scalable refactoring of a legacy messaging application. This project demonstrates the transformation of a monolithic "C with Classes" script into a robust, layered architecture adhering to **SOLID principles** and **Modern C++ (C++17/20)** standards.

## 🎯 Project Goal
The objective was to take a fragile, tightly coupled codebase and architect a solution that allows for:
* **Scalability:** Efficient data handling for large user bases.
* **Extensibility:** Easy addition of new user interfaces (GUI, Voice, VR) without touching business logic.
* **Testability:** Decoupled components that allow for unit testing via mocking.

## 🏗️ Architecture
The project uses a **Layered Architecture** with strict Separation of Concerns:

### 1. The Core Layer (`Core`)
* **`IMessageService` (Interface):** Defines the contract for messaging operations. This abstraction allows the backend to be swapped (e.g., from In-Memory to SQL) without breaking the app.
* **`InMemoryMessageService` (Implementation):** A concrete implementation using optimized data structures.
    * **Data Structures:** Replaced `std::vector` (O(n)) with `std::unordered_map` and `std::unordered_set` (O(1)) for instant user lookups and inbox retrieval.
    * **Memory Management:** Full RAII compliance. Zero manual `new`/`delete`. used `std::unique_ptr` and value semantics to prevent leaks.

### 2. The UI Layer (`UI`)
* **Dependency Injection:** The UI accepts an `IMessageService&` in its constructor, decoupling it from the specific backend implementation.
* **Dispatch Table:** Uses `std::unordered_map<std::string, std::function<void()>>` to map user inputs to actions, replacing fragile `if/else` chains and adhering to the Open/Closed Principle.

## ✨ Key Technical Features
* **Modern C++ Idioms:**
    * **Structured Binding:** For clean return values (e.g., `auto [iter, success] = ...`).
    * **Move Semantics:** Messages are `std::move`'d rather than copied, ensuring high performance even with large payloads.
    * **Smart Pointers & Containers:** Safe memory handling using standard library containers.
* **Safety:**
    * `[[nodiscard]]` attributes to enforce error checking.
    * `enum class` for type-safe return states.
    * **Const Correctness** applied rigorously to read-only methods.

## 📂 Project Structure

```text
├── Core/
│   ├── IMessageService.h       # Abstract Interface
│   ├── MessageService.h        # Concrete Header
│   └── MessageService.cpp      # Business Logic Implementation
├── UI/
│   ├── CommandLineUI.h         # CLI Header
│   └── CommandLineUI.cpp       # CLI Implementation (Input/Output)
└── main.cpp                    # Composition Root
```

## 🚀 Getting Started

### Prerequisites
* A C++ Compiler supporting **C++17** or higher (MSVC, GCC, Clang).
* Visual Studio (recommended) or CMake.

### Building (Visual Studio)
1.  Open the solution file (`.sln`).
2.  Ensure the project properties include `$(ProjectDir)` in **Additional Include Directories** to support modular includes.
3.  Build and Run.

## 🛠️ Extensibility Example
Because of the interface-based design, adding a new UI (e.g., for a Game Engine) is trivial:

```cpp
// You don't need to change the Service code at all!
class GameHUD : public IGameUI {
    Core::IMessageService& service;
public:
    void OnSendButtonClicked(string msg) {
        service.sendMessage("Player1", "Player2", msg);
    }
};
```
