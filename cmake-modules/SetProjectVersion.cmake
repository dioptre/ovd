function(OV_SET_PROJECT_VERSION)
	
	find_file(OVSDK_DOTINSTALL NAMES .install PATHS ${OPENVIBE_SDK_PATH} NO_DEFAULT_PATH)
	if(CMAKE_BUILD_TYPE)
		string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
		find_file(OVSDK_DOTINSTALL NAMES .install PATHS ${LIST_DEPENDENCIES_PATH} PATH_SUFFIXES openvibe-sdk-${CMAKE_BUILD_TYPE_LOWER} NO_DEFAULT_PATH)
	endif()

	file(STRINGS ${OVSDK_DOTINSTALL} ov_sdk_install_manifest)
	foreach(NameAndValue ${ov_sdk_install_manifest})
		string(STRIP NameAndValue ${NameAndValue})
		string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
		string(REPLACE "${Name}=" "" Value ${NameAndValue})
		if(${Name} STREQUAL "[Version]")
			set(OV_SDK_VERSION_STRING "${Value}" PARENT_SCOPE)
		endif()
	endforeach()

	set(OV_GLOBAL_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} PARENT_SCOPE)
	set(OV_GLOBAL_VERSION_MINOR ${PROJECT_VERSION_MINOR} PARENT_SCOPE)
	set(OV_GLOBAL_VERSION_PATCH ${PROJECT_VERSION_PATCH} PARENT_SCOPE)
	set(OV_GLOBAL_VERSION_BUILD ${PROJECT_VERSION_BUILD} PARENT_SCOPE)
	set(OV_GLOBAL_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD}" PARENT_SCOPE)
	set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} PARENT_SCOPE)

	set(OV_PROJECT_BRANCH ${PROJECT_BRANCH} PARENT_SCOPE)
	set(OV_PROJECT_COMMITHASH ${PROJECT_COMMITHASH} PARENT_SCOPE)

	# Write version to .install file
	string(TIMESTAMP BUILD_DATE "%d/%m/%y %H:%M:%S")
	set(INSTALL_MANIFEST_STR "[Package]=${PROJECT_NAME}\
\n[Version]=${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD}\
\n[BuildHash]=${PROJECT_COMMITHASH}\
\n[BuildBranch]=${PROJECT_BRANCH}\
\n[BuildDate]=${BUILD_DATE}"
		)
	file(WRITE ${CMAKE_BINARY_DIR}/.install "${INSTALL_MANIFEST_STR}")

endfunction(OV_SET_PROJECT_VERSION)

