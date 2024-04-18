cmake_minimum_required(VERSION 3.29.0)

project(MrClobberBackyard)
find_package(Allegro REQUIRED)
find_package(OpenSSL CONFIG REQUIRED)
find_package(CURL REQUIRED)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED 17)

file(GLOB windows_files windows/*.h)
file(GLOB networking_files networking/*.h)
file(GLOB save_files save/*.h)
file(GLOB logics_files logics/*.h)
file(GLOB encryption_files encryption/*.h)

add_executable(MrClobberBackyard
    main.c
    ${windows_files}
    ${networking_files}
    ${save_files}
    ${logics_files}
    ${encryption_files}
    map.h
    flagManager.h
    constants.h
    test.h
)

target_link_libraries(MrClobberBackyard 
    Allegro::allegro 
    Allegro::allegro_primitives 
    Allegro::allegro_image 
    Allegro::allegro_font 
    Allegro::allegro_audio 
    Allegro::allegro_ttf 
    Allegro::allegro_color 
    Allegro::allegro_dialog
)
target_link_libraries(MrClobberBackyard
    OpenSSL::Crypto OpenSSL::SSL
)
target_link_libraries(MrClobberBackyard 
    CURL::libcurl
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src DESTINATION .)

set_target_properties(MrClobberBackyard
    PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE)