# System Study Repository

This repository is a personal C++ study workspace used to build practical skills
through small programs, algorithm exercises, and focused experiments.

## Goals

1. Build a strong understanding of core C++ syntax and semantics.
2. Practice problem-solving with algorithms and data structures.
3. Improve debugging, testing, and code-reading habits.
4. Keep reusable examples for interview prep and daily reference.

## Repository layout

- `algorithms/`: algorithm exercises and problem-focused implementations.
- `sandbox/`: compile and run anything you want.
- `00.cpp_basics/`: language fundamentals and syntax drills.
- Additional folders are added over time for topics like systems, networking,
  and architecture-oriented C++ practice.

## Prerequisites

- A C++ compiler with C++17 support (for example: `g++`, `clang++`, MSVC).
- A terminal (PowerShell, Command Prompt, Bash, or similar).
- Optional: an editor/IDE such as VS Code, CLion, or Visual Studio.

## Build and run

### Linux/macOS (g++)

```bash
g++ -Wall -Wextra -Werror -std=c++17 1_hello_world.cpp -o hello
./hello
```

### Windows (MinGW g++)

```powershell
g++ -Wall -Wextra -Werror -std=c++17 .\1_hello_world.cpp -o hello.exe
.\hello.exe
```

### Sandbox quick run (Windows, `cpp\sandbox`)

```powershell
cd .\sandbox
.\build-and-run
```

### Windows (MSVC)

```powershell
cl /W4 /EHsc /std:c++17 1_hello_world.cpp
.\1_hello_world.exe
```

## Study approach

- Keep each example focused on one concept.
- Prefer readable, self-contained code over clever shortcuts.
- Add comments only when they explain intent or non-obvious tradeoffs.
- Revisit old exercises and refactor them with better style over time.

## Notes

- This is a personal learning repository.
- Please do not reuse this content for commercial use or academic submission
  without permission.
