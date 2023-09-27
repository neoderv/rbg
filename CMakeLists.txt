cmake_minimum_required(VERSION 3.10)
project(cgame)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_MakeAvailable(glad)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")

add_executable(cgame src/main.c src/util.c src/model.c src/linearAlg.c src/cubes.c src/controls.c src/gen.c)

set_property(TARGET cgame PROPERTY C_STANDARD 11)

include_directories(src/include)

IF (WIN32)
	target_link_libraries(cgame
		glfw
		glad
	)
ELSE()
	target_link_libraries(cgame
		glfw
		glad
		m
	)
ENDIF()

set_target_properties(cgame PROPERTIES RUNTIME_OUTPUT_DIRECTORY .)
install(TARGETS cgame DESTINATION .)
