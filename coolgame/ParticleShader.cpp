#include "ParticleShader.h"

#include "MiscCode/utils.h"

using namespace std;

int ParticleShader::getShaderIndex(int ID_, float radius_, glm::vec4 color_, std::vector<glm::vec3>& particles_) {

    for (int i = 0; i < shaders.size(); i++) {
        if (shaders[i].ID == ID_ && shaders[i].radius == radius_ && shaders[i].color == color_)
            return i;
    }

    if (ID_ == FOX) {
        ShaderData result;
        //std::string objName = "low-poly-fox-by-pixelmannen.obj";
        std::string vertShadName = "particle3.vert";
        std::string fragShadName = "particle3.frag";

        result.shaderProgram = loadShaderProgram(vertShadName, fragShadName);
        result.ID = ID_;
        result.color = color_;
        result.radius = radius_;

        glGenBuffers(1, &(result.vertexVBO));

        glBindBuffer(GL_ARRAY_BUFFER, result.vertexVBO);
        auto verticesNBytes = particles_.size() * sizeof(particles_[0]);
        glBufferData(GL_ARRAY_BUFFER, verticesNBytes, particles_.data(), GL_STATIC_DRAW);

        std::vector<glm::vec4> colorData;
        std::vector<float> allRadius;
        for (int i = 0; i < particles_.size(); i++) {
            colorData.push_back(color_);
            allRadius.push_back(radius_);
        }

        glGenBuffers(1, &(result.colorVBO));
        glBindBuffer(GL_ARRAY_BUFFER, result.colorVBO);
        auto colorsNBytes = colorData.size() * sizeof(colorData[0]);
        glBufferData(GL_ARRAY_BUFFER, colorsNBytes, colorData.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &(result.radiusVBO));
        glBindBuffer(GL_ARRAY_BUFFER, result.radiusVBO);
        auto radiusNBytes = allRadius.size() * sizeof(allRadius[0]);
        glBufferData(GL_ARRAY_BUFFER, radiusNBytes, allRadius.data(), GL_STATIC_DRAW);


        std::vector<uint32_t> indices;
        for (int i = 0; i < particles_.size(); i++) {
            indices.push_back(i);
        }
        // Generates and populates a VBO for the element indices
        glGenBuffers(1, &result.indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.indexVBO);
        auto indicesNBytes = indices.size() * sizeof(indices[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, indices.data(), GL_STATIC_DRAW);

        // Creates a vertex array object (VAO) for drawing the mesh
        glGenVertexArrays(1, &(result.vao));
        glBindVertexArray(result.vao);

        glBindBuffer(GL_ARRAY_BUFFER, result.vertexVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, result.colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, result.radiusVBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.indexVBO);

        glBindVertexArray(0); // unbinds the VAO

        result.numVertices = particles_.size();
        shaders.push_back(result);
        return shaders.size() - 1;
    }

    return -1;
}