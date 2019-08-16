add_library(AutoPilotPluginCommon
	ESP8266Component.cc
	ESP8266ComponentController.cc
	MotorComponent.cc
	RadioComponentController.cc
	SyslinkComponent.cc
	SyslinkComponentController.cc
)

add_custom_target(
    AutoPilotPluginCommonQml
SOURCES
    ESP8266Component.qml  
    ESP8266ComponentSummary.qml  
    MotorComponent.qml  
    RadioComponent.qml  
    SetupPage.qml  
    SyslinkComponent.qml
)

target_link_libraries(AutoPilotPluginCommon
	PRIVATE
		qgc
)
