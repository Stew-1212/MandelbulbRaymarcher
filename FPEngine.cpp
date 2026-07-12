//Stew Nowak
//Final Project Advanced Graphics
//Date last edited: 5/3/26

//include the stb_image.h and tiny_obj_loader.h here so we don't get double declaration trouble
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "FPEngine.h"


/*****************************/
/*ENGINE INTERFACE BELOW HERE*/
/*****************************/

FPEngine::FPEngine() {};

FPEngine::~FPEngine() {};

void FPEngine::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

 /**
 * a proxy/wrapper for the extension function vkCreateDebugUtilsMessengerEXT
 */
VkResult FPEngine::CreateDebugUtilsMessengerEXT(VkInstance instance,
                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkDebugUtilsMessengerEXT* pDebugMessenger) {
    //get address of the function we need using the vkGetInstanceProcAddr() func
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    //check to see if we received nullptr or not to know if we can use it or not
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/**
 * another proxy/wrapper for the extension function vkDestroyDebugUtilsMessengerEXT
 */
void FPEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

/*******************************/
/*CALLBACK FUNCTIONS BELOW HERE*/
/*******************************/

void FPEngine::onMouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void FPEngine::onMouseMove(double xPos, double yPos) {
    if (!mousePressed) return;

    double dx = xPos - lastMouseX;
    double dy = yPos - lastMouseY;

    camera.updateTheta(dx * rotateSpeed);
    camera.updatePhi(dy * rotateSpeed);
    camera.recomputeOrientation();
    lastMouseX = xPos;
    lastMouseY = yPos;
}

void FPEngine::onScroll(double yOffset) {
    if (yOffset > 0) {
        camera.zoomIn(zoomSpeed);
    } else if (yOffset < 0) {
        camera.zoomOut(zoomSpeed);
    }
}


void FPEngine::onKeyPress(int key, int mods, int action) {

    //only respond to press or repeat events
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    //create case conditions (if needed)
    bool shift = mods & GLFW_MOD_SHIFT;
    bool ctrl = mods & GLFW_MOD_CONTROL;
    bool alt = mods & GLFW_MOD_ALT;

    switch (key) {
        case GLFW_KEY_0:
            powerLevel = 0.0;
            printf("Fractal power level will be looping from 2-10\n");
            fflush(stdout);
            break;
        case GLFW_KEY_1:
            powerLevel = 10.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_2:
            powerLevel = 2.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_3:
            powerLevel = 3.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_4:
            powerLevel = 4.0;
            printf("Fractal power level is now at: %d\n",int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_5:
            powerLevel = 5.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_6:
            powerLevel = 6.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_7:
            powerLevel = 7.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_8:
            powerLevel = 8.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_9:
            powerLevel = 9.0;
            printf("Fractal power level is now at: %d\n", int(powerLevel));
            fflush(stdout);
            break;
        case GLFW_KEY_UP:
            hitEpsilon += 0.0001;
            printf("\rDistance estimator epsilon value is now at: %f\n", hitEpsilon);
            fflush(stdout);
            break;
        case GLFW_KEY_DOWN:
            hitEpsilon -= 0.0001;
            printf("\rDistance estimator epsilon value is now at: %f\n", hitEpsilon);
            fflush(stdout);
            break;
        case GLFW_KEY_N:
            if (colorNormal < 1.0) {
                colorNormal = 1.0;
                printf("\rNormal coloring deactivated, and point*normal v1 coloring activated!\n");
                fflush(stdout);                
            } else if (colorNormal < 1.5) {
                colorNormal = 2.0;
                printf("\rPoint*nomral v1 coloring deactivated, and point*normal v2 coloring activated!\n");
                fflush(stdout);
            } else {
                colorNormal = 0.5;
                printf("\rPoint*normal v2 coloring deactivated, normal coloring activated!\n");
            }
            break;
        case GLFW_KEY_L:
            dynamicLight = !dynamicLight; //negate the dynamic bool value to opposite
            if (dynamicLight) {
                printf("\rDynamic lighting activated!\n");
                fflush(stdout);                
            } else {
                lightPos = glm::vec3(-3.f, 4.f, -3.f);
                printf("\rStatic lighting activated!\n");
                fflush(stdout);
            }
            break;
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_LEFT:
            break;
        default:
            break;
        }
}

/*****************************/
/*MEMBER FUNCTIONS BELOW HERE*/
/*****************************/

void FPEngine::initWindow() {
    glfwInit();                                     //first initialize the GLFW library
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   //now tell it not to create an OpenGL context
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //how to disable handling resizable windows

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); //intialize the actual window now

    glfwSetWindowUserPointer(window, this);                            //function that allows you to store an arbitrary pointer inside of it
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); //register callback for frame buffer resize (resizing the window)
    glfwSetCursorPosCallback(window, mouseMoveCallback);               //register callback for cursor position
    glfwSetMouseButtonCallback(window, mouseButtonCallback);           //register callback for mouse button
    glfwSetScrollCallback(window, scrollCallback);                     //register callback for mouse scroller
    glfwSetKeyCallback(window, keyCallback);                           //register callback for key press
}    

void FPEngine::initVulkan() {
    //first thing we do is initialize the Vulkan library by creating an instance
    createInstance();             //use the createInstance() function
    setupDebugMessenger();       //setup the debugMessenger object for debug messages
    createSurface();             //get the surface object created
    pickPhysicalDevice();        //now pick what GPU or device we will be using for this instance
    createLogicalDevice();       //create the logical device that will interface with the physical device
    createSwapChain();           //create the swap chain
    createImageViews();          //create the image views for images in the swap chain
    createResolveRenderPass();   //create the render pass object
    createCommandPool();         //create the command pool

    createHDRImages();
    createHDRSampler();
    createCamera();    //create the arcballCamera    
    createUniformBuffers();       //create the uniform buffers
    createFramebuffers();         //create the frame buffers



    createComputeDescriptorSetLayout(); //create descriptor set layout for compute pipeline
    createResolveDescriptorSetLayout(); //create the descriptor set layout (for uniforms)
    createDescriptorPool();             //Create the descriptor pool
    createComputeDescriptorSets();      //create the descriptor sets for compute pipeline
    createResolveDescriptorSets();      //create the descriptor sets for graphics pipeline    

    createComputePipeline();     //create the compute pipeline
    createResolvePipeline();    //create the graphics pipeline

    createCommandBuffers(); //create the command buffers for graphics pipeline
    createComputeCommandBuffers(); //create the command buffers for compute pipeline
    createSyncObjects();    //create the sync objects
}

void FPEngine::mainLoop() {
    //while the window hasnt been closed poll for events
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }
    //should wait for the logical device to finish operations before exiting mainLoop and destroying the window
    vkDeviceWaitIdle(device);
}

void FPEngine::cleanup() {
    cleanupSwapChain();                            //call cleanupSwapChain() to clean up swapchain and related dependencies
    vkDestroySampler(device, hdrSampler, nullptr); //clean up the sampler used for the hdr storage image

    //clean up the uniform buffers's after we stop rendering
    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        //first UBO's
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorSetLayout(device, resolveDescriptorSetLayout, nullptr); //cleanup the descriptor set layout (layout should stick around while we may create new graphics pipelines)
    vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayout, nullptr); //cleanup the descriptor set layout for compute pipeline
    
    vkDestroyPipeline(device, resolvePipeline, nullptr);            //cleanup graphics pipeline
    vkDestroyPipelineLayout(device, resolvePipelineLayout, nullptr);        //cleanup the graphics pipeline layout
    vkDestroyPipeline(device, computePipeline, nullptr);             //cleanup the compute pipeline
    vkDestroyPipelineLayout(device, computePipelineLayout, nullptr); //cleanup the compute pipeline layout
    vkDestroyRenderPass(device, resolveRenderPass, nullptr);                //cleanup the render pass

    //cleanup the sync objects
    for (size_t i=0; i<swapChainImages.size(); i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr); //destroy rendered finished semaphores
    }
    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        //first graphics pipeline sync objects
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr); //destroy image available semaphores
        vkDestroyFence(device, inFlightFences[i], nullptr);               //destroy in flight fences
        
        //now compute pipeline sync objects
        vkDestroySemaphore(device, computeFinishedSemaphores[i], nullptr);
        vkDestroyFence(device, computeInFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(device, commandPool, nullptr);       //cleanup the command pool
    vkDestroyDescriptorPool(device, descriptorPool, nullptr); //clean up the descriptor pool -> automatically cleans up the descriptor sets 
    vkDestroyDevice(device, nullptr);                         //cleanup the logical device -Logical devices don't interact directly with instances, which is why it's not included as a parameter

    //check if validation layers are enabled
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr); //cleanup debug messenger object
    }

    vkDestroySurfaceKHR(instance, surface, nullptr); //destroy the surface object
    vkDestroyInstance(instance, nullptr);            //destroy the VkInstance (all other Vulkan resources should be cleaned up before the instance is destroyed)
    glfwDestroyWindow(window);                       //destroy the glfw window
    glfwTerminate();                                 //now terminate it
}

void FPEngine::createInstance() {
    //check validation layers
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{}; //create the application info. struct (optional but helpful)
    //fill in struct with relevant data (this is optional but helpful)  
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "A2: Perlin Noise";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "A2";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{}; //create next data struct (REQUIRED not optional)
    //fill in with necessary data (REQUIRED) and include the validation layer names
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = getRequiredExtensions(); //this portion utilizes the getRequiredExtensions() function
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    #ifdef __APPLE__ //for Prof. for Mac builds
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};    //create debugCreateInfo struct
    //last two members of struct determine the global validation layers to enable
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        //call the populateDebugMessengerCreateInfo function to fill in createInfo struct
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    //finally issue the instance call and double check that we got returned a success value
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    /*
        //this portion is not required but provides details about the Vulkan support
        //allocate an array to hold extension details - must get how many there are first
        //IMPORTANT: changed 'extensions' variable to be 'extensionProperties' so that later we can use 'extensions' for a different variable
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        //now allocate an array to hold the extension details
        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        //finally query the extension details
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());
        //list all extensions
        std::cout << "available extensions:\n";
        for (const auto& extension : extensionProperties) {
            std:: cout << '\t' << extension.extensionName << '\n';
        }
    */
}

bool FPEngine::checkValidationLayerSupport() {
    uint32_t layerCount;
    //list all of the avaiable layers using vkEnumerateInstanceLayerProperties function
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    //check if all the layers in validationLayers exist in the availableLayers list
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> FPEngine::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    #ifdef __APPLE__ //for Prof so Mac builds easier
    extensions.emplace_back(VK_KHR_PORTABLILITY_ENUMERATION_EXTENSION_NAME);
    #endif
    #ifdef __arm64__
    extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL FPEngine::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackdata,
    void* pUserData) {
        std::cerr << "validation layer: " << pCallbackdata->pMessage << std::endl;
        return VK_FALSE;
}

void FPEngine::setupDebugMessenger() {
    if (!enableValidationLayers) return; //if validations are not enabled return

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    //call the function to create the extension object if its available
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void FPEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    //fill in struct with details about the messenger and its callbacks
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    //specifies all the types of severities you would like your callback to be called for, all types except for VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT are specified (can change if neccessary)
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    //filters which type of message your callback is notified about, all types are enabled here (can change if neccessary)
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback; //specifies the pointer to the callback function
    createInfo.pUserData = nullptr; //optional
}

void FPEngine::pickPhysicalDevice() {
    uint32_t deviceCount = 0; //list the graphics cards available
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    //if there are 0 devices with Vulkan support then there is no point in going further
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    //else we
    std::vector<VkPhysicalDevice> devices(deviceCount); //allocate an array to hold all the VkPhysicalDevice handles
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //THIS IMPLEMENTATION IS WHERE WE PICK A PHYSCIAL DEVICE/GPU WITH HIGHEST SCORE BASED ON BELOW PARAMETERS
    std::multimap<int, VkPhysicalDevice> candidates; //use an ordered map to automatically sort candidates by increasing score

    //loop over all devices
    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);        //get a score for the device utilizing the rateDeviceSuitability() funct.
        candidates.insert(std::make_pair(score, device)); //insert this candidate with its corresponding score data
    }

    //check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;

    } else {
        throw std::runtime_error("failed to find suitable GPU!");
    }


    // Optional: print selected device properties
    // VkPhysicalDeviceProperties props{};
    // vkGetPhysicalDeviceProperties(physicalDevice, &props);

    // std::cout << "Selected GPU:\n";
    // std::cout << " Name: " << props.deviceName << "\n";

    // switch (props.deviceType) {
    //     case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    //         std::cout << " Type: Discrete GPU\n"; break;
    //     case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    //         std::cout << " Type: Integrated GPU\n"; break;
    //     case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    //         std::cout << " Type: Virtual GPU\n"; break;
    //     case VK_PHYSICAL_DEVICE_TYPE_CPU:
    //         std::cout << " Type: CPU\n"; break;
    //     default:
    //         std::cout << " Type: Other\n"; break;
    // }

    // std::cout << " Vendor ID: 0x" << std::hex << props.vendorID << std::dec << "\n";
    // std::cout << " Device ID: 0x" << std::hex << props.deviceID << std::dec << "\n";

    // std::cout << " Vulkan API Version: "
    //         << VK_VERSION_MAJOR(props.apiVersion) << "."
    //         << VK_VERSION_MINOR(props.apiVersion) << "."
    //         << VK_VERSION_PATCH(props.apiVersion) << "\n";

    // std::cout << " Max 2D Image Dimension: " << props.limits.maxImageDimension2D << "\n";

    // // Push constant size
    // uint32_t maxPush = props.limits.maxPushConstantsSize;
    // std::cout << " Max Push Constant Size: " << maxPush << " bytes\n";

    // //msaa limits
    // std::cout << "\n=== MSAA Sample Count Support ===\n";
    // VkSampleCountFlagBits maxSamples = getMaxUsableSampleCount();
    // std::cout << " Max Usable MSAA Samples: " << maxSamples << "\n";

    // // Compute shader limits
    // auto& limits = props.limits;

    // uint32_t maxGroupCountX = limits.maxComputeWorkGroupCount[0];
    // uint32_t maxGroupCountY = limits.maxComputeWorkGroupCount[1];
    // uint32_t maxGroupCountZ = limits.maxComputeWorkGroupCount[2];

    // uint32_t maxLocalSizeX = limits.maxComputeWorkGroupSize[0];
    // uint32_t maxLocalSizeY = limits.maxComputeWorkGroupSize[1];
    // uint32_t maxLocalSizeZ = limits.maxComputeWorkGroupSize[2];

    // uint32_t maxInvocations = limits.maxComputeWorkGroupInvocations;

    // std::cout << "\n===  Shader Limits ===\n";
    // std::cout << " Max Workgroup Count:  X=" << maxGroupCountX
    //         << "  Y=" << maxGroupCountY
    //         << "  Z=" << maxGroupCountZ << "\n";

    // std::cout << " Max Local Size:       X=" << maxLocalSizeX
    //         << "  Y=" << maxLocalSizeY
    //         << "  Z=" << maxLocalSizeZ << "\n";

    // std::cout << " Max Invocations Per Workgroup: " << maxInvocations << "\n";
}


int FPEngine::rateDeviceSuitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;              //create variable to hold device properties information
    vkGetPhysicalDeviceProperties(device, &deviceProperties); //basic device properties like name, type and supported Vulkan version can be queried using vkGetPhsyicalDeviceProperties() func

    VkPhysicalDeviceFeatures deviceFeatures;              //create variable to hold device features information   
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures); //support for optional features like texture compression, 64 bit floats and multi viewport rendering can be queried using the vkGetPhysicalDeviceFeatures() func

    int score = 0;    //create score

    //discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    //maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    //device must support queue family for commands we need
    QueueFamilyIndices indices = findQueueFamilies(device);
    if (!indices.isComplete()) {
        return 0;
    }

    bool extensionsSupported = checkDeviceExtensionSupport(device); //call the checkDeviceExtensionSupport() function to ensure we get the required extensions we need
    //if we did not get the required extensions give this device a 0 score
    if (!extensionsSupported) {
        return 0;
    }

    //utilize the querySwapChainSupport() function to verify that swap chain support is adequate
    //this must be done after we confirm that the extensions is available (above)
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    //if the swapchain support is still not adequate, give this device a score of 0
    if (!swapChainAdequate) {
        return 0;
    }

    //return the rated score
    return score;
}

FPEngine::QueueFamilyIndices FPEngine::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;    //create variable to hold indices
    uint32_t queueFamilyCount = 0; //create counter to queue families that could be found

    //retrieve list of queue families using the vkGetPhysicalDeviceQueueFamilyProperties() func
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //need to find at least one queue family that supports VK_QUEUE_GRAPHCIS_BIT and VK_QUEUE_COMPUTE_BIT
    int i = 0;
    for (const auto& queueFamily: queueFamilies) {
        if ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
            indices.graphicsAndComputeFamily = i;
        }

        //use function to look for a queue family that has the capability of presenting to our window surface
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        //check the value of the boolean and store the presentation family queue index
        if (presentSupport) {
            indices.presentFamily = i;
        }
        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void FPEngine::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice); //query to find all the available unique queue families on the selected phsyical device
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;          //create a container to hold the create info for each found queue family
    std::set<uint32_t>uniqueQueueFamilies = {indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

    //vulkan lets you assign priorities to queues to influence scheduling of command buffer execution-
    //using floating point numbers between 0.0-1.0. This is required even if there is a single queue
    float queuePriority = 1.f;
    //loop over all the uniqueQueueFamilies
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        //make the create info struct
        VkDeviceQueueCreateInfo queueCreateInfo{}; // make the create info --->queueCreateInfo structure describes number of queues we want for a single queue family
        //now fill in create info struct
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    //need to specify the set of device features we will be using (right now nothing special but later this will be used more)
    VkPhysicalDeviceFeatures deviceFeatures{};
    //deviceFeatures.samplerAnisotropy = VK_TRUE; //enable anisotropy sampling feature for the device
    //deviceFeatures.sampleRateShading = VK_TRUE; //enable sample shading feature for the device

    //with the last two structures in place, now can start filling the main VkDeviceCreateInfo structure
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();                            //add pointers to the queue creation info struct
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());  //set the count of create infos based on our container size
    createInfo.pEnabledFeatures = &deviceFeatures;                                     //add pointers to the device features struct
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()); //enable the VK_KHR_swapchain extension by filling in the size of the conatiner
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();                      //enable the VK_KHR_swapchain extension by filling in extensions data (names)

    /**below is code to have (not explicitly required) but good to have to be compatible with older implementations
    * previous implementations of Vulkan made a distinction between instance and device specific validation layers,
    * but this is no longer the case. That means that the enabledLayerCount and ppEnabledLayerNames fields
    * of VkDeviceCreateInfo are ignored by up-to-date implementations
    */

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    //we won't need any device specific extensions for now (later for things like raytracing)
    //now ready to instantiate the logical device with a proper call to the vkCreateDevice function
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    //utilize the vkGetDeviceQueue function to retrieve queue handles for each queue family
    vkGetDeviceQueue(device, indices.graphicsAndComputeFamily.value(), 0, &computeQueue);
    vkGetDeviceQueue(device, indices.graphicsAndComputeFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void FPEngine::createSurface() {
    //use the GLFW call
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

bool FPEngine::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    //querery extension details to get number of extensions supported
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    //make a container to hold all the VkExtensionProperties our device supports and populate with data
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    //make a set of the required extensions we need
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    //iterate over all extensions and erase the ones we have supported to see which required extensions we don't have
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

FPEngine::SwapChainSupportDetails FPEngine::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;                                                   //create handle to swapchain support details
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities); //query surface capabilites
    uint32_t formatCount;                                                              //create handle to format count
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);      //query the supported surface formats

    //ensure that the vector is resized to hold all available formats
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    //now query the supported presentation modes almost the same way
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    //resize again...
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR FPEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    //go through the list of avaialable formats and see whether the sRGB color space is supported or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag
    //also want to check that the format we want (VK_FORMAT_B8G8R8A8_SRGB) is available as well
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    //if this fails we could start ranking the available formats based on how "good" or what our specification is, but-
    //in most cases its okay to settle with the first format that is specified
    return availableFormats[0];
}

VkPresentModeKHR FPEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    /*the four choices of presentation modes are: VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR, and VK_PRESENT_MODE_MAILBOX_KHR
    * VK_PRESENT_MODE_MAILBOX_KHR (triple buffering) is a very nice tradeoff if power is not a concern
    * so, loop over all availablePresentModes in vector and check to see if it is available
    */
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    //only the VK_PRESENT_MODE_FIFO_KHR mode is guranteed to be available, so return this if VK_PRESENT_MODE_MAILBOX_KHR is not available
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D FPEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    //check to see if the current extent is the maximum value of uint32_t
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        //if not return currentExtent
        return capabilities.currentExtent;
    } else {
        //if so use glfwGetFrameBufferSize to query the resolution of the window in pixel before matching it against the minimum and maximum image extent
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        //now set the actual VkExtend2D object with proper queried results
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        //clamp values
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void FPEngine::createSwapChain() {
    //query and set the desired settings for our swapchain from the settings available on our device using the helper functions
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);     //first query all the settings available on our physical device/GPU
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats); //set desired surface format by utilizing helper function
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);  //set desired present mode by utilizng helper function
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);                  //set deisred extent by utilizng helper function

    /**aside from these necessary properties we must decide how many images we would like to have in the swap chain
    * this implementation specifies the minimum number that it requires to function (however sticking to this minimum means that we may sometimes have to 
    * wait on the driver to complete internal operations before we can acquire another image to render to, so it is recommended to request at least one more image than the minimum)
    */
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    //aso make sure to not exceed the maximum number of images while doing this, where 0 is a special value that means there is no maximum
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    //insantiate the createInfo struct for the swapchain and populate with details
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; //specifies amount of layers each image consists of (should always be 1 unless you are developing a stereoscopic 3D application)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //specifies what kind of operations we'll use the images in the swapchain for. In this implementation were going to render directly to them, which means theyre used as color attachment
    
    //need to specify how to handle swap chain images that will be used across multiple queue families (case in application if the graphics queue family is different from presentation queue i.e drawing images in the swap chain from the graphics queue and then submitting them on the presentation queue)
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

    //check to see if the graphics queue family is the same as the present queue family
    if (indices.graphicsAndComputeFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; //images can be used across multiple queue families without explicit ownership transfers
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; //an image is owned by one queue family at a time and ownershipt must be explicitly transferred before using it in another queue family
        createInfo.queueFamilyIndexCount = 0; //optional
        createInfo.pQueueFamilyIndices = nullptr; //optional
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform; //specifies that a certain transform should be applied to images in the swapchain if it is support (supportedTransforms in capabilities), ex. like a 90 degree clockwise rotation or horizontal flip (to specify you do not want any transformation, simply specify the current transformation)
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;            //compositeAlpha field specifies if the alpha channel should be used for blending with other windows in the window system (almost always ignore)
    createInfo.presentMode = presentMode;                                     //fill in present mode and clipped fields
    createInfo.clipped = VK_TRUE;                                             //if the clipped member is set to VK_TRUE then that means that we don't care about the color of pixels that are obscured
    createInfo.oldSwapchain = VK_NULL_HANDLE;                                 //last field is the oldSwapChain, which is where the swapchain becomes invalid or unoptimized while the application is running in which case a new one must be created (more on this later)


    //create the swapchain calling vkCreateSwapchainKHR() func
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }
    
    //need to retrieve the handles to the images created by the swapchain
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);                //start by querying the final number of images using vkGetSwapchainImagesKHR
    swapChainImages.resize(imageCount);                                              //now resize the container
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data()); //call the vk function again (vkGetSwapchainImagesKHR) to retrieve the handles

    //store the format and extent we've chosen for the swap chain images in member variables
    swapChainImageFormat = surfaceFormat.format; //store the format chosen for the swap chain in member variable
    swapChainExtent = extent;                    //store the extent chosen for the swap chain in member variable
}

void FPEngine::createImageViews() {
    //resize the list to fit all of the image views well be creating
    swapChainImageViews.resize(swapChainImages.size());

    //iterate over all the swap chain images
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        //create a image view for every swapchain
        swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

void FPEngine::createResolvePipeline() {
    /********************/
    /*SETTING UP SHADERS*/
    /********************/

    //call the readFile() funct to load the bytecode of the two shaders:
    auto vertShaderCode = readFile("shaders/resolveVert.spv");
    auto fragShaderCode = readFile("shaders/resolveFrag.spv");

    //make the VkShaderModules local variables because theyre allowed to be destroyed as soon as the pipeline creation is finished
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    //to use the shaders we need to assign them to a specific pipeline stage through VkPipelineShaderStageCreateInfo structures as part of the actual pipeline creation process
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; //tells VUlkan in which pipeline stage the shader is going to be used
    vertShaderStageInfo.module = vertShaderModule; //specifies the shader module containing the code
    vertShaderStageInfo.pName = "main"; //function to invoke aka entrypoint->its possible to combine multiple fragment shadrs into a single shader module and use different entry points to differentiate between their behaivors
    //one more optional member pSpecializationInfo: allows you to specify values for shader constants

    //same thing now for fragment shader
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    //define an array that contains these two structs, which well use later to reference them in the actual pipeline creation step
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    //VkPipelineVertexInputStateCreateInfo structure describes the format of the vertex data that will be passed to the vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; //wont be using any vertex input info...only need three vertices for full screen triangle
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    /****************************************/
    /*NOW HANDLING THE FIXED-FUNCTION STAGES*/
    /****************************************/

    //the VkPipelineInputAssemblyStateCreateInfo strcut describes two things: what kind of geometry will be drawn from vertices and if the primitive restart should be enabled.
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //set the viewport state createInfo
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    //set the VkPipelineRasterizationStateCreateInfo
    VkPipelineRasterizationStateCreateInfo rasterizer{};    //also performs depth test, face culling, and the scissor test. (also can be used for wireframe rendering)
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;                 //if set to VK_TRUE, then fragments that are beyond the near and far planes are clamped to them as opposed to discarding them (using this requires enabling a GPU feature)
    rasterizer.rasterizerDiscardEnable = VK_FALSE;          //if set to VK_TRUE, then geometry never passes through the rasterizer stage. (disables any output to framebuffer)
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;          //determines how fragments are generated for geometry (can use other modes but must enable a GPU feature)
    rasterizer.lineWidth = 1.0f;                            //thickness of lines in terms of number of fragments (anything thicker than 1.0f requires enable wideLines GPU feature)
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;            //determines the type of face culling to use
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //specifies the vertex order for faces to be considered
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; //optional
    rasterizer.depthBiasClamp = 0.0f;          //optional
    rasterizer.depthBiasSlopeFactor = 0.0f;    //optional

    //set the VkPipelineMultisampleStateCreateInfo
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE; //enable true fr enabling sample shading in the pipeline
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //set sampling to our class variable
    multisampling.minSampleShading = 1.0f;          //optional -->0.2f is min fraction for sample shading; closer to one is smoother
    multisampling.pSampleMask = nullptr;            //optional
    multisampling.alphaToCoverageEnable = VK_FALSE; //optional
    multisampling.alphaToOneEnable = VK_FALSE;      //optional

    //Depth and Stencil testing
    VkPipelineDepthStencilStateCreateInfo depthStencil{}; //dont need depth stencil for raymarching
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_FALSE;            //specifies if the depth of new fragments should be compared to the depth buffer to see if they are discareded
    depthStencil.depthWriteEnable = VK_FALSE;          //specifies if the new depth of fragments that pass the test should actually be written to the depth buffer
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS; //specifies the comparison that is performed to keep or discard fragments
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; //optional
    depthStencil.maxDepthBounds = 1.0f; //optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; //optional
    depthStencil.back = {}; //optional

    //set the VkPipelineColorBlendAttachmentState
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  //optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             //optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  //optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; //optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             //optional

    //set the VkPipelineColorBlendStateCreateInfo
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; //optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;  //optional
    colorBlending.blendConstants[1] = 0.0f;  //optional
    colorBlending.blendConstants[2] = 0.0f;  //optional
    colorBlending.blendConstants[3] = 0.0f;  //optional

    //most of the pipeline state needs to be baked into the pipeline state, a limited amount of the state can be changed without recreating the pipeline at draw time
    //examples include: size of viewport, line width and blend constants. If you want to use dynamic state and keep these properties out, then you have to fill in a VkPipelineDynamicStateCreateInfo struct like so:
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    //create the pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;                       //sets number of descriptor set layouts used
    pipelineLayoutInfo.pSetLayouts = &resolveDescriptorSetLayout;       //sets the descriptor set layout we want
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &resolvePipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    //create the graphics pipeline create info
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages; //start by referencing the array of VkPipelineShaderStageCreateInfo structs
    pipelineInfo.pVertexInputState = &vertexInputInfo; //now reference all the structures describing fixed-function stage
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = resolvePipelineLayout;
    pipelineInfo.renderPass = resolveRenderPass;
    pipelineInfo.subpass = 0; //index of the sub pass where this graphics pipeline will be used
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; //optional
    pipelineInfo.basePipelineIndex = -1; //optional
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &resolvePipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    //cleanup at the end by calling vkDestroyShaderModule
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

std::vector<char> FPEngine::readFile(const std::string& filename) {
    //start by opening the file with two flags: ate(start reading at the end of the file) and binary(read the file as a binary file)
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    //advantage of starting to read at the end of the file is that we can use the read positoin to determine the size of the file and allocate a buffer
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    //after getting buffer setup back to the beginning of the file and real all of the bytes at once
    file.seekg(0);
    file.read(buffer.data(), fileSize);

    //close the file and return the bytes
    file.close();
    return buffer;
}

VkShaderModule FPEngine::createShaderModule(const std::vector<char>& code) {
    //make the create info for shader module and populate its details
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType =VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    //VkShaderModule can now be create with call to vkCreateShaderModule()
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void FPEngine::createResolveRenderPass() {
    //in this case well have just a single color buffer attachment represented by one of the images from the swap chain
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;     //specifies which layout the image will havae before the render pass begins
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //specifies the layout to automatically trasnsition to when the render pass finishes

    //create an attachment reference for our subpass
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0; //specifies which attachment to reference by its index in the attachment descriptions array
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //specifies which layout we would like the attachment to have during a subpass that uses this reference

    //create the subpass using VkSubpassDescription struct
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    //Subpass dependencies are specified in VkSubpassDependency structs
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    //create the actual render pass itself
    std::array<VkAttachmentDescription, 1> attachments = {colorAttachment};
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 0; //dont actually need to use the dependency struct created above
    renderPassInfo.pDependencies = &dependency;
    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &resolveRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create resolve render pass!");
    }
}

void FPEngine::createFramebuffers() {
    //resize the container to hold all of the framebuffers 
    swapChainFramebuffers.resize(swapChainImageViews.size());

    //iterate through the image views and create framebuffers from them
    for (size_t i=0; i<swapChainImageViews.size(); i++) {
        std::array<VkImageView, 1> attachments = {
            swapChainImageViews[i] //only need our swapChainImage Views, no color resolve for MSAA (disabled), and no depth image (disabled)
        };

        //create the frame buffer create info struct and populate it
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = resolveRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        //create the frame buffer using vkCreateFrameBuffer() funct along with our createinfo struct
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void FPEngine::createCommandPool() {
    //command pool creation only takes two parameters one of which is queue families related to our GPU
    QueueFamilyIndices QueueFamilyIndices = findQueueFamilies(physicalDevice);

    //establish the command pool create info and populate it
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = QueueFamilyIndices.graphicsAndComputeFamily.value();

    //create the command pool by using vkCreateCommandPool() funct. along with create info struct
    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool");
    }
}

void FPEngine::createCommandBuffers() {
    //first resize the command buffers vector size of MAX_FRAMES_IN_FLIGHT
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    //need to make the VkCommandBufferAllocateInfo struct and popluate with data
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //specifies if the allocated command buffers are primary or secondary command buffers
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    //now call vkAllocateCommandBuffers() funct along with the allocInfo struct
    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void FPEngine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    //always begin recording a command buffer by calling vkBeginCommandBuffer with a small VkCommandBufferBeginInfo struct as an argument
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; //optional --specifies how were going to use the command buffer
    beginInfo.pInheritanceInfo = nullptr; //optional --only relevant for secondary command buffers(specifies which state to inherit from the calling primary command buffers)
    
    //call vkBeginCommandBuffer() along with beginInfo struct and ensure it was successful
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    //drawing starts by beginning the render pass with vkCmdBeginRenderPass
    //render pass is configured using some parameters in a VkRenderPassBeginInfo struct
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = resolveRenderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;
    std::array<VkClearValue, 1> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    //need an image memory barrier to ensure access control of the hdrImage (ensuring that compute is completely done touching it)
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = hdrImages[currentFrame];
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &barrier);

    //begin the render pass
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        //can now bind the graphics pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resolvePipeline);
        
        //we did specify viewport and scissor state for this pipeline to be dynamic so we need to set them in the command buffer before issuing our draw command
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChainExtent.width);
        viewport.height = static_cast<float>(swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        //bind the right descriptor set for each frame to the descriptors in the shader with vkCmdBindDescriptorSets
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, resolvePipelineLayout, 0, 1, &resolveDescriptorSets[currentFrame], 0, nullptr);

        //bind data and draw fullscreen triangle
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    //render pass can now be ended:
    vkCmdEndRenderPass(commandBuffer);

    //need image barrier to ensure the graphics side of things is done accessing the hdr image before compute touches it again
    VkImageMemoryBarrier barrier2{};
    barrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier2.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier2.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.image = hdrImages[currentFrame];
    barrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier2.subresourceRange.baseMipLevel = 0;
    barrier2.subresourceRange.levelCount = 1;
    barrier2.subresourceRange.baseArrayLayer = 0;
    barrier2.subresourceRange.layerCount = 1;
    barrier2.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier2.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &barrier2);

    //weve finished recording the command buffer
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void FPEngine::drawFrame() {
    //queue submission and synchronization is configured through parameters in the VkSubmitInfo struct
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    //Begin compute submission
    vkWaitForFences(device, 1, &computeInFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    updateUniformBuffer(currentFrame); //update the uniform buffer 

    vkResetFences(device, 1, &computeInFlightFences[currentFrame]);

    vkResetCommandBuffer(computeCommandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordComputeCommandBuffer(computeCommandBuffers[currentFrame]);

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &computeCommandBuffers[currentFrame];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &computeFinishedSemaphores[currentFrame];

    if (vkQueueSubmit(computeQueue, 1, &submitInfo, computeInFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit compute command buffer!");
    }

    //Begin Graphics submission
    //first wait until the previous frame has finished so that the command buffer and semaphores are available to use
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    //acquire an image from the swap chain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    //vkAcquireNextImageKHR can return a special value to signal swap chain is no longer adequate during presentation
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    //after waiting we need to manually reset the fence to the unsignaled state with the vkResetFences call
    vkResetFences(device, 1, &inFlightFences[currentFrame]); //only reset the fence if we are submitting work

    //with the imageIndex specifying the swap chain image to use, we can now record the command buffer
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);         //first reset the command buffer
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex); //call the recordCommandBuffer function to record commands we want

    //with a fully recorded command buffer, we can now submit it
    //queue submission and synchronization is configured through parameters in the VkSubmitInfo struct
    submitInfo = {}; //just reset the submitInfo struct used for compute submission
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO; //the .sType should still be the same VK_STRUCTURE_TYPE_SUBMIT_INFO
    VkSemaphore waitSemaphores[] = {computeFinishedSemaphores[currentFrame], imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 2; //changed this to two with compute pipeline (compute semahpore)
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    VkSemaphore signalSemaphores[] {renderFinishedSemaphores[imageIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    //submit the recorded command buffer using vkQueueSubmit() along with submit info struct
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    //last step of drawing a frame is submitting the result back to the swap chain to have it eventually show up on the screen
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; //optional
    result = vkQueuePresentKHR(presentQueue, &presentInfo); //submits the request to present an image to the swap chain

    //handle if the swap chain is suboptimal
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //dont forget to advance to next frame every time and get FPS output
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    createFPS();
}

void FPEngine::createSyncObjects() {
    //resize our synchronization object containers
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);   //resize the container to MAX_FRAMES_IN_FLIGHT for imageAvailable semaphores
    renderFinishedSemaphores.resize(swapChainImages.size()); //resize the container to swapChainImages for renderFinished semaphores
    computeFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);  //resize the container to MAX_FRAMES_IN_FLIGHT for computeFinished semaphores
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);             //resize the container to MAX_FRAMES_IN_FLIGHT for inFlight fences
    computeInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);      //resize the container to MAX_FRAMES_IN_FLIGHT for computeInFlight fences

    //fill out the VkSemaphoreCreateInfo
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    //fill out the VkFenceCreateInfo
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; //create the fence in the signaled state so that the first call to vkWaitForFences() returns immediately

    //loop over MAX_FRAMES_IN_FLIGHT and create a imageAvailable semaphore and inFlight fence for each frame
    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create graphics synchronization objects for a frame!");
        }

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &computeFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &computeInFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create compute synchronization objects for a frame!");
            }
    }

    //loop over swapChainImages size to create the renderFinished semaphore for each swapchain image
    for (size_t i=0; i<swapChainImages.size(); i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics synchronization objects for a frame!");
        }
    }
}

void FPEngine::recreateSwapChain() {
    //handle minmization of the application window
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    
    vkDeviceWaitIdle(device);  //first call vkDeviceWaitIdle because we should't touch resources that may still be in use
    cleanupSwapChain();        //cleanup old swapchain and related dependencies via cleanupSwapChain() funct

    //need to recreate the swap chain, the image views, and the framebuffers (dependencies)
    createSwapChain();      //recreate the swapchain
    createImageViews();     //recreate the image views
    createHDRImages();      //recreate the hdr images
    createFramebuffers();   //recreate the framebuffers
    vkResetDescriptorPool(device, descriptorPool, 0); //reset the descriptor pool before rebuilding our compute and resolve descriptor sets (otherwise run out of resources)
    createComputeDescriptorSets(); //recreate the compute descriptor sets
    createResolveDescriptorSets(); //recreate the resolve descriptor sets
}

void FPEngine::cleanupSwapChain() {
    // //cleanup image view, image, and memory for hdr Images
    for (auto& image : hdrImages) vkDestroyImage(device, image, nullptr);
    for (auto& view : hdrImageViews) vkDestroyImageView(device, view, nullptr);
    for (auto& memory : hdrImageMemories) vkFreeMemory(device, memory, nullptr);
    // vkFreeMemory(device, colorImageMemory, nullptr);

    //delete the framebuffers
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    //image views were explicitly created by us, so we need to destroy them all
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    //cleanup the swapchain object
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}


uint32_t FPEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    //first need to query info about the available types of memory using vkGetPhysicalDeviceMemoryProperties
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    //find a memory type that is suitable for the buffer itself
    //ensure that we are also able to write our data to that memory
    for (uint32_t i=0; i<memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    //otherwise throw an error
    throw std::runtime_error("failed to find suitable memory type!");
}

void FPEngine::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    //creating a buffer requires us to fill a VkBufferCreateInfo struct
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;                             //speicifes the size of the buffer in bytes
    bufferInfo.usage = usage;                           //indicates for which purposes the data in the buffer is going to be used
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //buffers can also be owned by a specific queue family or be shared between multiple at the same time
    //now create the buffer using vkCreateBuffer() funct
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    //need to allocate memory for the buffer so first we will query its memory requirements using vkGetBufferMemoryRequirements() funct
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    //now we can actually allocate the memory by filling in the VkMemoryAllocateInfo struct
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    //call the vkAllocateMemory() funct to allocate the memory specified
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    //if memory allocation was successful then we associate this memory with the buffer using vkBindBufferMemory() funct
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void FPEngine::createResolveDescriptorSetLayout() {
    //every binding needs to be described through a VkDescriptorSetLayoutBinding struct

    //create binding for 32bit float texture sampler image
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //is intended for the fragment shader

    //create it using vkCreateDescriptorSetLayout() funct --> we need to make/populate the createInfo struct first
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &samplerLayoutBinding;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &resolveDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
    
}

void FPEngine::createResolveDescriptorSets() {
    //a descriptor set allocation is described with a VkDescriptorSetAllocateInfo struct
    //create one descriptor set for each frame in flight, all with the same layout
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, resolveDescriptorSetLayout);

    //setup the alloc info struct and populate
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    //first resize our container then allocate
    resolveDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, resolveDescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    //descriptor sets have been allocated now, but still need to be configured
    //add a loop to populate every descriptor
    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {

        //descriptor image info now for texture image
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = hdrImageViews[i];
        imageInfo.sampler = hdrSampler;

        //configuration of descriptors is updated using the vkUpdateDscriptorSets funct which takes an array of VkWriteDescriptorSet structs as parameter
        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        //specify details for the image info
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = resolveDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        //update the descriptor sets
        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void FPEngine::createDescriptorPool() {
    //first need to describe which descriptor types our descriptor sets are going to contain and how many of them using VkDescriptorPoolSize structs
    //we will allocate one these descriptors for every frame
    std::array<VkDescriptorPoolSize, 3> poolSizes{};

    //UBO
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //create room for UBO in compute pipeline

    //HDR STORAGE IMAGE
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //create room for the storage image in compute pipeline

    //HDR SAMPLER
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT); //create room for the image sampler in resolve pipeline

    //pool size structure is referenced by the main VkDescriptorPoolCreateInfo
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(2 * MAX_FRAMES_IN_FLIGHT); //account space for both graphics and compute descriptor sets

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void FPEngine::createUniformBuffers() {
    //get size of buffer
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    //resize our UBO containers to hold only as many as MAX_FRAMES_IN_FLIGHT (one UBO per frame)
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    //loop over all buffers we want to make
    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        uniformBuffers[i], uniformBuffersMemory[i]);
        
        //map memory right after creation using vkMapMemory to get a pointer to which we can write the data later on
        //this is called "persistent mapping"
        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void FPEngine::updateUniformBuffer(uint32_t currentImage) {
    //create the uniform buffer object so as to populate our uniform data into it
    UniformBufferObject ubo{};

    //check if our light is dynamic mode or not
    if (dynamicLight) {
        lightPos = glm::vec3(0.0, 3.5*cos(classClock), 3.5*sin(classClock));
    }

    //setup camera, lighting, and time
    ubo.view = glm::inverse(camera.getCamViewMatrix());       //use the camera for the view matrix data
    ubo.proj = glm::inverse(camera.getCamProjectionMatrix()); //use the camera for the proj matrix data
    ubo.lightPos = lightPos;                                                   //position of our light -> user dependent on whether it moves dynamically or not
    ubo.viewPos = camera.getCamPos();                                          //set the view position of the camera
    ubo.resolution = glm::vec2(swapChainExtent.width, swapChainExtent.height); //resolution of our current frame
    ubo.params.x = powerLevel;                                               //set the powerLevel for our fractal
    ubo.params.y = hitEpsilon;                                               //set the distance estimator epsilon value
    ubo.params.z = classClock;                                                     //set the time uniform
    ubo.params.w = colorNormal;                                              //set the make-shift bool for how we color the fractal

    //all transformations are ready so can copy the data in the uniform buffer object to the current uniform buffer
    //same way as for vertex buffers except without a staging buffer (we only map the uniform buffer once, so we can directly write to it without having to map again)
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void FPEngine::createCamera() {
    //first calculate the proj matrix correctly
    glm::mat4 projMtx = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 45.0f);
    projMtx[1][1] *= -1; //flip so were not upside down when we render

    //create the camera object and attach to class handle
    camera = ArcballCam(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(0.0f, 0.f, 0.0f), projMtx);

}

void FPEngine::createComputePipeline() {
    auto computeShaderCode = readFile("shaders/raymarch.spv");

    VkShaderModule computeShaderModule = createShaderModule(computeShaderCode);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;
    computeShaderStageInfo.pName = "main";

    //setup of compute shader is simpler and the pipeline layout works the same as with the graphics pipeline
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &computeDescriptorSetLayout;

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &computePipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute pipeline layout!");
    }

    //since compute pipeline does not touch any of the rasterization state, it has a lot less state than a graphics pipeline
    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = computePipelineLayout;
    pipelineInfo.stage = computeShaderStageInfo;

    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute pipeline!");
    }

    vkDestroyShaderModule(device, computeShaderModule, nullptr);
}

void FPEngine::createComputeDescriptorSetLayout() {
    std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].pImmutableSamplers = nullptr;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT; //settings up descriptors for compute is almost identical to graphics but we need to have the VK_SHADER_STAGE_COMPUTE_BIT set to make it accessible by the compute stage. (CAN also make the descriptor accesible from the vertex stage and compute stage via VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_COMPUTE_BIT)

    layoutBindings[1].binding = 1;
    layoutBindings[1].descriptorCount = 1;
    layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    layoutBindings[1].pImmutableSamplers = nullptr;
    layoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = layoutBindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &computeDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute descriptor set layout!");
    }

}

void FPEngine::createComputeDescriptorSets() {
    //create one descriptor set for each frame in flight, all with the same layout
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, computeDescriptorSetLayout);

    //setup the alloc info struct and populate
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    //first resize our container then allocate
    computeDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, computeDescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        //delcare the uniformBuffer with a descriptorBufferInfo object
        VkDescriptorBufferInfo uniformBufferInfo{};
        uniformBufferInfo.buffer = uniformBuffers[i];
        uniformBufferInfo.offset = 0;
        uniformBufferInfo.range = sizeof(UniformBufferObject);

        //delcare the storage image with a descriptorBufferInfo object
        VkDescriptorImageInfo storageImageInfo{};
        storageImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        storageImageInfo.imageView = hdrImageViews[i];
        storageImageInfo.sampler = VK_NULL_HANDLE;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{}; //descriptor write structs (1 for UBO, 1 for hdr storage image)
        //UBO data first
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = computeDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &uniformBufferInfo;

        //now storage image
        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = computeDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &storageImageInfo;

        vkUpdateDescriptorSets(device, 2, descriptorWrites.data(), 0, nullptr);
    }
}

void FPEngine::recordComputeCommandBuffer(VkCommandBuffer commandBuffer) {

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording compute command buffer!");
    }

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout, 0, 1, &computeDescriptorSets[currentFrame], 0, nullptr);
    
    vkCmdDispatch(commandBuffer, (swapChainExtent.width + 15) / 16, (swapChainExtent.height + 15) / 16, 1); //dispatch across the xz-plane for our image, with using 16 as the ceiling to ensure we launch enough work groups

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record compute command buffer!");
    }
}

void FPEngine::createComputeCommandBuffers() {
    computeCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)computeCommandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, computeCommandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate compute command buffers!");
    }
}

void FPEngine::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
                           VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, 
                           VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    //parameters for an image are specified in a VkImageCreateInfo struct
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D; //specifies what kind of coordinate system the texels in the image are going to be addressed
    imageInfo.extent.width = width; //specifies dimensions
    imageInfo.extent.height = height; //specifies dimensions
    imageInfo.extent.depth = 1; //depth must be 1 instead of 0
    imageInfo.mipLevels = mipLevels; //function argument to control mip level of an image
    imageInfo.arrayLayers = 1;
    imageInfo.format = format; //use same format for texels as the pixels in the buffer otherwise the copy operation will fail
    imageInfo.tiling = tiling; //texels are laid out in row-major order (other option is VK_IMAGE_TILING_LINEAR)
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //only two possible values: VK_IMAGE_UNDEFINED(texels discarded at end) or VK_IMAGE_LAYOUT_PREINTIALIZED(texels preserved at the end)
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //image will only be used by one queue family
    imageInfo.samples = numSamples; //sets number of samples for MSAA
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0; //optional

    //call vkCreateImage along with the image info struct and ensure it was successful
    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    //allocate the memory for the image
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    //make the allocInfo struct
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    //allocate the memory using vkAllocateMemory() along with alloc info struct
    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    //bind the image to its memory
    vkBindImageMemory(device, image, imageMemory, 0);
}

VkImageView FPEngine::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
    //parameters for image view creation are specified in a VkImageViewCreateInfo struct
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    //viewType and format fields specify how the image data should be interpreted
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; //viewType paramter allows you treat images as 1D textures, 2D textures, 3D textures and cube maps
    viewInfo.format = format;
    //components field allows you to swizzle the color channels around
    //in this case were using the default mapping
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    //subresourceRange field describes what the image's purpose is and which part of the image should be accessed.
    //in this case our images will be used as color targets without any mipmapping levels or multiple layers
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    //creating the image view by calling vkCreateImageView after creating its handle
    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image view!");
    }

    //return the image view
    return imageView;
}

void FPEngine::transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
    //begin by creating the command buffer and start it
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    //create the image memory barrier to synchronize access to resources, like ensuring that a write to a buffer completes before readin from it
    //can also be used to transition image layouts and transfer queue family ownership when using VK_SHARING_MODE_EXCLUSIVE is used
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    //if you are using the barrier to transfer queue family ownership, then these two fields should be the indices of the queue families (Not applicable here)
    barrier.oldLayout = oldLayout; //specify layout transition ->is possible to use VK_IMAGE_UNDEFINED if you don't care about the existing contents of the image
    barrier.newLayout = newLayout; //specify layout transition
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;         //specify the image that is affected
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //specify part of image
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    //need to handle two transitions: undefined(transfer writes that don't need to wait on anything) and transfer destination(shader reading)
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    //now add correct masks and pipeline stages  
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        //undefined -> general (before compute writes)
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        //general -> shader_read_only_optimal (before fragment reads)
        barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
        //shader_read_only -> general (next frame compute)
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    } else {
        throw std::runtime_error("Unsupported HDR layout transition!");
    }

    //submit the pipeline barrier
    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    //submit the command buffer, end, and cleanup the command buffer
    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer FPEngine::beginSingleTimeCommands() {
    //memory transfer operations are executed using command buffers (just like drawing commands)
    //must first allocate a temporary command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    //finally create the command buffer with the info struct made
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    //immediately start recording the command buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    //now return the buffer
    return commandBuffer;
}

void FPEngine::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    //end the command
    vkEndCommandBuffer(commandBuffer);

    //now execute command buffer to complete the transfer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    //release the command buffer used for the transfer operation
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void FPEngine::createHDRImages() {
    //create the format for our HDR image
    VkFormat hdrFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

    //resize the containers based on MAX_FRAMES_IN_FLIGHT
    hdrImages.resize(MAX_FRAMES_IN_FLIGHT);
    hdrImageMemories.resize(MAX_FRAMES_IN_FLIGHT);
    hdrImageViews.resize(MAX_FRAMES_IN_FLIGHT);

    //iterate over MAX_FRAMES_IN_FLIGHT and create the images
    for (int i=0; i<MAX_FRAMES_IN_FLIGHT; i++) {
        createImage(swapChainExtent.width, swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT,
            hdrFormat, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, hdrImages[i], hdrImageMemories[i]);
        //creat the image views
        hdrImageViews[i] = createImageView(hdrImages[i], hdrFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        //transition image layout
        transitionImageLayout(hdrImages[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    }
}

void FPEngine::createHDRSampler() {
    //samplers are configured through a VkSamplerCreateInfo struct which specifies all filters and transformations that it should apply
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    //min and mag filter fields specify how to interpolate texels that are magnified or minified
    samplerInfo.magFilter = VK_FILTER_LINEAR; //concerns oversampling problem
    samplerInfo.minFilter = VK_FILTER_LINEAR; //concerns undersampling problem
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; //specifies which color is returned when sampling beyond the image with clamp to border addressing mode
    samplerInfo.unnormalizedCoordinates = VK_FALSE; //specifies which coordinate system you want to use to address texels in an image ->were using normalized device coordinates so VK_FALSE[0,1) if VK_TRUE then [0, texWidth]
    samplerInfo.compareEnable = VK_FALSE; //if enabled then texels will first be compared to a vlue and the result of that comparison is used in filtering operations
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerInfo.mipLodBias = 0.0f; //optional
    samplerInfo.minLod = 0.0f; //optinal
    samplerInfo.maxLod = 0.0f;

    //create the sampler
    if (vkCreateSampler(device, &samplerInfo, nullptr, &hdrSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void FPEngine::createFPS() {
    //initialize the clock/time stuff
    static auto lastTime = std::chrono::high_resolution_clock::now();                                        //get the start time
    static float accumulator = 0.0f; //will add up delta times until 1 second passes
    static int frameCount = 0;
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count(); //calculate time based on startTime and currentTime
    lastTime = currentTime;
    classClock += deltaTime; //update our class timer variable


    accumulator += deltaTime; //update accumulator
    frameCount++;

    currentFPS = 1.0f / deltaTime; //instantaneous

    if (accumulator >= 1.0f) { //when the accumulator has accumulated 1+ second then we display information
        averageFPS = frameCount / accumulator;

        printf("\rFPS: %d | Avg FPS: %d", currentFPS, averageFPS);
        fflush(stdout);

        accumulator = 0.0f;
        frameCount = 0;
    }
}


//callback functions
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto app = reinterpret_cast<FPEngine*>(glfwGetWindowUserPointer(window));
    app->onMouseButton(button, action);
}

void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {
    auto app = reinterpret_cast<FPEngine*>(glfwGetWindowUserPointer(window));
    app->onMouseMove(xPos, yPos);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    auto app = reinterpret_cast<FPEngine*>(glfwGetWindowUserPointer(window));
    app->onScroll(yOffset);
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<FPEngine*>(glfwGetWindowUserPointer(window));
    app->notifyFramebufferResized();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto app = reinterpret_cast<FPEngine*>(glfwGetWindowUserPointer(window));
    app->onKeyPress(key, mods, action);
}