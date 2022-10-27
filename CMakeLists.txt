cmake_minimum_required(VERSION 3.13)
project(Explore_Direct2D)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        main.cpp
        BaseWindow.h
        DPIScale.h
        Explore_Direct2D.cpp
        Explore_Direct2D.h
        MainWindow.cpp
        MainWindow.h
        MyEllipse.h
        Resources.h
        Resources.rc
        )

target_link_libraries(${PROJECT_NAME}
        D2d1.lib)

