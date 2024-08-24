set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_GENERATOR_PLATFORM "x64")
set(VCPKG_TARGET_TRIPLET "x64-windows")

cmake_minimum_required(VERSION 3.27.0)
project(RPC-server)

file(GLOB src 
	source/*.cpp 
	source/*.h
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6WebSockets CONFIG REQUIRED)

qt_standard_project_setup()

add_executable(${PROJECT_NAME} ${WIN32_GUI} ${src})

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Qt::Core Qt6::Core Qt::CorePrivate Qt6::CorePrivate
	Qt::WebSockets Qt6::WebSockets Qt::WebSocketsPrivate Qt6::WebSocketsPrivate
	)
	
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})