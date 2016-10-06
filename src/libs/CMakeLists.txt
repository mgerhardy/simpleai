include_directories(.)

if (SIMPLEAI_LUA)
	find_package(Lua)
	if (NOT LUA_FOUND)
		add_subdirectory(lua)
	endif()
endif()

add_subdirectory(tinyxml2)

find_package(GLM)
if (NOT GLM_FOUND)
	add_subdirectory(glm)
	target_include_directories(simpleai PUBLIC .)
endif()
