#include "ModelMesh.h"



ModelMesh::ModelMesh(const std::string &meshName, const std::string &textureName, float scale):
textureName(textureName){ 
	if(loadOBJ(meshName, scale, scale, scale)){
		std::cout << "mesh loaded succesfully" << std::endl;

	}
	else printf("ERROR: COULDN'T READ OBJECT\n");
}

ModelMesh::ModelMesh(const std::string &meshName, float sx, float sy, float sz){ 
	if(loadOBJ(meshName, sx, sy, sz)){
		
	}
	else printf("ERROR: COULDN'T READ OBJECT\n");
}

ModelMesh::~ModelMesh(){
	vertices.clear();
	normals.clear();
	uvs.clear();
}

glm::vec3 ModelMesh::getMaxVertexValues(){
    glm::vec3 maxValues = vertices[0];
    for(int i = 1; i<vertices.size(); ++i){
        if(vertices[i].x > maxValues.x) maxValues.x = vertices[i].x;
        if(vertices[i].y > maxValues.y) maxValues.y = vertices[i].y;
        if(vertices[i].z > maxValues.z) maxValues.z = vertices[i].z;
    }
    return maxValues;
}

glm::vec3 ModelMesh::getMinVertexValues(){
    glm::vec3 minValues = vertices[0];
    for(int i = 1; i<vertices.size(); ++i){
        if(vertices[i].x < minValues.x) minValues.x = vertices[i].x;
        if(vertices[i].y < minValues.y) minValues.y = vertices[i].y;
        if(vertices[i].z < minValues.z) minValues.z = vertices[i].z;
    }
    return minValues;
}

void ModelMesh::draw() const{
	glActiveTexture(GL_TEXTURE0);

    // Bind the texture by its set handle
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByName("mmdg"));

    
	for( unsigned int i=0; i<vertices.size()-2; i+=3){

		glBegin(GL_TRIANGLES);

		//Vertex 1
        glNormal3f(normals[i].x, normals[i].y, normals[i].z);
        glTexCoord2d(uvs[i].x, uvs[i].y);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

        //Vertex 2
        glNormal3f(normals[i+1].x, normals[i+1].y, normals[i+1].z);
        glTexCoord2d(uvs[i+1].x, uvs[i+1].y);
        glVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);

        //Vertex 3
        glNormal3f(normals[i+2].x, normals[i+2].y, normals[i+2].z);
        glTexCoord2d(uvs[i+2].x, uvs[i+2].y);
        glVertex3f(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);

        glEnd();
	}
	
}

bool ModelMesh::loadOBJ(const std::string &meshName, float sx, float sy, float sz){
	//printf("Loading mesh %s... ", meshName.c_str());

	if(meshName == "bigscene2")
		sy = 0.1f;

	std::string builder = "";
	builder += "objects/";
	builder += meshName;
	builder += ".obj";
	const char* path = builder.c_str();

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Resize
		vertex.x *= sx;
		vertex.y *= sy;
		vertex.z *= sz;

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs     .push_back(uv);
		normals .push_back(normal);
	
	}
	printf("verts: %i\n", (int)vertices.size());
	printf("normals: %i\n", (int)normals.size());
	printf("uvs: %i\n", (int)uvs.size());
	return true;
}

