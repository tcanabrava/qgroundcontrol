# Todo: Verify why splitting the Plugins into
# static libs breaks the build.
# add_subdirectory(APM)
# add_subdirectory(PX4)

add_library(FirmwarePlugin
	APM/APMFirmwarePlugin.cc
	APM/APMFirmwarePluginFactory.cc
	APM/APMParameterMetaData.cc
	APM/ArduCopterFirmwarePlugin.cc
	APM/ArduPlaneFirmwarePlugin.cc
	APM/ArduRoverFirmwarePlugin.cc
	APM/ArduSubFirmwarePlugin.cc
	APM/APMResources.qrc

	PX4/PX4FirmwarePlugin.cc
	PX4/PX4FirmwarePluginFactory.cc
	PX4/PX4ParameterMetaData.cc
	PX4/PX4Resources.qrc

	CameraMetaData.cc
	FirmwarePlugin.cc
	FirmwarePluginManager.cc
)

target_link_libraries(FirmwarePlugin
	PRIVATE
		qgc
)
