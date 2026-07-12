//Stew Nowak
//Final Project (FP) Advanced Graphics
//Date last edited: 5/3/26


#ifndef FP_ENGINE_H
#define FP_ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <fstream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/random.hpp>
#include <array>
#include <chrono>
#include <unordered_map>
#include <random>
#include "ArcballCam.hpp"

//defining the enableValidationLayers bool
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


class FPEngine {

    public:
        /*****************************/
        /*ENGINE INTERFACE BELOW HERE*/
        /*****************************/

        /**
         * constructor for our engine
         */
        FPEngine();

        /**
         * destructor for our engine
         */
        ~FPEngine();
        
        /**
         * run function for our application to be called in in main.cpp
         */
        void run();

         /**
         * a proxy/wrapper for the extension function vkCreateDebugUtilsMessengerEXT
         * \param instance
         * \param pCreateInfo
         * \param pAllocator
         * \param pDebugMessenger
         */
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator,
                                                VkDebugUtilsMessengerEXT* pDebugMessenger);

        /**
         * another proxy/wrapper for the extension function vkDestroyDebugUtilsMessengerEXT
         * \param instance
         * \param debugMessenger
         * \param pAllocator
         */
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

        /*******************************/
        /*CALLBACK FUNCTIONS BELOW HERE*/
        /*******************************/

        /**
         * mouse button callback function
         * \param button
         * \param action
         */
        void onMouseButton(int button, int action);

        /**
         * mouse callback function
         * \param xPos
         * \param yPos
         */
        void onMouseMove(double xPos, double yPos);

        /**
         * scroll callback function
         * \param yOffset
         */
        void onScroll(double yOffset);

        /**
         * key press callback function
         */
        void onKeyPress(int key, int mods, int action);

        /**
         * frame buffer resize callback function -> set framebufferResized member variable to true
         */
        void notifyFramebufferResized() {framebufferResized = true;}

        /********************/
        /*STRUCTS BELOW HERE*/
        /********************/

        /*GRAPHICS PIPELINE STRUCTS*/
        ///struct that bundles the indices of different queue families
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsAndComputeFamily;
            std::optional<uint32_t> presentFamily;
            bool isComplete() {
                return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
            }
        };

        ///struct for holding details that we need to create the swap chain: basic surface capabilities, surface formats and available presentation modes
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        /*RAYMARCHING PIPELINE STRUCTS*/

        ///UBO struct containing constant scene information
        struct UniformBufferObject {
            alignas(16) glm::mat4 view;       //view matrix
            alignas(16) glm::mat4 proj;       //proj. matrix
            alignas(16) glm::vec3 lightPos;   //light's position
            alignas(16) glm::vec3 viewPos;    //camera's position
            alignas(16) glm::vec2 resolution; //resolution of our current viewport
            alignas(16) glm::vec4 params;     //vector that holds the following values: powerLevel, hitEpsilon, time, and unused
            //x-value: power level of our mandelbulb
            //y-value: epsilon value for checking hits on fractal, shadows, 
            //z-value: time since start of program
            //w-value: how we color our fractal (through normal or orbit trap lerp)
        };


    private:
        
        /*************************/
        /*DATA MEMBERS BELOW HERE*/
        /*************************/

        /*GRAPHICS PIPELINE MEMBERS*/
        GLFWwindow* window;                               //handle to glfw window object
        VkInstance instance;                              //handle to Vulkan instance
        VkDebugUtilsMessengerEXT debugMessenger;          //handle to debug messenger
        VkSurfaceKHR surface;                             //KHR surface object member
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; //handle to our graphics card/physical device
        VkDevice device;                                  //member to store the logical device in
        VkQueue graphicsQueue;                            //handle to the graphics queue ----->device queuees are implicitly cleaned up when device is destroyed
        VkQueue presentQueue;                             //handle for presentation queue
        VkSwapchainKHR swapChain;                         //handle to the swapchain object
        std::vector<VkImage> swapChainImages;             //hanlde to container of VkImages
        VkFormat swapChainImageFormat;                    //variable to store the swap chain image format
        VkExtent2D swapChainExtent;                       //variable to store the swap chain chain image extent
        std::vector<VkImageView> swapChainImageViews;     //container to store the image views in
        VkDescriptorPool descriptorPool;                  //create the descriptor pool
        std::vector<VkFramebuffer> swapChainFramebuffers; //member to hold the framebuffers
        VkCommandPool commandPool;                        //member to hold the VkCommandPool
        std::vector<VkCommandBuffer> commandBuffers;      //member to hold VkCommandBuffer
        bool framebufferResized = false;                  //member variable that flags that a resize has happened
        uint32_t currentFrame = 0;                        //object to keep track of current frame i.e frame index

        /*SYNCHRONIZATION MEMBERS*/
        std::vector<VkSemaphore> imageAvailableSemaphores;  //container for semaphores for synchronizing between the presentation engine and the GPU (image is ready to render into)
        std::vector<VkSemaphore> renderFinishedSemaphores;  //container for semaphores for syncrhonizing bewteen the GPU and the presentation engine (image can be presented)
        std::vector<VkSemaphore> computeFinishedSemaphores; //container for semaphores for synchronizing between compute pipeline and the graphics pipeline commands (prevent read-after-write and write-after-read hazard)
        std::vector<VkFence> inFlightFences;                //container for fences for syncrhonizing between the CPU and the GPU (CPU must not start a new frame until GPU is finished with prev. one)
        std::vector<VkFence> computeInFlightFences;         //container for fences for synchronizing between compute pipeline and graphics pipeline

        /*COMPUTE/RAYMARCHING PASS MEMBERS*/
        VkQueue computeQueue;                               //handle for compute queue
        VkPipelineLayout computePipelineLayout;
        VkPipeline computePipeline;
        VkDescriptorSetLayout computeDescriptorSetLayout;
        std::vector<VkDescriptorSet> computeDescriptorSets; //member to hold the descriptor set handles (compute)
        std::vector<VkCommandBuffer> computeCommandBuffers; //container for compute pipeline command buffers
        std::vector<VkBuffer> uniformBuffers;               //container for uniformBuffers
        std::vector<VkDeviceMemory> uniformBuffersMemory;   //container for all memory used for our UBOs
        std::vector<void*> uniformBuffersMapped;            //void pointers for the data transfer for each of our UBOs
        std::vector<VkImage> hdrImages;                     //container for hdr storage images
        std::vector<VkDeviceMemory> hdrImageMemories;       //container for hdr storage images' memory
        std::vector<VkImageView> hdrImageViews;             //container for hdr image views
        float powerLevel = 8.0f;                                 //user controlled variable that controls the power level of the fractal mandelbulb
        float hitEpsilon = 0.0005f;                          //user controlled variable that controls how close we need to be to determine we hit the fractal
        float colorNormal = 0.5f;

        /*RESOLVE PASS MEMBERS*/
        VkRenderPass resolveRenderPass;                     //variable to hold render pass object (for our resolve pass)
        VkDescriptorSetLayout resolveDescriptorSetLayout;   //object for the descriptor set layout
        VkPipelineLayout resolvePipelineLayout;             //object to hold the pipeline layout
        VkPipeline resolvePipeline;                         //member to hold the VkPipeline object
        std::vector<VkDescriptorSet> resolveDescriptorSets; //member to hold the descriptor set handles (graphics)
        VkSampler hdrSampler;                               //member to hold the sampler for hdr storage image

        /*MOUSE MEMBERS*/
        bool mousePressed = false;  //bool for whether our mouse was pressed or not
        double lastMouseX = 0.0;    //last recorded xPos of mouse
        double lastMouseY = 0.0;    //last recorded yPos of mouse
        float rotateSpeed = 0.005f; //rotation speed of camera based on mouse movement
        float zoomSpeed = 0.10f;    //zoom speed of camera based on scroll/mouse movement
        
        /*SCENE/CONST. MEMBERS*/
        float classClock = 0.f;                                       //class "global" clock variable
        ArcballCam camera;                                      //container/handle to our arcball camera
        glm::vec3 lightPos = glm::vec3(-4.f, 6.f, -4.f);        //container for light's position
        bool dynamicLight = false;                              //bool that determines whether our point light moves or not
        const uint32_t WIDTH = 1200;                            //constant for window width
        const uint32_t HEIGHT = 800;                            //constant for window height
        const int MAX_FRAMES_IN_FLIGHT = 2;                     //constant that defines how many frames should be processed concurrently
        int currentFPS;                                         //variable to track our programs current FPS
        int averageFPS;                                         //variable to track our programs average FPS


        /*VALIDATION LAYERS & DEVICE EXTENSIONS*/
        ///configuration variables to specify the layers to enable and whether to enable them or not (aka list of validation layers)
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        ///list of required device extensions --Vulkan header file provides macro VK_KHR_SWAPCHAIN_EXTENSION_NAME that is defined as VK_KHR_swapchain
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        #ifdef __APPLE__ //for Mac builds so professor can grade better
        ,
            "VK_KHR_portability_subset"
        #endif
        };


        /**********************/
        /*FUNCTIONS BELOW HERE*/
        /**********************/

        /**
         * Initializes the glfw window using the glfw library
         */
        void initWindow();

        /**
         * This is where vulkan objects are created and called
         */   
        void initVulkan();

       /**
        * main rendering loop for frames
         */ 
        void mainLoop();

        /**
         * This is the cleanup function for deleting all objects when finished with rendering
         */
        void cleanup();

        /**
         * This is for creating a Vulkan instance and creating necessary structs to make an instance
         */
        void createInstance();

        /**
         * checks if all of the requested layers are available
         */ 
        bool checkValidationLayerSupport();

        /**
         * returns the required list of extensions based on whether validation layeres are enabled or not
         */
        std::vector<const char*> getRequiredExtensions();

        /**
         * debug callback function
         * \param messageSeverity
         * \param messageType
         * \param pCallbackdata
         * \param pUserData
         */
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackdata,
            void* pUserData);

        /**
         * sets up the debug messenger createInfo struct with neccessary data
         */
        void setupDebugMessenger();

        /**
         * creation and population of the messenger create info struct for debug messenger object
         * \param createInfo
         */
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        /**
         * makes a list of all suitable graphics cards/devices with Vulkan support
         * gives each device a score and ensures we pick the highest one
         */
        void pickPhysicalDevice();

        /**
         * evaulates a score to map to/for a physical device/GPU
         * This function is much like the isDeviceSuitable() function
         * this function can also be morphed in the future for more explicit selection process
         * Another idea is to display the names of the choices available and allow the user to choose
         * \param device
         */
        int rateDeviceSuitability(VkPhysicalDevice device);

        /**
         * finds queue family indiecs to populate the struct above
         * checks to see what queue families are supported by the device, and which of these supports the commands we want to use
         * \param device
         */
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        /**
         * creates a logical device to interface with physical device
         */
        void createLogicalDevice();

        /**
         * creates a window surface object
         */
        void createSurface();

        /**
         * checks all the extensions the device offers/has available and ensures the required extensions are present
         * \param device
         */
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        /**
         * populates the SwapChainSupportDetails struct for swapchain creation
         * \param device
         */
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        /**
         * To choose the right settings for the creation of the swap chain, we need to find the Surface format (color depth) settings i.e a format and a colorSpace member
         * pass the formats member of the SwapChainSupportDetails struct as the argument to see available formats we found when querying swap chain support details
         * \param availableFormats
         */
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        /**
         * To choose the right settings for the creation of the swap chain, we need to pick one of the four possible presentation modes
         * that bests represents what we need -- uses the VkPresentModeKHR vector from SwapChainSupportDetails struct
         */
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        /**
         * To choose the right settings for the creation of the swap chain, we need to find the right settings of the Swap extent (VkSurfaceCapabilities)
         * utilize the SwapChainSupportDetails struct to get the capabilites (VkSurfaceCapabilitiesKHR)
         * Swap extent is the resolution of the swap chain images - must specify the resolution in the correct unit
         * \param capabilities
         */
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        /**
         * Creates the swap chain utilizing all helper functions for the correct settings
         */
        void createSwapChain();

        /**
         * creates a basic image view for every image in the swap chain so that we can use them as color targets later on
         * image->VkImage object, image view->VkImageView
         */
        void createImageViews();

        /**
         * creates the graphics pipeline: all fixed-function stages and programmable stages
         */
        void createResolvePipeline();

        /**
         * helper function to load the binary data from the spir-v shader files
         * reads all of the bytes from the specified file and return them in a byte array managed by std::vector
         * \param filename
         */
        static std::vector<char> readFile(const std::string& filename);

        /**
         * creates a VkShaderModule object to pass the compiled shader code to pipeline
         * takes a buffer with the bytecode as parameter and creates a VkShaderModule from it
         * \param code
         */
        VkShaderModule createShaderModule(const std::vector<char>& code);

        /**
         * creates the render pass object that is used to specify how many color and depth buffers there will be
         * how many samples to use for each of them and how their contents should be handled throughout the rendering operations
         */
        void createResolveRenderPass();

        /**
         *creates the objects for the swapChainFramebuffers array
        */
        void createFramebuffers();

        /**
         * creates a command pool to manage the memory that is used to store the buffers and command buffers are allocated from them
         */
        void createCommandPool();

        /**
         * allocates a single command buffer from the command pool
         */
        void createCommandBuffers();

        /**
         * function that writes the commands we want to execute into a command buffer
         * takes the VkCommandBuffer (commandBuffer object) and the index of the current swapchain image we want to write to as parameters
         * \param commandBuffer
         * \param imageIndex
         */
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        /**
         * renders to a frame
         */
        void drawFrame();

        /**
         * creates the sync objects (semaphores and fences)
         */
        void createSyncObjects();

        /**
         * calls createSwapChain and all of the creation functions for the objects that depend on the swap chain or window size
        */
        void recreateSwapChain();

        /**
         * makes sure that the old versions of the swap chain, image view, and framebuffers are cleanup before recreating them
        */
        void cleanupSwapChain();

        /**
         * combine the requirements of the buffer and our own application to find the right type of memory to use
         * \param typeFilter
         * \param properties
         */
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        /**
         * creates a buffer to be used for a variety of purposes which is why there are so many arguments 
         * (can specify what kind of buffer we want to create)
         * \param size
         * \param usage
         * \param properties
         * \param buffer
         * \param bufferMemory
         */
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        /**
         * creates a descriptor layout to provide details about every descriptor binding used in the shaders for pipeline creation
         */
        void createResolveDescriptorSetLayout();

        /**
         * allocates descriptor sets
         */
        void createResolveDescriptorSets();

        /**
         * creates a descriptor pool so we can allocate descriptor sets
         */
        void createDescriptorPool();        
    
        /**
         * creates the uniform buffers
         */
        void createUniformBuffers();

        /**
         * generates a new transformation every frame to make the geometry spin around
         * \param currentImage
         */
        void updateUniformBuffer(uint32_t currentImage);

        /**
         * creates an arcball camera and sets the parameters as wanted
         */
        void createCamera();    

        /**
         * creates the dedicated compute pipeline for running compute commands
         */
        void createComputePipeline();

        /**
         * creates the descriptor set layout for compute pipeline
         */
        void createComputeDescriptorSetLayout();

        /**
         * creates the compute descriptor sets allowing compute shader to have access to the last and current frames' SSBOs
         */
        void createComputeDescriptorSets();

        /**
         * records the command buffer for the compute pipeline
         */
        void recordComputeCommandBuffer(VkCommandBuffer commandBuffer);

        /**
         * creates the compute pipeline's command buffers
         */
        void createComputeCommandBuffers();

        /**
         * abstracted function that creates Images
         * \param width
         * \param height
         * \param format
         * \param tiling
         * \param usage
         * \param properties
         * \param image
         * \param imageMemory
         */
        void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        
        /**
         * function that abstracts making an image view object (VkImageView)
         * \param image
         * \param format
         * \param aspectFlags 
         */
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        
        /**
         *  handles layout transitions
         * \param image
         * \param format
         * \param oldLayout
         * \param newLayout
         */
        void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
        
        /**
         * records and executes a command buffer
         */
        VkCommandBuffer beginSingleTimeCommands();

        /**
         * submits a one time time commandBuffer to the queue and frees the command buffer
         * \param commandBuffer
         */
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        /**
         * creates the hdr storage images (16-bit float format)
         */
        void createHDRImages();

        /**
         * creates the sampler for sampling the hdr storage image
         */
        void createHDRSampler();

        /**
         * creates the FPS tracker
         */
        void createFPS();

    };


/*CALLBACK FUNCTION DELCARATIONS*/
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);



#endif