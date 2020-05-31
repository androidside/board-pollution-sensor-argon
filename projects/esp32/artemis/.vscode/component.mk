# ARDUINO_ALL_LIBRARIES := $(patsubst $(COMPONENT_PATH)/libraries/%,%,$(wildcard $(COMPONENT_PATH)/libraries/*))

# # Macro returns non-empty if Arduino library $(1) should be included in the build
# # (either because selective compilation is of, or this library is enabled
# define ARDUINO_LIBRARY_ENABLED
# $(if $(CONFIG_ARDUINO_SELECTIVE_COMPILATION),$(CONFIG_ARDUINO_SELECTIVE_$(1)),y)
# endef

# ARDUINO_ENABLED_LIBRARIES := $(foreach LIBRARY,$(sort $(ARDUINO_ALL_LIBRARIES)),$(if $(call ARDUINO_LIBRARY_ENABLED,$(LIBRARY)),$(LIBRARY)))

# $(info Arduino libraries in build: $(ARDUINO_ENABLED_LIBRARIES))

# # Expand all subdirs under $(1)
# define EXPAND_SUBDIRS
# $(sort $(dir $(wildcard $(1)/* $(1)/*/* $(1)/*/*/* $(1)/*/*/*/* $(1)/*/*/*/*/*)))
# endef

# # Macro returns SRCDIRS for library
# define ARDUINO_LIBRARY_GET_SRCDIRS
# 	$(if $(wildcard $(COMPONENT_PATH)/libraries/$(1)/src/.), 							\
# 		$(call EXPAND_SUBDIRS,$(COMPONENT_PATH)/libraries/$(1)/src), 					\
# 		$(filter-out $(call EXPAND_SUBDIRS,$(COMPONENT_PATH)/libraries/$(1)/examples),  \
# 			$(call EXPAND_SUBDIRS,$(COMPONENT_PATH)/libraries/$(1)) 				   	\
# 		) 																				\
# 	)
# endef

# # Make a list of all srcdirs in enabled libraries
# ARDUINO_LIBRARY_SRCDIRS := $(patsubst $(COMPONENT_PATH)/%,%,$(foreach LIBRARY,$(ARDUINO_ENABLED_LIBRARIES),$(call ARDUINO_LIBRARY_GET_SRCDIRS,$(LIBRARY))))

# #$(info Arduino libraries src dirs: $(ARDUINO_LIBRARY_SRCDIRS))

# COMPONENT_ADD_INCLUDEDIRS := cores/esp32 variants/esp32 $(ARDUINO_LIBRARY_SRCDIRS)
# COMPONENT_PRIV_INCLUDEDIRS := cores/esp32/libb64
# COMPONENT_SRCDIRS := cores/esp32/libb64 cores/esp32 variants/esp32 $(ARDUINO_LIBRARY_SRCDIRS)
# CXXFLAGS += -fno-rtti
COMPONENT_SRCDIRS := libraries/Wire/src tools/sdk/include/config tools/sdk/include/app_trace tools/sdk/include/app_update tools/sdk/include/asio tools/sdk/include/bootloader_support tools/sdk/include/bt tools/sdk/include/coap tools/sdk/include/console tools/sdk/include/driver tools/sdk/include/esp-tls tools/sdk/include/esp32 tools/sdk/include/esp_adc_cal tools/sdk/include/esp_event tools/sdk/include/esp_http_client tools/sdk/include/esp_http_server tools/sdk/include/esp_https_ota tools/sdk/include/esp_ringbuf tools/sdk/include/ethernet tools/sdk/include/expat tools/sdk/include/fatfs tools/sdk/include/freemodbus tools/sdk/include/freertos tools/sdk/include/heap tools/sdk/include/idf_test tools/sdk/include/jsmn tools/sdk/include/json tools/sdk/include/libsodium tools/sdk/include/log tools/sdk/include/lwip tools/sdk/include/mbedtls tools/sdk/include/mdns tools/sdk/include/micro-ecc tools/sdk/include/mqtt tools/sdk/include/newlib tools/sdk/include/nghttp tools/sdk/include/nvs_flash tools/sdk/include/openssl tools/sdk/include/protobuf-c tools/sdk/include/protocomm tools/sdk/include/pthread tools/sdk/include/sdmmc tools/sdk/include/smartconfig_ack tools/sdk/include/soc tools/sdk/include/spi_flash tools/sdk/include/spiffs tools/sdk/include/tcp_transport tools/sdk/include/tcpip_adapter tools/sdk/include/ulp tools/sdk/include/vfs tools/sdk/include/wear_levelling tools/sdk/include/wifi_provisioning tools/sdk/include/wpa_supplicant tools/sdk/include/xtensa-debug-module tools/sdk/include/esp-face tools/sdk/include/esp32-camera tools/sdk/include/fb_gfx cores/esp32 variants/esp32 libraries/ArduinoOTA/src libraries/AsyncUDP/src libraries/AzureIoT/src libraries/BLE/src libraries/BluetoothSerial/src libraries/DNSServer/src libraries/EEPROM/src libraries/ESP32/src libraries/ESPmDNS/src libraries/FFat/src libraries/FS/src libraries/HTTPClient/src libraries/HTTPUpdate/src libraries/NetBIOS/src libraries/Preferences/src libraries/SD/src libraries/SD_MMC/src libraries/SPI/src libraries/SPIFFS/src libraries/SimpleBLE/src libraries/Ticker/src libraries/Update/src libraries/WebServer/src libraries/WiFi/src libraries/WiFiClientSecure/src
COMPONENT_ADD_INCLUDEDIRS := libraries/Wire/src tools/sdk/include/config tools/sdk/include/app_trace tools/sdk/include/app_update tools/sdk/include/asio tools/sdk/include/bootloader_support tools/sdk/include/bt tools/sdk/include/coap tools/sdk/include/console tools/sdk/include/driver tools/sdk/include/esp-tls tools/sdk/include/esp32 tools/sdk/include/esp_adc_cal tools/sdk/include/esp_event tools/sdk/include/esp_http_client tools/sdk/include/esp_http_server tools/sdk/include/esp_https_ota tools/sdk/include/esp_ringbuf tools/sdk/include/ethernet tools/sdk/include/expat tools/sdk/include/fatfs tools/sdk/include/freemodbus tools/sdk/include/freertos tools/sdk/include/heap tools/sdk/include/idf_test tools/sdk/include/jsmn tools/sdk/include/json tools/sdk/include/libsodium tools/sdk/include/log tools/sdk/include/lwip tools/sdk/include/mbedtls tools/sdk/include/mdns tools/sdk/include/micro-ecc tools/sdk/include/mqtt tools/sdk/include/newlib tools/sdk/include/nghttp tools/sdk/include/nvs_flash tools/sdk/include/openssl tools/sdk/include/protobuf-c tools/sdk/include/protocomm tools/sdk/include/pthread tools/sdk/include/sdmmc tools/sdk/include/smartconfig_ack tools/sdk/include/soc tools/sdk/include/spi_flash tools/sdk/include/spiffs tools/sdk/include/tcp_transport tools/sdk/include/tcpip_adapter tools/sdk/include/ulp tools/sdk/include/vfs tools/sdk/include/wear_levelling tools/sdk/include/wifi_provisioning tools/sdk/include/wpa_supplicant tools/sdk/include/xtensa-debug-module tools/sdk/include/esp-face tools/sdk/include/esp32-camera tools/sdk/include/fb_gfx cores/esp32 variants/esp32 libraries/ArduinoOTA/src libraries/AsyncUDP/src libraries/AzureIoT/src libraries/BLE/src libraries/BluetoothSerial/src libraries/DNSServer/src libraries/EEPROM/src libraries/ESP32/src libraries/ESPmDNS/src libraries/FFat/src libraries/FS/src libraries/HTTPClient/src libraries/HTTPUpdate/src libraries/NetBIOS/src libraries/Preferences/src libraries/SD/src libraries/SD_MMC/src libraries/SPI/src libraries/SPIFFS/src libraries/SimpleBLE/src libraries/Ticker/src libraries/Update/src libraries/WebServer/src libraries/WiFi/src libraries/WiFiClientSecure/src