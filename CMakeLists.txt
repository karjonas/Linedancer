cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(linedancer)

set(SOURCES_CPP
  src/main.cpp
  src/Drawing.cpp
  src/GlobalState.cpp
)

add_executable(linedancer ${SOURCES_CPP})

set(LIBS
  allegro
  allegro_primitives
  allegro_font
  allegro_ttf
  allegro_audio
  allegro_acodec
  allegro_image
)


set(DATA_DIR ${PROJECT_SOURCE_DIR}/data CACHE PATH "Directory for game data")
add_definitions(-DDATA_DIR="${DATA_DIR}")

target_link_libraries(linedancer ${LIBS})

install(TARGETS linedancer DESTINATION bin)
install(DIRECTORY data DESTINATION share/linedancer)
