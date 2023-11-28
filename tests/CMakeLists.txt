include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

set(SOURCES
    ../src/Convolver.cpp 
    ../src/WavData.cpp
    ../src/WavFile.cpp
    ../src/WavHeader.cpp
    tests.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/guitar_short.wav
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(tests ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
