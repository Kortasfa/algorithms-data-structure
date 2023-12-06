cmake_minimum_required(VERSION 3.10)

project(algorithms-data-structure)

# Имя подкаталога становится именем цели сборки (и исполняемого файла)
function(add_sample NAME)
    file(GLOB SAMPLE_SRC "${NAME}/*.cpp" "${NAME}/*.h")
    add_executable(${NAME} ${SAMPLE_SRC})
    target_include_directories(${NAME} PRIVATE ${SFML_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
    target_compile_features(${NAME} PUBLIC cxx_std_17)
    target_compile_definitions(${NAME} PRIVATE SFML_STATIC)
    target_link_libraries(${NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${JPEG_LIBRARIES} ${FREETYPE_LIBRARIES})
endfunction()

add_sample(lw4)
