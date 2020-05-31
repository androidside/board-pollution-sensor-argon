# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# set(EXTRA_COMPONENT_DIRS $ENV{COMPONENTS}/framework-arduinoespressif32)
#set(PRIV_INCLUDE_DIRS $ENV{COMPONENTS}/arduino)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(artemis)