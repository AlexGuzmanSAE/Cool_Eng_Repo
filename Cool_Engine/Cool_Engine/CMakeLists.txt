cmake_minimum_required(VERSION 3.18)
project(Cool_EngineX)

# 1. Exportar configuración para que LazyVim (clangd) no marque errores falsos
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 2. Regla global: Obligar a Sol2 a usar la versión correcta de Lua en todos los archivos
add_compile_definitions(SOL_ALL_SAFETIES_ON=1 SOL_LUA54=1)

# 3. Buscar todos tus archivos de código fuente
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 4. Crear el ejecutable del motor
add_executable(Cool_EngineX ${SOURCES})

# 5. Buscar las dependencias nativas de Arch Linux
# Usamos PkgConfig porque es el método más seguro para encontrar LuaJIT en Linux
find_package(PkgConfig REQUIRED)
pkg_search_module(LUA REQUIRED luajit)

find_package(raylib REQUIRED)
find_package(box2d REQUIRED)
find_package(sol2 REQUIRED)

# 6. Darle al compilador las rutas donde están tus .h y raygui.h
target_include_directories(Cool_EngineX PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/include
    ${LUA_INCLUDE_DIRS}
)

# 7. Conectar las librerías al ejecutable final
target_link_libraries(Cool_EngineX PRIVATE 
    raylib 
    box2d::box2d 
    ${LUA_LIBRARIES}
    sol2::sol2
)
