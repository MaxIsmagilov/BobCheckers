# Add your source files here
set(BobCheckersHeaders
    utils.hpp
    move.hpp
    board.hpp
    board_stack.hpp
    evaluation.hpp
    move_generator.cpp
    transposition.hpp
    algorithm.hpp
    api.hpp
)

# Add your header files here
set(BobCheckersSources
    utils.cpp
    move.cpp
    board.cpp
    board_stack.cpp
    evaluation.cpp
    move_generator.cpp
    transposition.cpp
    algorithm.cpp
    api.cpp
)

ADD_LIBRARY(BobCheckersObjects OBJECT 
    ${BobCheckersSources}
)

TARGET_COMPILE_OPTIONS(
    BobCheckersObjects PUBLIC "-std=c++20" "-O3" "-Wall" "-Wextra" "-Wpedantic"  "-Werror" 
)

target_include_directories(BobCheckersObjects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

ADD_LIBRARY(BobCheckers $<TARGET_OBJECTS:BobCheckersObjects>)

ADD_LIBRARY(BobCheckers::BobCheckers ALIAS BobCheckers)

SET_TARGET_PROPERTIES( BobCheckers
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    COMPILE_OPTIONS "-O3 -Wall -Wextra -Wpedantic -Werror -std=c++20"
    PUBLIC_HEADER "${BobCheckersHeaders}"
)

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS BobCheckers
    EXPORT BobCheckersTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/src
)

INSTALL(EXPORT BobCheckersTargets
    FILE BobCheckersTargets.cmake
    NAMESPACE BobCheckers::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BobCheckers
)

ADD_EXECUTABLE(BobCheckers_${PROJECT_VERSION} BobCheckers.cpp)
TARGET_LINK_LIBRARIES(BobCheckers_${PROJECT_VERSION} BobCheckers::BobCheckers)