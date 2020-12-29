cmake_minimum_required(VERSION 3.17)
project(serverhttp)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "start running cmake...")


# WINDOWS
#[[
find_package(Boost 1.71.0 COMPONENTS system filesystem REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(serverhttp main.cpp crypto.cpp crypto.h connection.cpp connection.h mime_types.cpp mime_types.h
        header.h reply.cpp reply.h request.h request_handler.cpp request_handler.h request_parser.cpp request_parser.h
        server.cpp server.h)

if(Boost_FOUND)
    target_link_libraries(serverhttp ${Boost_LIBRARIES} -lwsock32 -lws2_32 -lboost_thread)
endif()

set(OPENSSL_USE_STATIC_LIBS TRUE)
include_directories(OPENSSL_INCLUDE_DIR "C:/OpenSSL/DLL/x64")
find_package(OpenSSL)
if(OpenSSL_FOUND)
    message(STATUS "OpenSSL Found: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")
    target_link_libraries(serverhttp OpenSSL::Crypto)
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's ow
endif()
]]



#######################################################################################################################



# LINUX
set(Boost_INCLUDE_DIR /home/daniela/Scaricati/boost_1_74_0)
set(Boost_LIBRARY_DIR /home/daniela/Scaricati/boost_1_74_0/stage/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
add_executable(serverhttp main.cpp crypto.cpp crypto.h connection.cpp connection.h mime_types.cpp mime_types.h
        header.h reply.cpp reply.h request.h request_handler.cpp request_handler.h request_parser.cpp request_parser.h
        server.cpp server.h)
target_link_libraries(serverhttp ${Boost_LIBRARIES} -lpthread -lboost_thread -lboost_system )
target_link_libraries(serverhttp stdc++fs)
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's ow
endif()
target_link_libraries(serverhttp ${OPENSSL_LIBRARIES} -lssl -lcrypto)
