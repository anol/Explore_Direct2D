cmake_minimum_required(VERSION 3.13)
project(Explore_Direct2D)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/BaseWindow.h
        src/DPIScale.h
        src/Explore_Direct2D.cpp
        src/Explore_Direct2D.h
        src/MainWindow.cpp
        src/MainWindow.h
        src/MyEllipse.h
        src/Resources.h
        src/Resources.rc
        src/My_grid.cpp
        src/My_grid.h
        src/Explore_utility.cpp
        src/Explore_utility.h

        src/Unit_test/Unit_test.cpp)

target_link_libraries(${PROJECT_NAME}
        D2d1.lib)

add_subdirectory(src/Unit_test)
