add_executable(readfile readfile.cpp)
add_executable(appendfile appendfile.cpp)

find_package(Boost COMPONENTS json log program_options REQUIRED)

target_compile_features(readfile PRIVATE cxx_std_20)
target_compile_features(appendfile PRIVATE cxx_std_20)

target_link_libraries(readfile
    MOTION::motion
    Boost::json
    Boost::log
    Boost::program_options
)

target_link_libraries(appendfile
    MOTION::motion
    Boost::json
    Boost::log
    Boost::program_options
)
