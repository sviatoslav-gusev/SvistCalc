# shared lib
add_library(ExternalEngine SHARED
    ExternalEngine.cpp  # Wrapper with extern "C" func
)

# Enable __declspec(dllexport) for Win
target_compile_definitions(ExternalEngine PRIVATE BUILD_EXTERNAL_ENGINE)


target_link_libraries(ExternalEngine PRIVATE
    SharedMath
)