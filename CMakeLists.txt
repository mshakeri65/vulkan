cmake_minimum_required(VERSION 3.16)

project(VulkanImageViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp


    vulkanwindow.h
    vulkanwindow.cpp
    resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VulkanImageViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(VulkanImageViewer ${PROJECT_SOURCES})
endif()

# Vulkan Setup
find_package(Vulkan)

if(Vulkan_FOUND)
    message("Vulkan found in default path: ${Vulkan_LIBRARY}")
    target_include_directories(VulkanImageViewer PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(VulkanImageViewer PUBLIC Vulkan::Vulkan)
else()
    # Fallback for your specific Vulkan SDK path
    set(VULKAN_SDK "/opt/vulkans/1.3.296.0/x86_64")
    find_library(Vulkan_LIBRARY NAMES vulkan HINTS "${VULKAN_SDK}/lib")
    if(Vulkan_LIBRARY)
        message("Vulkan found in custom path: ${Vulkan_LIBRARY}")
        target_include_directories(VulkanImageViewer PUBLIC "${VULKAN_SDK}/include")
        target_link_libraries(VulkanImageViewer PUBLIC ${Vulkan_LIBRARY})
    else()
        message(FATAL_ERROR "Vulkan not found! Please ensure it's installed or set up correctly.")
    endif()
endif()

# GLFW Setup
find_package(glfw3 REQUIRED)
target_link_libraries(VulkanImageViewer PUBLIC glfw)

# Platform-Specific Setup
if(WIN32)
    # Additional Windows-specific configurations (if needed)
elseif(APPLE)
    # Additional macOS-specific configurations (if needed)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(XCB REQUIRED xcb)

    if(XCB_LIBRARIES)
        target_link_libraries(VulkanImageViewer PRIVATE ${XCB_LIBRARIES})
        message("XCB libraries found: ${XCB_LIBRARIES}")
    else()
        message(FATAL_ERROR "XCB libraries not found!")
    endif()
endif()

# Link Qt Widgets
target_link_libraries(VulkanImageViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Bundle properties for macOS
if(APPLE)
    set_target_properties(VulkanImageViewer PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VulkanImageViewer
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
elseif(WIN32)
    set_target_properties(VulkanImageViewer PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS VulkanImageViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VulkanImageViewer)
endif()
