add_library(AutoPilotPluginsGeneric
	GenericAutoPilotPlugin.cc
)

target_link_libraries(AutoPilotPluginsGeneric
	PRIVATE
		qgc
)
