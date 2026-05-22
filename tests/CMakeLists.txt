find_package(Qt6 REQUIRED COMPONENTS Test)

set(TEST_SOURCES
    ../src/core/json_parser.cpp
    ../src/core/models.cpp
    ../src/core/markdown_parser.cpp
)

# Markdown parser tests
add_executable(test_markdown test_markdown.cpp ${TEST_SOURCES})
target_include_directories(test_markdown PRIVATE ../src)
target_link_libraries(test_markdown PRIVATE Qt6::Test Qt6::Core Qt6::Network)
add_test(NAME test_markdown COMMAND test_markdown)

# JSON parser tests
add_executable(test_json test_json.cpp ${TEST_SOURCES})
target_include_directories(test_json PRIVATE ../src)
target_link_libraries(test_json PRIVATE Qt6::Test Qt6::Core Qt6::Network)
add_test(NAME test_json COMMAND test_json)
