cmake_minimum_required(VERSION 3.0)

project("Level 6: EDA CUP")

set (CMAKE_CXX_STANDARD 11)

# From "Working with CMake" documentation:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # AddressSanitizer (ASan)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # UndefinedBehaviorSanitizer (UBSan)
    add_compile_options(-fsanitize=undefined)
    add_link_options(-fsanitize=undefined)
endif()


include_directories(include)
file(GLOB PLAYERS "players/*.cpp")
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES} ${PLAYERS})


find_package(mosquitto REQUIRED)
find_package(OpenSSL REQUIRED)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(raylib CONFIG REQUIRED)
    target_include_directories(main PRIVATE ${RAYLIB_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
    target_link_libraries(main PRIVATE ${RAYLIB_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(raylib CONFIG REQUIRED)
    target_include_directories(main PRIVATE ${raylib_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS})
    target_link_libraries(main PRIVATE ${raylib_LIBRARIES} ${MOSQUITTO_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto)
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # From "Working with CMake" documentation:
    target_link_libraries(main PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(main PRIVATE m ${CMAKE_DL_LIBS} pthread GL rt X11)
endif()
