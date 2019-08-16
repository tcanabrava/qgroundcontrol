add_library(APMFirmwarePlugin
	APMFirmwarePlugin.cc
	APMFirmwarePluginFactory.cc
	APMParameterMetaData.cc
	ArduCopterFirmwarePlugin.cc
	ArduPlaneFirmwarePlugin.cc
	ArduRoverFirmwarePlugin.cc
	ArduSubFirmwarePlugin.cc
	APMResources.qrc
)

target_link_libraries(APMFirmwarePlugin
PRIVATE
	qgc
)