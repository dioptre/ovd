# ---------------------------------
# Finds third party boost
# Adds library to target
# Adds include path
# ---------------------------------

IF(UNIX)
    FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem-mt" PATHS ${LIST_DEPENDENCIES_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem-mt" PATHS ${LIST_DEPENDENCIES_PATH} PATH_SUFFIXES lib)
	FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem" PATHS ${LIST_DEPENDENCIES_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH)
	FIND_LIBRARY(LIB_Boost_Filesystem NAMES "boost_filesystem" PATHS ${LIST_DEPENDENCIES_PATH} PATH_SUFFIXES lib)
	IF(LIB_Boost_Filesystem)
		debug_message( "    [  OK  ] lib ${LIB_Boost_Filesystem}")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_Boost_Filesystem} )
	ELSE(LIB_Boost_Filesystem)
		MESSAGE(WARNING "    [FAILED] lib boost_Filesystem-mt")
	ENDIF(LIB_Boost_Filesystem)

	# For Fedora
	FIND_LIBRARY(LIB_STANDARD_MODULE_PTHREAD pthread)
	IF(LIB_STANDARD_MODULE_PTHREAD)
		debug_message( "  Found pthread...")
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_STANDARD_MODULE_PTHREAD})
	ELSE(LIB_STANDARD_MODULE_PTHREAD)
		MESSAGE(WARNING "  FAILED to find pthread...")
	ENDIF(LIB_STANDARD_MODULE_PTHREAD)
ENDIF(UNIX)

IF(WIN32)
	OV_LINK_BOOST_LIB("filesystem" ${OV_WIN32_BOOST_VERSION} )
	OV_LINK_BOOST_LIB("system" ${OV_WIN32_BOOST_VERSION} )			# filesystem depends on system
ENDIF(WIN32)
