// Renderer.h

#pragma once

#include <glm/vec3.hpp>

#include "RenderObject.h"
#include "ShaderProgram.h"

class Renderer
{
public:
	void Init();
	void Render() const;

	void ResizeViewport(int width, int height);

	void SetClearColor(glm::vec3 const & color) { m_clear_color = color; }
	void SetViewTransform(glm::mat4 const & view_transform) { m_view_transform = view_transform; }

	size_t CreateShaderProgram(std::filesystem::path const & vert_shader_path, std::filesystem::path const & frag_shader_path);
	void AddRenderObject(std::shared_ptr<RenderObject> render_object);

private:
	glm::vec3 m_clear_color;

	glm::mat4 m_view_transform;
	glm::mat4 m_proj_transform;

	std::vector<ShaderProgram> m_shader_programs;
	std::vector<std::shared_ptr<RenderObject>> m_render_objects;
};
