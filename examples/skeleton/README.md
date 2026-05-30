# Auction Viewer — Skeleton Example

This folder contains a minimal, cross-platform C++ skeleton demonstrating an abstract UI interface and a simple console UI implementation.

Build (Linux/macOS):

```bash
cmake -S . -B build
cmake --build build --config Release
```

Build (Windows, Visual Studio):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

Run:

```bash
./build/auction_skeleton
```

Notes:

- The UI is abstracted by `IUI` in `include/auction_viewer/ui.h`.
- Add toolkit-specific adapters (Qt, GTK, etc.) implementing `IUI` and plug them into `main.cpp`.
