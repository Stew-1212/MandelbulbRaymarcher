cmake_minimum_required(VERSION 3.14)
project(FP)
set(CMAKE_CXX_STANDARD_17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILES main.cpp FPEngine.h FPEngine.cpp ArcballCam.hpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# ----------------------------------------
# Vulkan
# ----------------------------------------
find_package(Vulkan REQUIRED)

# ----------------------------------------
# Environment Files (GLFW / GLM)
# ----------------------------------------
set(ENV_INCLUDE_DIR "C:/Users/stewa/Schoolwork/AdvancedGraphics/environmentFiles/include")
set(ENV_LIB_DIR     "C:/Users/stewa/Schoolwork/AdvancedGraphics/environmentFiles/lib")

# ----------------------------------------
# GLFW
# ----------------------------------------
find_library(GLFW_LIBRARY
    NAMES glfw3 glfw glfw3dll
    PATHS ${ENV_LIB_DIR}
    REQUIRED
)

# ----------------------------------------
# Include Directories
# ----------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ENV_INCLUDE_DIR}
)

# ----------------------------------------
# Link Directories (Windows)
# ----------------------------------------
if (WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${ENV_LIB_DIR}
    )
endif()

# ----------------------------------------
# Link Libraries
# ----------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    ${GLFW_LIBRARY}
)
