cmake_minimum_required(VERSION 3.13)
project(Explore_Direct2D)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/BaseWindow.h
        src/DPIScale.h
        src/MainWindow.cpp
        src/MainWindow.h
        src/MyEllipse.h
        src/Resources.h
        src/Resources.rc
        src/Explore_utility.cpp
        src/Explore_utility.h
        src/My_curve.h)

target_link_libraries(${PROJECT_NAME}
        D2d1.lib
        Dwrite.lib
        )

add_subdirectory(src/Unit_test)
