cmake_minimum_required(VERSION 3.21)

project(PinkReader
    VERSION 1.0.0
    DESCRIPTION "Open source Reddit client for Android written in C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Multimedia
    MultimediaWidgets
    LinguistTools
    Svg
)

# Android-specific settings
if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE STRING "")
    set(ANDROID_MIN_SDK_VERSION 21)
    set(ANDROID_TARGET_SDK_VERSION 35)
    set(ANDROID_COMPILE_SDK_VERSION 35)
    set(ANDROID_BUILD_TOOLS_VERSION "35.0.0")
endif()

# Source files
set(SOURCES
    # Core
    src/main.cpp
    src/core/application.cpp
    src/core/pinkreader.cpp
    src/core/constants.cpp
    src/core/version.cpp

    # Models (data classes)
    src/models/reddit_post.cpp
    src/models/reddit_comment.cpp
    src/models/reddit_subreddit.cpp
    src/models/reddit_user.cpp
    src/models/reddit_message.cpp
    src/models/reddit_listing.cpp
    src/models/reddit_thing.cpp
    src/models/reddit_more.cpp
    src/models/reddit_vote.cpp
    src/models/reddit_save_state.cpp
    src/models/reddit_media_metadata.cpp
    src/models/reddit_flair.cpp
    src/models/reddit_award.cpp
    src/models/reddit_multireddit.cpp
    src/models/reddit_trophy.cpp
    src/models/reddit_wiki_page.cpp
    src/models/reddit_link_flair.cpp
    src/models/reddit_user_flair.cpp
    src/models/reddit_subreddit_rule.cpp
    src/models/reddit_mod_action.cpp
    src/models/reddit_report.cpp
    src/models/reddit_collection.cpp

    # Network/API
    src/network/reddit_api.cpp
    src/network/reddit_oauth.cpp
    src/network/reddit_requester.cpp
    src/network/http_client.cpp
    src/network/json_parser.cpp
    src/network/rate_limiter.cpp
    src/network/url_builder.cpp
    src/network/response_handler.cpp
    src/network/request_builder.cpp
    src/network/api_endpoints.cpp
    src/network/network_monitor.cpp
    src/network/reddit_error.cpp
    src/network/image_hosts/image_host.cpp
    src/network/image_hosts/imgur_api.cpp
    src/network/image_hosts/reddit_gallery.cpp
    src/network/image_hosts/reddit_video.cpp
    src/network/image_hosts/gfycat_api.cpp
    src/network/image_hosts/redgifs_api.cpp
    src/network/image_hosts/streamable_api.cpp
    src/network/image_hosts/deviantart_api.cpp
    src/network/image_hosts/reddit_image.cpp

    # Cache
    src/cache/cache_manager.cpp
    src/cache/cache_db.cpp
    src/cache/disk_cache.cpp
    src/cache/cache_entry.cpp
    src/cache/cache_downloader.cpp
    src/cache/download_queue.cpp
    src/cache/cache_compression.cpp
    src/cache/cache_pruner.cpp
    src/cache/cache_stats.cpp
    src/cache/cache_request.cpp
    src/cache/cache_writer.cpp
    src/cache/cache_reader.cpp
    src/cache/image_cache.cpp
    src/cache/thumbnail_cache.cpp
    src/cache/precache_manager.cpp

    # UI - Views
    src/ui/main_window.cpp
    src/ui/main_menu.cpp
    src/ui/post_list_view.cpp
    src/ui/comment_view.cpp
    src/ui/image_viewer.cpp
    src/ui/album_viewer.cpp
    src/ui/web_view.cpp
    src/ui/inbox_view.cpp
    src/ui/post_submit_view.cpp
    src/ui/comment_reply_view.cpp
    src/ui/pm_compose_view.cpp
    src/ui/login_dialog.cpp
    src/ui/settings_window.cpp
    src/ui/search_view.cpp
    src/ui/user_profile_view.cpp
    src/ui/changelog_view.cpp
    src/ui/bug_report_view.cpp
    src/ui/link_dispatcher.cpp
    src/ui/two_pane_layout.cpp
    src/ui/subreddit_sidebar.cpp
    src/ui/multireddit_view.cpp
    src/ui/mod_queue_view.cpp
    src/ui/saved_items_view.cpp
    src/ui/history_view.cpp
    src/ui/drafts_view.cpp
    src/ui/about_dialog.cpp
    src/ui/flair_selector.cpp
    src/ui/sort_dialog.cpp
    src/ui/share_dialog.cpp
    src/ui/image_info_dialog.cpp

    # UI - Custom Widgets
    src/widgets/post_card.cpp
    src/widgets/comment_widget.cpp
    src/widgets/inbox_item_widget.cpp
    src/widgets/swipe_container.cpp
    src/widgets/markdown_label.cpp
    src/widgets/html_label.cpp
    src/widgets/linkified_label.cpp
    src/widgets/video_player.cpp
    src/widgets/gif_player.cpp
    src/widgets/loading_spinner.cpp
    src/widgets/bezel_toolbar.cpp
    src/widgets/indent_view.cpp
    src/widgets/flair_label.cpp
    src/widgets/score_widget.cpp
    src/widgets/thumbnail_view.cpp
    src/widgets/subreddit_toolbar.cpp
    src/widgets/search_header.cpp
    src/widgets/load_more_button.cpp
    src/widgets/announcement_banner.cpp
    src/widgets/error_view.cpp
    src/widgets/empty_state_view.cpp
    src/widgets/progress_bar.cpp
    src/widgets/tab_bar.cpp
    src/widgets/bottom_toolbar.cpp
    src/widgets/action_button.cpp
    src/widgets/pull_to_refresh.cpp
    src/widgets/context_menu.cpp
    src/widgets/toast_notification.cpp
    src/widgets/snackbar.cpp
    src/widgets/badge_widget.cpp
    src/widgets/divider_widget.cpp
    src/widgets/avatar_widget.cpp
    src/widgets/collapsible_section.cpp
    src/widgets/sliding_panel.cpp
    src/widgets/navigation_stack.cpp
    src/widgets/theme_switch.cpp
    src/widgets/font_scale_slider.cpp
    src/widgets/color_picker.cpp
    src/widgets/image_upload_button.cpp

    # Adapters
    src/adapters/post_list_adapter.cpp
    src/adapters/comment_list_adapter.cpp
    src/adapters/inbox_adapter.cpp
    src/adapters/listing_controller.cpp
    src/adapters/search_adapter.cpp
    src/adapters/subreddit_list_adapter.cpp
    src/adapters/user_list_adapter.cpp
    src/adapters/multireddit_adapter.cpp
    src/adapters/image_gallery_adapter.cpp
    src/adapters/draft_adapter.cpp

    # Accounts
    src/accounts/account_manager.cpp
    src/accounts/account.cpp
    src/accounts/account_store.cpp
    src/accounts/account_switcher.cpp
    src/accounts/oauth_token_store.cpp

    # Settings/Preferences
    src/settings/preferences.cpp
    src/settings/theme_manager.cpp
    src/settings/settings_serializer.cpp
    src/settings/theme_presets.cpp
    src/settings/font_manager.cpp
    src/settings/language_manager.cpp
    src/settings/layout_manager.cpp
    src/settings/gesture_settings.cpp
    src/settings/cache_settings.cpp
    src/settings/notification_settings.cpp
    src/settings/accessibility_settings.cpp
    src/settings/privacy_settings.cpp

    # Utilities
    src/utils/logging.cpp
    src/utils/time_utils.cpp
    src/utils/string_utils.cpp
    src/utils/url_parser.cpp
    src/utils/markdown_parser.cpp
    src/utils/html_parser.cpp
    src/utils/file_utils.cpp
    src/utils/clipboard.cpp
    src/utils/translation.cpp
    src/utils/network_utils.cpp
    src/utils/image_utils.cpp
    src/utils/color_utils.cpp
    src/utils/math_utils.cpp
    src/utils/encryption.cpp
    src/utils/data_migration.cpp
    src/utils/crash_handler.cpp
    src/utils/performance_tracker.cpp
    src/utils/deep_link_handler.cpp
    src/utils/backup_manager.cpp
    src/utils/intent_handler.cpp
)

set(HEADERS
    # Core
    src/core/application.h
    src/core/pinkreader.h
    src/core/constants.h
    src/core/version.h

    # Models
    src/models/reddit_post.h
    src/models/reddit_comment.h
    src/models/reddit_subreddit.h
    src/models/reddit_user.h
    src/models/reddit_message.h
    src/models/reddit_listing.h
    src/models/reddit_thing.h
    src/models/reddit_more.h
    src/models/reddit_vote.h
    src/models/reddit_save_state.h
    src/models/reddit_media_metadata.h
    src/models/reddit_flair.h
    src/models/reddit_award.h
    src/models/reddit_multireddit.h
    src/models/reddit_trophy.h
    src/models/reddit_wiki_page.h
    src/models/reddit_link_flair.h
    src/models/reddit_user_flair.h
    src/models/reddit_subreddit_rule.h
    src/models/reddit_mod_action.h
    src/models/reddit_report.h
    src/models/reddit_collection.h

    # Network
    src/network/reddit_api.h
    src/network/reddit_oauth.h
    src/network/reddit_requester.h
    src/network/http_client.h
    src/network/json_parser.h
    src/network/rate_limiter.h
    src/network/url_builder.h
    src/network/response_handler.h
    src/network/request_builder.h
    src/network/api_endpoints.h
    src/network/network_monitor.h
    src/network/reddit_error.h
    src/network/image_hosts/image_host.h
    src/network/image_hosts/imgur_api.h
    src/network/image_hosts/reddit_gallery.h
    src/network/image_hosts/reddit_video.h
    src/network/image_hosts/gfycat_api.h
    src/network/image_hosts/redgifs_api.h
    src/network/image_hosts/streamable_api.h
    src/network/image_hosts/deviantart_api.h
    src/network/image_hosts/reddit_image.h

    # Cache
    src/cache/cache_manager.h
    src/cache/cache_db.h
    src/cache/disk_cache.h
    src/cache/cache_entry.h
    src/cache/cache_downloader.h
    src/cache/download_queue.h
    src/cache/cache_compression.h
    src/cache/cache_pruner.h
    src/cache/cache_stats.h
    src/cache/cache_request.h
    src/cache/cache_writer.h
    src/cache/cache_reader.h
    src/cache/image_cache.h
    src/cache/thumbnail_cache.h
    src/cache/precache_manager.h

    # UI
    src/ui/main_window.h
    src/ui/main_menu.h
    src/ui/post_list_view.h
    src/ui/comment_view.h
    src/ui/image_viewer.h
    src/ui/album_viewer.h
    src/ui/web_view.h
    src/ui/inbox_view.h
    src/ui/post_submit_view.h
    src/ui/comment_reply_view.h
    src/ui/pm_compose_view.h
    src/ui/login_dialog.h
    src/ui/settings_window.h
    src/ui/search_view.h
    src/ui/user_profile_view.h
    src/ui/changelog_view.h
    src/ui/bug_report_view.h
    src/ui/link_dispatcher.h
    src/ui/two_pane_layout.h
    src/ui/subreddit_sidebar.h
    src/ui/multireddit_view.h
    src/ui/mod_queue_view.h
    src/ui/saved_items_view.h
    src/ui/history_view.h
    src/ui/drafts_view.h
    src/ui/about_dialog.h
    src/ui/flair_selector.h
    src/ui/sort_dialog.h
    src/ui/share_dialog.h
    src/ui/image_info_dialog.h

    # Widgets
    src/widgets/post_card.h
    src/widgets/comment_widget.h
    src/widgets/inbox_item_widget.h
    src/widgets/swipe_container.h
    src/widgets/markdown_label.h
    src/widgets/html_label.h
    src/widgets/linkified_label.h
    src/widgets/video_player.h
    src/widgets/gif_player.h
    src/widgets/loading_spinner.h
    src/widgets/bezel_toolbar.h
    src/widgets/indent_view.h
    src/widgets/flair_label.h
    src/widgets/score_widget.h
    src/widgets/thumbnail_view.h
    src/widgets/subreddit_toolbar.h
    src/widgets/search_header.h
    src/widgets/load_more_button.h
    src/widgets/announcement_banner.h
    src/widgets/error_view.h
    src/widgets/empty_state_view.h
    src/widgets/progress_bar.h
    src/widgets/tab_bar.h
    src/widgets/bottom_toolbar.h
    src/widgets/action_button.h
    src/widgets/pull_to_refresh.h
    src/widgets/context_menu.h
    src/widgets/toast_notification.h
    src/widgets/snackbar.h
    src/widgets/badge_widget.h
    src/widgets/divider_widget.h
    src/widgets/avatar_widget.h
    src/widgets/collapsible_section.h
    src/widgets/sliding_panel.h
    src/widgets/navigation_stack.h
    src/widgets/theme_switch.h
    src/widgets/font_scale_slider.h
    src/widgets/color_picker.h
    src/widgets/image_upload_button.h

    # Adapters
    src/adapters/post_list_adapter.h
    src/adapters/comment_list_adapter.h
    src/adapters/inbox_adapter.h
    src/adapters/listing_controller.h
    src/adapters/search_adapter.h
    src/adapters/subreddit_list_adapter.h
    src/adapters/user_list_adapter.h
    src/adapters/multireddit_adapter.h
    src/adapters/image_gallery_adapter.h
    src/adapters/draft_adapter.h

    # Accounts
    src/accounts/account_manager.h
    src/accounts/account.h
    src/accounts/account_store.h
    src/accounts/account_switcher.h
    src/accounts/oauth_token_store.h

    # Settings
    src/settings/preferences.h
    src/settings/theme_manager.h
    src/settings/settings_serializer.h
    src/settings/theme_presets.h
    src/settings/font_manager.h
    src/settings/language_manager.h
    src/settings/layout_manager.h
    src/settings/gesture_settings.h
    src/settings/cache_settings.h
    src/settings/notification_settings.h
    src/settings/accessibility_settings.h
    src/settings/privacy_settings.h

    # Utils
    src/utils/logging.h
    src/utils/time_utils.h
    src/utils/string_utils.h
    src/utils/url_parser.h
    src/utils/markdown_parser.h
    src/utils/html_parser.h
    src/utils/file_utils.h
    src/utils/clipboard.h
    src/utils/translation.h
    src/utils/network_utils.h
    src/utils/image_utils.h
    src/utils/color_utils.h
    src/utils/math_utils.h
    src/utils/encryption.h
    src/utils/data_migration.h
    src/utils/crash_handler.h
    src/utils/performance_tracker.h
    src/utils/deep_link_handler.h
    src/utils/backup_manager.h
    src/utils/intent_handler.h
)

set(RESOURCES
    resources/resources.qrc
)

set(TEST_SOURCES
    tests/test_main.cpp
    tests/test_reddit_api.cpp
    tests/test_json_parser.cpp
    tests/test_cache.cpp
    tests/test_models.cpp
    tests/test_markdown.cpp
    tests/test_url_parser.cpp
    tests/test_settings.cpp
    tests/test_oauth.cpp
    tests/test_html_parser.cpp
    tests/test_listing_controller.cpp
    tests/test_account_manager.cpp
    tests/test_cache_compression.cpp
    tests/test_rate_limiter.cpp
    tests/test_image_hosts.cpp
)

# Create the main application library
add_library(pinkreader_lib STATIC
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_include_directories(pinkreader_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(pinkreader_lib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Svg
)

# Main executable
if(NOT ANDROID)
    add_executable(PinkReader ${SOURCES} ${HEADERS} ${RESOURCES})
    target_include_directories(PinkReader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(PinkReader PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Svg
    )
endif()

# Android APK target
if(ANDROID)
    qt_add_executable(PinkReader
        ${SOURCES}
        ${HEADERS}
        ${RESOURCES}
    )
    target_include_directories(PinkReader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(PinkReader PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::Sql
        Qt6::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Svg
    )
endif()

# Tests
option(BUILD_TESTS "Build test suite" ON)
if(BUILD_TESTS AND NOT ANDROID)
    enable_testing()
    find_package(Qt6 REQUIRED COMPONENTS Test)
    add_executable(pinkreader_tests ${TEST_SOURCES})
    target_include_directories(pinkreader_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    target_link_libraries(pinkreader_tests PRIVATE
        Qt6::Core
        Qt6::Test
        Qt6::Network
        Qt6::Sql
        pinkreader_lib
    )
    add_test(NAME pinkreader_tests COMMAND pinkreader_tests)
endif()

# Install
install(TARGETS PinkReader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Generate translations
if(Qt6LinguistTools_FOUND AND NOT ANDROID)
    file(GLOB TS_FILES resources/translations/*.ts)
    qt_add_translations(PinkReader
        TS_FILES ${TS_FILES}
    )
endif()
