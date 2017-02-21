
aux_source_directory(. GEN_SRCS)

SET(DDS_HEADER_INCLUDE_DIR /opt/PrismTech/Vortex_v2/Device/VortexLite/2.0.4/include)
include_directories (${DDS_HEADER_INCLUDE_DIR} )

# === Conditional build ============
# cmake .. -DENABLE_SHAREDLIB=TRUE

option (ENABLE_SHAREDLIB "Shared or static flag" OFF)

if (ENABLE_SHAREDLIB)
    add_library(gen SHARED ${GEN_SRCS})
else()
    add_library(gen STATIC ${GEN_SRCS})
endif()

# target location
install(TARGETS gen DESTINATION lib)
install(FILES mb.h DESTINATION include)