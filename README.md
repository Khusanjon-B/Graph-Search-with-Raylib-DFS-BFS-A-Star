# Graph Search with Raylib

A C++17 grid-pathfinding demo that runs Depth-First Search (DFS), Breadth-First Search (BFS), and A* on the same grid, then displays their paths in a Raylib window.

## Features

- Enter a rectangular grid with open cells and obstacles.
- Choose start and target coordinates.
- Compare the paths and costs produced by DFS, BFS, and A*.
- Move in eight directions: horizontal, vertical, and diagonal.
- Orthogonal moves cost 10; diagonal moves cost 14.

## Requirements

- CMake 3.20 or newer
- A C and C++ compiler with C++17 support
- Git and an internet connection when configuring, so CMake can download Raylib

Raylib is fetched automatically through CMake's `FetchContent`; no separate Raylib installation is needed.

## Build

From the project directory, run:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

With Visual Studio's generator, the executable is typically `build\Release\GraphSearch.exe`. With single-configuration generators, it is typically `build\GraphSearch.exe`.

## Run

Launch `GraphSearch.exe`. Enter the start coordinates, target coordinates, and then the grid rows. Coordinates are zero-based `(x, y)`, with `(0, 0)` at the top-left. Each grid row must have the same number of characters:

- `0` is an open cell.
- `X` is an obstacle.
- Enter `done` on its own line after the last row.

Example input:

```text
0 0
5 3
000000
000X00
000X00
000000
done
```

Both coordinates must refer to open cells in the grid. Diagonal moves are allowed even when the two adjacent orthogonal cells are obstacles.

## Results

The window overlays all three paths on the grid and displays each algorithm's accumulated movement cost in a legend:

- **DFS** — explores neighbors in a fixed order and returns the first path it finds; it does not necessarily find the lowest-cost path.
- **BFS** — finds a path with the fewest moves; because diagonal and orthogonal moves have different costs, that path is not necessarily the lowest-cost one.
- **A\*** — uses accumulated movement cost and a Manhattan-distance heuristic to guide its search.

The paths are drawn in red (DFS), blue (BFS), and black (A*). The start is marked in cyan and the target in red. If an algorithm cannot reach the target, its result is shown at the start cell with cost 0. Close the window or press `Esc` to exit.
