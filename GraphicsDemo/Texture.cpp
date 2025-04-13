// Texture.cpp

module;

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

module Texture;

class ImageData
{
public:
	ImageData() = default;
	ImageData(std::filesystem::path const & filepath)
	{
		LoadImage(filepath);
	}

	~ImageData()
	{
		if (m_data != nullptr)
			stbi_image_free(m_data);
	}

	void LoadImage(std::filesystem::path const & filepath)
	{
		static_assert(std::same_as<stbi_uc, unsigned char>);

		//stbi_set_flip_vertically_on_load(true);

		m_data = stbi_load(filepath.string().c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
		if (!IsValid())
			std::cout << "ImageData::LoadImage() failed to load image: " << filepath << std::endl;
	}

	bool IsValid() const { return m_data != nullptr && m_width > 0 && m_height > 0; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	unsigned char * GetData() const { return m_data; }
	size_t GetSize() const { return static_cast<size_t>(m_width * m_height * 4); }

private:
	int m_width{ 0 };
	int m_height{ 0 };
	int m_channels{ 0 };
	unsigned char * m_data{ nullptr };
};

Texture::Texture(std::filesystem::path const & filepath)
{
	ImageData image(filepath);
	if (!image.IsValid())
		return;

	m_type = GL_TEXTURE_2D;
	glGenTextures(1, &m_tex_id);
	glBindTexture(m_type, m_tex_id);

	glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(m_type, 0 /*level*/, GL_RGBA, image.GetWidth(), image.GetHeight(), 0 /*border*/, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData());
	glGenerateMipmap(m_type);
}

Texture::Texture(std::array<std::filesystem::path, 6> const & filepaths)
{
	std::array<ImageData, 6> images;
	for (unsigned int i = 0; i < filepaths.size(); i++)
		images[i].LoadImage(filepaths[i]);

	if (std::ranges::any_of(images, [](ImageData const & image) { return !image.IsValid(); }))
		return;

	m_type = GL_TEXTURE_CUBE_MAP;
	glGenTextures(1, &m_tex_id);
	glBindTexture(m_type, m_tex_id);

	glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < images.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, images[i].GetWidth(), images[i].GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i].GetData());
	}
}

Texture::~Texture()
{
	destroy_texture();
}

void Texture::destroy_texture()
{
	glDeleteTextures(1, &m_tex_id);
	m_tex_id = 0;
	m_type = 0;
}

Texture::Texture(Texture && other)
{
	*this = std::move(other);
}

Texture & Texture::operator=(Texture && other)
{
	if (this == &other)
		return *this;

	destroy_texture();

	m_tex_id = other.m_tex_id;
	m_type = other.m_type;

	other.m_tex_id = 0;
	other.m_type = 0;

	return *this;
}

bool Texture::IsValid() const
{
	return m_tex_id != 0 && m_type != 0;
}

void Texture::Bind() const
{
	glBindTexture(m_type, m_tex_id);
}
