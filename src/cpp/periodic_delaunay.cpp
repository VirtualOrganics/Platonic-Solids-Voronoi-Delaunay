#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "Delaunay_psm.h"
#include <iostream>
#include <memory>
#include <vector>

// Global initialization flag
static bool g_geogram_initialized = false;

// Initialize Geogram once
void initialize_geogram() {
    if (!g_geogram_initialized) {
        // Initialize Geogram using the PSM's initialize function
        GEO::initialize();
        g_geogram_initialized = true;
        std::cout << "Geogram initialized." << std::endl;
    }
}

// Wrapper function that uses Emscripten's val for easier JavaScript interaction
emscripten::val compute_periodic_delaunay_js(emscripten::val points_array, int num_points, bool is_periodic) {
    // --- 1. Initialize ---
    initialize_geogram();
    std::cout << "Starting Delaunay computation..." << std::endl;

    // --- 2. Create Delaunay Object ---
    std::unique_ptr<GEO::PeriodicDelaunay3d> delaunay;
    
    if (is_periodic) {
        delaunay = std::make_unique<GEO::PeriodicDelaunay3d>(GEO::vec3(1.0, 1.0, 1.0));
    } else {
        delaunay = std::make_unique<GEO::PeriodicDelaunay3d>(false);
    }
    
    delaunay->set_stores_cicl(false);

    std::cout << "Delaunay object created. Periodic mode: " << is_periodic << std::endl;
    std::cout << "Processing " << num_points << " points." << std::endl;

    // --- 3. Get points from JavaScript array ---
    std::vector<double> vertices;
    vertices.reserve(num_points * 3);
    
    // Extract points from JavaScript Float64Array
    for (int i = 0; i < num_points * 3; i++) {
        double coord = points_array[i].as<double>();
        // Ensure coordinates are in [0,1) range
        while (coord < 0.0) coord += 1.0;
        while (coord >= 1.0) coord -= 1.0;
        vertices.push_back(coord);
    }
    
    // Print first few points for debugging
    std::cout << "First 3 points:" << std::endl;
    for (int i = 0; i < std::min(3, num_points); i++) {
        std::cout << "  Point " << i << ": (" 
                  << vertices[i*3] << ", " 
                  << vertices[i*3+1] << ", " 
                  << vertices[i*3+2] << ")" << std::endl;
    }

    // --- 4. Set vertices ---
    delaunay->set_vertices(num_points, vertices.data());
    std::cout << "Vertices set. Actual vertex count: " << delaunay->nb_vertices() << std::endl;

    // --- 5. Compute ---
    try {
        delaunay->compute();
        std::cout << "Delaunay computation successful." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception during compute: " << e.what() << std::endl;
        return emscripten::val::null();
    } catch (...) {
        std::cerr << "Unknown exception during compute." << std::endl;
        return emscripten::val::null();
    }

    // --- 6. Get results ---
    int num_tets = delaunay->nb_cells();
    std::cout << "Found " << num_tets << " tetrahedra." << std::endl;
    
    // Also check if we have a valid triangulation
    if (num_tets == 0 && num_points >= 4) {
        std::cout << "WARNING: No tetrahedra generated despite having " << num_points << " points." << std::endl;
        std::cout << "This might indicate degenerate point configuration." << std::endl;
    }
    
    if (num_tets == 0) {
        return emscripten::val::array();
    }
    
    // Create JavaScript array for results
    emscripten::val result = emscripten::val::array();
    
    for (int t = 0; t < num_tets; ++t) {
        emscripten::val tet = emscripten::val::array();
        tet.set(0, delaunay->cell_vertex(t, 0));
        tet.set(1, delaunay->cell_vertex(t, 1));
        tet.set(2, delaunay->cell_vertex(t, 2));
        tet.set(3, delaunay->cell_vertex(t, 3));
        result.set(t, tet);
    }

    return result;
}

// --- 7. Embind module ---
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("compute_delaunay", &compute_periodic_delaunay_js);
} 