add_library(PX4FirmwarePlugin
	PX4FirmwarePlugin.cc
	PX4FirmwarePluginFactory.cc
	PX4ParameterMetaData.cc
	PX4Resources.qrc
)

target_link_libraries(PX4FirmwarePlugin
PRIVATE
	qgc
)