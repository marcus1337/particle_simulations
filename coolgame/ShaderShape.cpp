#include "ShaderShape.h"

#include "MiscCode/utils.h"
#include "MiscCode/utils2.h"
#include "MiscCode/OBJ_Loader.h"

using namespace std;


void ShaderShape::initParticleShader1() {
    vertShadName = "particle2.vert";
    fragShadName = "particle2.frag";

    shaderProgram = loadShaderProgram(vertShadName, fragShadName);

    glGenVertexArrays(1, &(ctx.meshVAO.vao));
    glBindVertexArray(ctx.meshVAO.vao);
    glBindVertexArray(ctx.defaultVAO); // unbinds the VAO
}

void ShaderShape::init() {

    shaderProgram = loadShaderProgram(vertShadName, fragShadName);

    objl::Loader Loader;

    bool loadout = Loader.LoadFile(path3D + objName);

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


    //C:\Users\Marcus\source\repos\coolgame\Debug\3d_models
    /*OBJMesh obj_mesh;
    objMeshLoad(obj_mesh, "C:\\Users\\Marcus\\source\\repos\\coolgame\\Debug\\3d_models\\low-poly-fox-by-pixelmannen.obj");
    ctx.mesh.vertices = obj_mesh.vertices;
    ctx.mesh.normals = obj_mesh.normals;
    ctx.mesh.indices = obj_mesh.indices;*/
    //loadMesh("C:\\Users\\Marcus\\source\\repos\\coolgame\\Debug\\3d_models\\fox.obj", &ctx.mesh);
    createMeshVAO(ctx, ctx.mesh, &ctx.meshVAO);
}

void ShaderShape::cleanup() {
    glDeleteVertexArrays(1, &ctx.meshVAO.vao);
    glDeleteBuffers(1, &ctx.meshVAO.vertexVBO);
    glDeleteBuffers(1, &ctx.meshVAO.indexVBO);
    glDeleteBuffers(1, &ctx.meshVAO.normalVBO);
}







/*void loadShaders() {
    std::ifstream t("triangle.vert");
    std::stringstream buffer;
    buffer << t.rdbuf();
    string test = buffer.str();
    const GLchar* vertexShaderSource = test.c_str();
    t.close();
    buffer.str(std::string());
    buffer.clear();

    t.open("triangle.frag");
    buffer << t.rdbuf();
    string test2 = buffer.str();
    const GLchar* fragmentShaderSource = test2.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}*/