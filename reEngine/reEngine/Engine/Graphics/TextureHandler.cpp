#include "TextureHandler.h"


std::unique_ptr<TextureHandler>TextureHandler::textureInstance = nullptr;
std::map<std::string, Texture*>TextureHandler::textures = std::map<std::string, Texture*>();

TextureHandler::TextureHandler()
{
}


TextureHandler::~TextureHandler()
{
	if (textures.size() > 0) {
		for (auto t : textures) {
			glDeleteTextures(sizeof(GLuint), &t.second->textureID);
			delete t.second;
			t.second = nullptr;
		}
		textures.clear();
	}
}

TextureHandler* TextureHandler::GetInstance()
{
	if (textureInstance.get() == nullptr) {
		textureInstance.reset(new TextureHandler);
	}
	return textureInstance.get();
}

void TextureHandler::CreateTexture(const std::string & textureName_, const std::string & textureFileName_)
{
	Texture* t = new Texture();
	SDL_Surface* surface = nullptr;

	try {
		surface = IMG_Load(textureFileName_.c_str());
	}
	catch (std::exception e) {
		Debug::Error("Texture " + textureName_ + " failed to load.", __FILE__, __LINE__);
	}

	if (surface == nullptr) {
		Debug::Error("Surface for texture " + textureName_ + " failed to be created.", __FILE__, __LINE__);
		return;
	}
	t->width = surface->w;
	t->height = surface->h;

	glGenTextures(1, &t->textureID);
	glBindTexture(GL_TEXTURE_2D, t->textureID);
	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Hide the texture so it cant be messed with
	glBindTexture(GL_TEXTURE_2D, 0);
	textures[textureName_] = t;
	SDL_FreeSurface(surface);
	surface = nullptr;


}

const GLuint TextureHandler::GetTexture(const std::string & textureName_)
{
	if (textures.find(textureName_.c_str()) != textures.end()) {
		return textures[textureName_]->textureID;
	}
	return 0;
}

const Texture * TextureHandler::GetTextureData(const std::string & textureName_)
{
	if (textures.find(textureName_.c_str()) != textures.end()) {
		return textures[textureName_];
	}
	return 0;
}
