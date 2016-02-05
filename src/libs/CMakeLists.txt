if (SIMPLEAI_LUA)
	find_package(Lua)
	if (NOT LUA_FOUND)
		add_subdirectory(lua)
	endif()
endif()

if (SIMPLEAI_XML)
	add_subdirectory(tinyxml2)
endif()

add_subdirectory(glm)
