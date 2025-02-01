cmake_minimum_required(VERSION 3.26)
project(Engine VERSION 0.1.0 LANGUAGES C CXX)

set (FullOutputDir "${CMAKE_SOURCE_DIR}/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/Static Libraries")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

set (SrcPath "${CMAKE_SOURCE_DIR}/src/Source Files")
set (HeaderPath "${CMAKE_SOURCE_DIR}/src/Header Files")
file (GLOB Sources CONFIGURE_DEPENDS "${SrcPath}/*.cpp" "${HeaderPath}/*.hpp" "${SrcPath}/*.c" "${HeaderPath}/*.h")

include_directories(${HeaderPath} ${SrcPath} $ENV{VULKAN_PATH})
link_directories($ENV{VULKAN_PATH})

link_libraries("glfw3" "vulkan-1")

add_executable(${PROJECT_NAME} ${Sources})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

