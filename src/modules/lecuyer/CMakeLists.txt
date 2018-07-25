add_library(lecuyer STATIC lecuyer.cc RngStream.cc RngStreamFactory.cc RngStream.h RngStreamFactory.h)
target_include_directories(lecuyer PRIVATE ${CMAKE_SOURCE_DIR}/src/include ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS lecuyer DESTINATION lib/JAGS/modules-5)