
add_subdirectory(APM)
add_subdirectory(Common)
add_subdirectory(Generic)
add_subdirectory(PX4)

add_library(AutoPilotPlugins
	AutoPilotPlugin.cc
)

target_link_libraries(AutoPilotPlugins
	PRIVATE
		qgc
)

target_include_directories(
	AutoPilotPlugins
INTERFACE 
	${CMAKE_CURRENT_SOURCE_DIR}
)
