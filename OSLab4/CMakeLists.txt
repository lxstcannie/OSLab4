# CMakeList.txt: проект CMake для OSLab4; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
project(SenderReceiver)
# Добавьте источник в исполняемый файл этого проекта.
set(CMAKE_CXX_STANDARD 98)

add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
