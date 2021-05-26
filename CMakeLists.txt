cmake_minimum_required(VERSION 3.2)

# Project name
PROJECT(E_trading_platform_management)

# common settings
set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(
include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -fopenmp -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Ofast -fopenmp -Wall")

#compile E_trading_platform_management
file(GLOB DIR_SRCS CONFIGURE_DEPENDS "src/*.cpp" "src/*.c" "src/*.hpp")
file(GLOB_RECURSE DIR_SERVER_SRCS CONFIGURE_DEPENDS "src/server/*.cpp" "src/server/*.c" "src/server/*.hpp")
file(GLOB_RECURSE DIR_UTILS_SRCS CONFIGURE_DEPENDS "src/utils/*.cpp" "src/utils/*.c" "src/server/*.hpp")
# list(REMOVE_ITEM DIR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.cpp)
# message(${DIR_SRCS})

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} 
${DIR_SRCS} 
${DIR_SERVER_SRCS}
${DIR_UTILS_SRCS}
)

# message(${DIR_APPS_GRAPH_ANALYSIS_SRCS})
# ADD_EXECUTABLE(graphanalysis 
# ${DIR_SRCS} 
# ${DIR_APPS_GRAPH_ANALYSIS_SRCS}
# ${DIR_ENGINE_SRCS}
# ${DIR_ENGINE_GRAPHSAMPLING_SRCS}
# ${DIR_GRAPH_SRCS}
# ${DIR_SAMPLING_SRCS}
# ${DIR_UTILS_SRCS}
# )

# compile gen
# ADD_EXECUTABLE(gen src/preprocess/gen.cpp)