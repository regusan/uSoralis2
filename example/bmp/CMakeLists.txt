add_executable(usoralis-bmp
    main.cpp
    BmpWriter.hpp
)
target_link_libraries(usoralis-bmp PRIVATE usoralis)

if(BUILD_TESTING)
    add_test(
        NAME bmp-output
        COMMAND usoralis-bmp ${CMAKE_CURRENT_BINARY_DIR}/output.bmp
    )
endif()
