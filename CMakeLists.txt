cmake_minimum_required(VERSION 3.5)

project(DetectionLive LANGUAGES CXX)
set(CUDA_TOOLKIT_ROOT_DIR "D:/Apps/NVIDIA/NVIDIA GPU Computing Toolkit/CUDA/v11.0")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(OpenCV 3.4.11 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
set(TS_FILES DetectionLive_zh_CN.ts)

set(${PROJECT_NAME}_HEADERS
    mainwindow.h
    detection.h
    videoprocess.h
    preprocess.h
)
set(${PROJECT_NAME}_SOURCES
    main.cpp
    mainwindow.cpp
    detection.cpp
    videoprocess.cpp
    preprocess.cpp
    #icon.rc
)
set(${PROJECT_NAME}_FORMS
    mainwindow.ui
    detection.ui
)
set(${PROJECT_NAME}_RESOURCES
    #Resources/Godric.qrc
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_RESOURCES} ${${PROJECT_NAME}_FORMS})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets ${OpenCV_LIBRARIES} ${VTK_LIBRARIES} ${CUDA_LIBRARIES})
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
