// GraphicsApi.ixx

module;

#include <functional>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module GraphicsApi;

import <string>;
import <vector>;

export class GraphicsApi
{
public:
	GraphicsApi(
		GLFWwindow * window,
		std::string const & app_title,
		uint32_t extension_count,
		char const ** extensions);

	~GraphicsApi();

	void DrawFrame(std::function<void()> render_fn);
	void WaitForLastFrame() const;

	VkDevice GetDevice() const { return m_logical_device; }
	VkFormat GetSwapChainImageFormat() const { return m_swap_chain_image_format; }
	VkExtent2D GetSwapChainExtent() const { return m_swap_chain_extent; }
	VkRenderPass GetRenderPass() const { return m_render_pass; }
	VkCommandBuffer GetCommandBuffer() const { return m_command_buffer; }
	VkFramebuffer GetCurFrameBuffer() const { return m_swap_chain_framebuffers[m_current_image_index]; }

private:
	VkInstance m_instance = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;

	VkPhysicalDevice m_physical_device = VK_NULL_HANDLE; // Automatically cleaned up when m_instance is destroyed
	VkDevice m_logical_device = VK_NULL_HANDLE;
	VkQueue m_graphics_queue = VK_NULL_HANDLE; // Automatically cleaned up when m_logical_device is destroyed
	VkQueue m_present_queue = VK_NULL_HANDLE; // Automatically cleaned up when m_logical_device is destroyed

	VkSwapchainKHR m_swap_chain = VK_NULL_HANDLE;
	std::vector<VkImage> m_swap_chain_images; // Automatically cleaned up when m_swap_chain is destroyed
	VkFormat m_swap_chain_image_format = VK_FORMAT_UNDEFINED;
	VkExtent2D m_swap_chain_extent{ 0, 0 };

	VkRenderPass m_render_pass = VK_NULL_HANDLE;
	std::vector<VkImageView> m_swap_chain_image_views;
	std::vector<VkFramebuffer> m_swap_chain_framebuffers;
	uint32_t m_current_image_index = 0;

	VkCommandPool m_command_pool = VK_NULL_HANDLE;
	VkCommandBuffer m_command_buffer = VK_NULL_HANDLE; // Automatically cleaned up when m_comand_pool is destroyed

	VkSemaphore m_image_available_semaphore;
	VkSemaphore m_render_finished_semaphore;
	VkFence m_in_flight_fence;

	std::vector<const char *> const m_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<char const *> const m_validation_layers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	bool const m_enable_validation_layers = false;
#else
	bool const m_enable_validation_layers = true;
#endif
};
