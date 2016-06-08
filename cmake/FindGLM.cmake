include(FindPackageHandleStandardArgs)

find_path(GLM_INCLUDE_DIR
	HINTS ENV GLMDIR
	NAMES glm/glm.hpp
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
)

find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

if (GLM_FOUND)
	set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

mark_as_advanced(GLM_INCLUDE_DIR)
