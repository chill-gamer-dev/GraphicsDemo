#version 450

layout(push_constant) uniform ObjectData {
	mat4 model;
} obj_data;

layout(binding = 0) uniform ViewProjUniform {
	mat4 view;
	mat4 proj;
} transform_ubo;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

layout(location = 0) out vec3 out_pos_world;
layout(location = 1) out vec3 out_normal_world;
layout(location = 2) out vec3 out_color;

void main()
{
	vec4 pos_world_vec4 = obj_data.model * vec4(in_pos, 1.0);

	out_pos_world = vec3(pos_world_vec4);
	out_normal_world = vec3(obj_data.model * vec4(in_normal, 0.0));
	out_color = in_color;

	gl_Position = transform_ubo.proj * transform_ubo.view * pos_world_vec4;
}
