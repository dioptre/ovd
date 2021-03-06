PROJECT(openvibe-plugins-designer-advanced-visualization)
SET(PROJECT_PRODUCT_NAME "Mensia Advanced Visualization Toolset")
MESSAGE(STATUS "Now building ${PROJECT_NAME} ${PROJECT_VERSION} (${OV_PROJECT_BRANCH}~${OV_PROJECT_COMMITHASH})" )

FILE(GLOB_RECURSE source_files src/*.cpp src/*.h src/*.hpp src/*.inl include/*.h)

INCLUDE("FindSourceRCProperties")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${source_files})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${RLM_LIB_FILES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	COMPILE_FLAGS "-DOVP_Exports -DOVP_Shared")

IF(MENSIA_ADVANCED_VISUALIZATION_EXPERIMENTAL)
	MESSAGE(STATUS "  MENSIA_ADVANCED_VISUALIZATION_EXPERIMENTAL is set, experimental visualization will be built.")
	ADD_DEFINITIONS(-DTARGET_Has_Experimental)
ENDIF()

INCLUDE("AddOpenViBESDKComponents")
include("FindOpenViBEVisualizationToolkit")
INCLUDE("FindThirdPartyGTK")

INCLUDE("FindLibMensiaAdvancedVisualisation")
INCLUDE("FindThirdPartyOpenGL")

INCLUDE("GenerateDocumentation")

# ---------------------------------
# Target macros
# Defines target operating system
# Defines target architecture
# Defines target compiler
# ---------------------------------
SET_BUILD_PLATFORM()

# -----------------------------
# Install files
# -----------------------------
INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${DIST_BINDIR}
	LIBRARY DESTINATION ${DIST_LIBDIR}
	ARCHIVE DESTINATION ${DIST_LIBDIR})

INSTALL(DIRECTORY share/ DESTINATION ${DIST_DATADIR}/openvibe/plugins)
INSTALL(DIRECTORY box-tutorials/  DESTINATION ${DIST_DATADIR}/openvibe/scenarios/box-tutorials/advanced-visualization)
