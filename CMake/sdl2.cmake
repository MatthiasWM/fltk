#######################################################################
# SDL2
#
# Option to request SDL2 backend.
# Download and compile SDL2.
# provide functions to build with SDL2.
#

option (FLTK_USE_SDL "generate FLTK with SDL2 backend" OFF)

if (FLTK_USE_SDL)

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED ON CACHE BOOL "" FORCE )
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL2_DISABLE_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL2_DISABLE_UNINSTALL ON CACHE BOOL "" FORCE )
set( SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE )
#set( SDL2TTF_HARFBUZZ ON CACHE BOOL "" FORCE )
#set( SDL2TTF_VENDORED ON CACHE BOOL "" FORCE )

# Use FetchContent to clone SDL from GitHub
include(FetchContent)
FetchContent_Declare(
   SDL2
   GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
   GIT_TAG release-2.26.0
)
#FetchContent_Declare(
#    SDL2_ttf
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
#    GIT_TAG SDL2
#)
#FetchContent_MakeAvailable(SDL2 SDL2_ttf)
FetchContent_MakeAvailable(SDL2)

include_directories (${SDL2_SOURCE_DIR}/include)
link_directories(${PROJECT_BINARY_DIR}/Debug)

export (TARGETS SDL2-static sdl-build-options FILE ${CMAKE_CURRENT_BINARY_DIR}/SDL-Targets.cmake)

# Set the SDL_SKIP_INSTALL option to avoid generating the uninstall target
#set_target_properties(SDL PROPERTIES SDL2_DISABLE_INSTALL ON)
#set_target_properties(SDL PROPERTIES SDL2_DISABLE_UNINSTALL ON)
#set_target_properties(SDL2main PROPERTIES SKIP_INSTALL_ALL ON)
#set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

#target_link_libraries(
#    client
#    OpenGL::GL
#    SDL2::SDL2
#    SDL2::SDL2main
#    SDL2_ttf::SDL2_ttf)


# Add your project executable
#add_executable(my_executable main.cpp)

# Link your project with the SDL libraries
#target_link_libraries(my_executable SDL2 SDL2main)


#add_executable(sdl_test main.cpp)
#target_link_libraries(sdl_test SDL2::SDL2main SDL2::SDL2-static)
#target_link_libraries(sdl_test SDL2::SDL2-static)

endif(FLTK_USE_SDL)

