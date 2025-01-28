cmake_minimum_required(VERSION 3.16)
project(RecorderUsingApiOpenCV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCV_DIR "C:/opencv/build")
set(PortAudio_DIR "C:/portaudio/build")


find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Multimedia)
find_package(OpenCV REQUIRED)
find_package(PortAudio REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} )
include_directories("C:/portaudio/include")


qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appRecorderUsingApiOpenCV
    main.cpp
    VideoRecorder.cpp
    VideoRecorder.h
)

qt_add_qml_module(appRecorderUsingApiOpenCV
    URI RecorderUsingApiOpenCV
    VERSION 1.0
    QML_FILES
        Main.qml
        Video.qml
        Audio.qml
        Sidebar.qml
        Player.qml
    SOURCES
        filemanager.h
        filemanager.cpp
SOURCES audiorecorder.h audiorecorder.cpp
)

set_target_properties(appRecorderUsingApiOpenCV PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appRecorderUsingApiOpenCV
    PRIVATE Qt6::Quick
            Qt6::Multimedia
            ${OpenCV_LIBS}
            ${PORTAUDIO_LIBRARIES} portaudio
        )

# Set custom output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# Copy OpenCV DLLs to the output directory
file(GLOB OPENCV_DLLS "${OpenCV_DIR}/../bin/*.dll")
file(GLOB PORTAUDIO_DLLS "${PortAudio_DIR}/../../bin/*.dll")
foreach(file ${OPENCV_DLLS} ${PORTAUDIO_DLLS})
    add_custom_command(TARGET appRecorderUsingApiOpenCV POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${file}
        $<TARGET_FILE_DIR:appRecorderUsingApiOpenCV>)
endforeach()

include(GNUInstallDirs)
install(TARGETS appRecorderUsingApiOpenCV
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
