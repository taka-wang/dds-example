
aux_source_directory(. POOL_SRCS)

# === Conditional build ============
# cmake .. -DENABLE_SHAREDLIB=TRUE

option (ENABLE_SHAREDLIB "Shared or static flag" OFF)

if (ENABLE_SHAREDLIB)
    add_library(pool SHARED ${POOL_SRCS})
else()
    add_library(pool STATIC ${POOL_SRCS})
endif()

# target location
install(TARGETS pool DESTINATION lib)
install(FILES threadpool.h DESTINATION include)