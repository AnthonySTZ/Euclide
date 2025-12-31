#include "interface/engine.h"

#include "nodes/primitive/grid.h"
#include "nodes/attributes/attribute_noise.h"
#include "nodes/attributes/attribute_axia.h"

int main() {
    // euclide::GridSettings gridSettings{.divisions = {1000, 1000}};
    // euclide::Mesh mesh;
    // euclide::Grid::createGrid(mesh, gridSettings);
    // euclide::PerlinNoise::PerlinSettings perlinSettings{.octaves = 6, .frequency = 1.0};
    // euclide::PerlinNoise perlin{};
    // perlin.applyToMesh(mesh, mesh.pointAttribs, "height", 1, perlinSettings);

    // const std::string script = "float amp = 1.0; float height = 2.0;";
    // euclide::AttributeAXIA::computeAXIA(mesh.pointAttribs, script);

    // return 0;
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();
    engine.interface().addSpreadsheet();

    engine.mainLoop();
    return 0;
}