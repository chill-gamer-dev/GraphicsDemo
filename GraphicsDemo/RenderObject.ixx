// RenderObject.ixx

module;

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

export module RenderObject;

export class RenderObject
{
public:
	RenderObject(int mesh_id, int shader_id, int tex_id = -1)
		: m_mesh_id(mesh_id)
		, m_shader_id(shader_id)
		, m_tex_id(tex_id)
		, m_color({ 1.0, 1.0, 1.0 })
	{}

	void SetMeshId(int mesh_id) { m_mesh_id = mesh_id; }
	void SetShaderId(int shader_id) { m_shader_id = shader_id; }
	void SetTextureId(int tex_id) { m_tex_id = tex_id; }
	void SetColor(glm::vec3 const & color) { m_color = color; }
	void SetDrawWireframe(bool wireframe = true) { m_draw_wireframe = wireframe; }

	int GetMeshId() const { return m_mesh_id; }
	int GetShaderId() const { return m_shader_id; }
	int GetTextureId() const { return m_tex_id; }
	glm::vec3 const & GetColor() const { return m_color; }
	bool GetDrawWireframe() const { return m_draw_wireframe; }

	glm::mat4 & ModifyModelTransform() { return m_world_transform; }
	glm::mat4 const & GetModelTransform() const { return m_world_transform; }

private:
	int m_mesh_id{ -1 };
	int m_shader_id{ -1 };
	int m_tex_id{ -1 };

	glm::vec3 m_color;

	bool m_draw_wireframe{ false };

	glm::mat4 m_world_transform{ glm::mat4(1.0) };
};
