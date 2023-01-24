set(INCDIR ${CMAKE_CURRENT_LIST_DIR}/include/)
set(LIBHEADERS
    ${INCDIR}cpu.h
    ${INCDIR}screen.h
)


set(LIBSOURCES
    ${CMAKE_CURRENT_LIST_DIR}/cpu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/screen.cpp
)


include_directories(${INCDIR})
add_library(${LIBNAME} STATIC ${LIBSOURCES})
target_include_directories(${LIBNAME} PUBLIC ${INCDIR})
target_link_libraries(${LIBNAME} PRIVATE sfml-graphics sfml-window sfml-system)
