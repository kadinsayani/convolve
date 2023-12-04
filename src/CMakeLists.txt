set(SOURCES
    Convolver.cpp
    WavData.cpp 
    WavFile.cpp 
    WavHeader.cpp
    main.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/big_hall_mono.wav
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/big_hall_stereo.wav
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/guitar.wav
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/guitar_short.wav
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(convolve ${SOURCES})

