add_library(SharedMath STATIC
    include/shared/SharedMath.h
    src/SharedMath.cpp
)

# To be able include static lib into dynamic one
set_target_properties(SharedMath PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# All parent linkers - look at my headers
target_include_directories(SharedMath PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shared
)
