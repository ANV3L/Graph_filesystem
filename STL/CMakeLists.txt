## ------------------------------ STL TESTS ----------------------------

set(STL_TESTS_SRC
        TESTS/CapacityTests.cpp
        TESTS/LookUpTests.cpp
        TESTS/ElementAccessTests.cpp
        TESTS/ModifiersTests.cpp
        TESTS/ElseTests.cpp
        TESTS/ObserversTests.cpp
        TESTS/IteratorsTests.cpp
        TESTS/constructorsTests.cpp
)

add_library(STL_TEST OBJECT ${STL_TESTS_SRC})

target_include_directories(STL_TEST PRIVATE
        ${STL_DIR}
)

target_link_libraries(STL_TEST PRIVATE coverage)

## ------------------------------ END OF STL TESTS ---------------------