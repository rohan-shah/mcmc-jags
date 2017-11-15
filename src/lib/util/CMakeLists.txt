set(util_cpp nainf.c naconst.cc dim.cc integer.cc)
add_library(util OBJECT ${util_cpp})
target_include_directories(util PUBLIC .)