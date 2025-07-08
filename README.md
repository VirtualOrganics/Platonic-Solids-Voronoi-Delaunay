# Geogram-Three.js

A high-performance JavaScript library and visualization tool for 3D computational geometry in the browser, powered by the [Geogram C++ library](https://github.com/BrunoLevy/geogram) (via WebAssembly) and rendered with Three.js.

## Live Demo

🎮 **[Try the Live Demo](https://virtualorganics.github.io/Geogram-Three.js/examples/basic/)**

<p align="left">
  <img src="docs/Geogram1.png" alt="3D Periodic Delaunay Triangulation" width="50%">
</p>

This project provides a working implementation of a **Periodic 3D Delaunay Triangulation**, a feature not commonly available in standard JavaScript libraries.

## Features

- 🚀 High-performance 3D Delaunay Triangulation via Geogram/WASM
- 🔄 Support for **Periodic** and Non-Periodic boundary conditions
- 🎨 Interactive Three.js visualization for exploring the tetrahedral mesh
- 📊 Real-time statistics (vertices, edges, tetrahedra)
- 🎯 Well-distributed point generation with minimum distance constraints
- 🔍 Visual distinction for periodic boundary-crossing edges
- 🛠️ Clean interface for future expansion (e.g., Voronoi diagrams)

## Screenshots
<p align="left">
  <img src="docs/screenshot.png" alt="3D Periodic Delaunay Triangulation" width="50%">
</p>
*3D Periodic Delaunay triangulation with 500 vertices*

## How to Run

1. Clone this repository:
   ```bash
   git clone https://github.com/YourUsername/Geogram-Three.js.git
   cd Geogram-Three.js
   ```

2. Since this project uses WebAssembly, it must be run from a web server. Start a simple server:
   ```bash
   # Using Python 3
   python -m http.server 8000
   
   # Or using Node.js
   npx http-server -p 8000
   ```

3. Open your browser to: `http://localhost:8000/examples/basic/`

## Project Structure

```
Geogram-Three.js/
├── src/cpp/              # C++ source files
│   ├── periodic_delaunay.cpp
│   ├── Delaunay_psm.h
│   └── Delaunay_psm.cpp
├── dist/                 # Compiled WASM/JS files
│   ├── periodic_delaunay.js
│   └── periodic_delaunay.wasm
├── examples/basic/       # Basic visualization example
│   └── index.html
├── .gitignore
└── README.md
```

## Usage

The visualization provides interactive controls:

- **Periodic Mode**: Toggle between periodic and non-periodic triangulation
- **Number of Points**: Adjust the number of vertices (4-200)
- **Min Distance**: Set minimum distance between points for well-distributed generation
- **Show/Hide Elements**: Toggle visibility of vertices, edges, tetrahedra, and boundary box

### Periodic vs Non-Periodic Mode

- **Non-periodic**: Standard Delaunay triangulation within the unit cube
- **Periodic**: Delaunay triangulation with periodic boundary conditions, where opposite faces of the cube are connected

Edges that cross periodic boundaries are highlighted in orange.

## Technical Details

### WebAssembly Integration

The project uses Emscripten to compile the Geogram C++ library to WebAssembly, providing near-native performance in the browser. The main components:

- **Geogram PSM**: Portable Software Module version of Geogram's periodic Delaunay implementation
- **C++ Bindings**: Custom wrapper using Embind for JavaScript interoperability
- **ES Modules**: Modern JavaScript module system for Three.js integration

### Key Algorithms

- **Delaunay Triangulation**: Divides 3D space into tetrahedra such that no point is inside the circumsphere of any tetrahedron
- **Periodic Boundaries**: Implements toroidal topology where the unit cube wraps around on all three axes
- **Well-Distributed Points**: Uses minimum distance constraints to avoid degenerate configurations

## Building from Source

Prerequisites:
- Emscripten SDK
- C++17 compatible compiler

```bash
# Install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Build the project
cd src/cpp
em++ --bind -o ../../dist/periodic_delaunay.js periodic_delaunay.cpp Delaunay_psm.cpp \
  -I. -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 \
  -s EXPORT_NAME="PeriodicDelaunayModule" -s ASSERTIONS=1 -std=c++17 -O2
```

## Future Development

Planned features:
- 3D Voronoi diagram generation and visualization
- Export functionality (OBJ, PLY formats)
- Performance optimizations for larger point sets
- Additional geometric algorithms from Geogram

## Credits

- **[Geogram](https://github.com/BrunoLevy/geogram)**: Created by Bruno Levy and the ALICE project-team
- **[Three.js](https://github.com/mrdoob/three.js)**: Created by Ricardo Cabello (Mr.doob) and contributors
- **Development**: Assisted by Google's AI and Claude

## License

This project is open source. The Geogram library is available under its own license terms.

---

*Developed with the assistance of Google's AI and Claude (Anthropic)* 
