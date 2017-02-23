
aux_source_directory(. POOL_SRCS)

# === Conditional build ============
# cmake .. -DENABLE_SHAREDLIB=TRUE

option (ENABLE_SHAREDLIB "Shared or static flag" OFF)

if (ENABLE_SHAREDLIB)
    add_library(lftpool SHARED ${POOL_SRCS})
else()
    add_library(lftpool STATIC ${POOL_SRCS})
endif()

# target location
install(TARGETS lftpool DESTINATION lib)
install(FILES tpool.h DESTINATION include)