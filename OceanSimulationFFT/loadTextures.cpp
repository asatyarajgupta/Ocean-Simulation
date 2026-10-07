#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"

unsigned int loadTexture(const char* imagePath, std::string type) {
	unsigned int textureID;
	GLenum format1 = GL_SRGB_ALPHA;
	GLenum format2 = GL_RGBA;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nChannels;
	unsigned char* data = stbi_load(imagePath, &width, &height, &nChannels, 0);
	if (data) {
		if (type == "simple") {
			if (nChannels == 1)
				format1 = GL_RED;
			else if (nChannels == 3) {
				format1 = GL_SRGB;
				format2 = GL_RGB;
			}
			else if (nChannels == 4) {
				format1 = GL_SRGB_ALPHA;
				format2 = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format1, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (type == "normal") {
			if (nChannels == 1)
				format1 = GL_RED;
			else if (nChannels == 3) {
				format1 = GL_RGB;
				format2 = GL_RGB;
			}
			else if (nChannels == 4) {
				format1 = GL_RGBA;
				format2 = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format1, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		
	}
	else {
		std::cout << "Failed to load image" << imagePath << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}

unsigned int loadCubemap(std::vector<const char*> texture_faces) {
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	
	GLenum format = GL_RGBA;
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < texture_faces.size(); i++) {
		data = stbi_load(texture_faces[i], &width, &height, &nrChannels, 0);
		if (data) {
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

		}
		else {
			std::cout << "Error Cant load Image\n";
			stbi_image_free(data);
		}
		
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureId;

}