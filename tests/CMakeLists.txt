include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(Catch2)

set(SOURCES
    tests.cpp
)

add_executable(tests ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
