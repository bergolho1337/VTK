set(SOURCE_FILES_TISSUE
        control_volume.h control_volume.cpp)

ADD_LIBRARY(tissue STATIC ${SOURCE_FILES_TISSUE})

set_property(TARGET tissue PROPERTY POSITION_INDEPENDENT_CODE ON)
