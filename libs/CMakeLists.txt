CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(JumperBallLibs)

ADD_SUBDIRECTORY(glad)

SET(GLFW_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/libs/GLFW_install/)
SET(GLM_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/libs/GLM_install/)

INCLUDE(ExternalProject)
ExternalProject_Add( _GLFW_
  URL 		"https://github.com/glfw/glfw/archive/3.3.tar.gz"
  SOURCE_DIR 	"${CMAKE_SOURCE_DIR}/libs/glfw-3.3"
  PREFIX 	GLFW_library	
  CMAKE_ARGS 	-DGLFW_BUILD_DOCS=OFF 
  		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_EXAMPLES=OFF
		-DCMAKE_INSTALL_PREFIX=${GLFW_INSTALL_PREFIX}
  TEST_COMMAND 	""
  )

INCLUDE(ExternalProject)
ExternalProject_Add( _GLM_
  URL 		"https://github.com/g-truc/glm/archive/0.9.9.0.tar.gz"
  SOURCE_DIR 	"${CMAKE_SOURCE_DIR}/libs/glm-0.9.9.0"
  PREFIX 	GLM_library
  CMAKE_ARGS 	-DGLM_TEST_ENABLE=OFF
 		-DCMAKE_INSTALL_PREFIX=${GLM_INSTALL_PREFIX}
  TEST_COMMAND 	""
  )

