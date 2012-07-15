include_directories (${3DLIB_SOURCE_DIR}/source)
link_directories (${3DLIB_BINARY_DIR}/source)
add_executable (3dlib ${3DLIB_SOURCE_DIR}/main.cpp)
target_link_libraries (3dlib ${GLU_LIBRARY} ${SDL_LIBRARY} ${OPENGL_LIBRARY} source)