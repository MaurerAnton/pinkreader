# Changelog

All notable changes to PinkReader will be documented in this file.

## [1.0.0-alpha] - 2024-01-15

### Added
- Initial release of PinkReader, a C++ port of RedReader
- Complete Reddit browsing functionality
- View front page, /r/all, /r/popular, and custom subreddits
- Post sorting (Hot, New, Top, Rising, Controversial, Best)
- Full comment thread viewing with indented threading
- Comment sorting and collapse/expand
- Upvote/downvote with score display
- Save, hide, report, edit, and delete actions
- Inbox with message filtering
- Private message composition
- Modmail viewing
- Built-in OpenGL ES image viewer with zoom and pan
- Video player with playback controls
- GIF player with speed control
- Image gallery/album viewer
- Reddit gallery multi-image support
- Imgur API v3 integration
- Reddit video (v.redd.it) support
- Multiple account support
- OAuth 2.0 authentication
- 6 built-in themes
- Material Design 3 interface
- Font scaling
- Two-pane tablet mode
- Left-handed mode
- Customizable swipe actions
- Cache management with SQLite + disk cache
- Image and comment precaching
- Cache compression using Zstd
- 31 language translations
- Accessibility features
- Tor/Orbot support
- GitHub Actions CI pipeline
- APK builds for all Android architectures

### Technical Details
- Written in C++20 using Qt 6.7
- 355+ source files
- 22 data model classes
- 12 network/API classes
- 9 image host API integrations
- 15 cache management classes
- 30 UI view classes
- 39 custom widget classes
- 10 data adapter classes
- 12 settings management classes
- 20 utility classes
- 14 unit test files
