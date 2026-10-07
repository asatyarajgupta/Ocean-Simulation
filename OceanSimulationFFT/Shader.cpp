#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragPath) {
	std::string vertexCode;
	std::string fragCode;
	std::ifstream vertexFile;
	std::ifstream fragFile;
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);
		std::stringstream vertexStream, fragStream;

		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();
		vertexFile.close();
		fragFile.close();

		vertexCode = vertexStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error occured while reading the shader files: " << std::endl;
	}
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	int success;
	char infoLog[512];
	//create and compile vShader
	unsigned int vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vShader);

	//checking for compilation errors

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader compilation failed with " << infoLog << std::endl;
	}
	//create and compile fShader
	glShaderSource(fShader, 1, &fragShaderCode, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader with " << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Failed to link the program with " << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
}
Shader::Shader(const char* vertexPath, const char* fragPath, const char* geomPath) {
	std::string vertexCode;
	std::string fragCode;
	std::string geomCode;
	std::ifstream vertexFile;
	std::ifstream fragFile;
	std::ifstream geomFile;
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geomFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);
		geomFile.open(geomPath);
		std::stringstream vertexStream, fragStream, geomStream;

		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();
		geomStream << geomFile.rdbuf();
		vertexFile.close();
		fragFile.close();
		geomFile.close();

		vertexCode = vertexStream.str();
		fragCode = fragStream.str();
		geomCode = geomStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error occured while reading the shader files: " << std::endl;
	}
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragShaderCode = fragCode.c_str();
	const char* geomShaderCode = geomCode.c_str();

	int success;
	char infoLog[512];
	//create and compile vShader
	unsigned int vShader, fShader, gShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	gShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(vShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vShader);

	//checking for compilation errors

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader compilation failed with " << infoLog << std::endl;
	}
	//create and compile fShader
	glShaderSource(fShader, 1, &fragShaderCode, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader with " << infoLog << std::endl;
	}
	glShaderSource(gShader, 1, &geomShaderCode, NULL);
	glCompileShader(gShader);
	glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(gShader, 512, NULL, infoLog);
		std::cout << "Failed to compile the geometry shader with " << infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glAttachShader(ID, gShader);

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Failed to link the program with " << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(gShader);
}

void Shader::use() {
	glUseProgram(ID);
}


void Shader::setBool(const std::string& name, bool value) const {
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setVec3(const std::string& name, glm::vec3 vector) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}void Shader::setVec2(const std::string& name, glm::vec2 vector) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}
void Shader::setVec4(const std::string& name, glm::vec4 vector) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}