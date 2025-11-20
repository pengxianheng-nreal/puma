PROJECT_INIT_TARGET_VARIABLE(interface)

add_library( ${TARGET_NAME} INTERFACE )

target_include_directories( ${TARGET_NAME}
	INTERFACE
	"$<BUILD_INTERFACE:${TARGET_INCLUDE_PATH}>"
	"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
	"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/channel>"
	)

######################################################################################
##  							INSTALL  					##
######################################################################################
install(FILES ${TARGET_HEADER_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${TARGET_NAME}")

PROJECT_INSTALL(${TARGET_NAME} ${TARGETS_EXPORT_NAME})
