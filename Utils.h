#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
GLuint loadBMP(const char * imagepath);
bool loadOBJtoVectors(const char * path, std::vector <glm::vec3 > & out_vertices, std::vector <glm::vec2 > & out_uvs, std::vector <glm::vec3 > & out_normals, std::vector <glm::vec3 > & out_indices);
bool loadOBJtoBuffers(const char * path, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer, GLuint &elementBuffer, int &size);
bool loadStaticMeshtoBuffers(const char * path, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer, GLuint &elementBuffer, int &size);
void indexVBO(std::vector<glm::vec3> & in_vertices, std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals, std::vector<unsigned int> & out_indices, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);
void createEmptyTexture(GLuint &texture, int w, int h);
void generateScreenQuad(GLuint &quad_vertexbuffer);