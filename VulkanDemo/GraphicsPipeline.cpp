// GraphicsPipeline.cpp

module;

#include <iostream>

#include <vulkan/vulkan.h>

module GraphicsPipeline;

import GraphicsApi;

namespace
{
	VkPipelineLayout create_pipeline_layout(
		VkDevice device,
		VkDescriptorSetLayout descriptor_set_layout,
		std::vector<VkPushConstantRange> push_constants_ranges)
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &descriptor_set_layout,
			.pushConstantRangeCount = static_cast<uint32_t>(push_constants_ranges.size()),
			.pPushConstantRanges = push_constants_ranges.data(),
		};

		VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
		VkResult result = vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &pipeline_layout);
		if (result != VK_SUCCESS)
			std::cout << "Failed to create pipeline layout" << std::endl;

		return pipeline_layout;
	}

	VkPipeline create_graphics_pipeline(
		VkDevice device,
		VkRenderPass render_pass,
		VkPipelineLayout pipeline_layout,
		std::vector<VkPipelineShaderStageCreateInfo> const & shader_stages,
		VkVertexInputBindingDescription const & binding_desc,
		std::vector<VkVertexInputAttributeDescription> const & attrib_descs)
	{
		std::vector<VkDynamicState> dynamic_states = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamic_state{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
			.pDynamicStates = dynamic_states.data()
		};

		VkPipelineViewportStateCreateInfo viewport_state{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount = 1
		};

		VkPipelineVertexInputStateCreateInfo vertex_input_info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &binding_desc,
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrib_descs.size()),
			.pVertexAttributeDescriptions = attrib_descs.data()
		};

		VkPipelineInputAssemblyStateCreateInfo input_assembly{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};

		VkPipelineRasterizationStateCreateInfo rasterizer{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f
		};

		VkPipelineMultisampleStateCreateInfo multisampling{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		VkPipelineColorBlendAttachmentState color_blend_attachment{ // per framebuffer blending options
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			//.blendEnable = VK_TRUE,
			//.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			//.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			//.colorBlendOp = VK_BLEND_OP_ADD,
			//.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			//.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			//.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		VkPipelineColorBlendStateCreateInfo color_blending{ // global blending options
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &color_blend_attachment,
			.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
		};

		VkGraphicsPipelineCreateInfo pipeline_info{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = static_cast<uint32_t>(shader_stages.size()),
			.pStages = shader_stages.data(),
			.pVertexInputState = &vertex_input_info,
			.pInputAssemblyState = &input_assembly,
			.pViewportState = &viewport_state,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multisampling,
			.pDepthStencilState = nullptr,
			.pColorBlendState = &color_blending,
			.pDynamicState = &dynamic_state,
			.layout = pipeline_layout,
			.renderPass = render_pass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		VkPipeline graphics_pipeline = VK_NULL_HANDLE;
		VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline);
		if (result != VK_SUCCESS)
			std::cout << "Failed to create graphics pipeline" << std::endl;

		return graphics_pipeline;
	}

	VkDescriptorSetLayout create_descriptor_set_layout(VkDevice device)
	{
		VkDescriptorSetLayoutBinding ubo_layout_binding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.pImmutableSamplers = nullptr
		};

		VkDescriptorSetLayoutCreateInfo layout_info{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &ubo_layout_binding
		};

		VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
		VkResult result = vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &descriptor_set_layout);
		if (result != VK_SUCCESS)
			std::cout << "Failed to create ubo descriptor set layout";

		return descriptor_set_layout;
	}

	VkDescriptorPool create_descriptor_pool(VkDevice device, uint32_t descriptor_count)
	{
		VkDescriptorPoolSize pool_size{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = descriptor_count
		};

		VkDescriptorPoolCreateInfo pool_info{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = descriptor_count,
			.poolSizeCount = 1,
			.pPoolSizes = &pool_size
		};

		VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
		VkResult result = vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptor_pool);
		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor pool!");

		return descriptor_pool;
	}

	template <uint32_t count>
	std::array<VkDescriptorSet, count> create_descriptor_sets(
		VkDevice device,
		VkDescriptorSetLayout layout,
		VkDescriptorPool pool,
		std::array<VkBuffer, count> uniform_buffers,
		VkDeviceSize uniform_size)
	{
		std::array<VkDescriptorSetLayout, count> layouts;
		layouts.fill(layout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = count;
		allocInfo.pSetLayouts = layouts.data();

		std::array<VkDescriptorSet, count> descriptor_sets;
		if (vkAllocateDescriptorSets(device, &allocInfo, descriptor_sets.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");

		for (size_t i = 0; i < count; i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniform_buffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = uniform_size; // or VK_WHOLE_SIZE

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptor_sets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr;
			descriptorWrite.pTexelBufferView = nullptr;

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}

		return descriptor_sets;
	}
}

GraphicsPipeline::GraphicsPipeline(GraphicsApi const & graphics_api,
	VkShaderModule vert_shader_module,
	VkShaderModule frag_shader_module,
	VkVertexInputBindingDescription const & binding_desc,
	std::vector<VkVertexInputAttributeDescription> const & attrib_descs,
	std::vector<VkPushConstantRange> push_constants_ranges,
	VkDeviceSize uniform_size,
	PerFrameConstantsCallback per_frame_constants_callback,
	PerObjectConstantsCallback per_object_constants_callback)
	: m_graphics_api(graphics_api)
	, m_per_frame_constants_callback(per_frame_constants_callback)
	, m_per_object_constants_callback(per_object_constants_callback)
{
	VkDevice device = m_graphics_api.GetDevice();

	m_descriptor_set_layout = create_descriptor_set_layout(device);
	m_descriptor_pool = create_descriptor_pool(device, GraphicsApi::m_max_frames_in_flight);
	create_uniform_buffers(uniform_size);
	m_descriptor_sets = create_descriptor_sets<GraphicsApi::m_max_frames_in_flight>(
		device, m_descriptor_set_layout, m_descriptor_pool, m_uniform_buffers, uniform_size);

	m_pipeline_layout = create_pipeline_layout(device, m_descriptor_set_layout, push_constants_ranges);
	if (m_pipeline_layout == VK_NULL_HANDLE)
		return;

	VkPipelineShaderStageCreateInfo vert_shader_stage_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vert_shader_module,
		.pName = "main"
	};

	VkPipelineShaderStageCreateInfo frag_shader_stage_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = frag_shader_module,
		.pName = "main"
	};

	std::vector<VkPipelineShaderStageCreateInfo> shader_stages{
		vert_shader_stage_info,
		frag_shader_stage_info
	};

	m_graphics_pipeline = create_graphics_pipeline(
		device,
		m_graphics_api.GetRenderPass(),
		m_pipeline_layout,
		shader_stages,
		binding_desc,
		attrib_descs);
}

GraphicsPipeline::~GraphicsPipeline()
{
	VkDevice device = m_graphics_api.GetDevice();

	for (size_t i = 0; i < m_uniform_buffers.size(); i++) {
		vkDestroyBuffer(device, m_uniform_buffers[i], nullptr);
		vkFreeMemory(device, m_uniform_buffers_memory[i], nullptr);
	}

	vkDestroyDescriptorPool(device, m_descriptor_pool, nullptr);
	vkDestroyDescriptorSetLayout(device, m_descriptor_set_layout, nullptr);

	vkDestroyPipeline(device, m_graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(device, m_pipeline_layout, nullptr);
}

void GraphicsPipeline::Activate() const
{
	if (m_graphics_pipeline == VK_NULL_HANDLE)
	{
		std::cout << "Activating invalid graphics pipeline" << std::endl;
		return;
	}

	VkCommandBuffer command_buffer = m_graphics_api.GetCurCommandBuffer();

	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);

	VkDescriptorSet descriptor_set = m_descriptor_sets[m_graphics_api.GetCurFrameIndex()];
	vkCmdBindDescriptorSets(command_buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_pipeline_layout,
		0,
		1,
		&descriptor_set,
		0,
		nullptr);
}

void GraphicsPipeline::UpdatePerFrameConstants() const
{
	if (m_per_frame_constants_callback)
		m_per_frame_constants_callback(*this);
}

void GraphicsPipeline::UpdatePerObjectConstants(RenderObject const & obj) const
{
	if (m_per_object_constants_callback)
		m_per_object_constants_callback(*this, obj);
}

void GraphicsPipeline::create_uniform_buffers(VkDeviceSize buffer_size)
{
	VkDevice device = m_graphics_api.GetDevice();

	for (size_t i = 0; i < GraphicsApi::m_max_frames_in_flight; i++)
	{
		m_graphics_api.CreateBuffer(
			buffer_size,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_uniform_buffers[i],
			m_uniform_buffers_memory[i]);

		vkMapMemory(
			device,
			m_uniform_buffers_memory[i],
			0 /*offset*/,
			buffer_size,
			0 /*flags*/,
			&m_uniform_buffers_mapped[i]);
	}
}
