PROJECT_INIT_TARGET_VARIABLE(util)

add_library( ${TARGET_NAME} OBJECT )

target_sources( ${TARGET_NAME}
	PRIVATE
	${TARGET_HEADER_FILES}
	${TARGET_SOURCE_FILES}
	)

target_include_directories( ${TARGET_NAME}
	PUBLIC	
	${EXTERNAL_INCLUDE_DIR}
	"${TARGET_INCLUDE_PATH}"
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	)

target_link_libraries(${TARGET_NAME}
	PUBLIC
	framework::framework
	)
