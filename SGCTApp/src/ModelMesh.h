#include <vector> 
#include <string>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include <sgct.h>

#ifndef MODEL_MESH_H
#define MODEL_MESH_H

class ModelMesh{
public:
	ModelMesh(const std::string &meshName, const std::string &textureName, float scale = 1.0f);
	ModelMesh(const std::string &meshName, float sx, float sy, float sz);
	~ModelMesh();
	
	glm::vec3 getMaxVertexValues();
    glm::vec3 getMinVertexValues();

    void draw() const;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::string textureName;

private:

	bool loadOBJ(const std::string &meshName, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);

};

#endif