#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGuiApplication>
#include <QImage>
#include <QMenuBar>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QWindow>

#ifdef Q_OS_WIN
#define VK_USE_PLATFORM_WIN32_KHR
#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME     VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(Q_OS_LINUX)
#define VK_USE_PLATFORM_XCB_KHR
#define VK_KHR_XCB_SURFACE
#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME     VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(Q_OS_MACOS)
#define VK_USE_PLATFORM_MACOS_MVK
#define VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME     VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#endif

#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <vector>

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class VulkanWindow : public QWindow
{
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject
    {
        float scaleX;
        float scaleY;
        float offsetX;
        float offsetY;
    };

    const std::vector<Vertex> vertices = {{{-1.0f, -1.0f}, {0.0f, 0.0f}},
                                          {{1.0f, -1.0f}, {1.0f, 0.0f}},
                                          {{1.0f, 1.0f}, {1.0f, 1.0f}},
                                          {{-1.0f, 1.0f}, {0.0f, 1.0f}}};

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

public:
    VulkanWindow();

protected:
    bool event(QEvent *e) override;

private:
    int m_timerId;
    bool m_mousePressed = false;
    QPoint m_panStart;
    int32_t m_mipLevels;
    VkExtent2D m_viewport;
    VkOffset2D m_viewportOffset;
    UniformBufferObject m_dynamicParameters;
    int32_t m_texWidth;
    int32_t m_texHeight;
    float m_zoomValueX;
    float m_zoomValueY;
    int32_t m_offsetX;
    int32_t m_offsetY;
    float m_normOffsetX;
    float m_normOffsetY;

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkSurfaceKHR m_surface;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;

    VkRenderPass m_renderPass;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkCommandPool m_commandPool;

    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;
    std::vector<void *> m_uniformBuffersMapped;

    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkCommandBuffer> m_commandBuffers;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    uint32_t m_currentFrame = 0;

    bool m_framebufferResized = false;
    VkSurfaceKHR createSurface(QWindow *window, VkInstance instance);

    void initVulkan(const QString &imageName);

    void cleanupSwapChain();

    void cleanup();

    void recreateSwapChain();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    void setupDebugMessenger();

    void createInstance();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();

    void createCommandPool();

    void initializeScaling(uint32_t imageWidth,
                           uint32_t imageHeight,
                           uint32_t viewportWidth,
                           uint32_t viewportHeight);

    void imagePan(int32_t dY, int32_t dX);

    void onZoomToPixel(float cursorX, float cursorY, bool zoomIn);

    void createTextureImage(const QString &imageName);

    void generateMipmaps(VkImage image,
                         VkFormat imageFormat,
                         int32_t texWidth,
                         int32_t texHeight,
                         uint32_t mipLevels);

    void createTextureImageView();

    void createTextureSampler();

    VkImageView createImageView(VkImage image, VkFormat format, uint32_t mipLevels);

    void createImage(uint32_t width,
                     uint32_t height,
                     uint32_t mipLevels,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory);

    void transitionImageLayout(VkImage image,
                               VkFormat format,
                               VkImageLayout oldLayout,
                               VkImageLayout newLayout,
                               uint32_t mipLevels);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createDescriptorPool();

    void createDescriptorSets();

    void createBuffer(VkDeviceSize size,
                      VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage);

    void drawFrame();

    VkShaderModule createShaderModule(const std::vector<char> &code);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char *> getRequiredExtensions();

    static std::vector<char> readFile(const std::string &filename);

    bool checkValidationLayerSupport();

    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData);

    QString openImage();
};

#endif // VULKANWINDOW_H
