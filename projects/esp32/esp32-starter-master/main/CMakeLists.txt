set(COMPONENT_SRCDIRS "." )
# set(COMPONENT_SRCS "main.c" "fetch.c" "connect.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

set(COMPONENT_EMBED_TXTFILES
    "files/index.html"
    "files/sample.txt"
)

set(COMPONENT_EMBED_FILES
    "files/pinout.jpg"    
)

register_component()
spiffs_create_partition_image(storage ../spiffs_dir FLASH_IN_PROJECT)
