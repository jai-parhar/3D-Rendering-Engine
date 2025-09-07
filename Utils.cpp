#include "Utils.h"

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s.\n", vertex_file_path);
		getchar();
		return 0;
	}

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint loadBMP(const char * imagepath) {
	unsigned char header[54]; //BMP files have a 54 byte header
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imgSize;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Error load image: %s\n", imagepath);
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a BMP file: %s\n", imagepath);
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("BMP file is corrupted:%s\n", imagepath);
	}

	dataPos = *(int*)&(header[0x0A]);
	imgSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imgSize == 0)
	{
		imgSize = width * height * 3;
	}
	if (dataPos == 0)
	{
		dataPos = 54;
	}

	data = new unsigned char[imgSize];
	fread(data, 1, imgSize, file);
	fclose(file);

	GLuint texID;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return texID;
}

bool loadOBJtoVectors(const char * path, std::vector <glm::vec3 > & out_vertices, std::vector <glm::vec2 > & out_uvs, std::vector <glm::vec3 > & out_normals, std::vector<unsigned int > & out_indices)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL) 
	{
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1)
	{

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) 
		{
			break; // EOF = End Of File. Quit the loop.
		}

		if (strcmp(lineHeader, "v") == 0) 
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) 
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else 
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	std::vector<glm::vec3> v_notIndexed;
	std::vector<glm::vec2> u_notIndexed;
	std::vector<glm::vec3> n_notIndexed;

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		
		v_notIndexed.push_back(vertex);
		u_notIndexed.push_back(uv);
		n_notIndexed.push_back(normal);
	}

	indexVBO(v_notIndexed, u_notIndexed, n_notIndexed, out_indices, out_vertices, out_uvs, out_normals);

}

bool loadOBJtoBuffers(const char * path, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer, GLuint &elementbuffer, int &size)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1)
	{

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // EOF = End Of File. Quit the loop.
		}

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	std::vector<glm::vec3> v_notIndexed;
	std::vector<glm::vec2> u_notIndexed;
	std::vector<glm::vec3> n_notIndexed;
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		v_notIndexed.push_back(vertex);
		u_notIndexed.push_back(uv);
		n_notIndexed.push_back(normal);
	}

	std::vector<glm::vec3> v;
	std::vector<glm::vec2> u;
	std::vector<glm::vec3> n;
	std::vector<unsigned int> i;
	indexVBO(v_notIndexed, u_notIndexed, n_notIndexed, i, v, u, n);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, u.size() * sizeof(glm::vec2), &u[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, n.size() * sizeof(glm::vec3), &n[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), &i[0], GL_STATIC_DRAW);

	size = i.size();
}

bool loadStaticMeshtoBuffers(const char * path, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer, GLuint &elementbuffer, int &size)
{
	std::string sMeshDef = "Type:StaticMesh";
	std::string rMeshDef = "Type:RiggedMesh";

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);

	std::string headerLine;
	getline(file, headerLine);
	headerLine = headerLine.substr(headerLine.find(":")+1);

	std::streampos headerEnd = stoi(headerLine);

	file.seekg(0, std::ios::beg);

	std::string header = "";
	char headerChar;
	while (file.tellg() < headerEnd)
	{
		file.read(&headerChar, sizeof(headerChar));
		header += headerChar;
	}

	if (header.find(sMeshDef) != -1)
	{
		header.erase(0, header.find(sMeshDef) + sMeshDef.length() - 1);
		int vertexCount, uvCount, normalCount, indicesCount;
		
		vertexCount = std::stoi( header.substr(0, header.find("UV_Count:")).erase(0, strlen("Vertices_Count:") + 2) )/3;
		header.erase(0, header.find("UV_Count:"));

		uvCount = std::stoi(header.substr(0, header.find("Normals_Count:")).erase(0, strlen("UV_Count:")))/2;
		header.erase(0, header.find("Normals_Count:"));

		normalCount = std::stoi(header.substr(0, header.find("Indices_Count:")).erase(0, strlen("Normals_Count:")))/3;
		header.erase(0, header.find("Indices_Count:"));

		indicesCount = std::stoi(header.erase(0, strlen("Indices_Count:")))/3;
		header.erase();

		file.seekg(headerEnd);

		for (int v = 0; v < vertexCount; v++)
		{
			float x, y, z;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			file.read(reinterpret_cast<char*>(&z), sizeof(z));
			temp_vertices.push_back(glm::vec3(x, y, z));
		}

		for (int u = 0; u < uvCount; u++)
		{
			glm::vec2 uv;
			float x, y;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			temp_uvs.push_back(glm::vec2(x, y));
		}

		for (int n = 0; n < normalCount; n++)
		{
			float x, y, z;
			file.read(reinterpret_cast<char*>(&x), sizeof(x));
			file.read(reinterpret_cast<char*>(&y), sizeof(y));
			file.read(reinterpret_cast<char*>(&z), sizeof(z));
			temp_normals.push_back(glm::vec3(x, y, z));
		}
		
		unsigned int index;
		for (int i = 0; i < indicesCount; i++)
		{
			file.read(reinterpret_cast<char*>(&index), sizeof(index));
			vertexIndices.push_back(index);
		}

		for (int i = 0; i < indicesCount; i++)
		{
			file.read(reinterpret_cast<char*>(&index), sizeof(index));
			uvIndices.push_back(index);
		}

		for (int i = 0; i < indicesCount; i++)
		{
			file.read(reinterpret_cast<char*>(&index), sizeof(index));
			normalIndices.push_back(index);
		}

		printf("index = %d %d %d\n", vertexIndices.at(vertexIndices.size()-1), uvIndices.at(uvIndices.size() - 1), normalIndices.at(vertexIndices.size() - 1));

		std::vector<glm::vec3> v_notIndexed;
		std::vector<glm::vec2> u_notIndexed;
		std::vector<glm::vec3> n_notIndexed;
		for (unsigned int i = 0; i < indicesCount; i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 vertex = temp_vertices[vertexIndex];
			glm::vec2 uv = temp_uvs[uvIndex];
			glm::vec3 normal = temp_normals[normalIndex];

			v_notIndexed.push_back(vertex);
			u_notIndexed.push_back(uv);
			n_notIndexed.push_back(normal);
		}

		std::vector<glm::vec3> v;
		std::vector<glm::vec2> u;
		std::vector<glm::vec3> n;
		std::vector<unsigned int> i;
		indexVBO(v_notIndexed, u_notIndexed, n_notIndexed, i, v, u, n);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, u.size() * sizeof(glm::vec2), &u[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, n.size() * sizeof(glm::vec3), &n[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), &i[0], GL_STATIC_DRAW);

		size = i.size();
	}
	else
	{
		printf("The mesh isn't static.\n");
		return false;
	}
	return true;
}

bool getSimilarVertexIndex_fast(PackedVertex & packed, std::map<PackedVertex, unsigned int> & VertexToOutIndex, unsigned int & result) {
	std::map<PackedVertex, unsigned int>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec2> & in_uvs,
	std::vector<glm::vec3> & in_normals,

	std::vector<unsigned int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	std::map<PackedVertex, unsigned int> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned int newindex = (unsigned int)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void createEmptyTexture(GLuint &texture, int w, int h)
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void generateScreenQuad(GLuint & quad_vertexbuffer)
{
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

