#include "ShaderShape.h"

#include "MiscCode/utils.h"
#include "MiscCode/utils2.h"
#include "MiscCode/OBJ_Loader.h"

using namespace std;


void ShaderShape::init() {

    shaderProgram = loadShaderProgram(vertShadName, fragShadName);

    objl::Loader Loader;

    bool loadout = Loader.LoadFile( myLoc::project_location + path3D + objName);

    if (!loadout)
    {
        std::cout << "Loading OBJ error." << std::endl;
        exit(EXIT_FAILURE);
    }

    objl::Mesh curMesh = Loader.LoadedMeshes[0];
    for (int j = 0; j < curMesh.Vertices.size(); j++)
    {
        ctx.mesh.normals.push_back(glm::vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z));
        ctx.mesh.vertices.push_back(glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z));
    }
    for (int j = 0; j < curMesh.Indices.size(); j++)
    {
        ctx.mesh.indices.push_back(curMesh.Indices[j]);
    }

    createMeshVAO(ctx, ctx.mesh, &ctx.meshVAO);
}

void ShaderShape::cleanup() {
    glDeleteVertexArrays(1, &ctx.meshVAO.vao);
    glDeleteBuffers(1, &ctx.meshVAO.vertexVBO);
    glDeleteBuffers(1, &ctx.meshVAO.indexVBO);
    glDeleteBuffers(1, &ctx.meshVAO.normalVBO);
}
