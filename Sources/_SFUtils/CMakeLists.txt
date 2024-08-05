# _SFUtils/CMakeLists.txt

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML2 REQUIRED libxml-2.0)

# Collect all source files
file(GLOB_RECURSE UTILS_SOURCES "*.cpp")
file(GLOB_RECURSE UTILS_HEADERS "include/*.h")

# Include directories
include_directories(${LIBXML2_INCLUDE_DIRS})

# Link directories
link_directories(${LIBXML2_LIBRARY_DIRS})

# Create a static library for the utility code
add_library(SFUtils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})

target_link_libraries(SFUtils PRIVATE ${LIBXML2_LIBRARIES})

# Specify the include directories
target_include_directories(SFUtils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LIBXML2_INCLUDE_DIRS}
    ${UTILS_HEADERS}
)

# Optionally set properties for the library
set_target_properties(SFUtils PROPERTIES VERSION 1.0 SOVERSION 1)
