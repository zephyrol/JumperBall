CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(JumperBallLibs)

ADD_SUBDIRECTORY(glad)

SET(GLFW_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/libs/GLFW_install/)
SET(GLM_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/libs/GLM_install/)

INCLUDE(ExternalProject)
ExternalProject_Add( _GLFW_
  URL 		"https://github.com/glfw/glfw/archive/3.3.2.tar.gz"
  SOURCE_DIR 	"${CMAKE_SOURCE_DIR}/libs/glfw-3.3.2"
  PREFIX 	GLFW_library	
  CMAKE_ARGS 	-DGLFW_BUILD_DOCS=OFF 
  		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_EXAMPLES=OFF
		-DCMAKE_INSTALL_PREFIX=${GLFW_INSTALL_PREFIX}
  TEST_COMMAND 	""
  )

  SET( LIST_LIBRARIES 
	  "${GLFW_INSTALL_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}") 


  if(UNIX)
	  find_package(Threads REQUIRED)
	  find_package(X11 REQUIRED)
		
	  FOREACH ( X11_LIB X11_Xrandr X11_xf86vmode X11_Xcursor X11_Xinerama X11_Xinerama)
		
		  IF(NOT ${X11_LIB}_FOUND)
			  MESSAGE(FATAL_ERROR "${X11_LIB} library is required for GLFW but not found ...")
		  ENDIF()
	  ENDFOREACH(X11_LIB)

	  LIST(APPEND LIST_LIBRARIES
		  "${X11_Xrandr_LIB}" 
		  "${X11_Xxf86vm_LIB}"
		  "${X11_Xcursor_LIB}"
		  "${X11_Xinerama_LIB}"
		  "${X11_Xinput_LIB}"
		  "${CMAKE_THREAD_LIBS_INIT}" -lrt -ldl -lX11)
  ENDIF()

  set( GLFW_LIBRARIES ${LIST_LIBRARIES} CACHE STRING "") 
  

INCLUDE(ExternalProject)
ExternalProject_Add( _GLM_
  URL 		"https://github.com/g-truc/glm/archive/0.9.9.0.tar.gz"
  SOURCE_DIR 	"${CMAKE_SOURCE_DIR}/libs/glm-0.9.9.0"
  PREFIX 	GLM_library
  CMAKE_ARGS 	-DGLM_TEST_ENABLE=OFF
 		-DCMAKE_INSTALL_PREFIX=${GLM_INSTALL_PREFIX}
  TEST_COMMAND 	""
  )



