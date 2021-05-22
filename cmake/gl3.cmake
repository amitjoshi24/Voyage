FIND_PACKAGE(GLEW REQUIRED)

if (APPLE)
         set(GLEW_INCLUDE_DIRS "/usr/local/Cellar/glew/2.2.0_1/include")
         set(GLEW_LIBRARIES "/usr/local/Cellar/glew/2.2.0_1/lib/libGlEW.a")
endif ()
INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
LINK_LIBRARIES(${GLEW_LIBRARIES})

IF (WIN32)
	find_package(glfw3 CONFIG REQUIRED)
	LIST(APPEND stdgl_libraries glfw ${GLEW_LIBRARIES})
ELSE ()
	FIND_PACKAGE(PkgConfig REQUIRED)
	#pkg_search_module(GLFW3 REQUIRED glfw3)
        set(GLFW3_INCLUDE_DIRS "/usr/local/include")
	INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIRS})
        set(GLFW3_STATIC_LIBRARIES "/usr/local/lib/libglfw3.a")
	LIST(APPEND stdgl_libraries ${GLFW3_STATIC_LIBRARIES} ${GLEW_LIBRARIES})
ENDIF ()

message(STATUS "GLEW_LIBRARIES=${GLEW_LIBRARIES}")
message(STATUS "GLFW_LIBRARIES=${GLFW3_STATIC_LIBRARIES}")
