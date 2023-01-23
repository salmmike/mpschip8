set(INCDIR ${CMAKE_CURRENT_LIST_DIR}/include/)
set(LIBHEADERS
    ${INCDIR}cpu.h
)


set(LIBSOURCES
    ${CMAKE_CURRENT_LIST_DIR}/cpu.cpp
)


include_directories(${INCDIR})
add_library(${LIBNAME} STATIC ${LIBSOURCES})
target_include_directories(${LIBNAME} PUBLIC ${INCDIR})

