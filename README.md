# Geogram-Three.js

[Live Demo](https://virtualorganics.github.io/Geogram-Three.js/)

A WebAssembly-powered 3D periodic Delaunay-Voronoi triangulation library for the browser, combining the computational power of [Geogram](https://github.com/BrunoLevy/geogram) with the visualization capabilities of [Three.js](https://github.com/mrdoob/three.js).

<img src="docs/Geogram1.png" alt="Geogram-Three.js Demo" width="33%"> <img src="docs/screenshot.png" alt="Geogram-Three.js Screenshot" width="33%">

## Features

- **3D Delaunay Triangulation**: Compute Delaunay tetrahedralization of 3D point sets
- **3D Voronoi Diagrams**: Generate Voronoi cells from Delaunay triangulation using barycenter method
- **Periodic Boundary Conditions**: Support for periodic (toroidal) domains
- **WebAssembly Performance**: Native-speed computation in the browser
- **Three.js Visualization**: Interactive 3D rendering with orbit controls
- **Clean JavaScript API**: Simple, promise-based interface hiding WASM complexity

## Demo

[Live Demo](https://virtualorganics.github.io/Geogram-Three.js/)

## Quick Start

### Using the Library

```javascript
import { DelaunayComputation } from './src/js/DelaunayComputation.js';

// Initialize the WASM module
const Module = await window.PeriodicDelaunayModule();

// Create points (array of [x, y, z] coordinates in range [0, 1])
const points = [
    [0.1, 0.2, 0.3],
    [0.4, 0.5, 0.6],
    // ... more points
];

// Create computation instance
const computation = new DelaunayComputation(points, true); // true for periodic

// Run the computation
await computation.compute(Module);

// Access results
console.log(`Computed ${computation.tetrahedra.length} tetrahedra`);
console.log(`Generated ${computation.voronoiEdges.length} Voronoi edges`);

// Tetrahedra: array of [v0, v1, v2, v3] vertex indices
// Voronoi edges: array of {start: [x,y,z], end: [x,y,z], isPeriodic: boolean}
```

### Visualization Example

```javascript
// Draw Delaunay edges
for (const tet of computation.tetrahedra) {
    // Extract and draw edges from tetrahedron
}

// Draw Voronoi diagram
for (const edge of computation.voronoiEdges) {
    const geometry = new THREE.BufferGeometry();
    const positions = new Float32Array([
        edge.start[0], edge.start[1], edge.start[2],
        edge.end[0], edge.end[1], edge.end[2]
    ]);
    geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));
    
    const material = edge.isPeriodic ? 
        new THREE.LineBasicMaterial({ color: 0xffaa00 }) : 
        new THREE.LineBasicMaterial({ color: 0xff6600 });
    
    const line = new THREE.Line(geometry, material);
    scene.add(line);
}
```

## API Reference

### DelaunayComputation Class

#### Constructor
```javascript
new DelaunayComputation(points, isPeriodic = true)
```
- `points`: Array of 3D points as `[[x,y,z], ...]` or flat array `[x,y,z,x,y,z,...]`
- `isPeriodic`: Boolean, whether to use periodic boundary conditions

#### Methods
```javascript
async compute(wasmModule)
```
Runs the Delaunay-Voronoi computation. Returns the instance for chaining.

```javascript
getStats()
```
Returns an object with computation statistics:
```javascript
{
    numPoints: number,
    numTetrahedra: number,
    numVoronoiEdges: number,
    isPeriodic: boolean
}
```

```javascript
getPeriodicDistance(p1, p2)
```
Calculates the minimum image distance between two points in periodic space.

#### Properties
- `pointsArray`: Array of input points as `[[x,y,z], ...]`
- `tetrahedra`: Array of tetrahedra as `[[v0,v1,v2,v3], ...]`
- `voronoiEdges`: Array of Voronoi edges as `[{start, end, tetraIndices, isPeriodic}, ...]`
- `barycenters`: Array of tetrahedra barycenters as `[[x,y,z], ...]`

## Building from Source

### Prerequisites
- Emscripten SDK
- Node.js
- C++17 compiler

### Build Steps
```bash
# Clone the repository
git clone https://github.com/VirtualOrganics/Geogram-Three.js.git
cd Geogram-Three.js

# Build the WASM module
cd src/cpp
./build.sh

# The compiled files will be in dist/
```

## Implementation Details

### Voronoi Computation
The library computes Voronoi diagrams using the barycenter method:
1. Calculate the barycenter (center) of each Delaunay tetrahedron
2. Build adjacency information for tetrahedra sharing faces
3. Connect barycenters of adjacent tetrahedra to form Voronoi edges

### Periodic Boundaries
When periodic mode is enabled:
- Points are assumed to be in the unit cube [0, 1]³
- The space wraps around at the boundaries (toroidal topology)
- Edges crossing boundaries are marked with `isPeriodic: true`

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments

- [Geogram](https://github.com/BrunoLevy/geogram) by Bruno Levy for the computational geometry algorithms
- [Three.js](https://github.com/mrdoob/three.js) for 3D visualization
- [Emscripten](https://emscripten.org/) for WebAssembly compilation
