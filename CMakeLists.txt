project(Perc)
cmake_minimum_required(VERSION 3.2)
#-fopenmp -fsanitize=address -fsanitize-recover=address
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_NAME "ESN")
set(MY_INCLUDES . ./ ./*/ ./*/*/)
file(GLOB SRC
    "./*/*/*.cpp"
    "./*/*.cpp"
    "./*cpp"
)

#Subdirectories
option(USE_CLANG "build application with clang" ON)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(${EXECUTABLE_NAME} ${SRC} )
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${MY_INCLUDES})
TARGET_LINK_LIBRARIES(  ${EXECUTABLE_NAME}  ${ARMADILLO_LIBRARIES} )

