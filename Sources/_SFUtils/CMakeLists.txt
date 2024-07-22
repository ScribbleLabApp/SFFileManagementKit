# _SFUtils/CMakeLists.txt

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all source files
file(GLOB_RECURSE UTILS_SOURCES "*.cpp")
file(GLOB_RECURSE UTILS_HEADERS "*.h")

# Create a static library for the utility code
add_library(SFUtils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})

# Specify the include directories
target_include_directories(SFUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optionally set properties for the library
set_target_properties(SFUtils PROPERTIES VERSION 1.0 SOVERSION 1)
