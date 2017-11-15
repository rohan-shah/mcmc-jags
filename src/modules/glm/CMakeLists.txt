add_subdirectory(samplers)
add_subdirectory(SSparse)

add_library(glm STATIC glm.cc)
target_include_directories(glm PUBLIC .)
target_link_libraries(glm PUBLIC CHOLMODCore)