set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Qml Quick REQUIRED)

# 1. Code helpers from Qt Creator
set(QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "QML Import Path" FORCE)

# 2. QML resources
set(QRC_SOURCES qml.qrc)
qt5_add_resources(RESOURCES ${QRC_SOURCES})




add_executable(SvistCalcApp
    src/main.cpp

    include/client/SafeQueue.h

    include/client/Task.h
    src/Task.cpp

    include/client/WorkerThread.h
    src/WorkerThread.cpp

    include/client/ExternalEngineMgr.h
    src/ExternalEngineMgr.cpp

    ${RESOURCES}
    include/client/Application.h
    src/Application.cpp
)

target_include_directories(SvistCalcApp PUBLIC
    include/client
    ../shared
)

target_link_libraries(SvistCalcApp PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    SharedMath
)

# hide terminal
if(WIN32)
    set_target_properties(SvistCalcApp PROPERTIES WIN32_EXECUTABLE ON)
endif()
