cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project name
project(20___C___Projects__SFML_)

# Set SFML root directory
set(SFML_DIR ${CMAKE_SOURCE_DIR}/dependencies/SFML)

# Add SFML include directory
include_directories(${SFML_DIR}/include)

# Define SFML static libraries for Debug and Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SFML_LIBRARIES
        ${SFML_DIR}/lib/debug/sfml-system-s-d.lib
        ${SFML_DIR}/lib/debug/sfml-graphics-s-d.lib
        ${SFML_DIR}/lib/debug/sfml-window-s-d.lib
        ${SFML_DIR}/lib/debug/sfml-network-s-d.lib
        ${SFML_DIR}/lib/debug/sfml-audio-s-d.lib
    )
    set(EXTRA_LIBRARIES
        ${SFML_DIR}/lib/freetype.lib  # Debug version of FreeType
    )
else()
    set(SFML_LIBRARIES
        ${SFML_DIR}/lib/release/sfml-system-s.lib
        ${SFML_DIR}/lib/release/sfml-graphics-s.lib
        ${SFML_DIR}/lib/release/sfml-window-s.lib
        ${SFML_DIR}/lib/release/sfml-network-s.lib
        ${SFML_DIR}/lib/release/sfml-audio-s.lib
    )
    set(EXTRA_LIBRARIES
        ${SFML_DIR}/lib/freetype.lib  # Release version of FreeType
    )
endif()

# Add additional libraries required for static linking
list(APPEND EXTRA_LIBRARIES
    opengl32.lib  # OpenGL library
    winmm.lib     # Windows multimedia library
    gdi32.lib     # Graphics Device Interface
)

# Define SFML_STATIC for static builds
add_definitions(-DSFML_STATIC)

# Recursively add all .cpp files in the src/ directory
# file(GLOB_RECURSE SOURCES "src/*.cpp")


# Create executable target
# add_executable(20___C___Projects__SFML_ ${SOURCES})


# add_executable(20___C___Projects__SFML_ "src/02 - Bouncing Ball/BounceBall.cpp" )
 add_executable(20___C___Projects__SFML_ "src/03 - Paddle and Ball/Main.cpp")


# Link SFML and additional libraries
target_link_libraries(20___C___Projects__SFML_ ${SFML_LIBRARIES} ${EXTRA_LIBRARIES})

# Set C++ standard to C++17
if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET 20___C___Projects__SFML_ PROPERTY CXX_STANDARD 17)
endif()

# Enable static runtime for MSVC
if(MSVC)
    set_target_properties(20___C___Projects__SFML_ PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # Set CWD for Visual Studio
    set_property(TARGET 20___C___Projects__SFML_ PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

endif()