cmake_minimum_required(VERSION 3.21)

project(PinkReader
    VERSION 0.1.0
    DESCRIPTION "Android Reddit client in C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 configuration
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    Network
    Sql
    Widgets
    QuickControls2
    Multimedia
)

# Source files - Core layer
set(CORE_SOURCES
    src/core/models.cpp
    src/core/reddit_client.cpp
    src/core/json_parser.cpp
    src/core/cache_manager.cpp
    src/core/api_strategy.cpp
    src/core/strategies/oauth_json_strategy.cpp
    src/core/strategies/anonymous_json_strategy.cpp
    src/core/strategies/rss_strategy.cpp
    src/core/strategies/alt_frontend_strategy.cpp
    src/core/strategies/scrape_strategy.cpp
    src/core/account_manager.cpp
    src/core/content_resolver.cpp
    src/core/image_cache.cpp
    src/core/media_loader.cpp
    src/core/oauth_flow.cpp
    src/core/offline_detector.cpp
    src/core/offline_queue.cpp
    src/core/theme_manager.cpp
    src/core/markdown_parser.cpp
)

set(CORE_HEADERS
    src/core/models.hpp
    src/core/api_routes.hpp
    src/core/reddit_client.hpp
    src/core/json_parser.hpp
    src/core/cache_manager.hpp
    src/core/api_strategy.hpp
    src/core/strategies/oauth_json_strategy.hpp
    src/core/strategies/anonymous_json_strategy.hpp
    src/core/strategies/rss_strategy.hpp
    src/core/strategies/alt_frontend_strategy.hpp
    src/core/strategies/scrape_strategy.hpp
    src/core/account_manager.hpp
    src/core/content_resolver.hpp
    src/core/image_cache.hpp
    src/core/media_loader.hpp
    src/core/oauth_flow.hpp
    src/core/offline_detector.hpp
    src/core/offline_queue.hpp
    src/core/theme_manager.hpp
    src/core/markdown_parser.hpp
)

# Source files - UI layer
set(UI_SOURCES
    src/ui/post_list_model.cpp
    src/ui/comment_tree_model.cpp
    src/ui/app_controller.cpp
    src/ui/subreddit_model.cpp
    src/ui/account_model.cpp
    src/ui/subreddit_list_model.cpp
)

set(UI_HEADERS
    src/ui/post_list_model.hpp
    src/ui/comment_tree_model.hpp
    src/ui/app_controller.hpp
    src/ui/subreddit_model.hpp
    src/ui/account_model.hpp
    src/ui/subreddit_list_model.hpp
)

# QML resources
qt_add_qml_module(app
    URI "PinkReader"
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/pages/FeedPage.qml
        qml/pages/PostDetailPage.qml
        qml/pages/SubredditPage.qml
        qml/pages/SettingsPage.qml
        qml/pages/LoginPage.qml
        qml/pages/AccountsPage.qml
        qml/pages/DiscoverPage.qml
        qml/pages/ProfilePage.qml
        qml/pages/SubmitPage.qml
        qml/components/PostCard.qml
        qml/components/CommentItem.qml
        qml/components/MediaViewer.qml
        qml/components/LoadingIndicator.qml
        qml/components/ErrorBanner.qml
        qml/components/VoteButtons.qml
        qml/components/SubredditChip.qml
        qml/components/AsyncImage.qml
        qml/components/GalleryView.qml
        qml/components/VideoPlayer.qml
        qml/components/SubredditSidebar.qml
)

# Android resources
if(ANDROID)
    set(ANDROID_SOURCES
        src/android/platform_utils.cpp
    )
    set(ANDROID_HEADERS
        src/android/platform_utils.hpp
    )
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android" CACHE STRING "")
endif()

# Main executable
qt_add_executable(pinkreader
    src/main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${ANDROID_SOURCES}
    ${ANDROID_HEADERS}
)

target_include_directories(pinkreader PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(pinkreader PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::Sql
    Qt6::Widgets
    Qt6::QuickControls2
    Qt6::Multimedia
)

# Platform-specific settings
if(ANDROID)
    target_link_libraries(pinkreader PRIVATE
        Qt6::CorePrivate
    )
endif()

# Install
install(TARGETS pinkreader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Tests (optional, desktop only)
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS AND NOT ANDROID)
    enable_testing()
    add_subdirectory(tests)
endif()
