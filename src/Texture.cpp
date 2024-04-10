#include <Texture.h>
#include <stb_image.h>

#include <stdexcept>

GLuint loadTexture2DFromFilePath(const std::string& path) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width;
	int height;
	int channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	if (!data) {
		stbi_image_free(data);
		throw std::runtime_error("Failed to load texture: " + path);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	setDefaultTexture2DParameters(texture);
	return texture;
}

void setDefaultTexture2DParameters(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Y
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); // Z
}
