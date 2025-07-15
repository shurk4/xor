cmake_minimum_required(VERSION 3.5)

project(xor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(xor
    main.cpp

    mainwindow.h
    mainwindow.cpp
    mainwindow.ui

    # Основной движок программы
    engine.h
    engine.cpp

    # Класс проверки новых файлов для шифрования
    # Запускается в отдельном потоке
    inputfileschecker.h
    inputfileschecker.cpp

    # Класс шифрования файлов
    # Запускается в глобальном пуле потоков
    encryptfile.h
    encryptfile.cpp
    )

target_link_libraries(xor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
