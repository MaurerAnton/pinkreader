# PinkReader - C++ Reddit Client for Android

[![Build APK](https://github.com/pinkreader/pinkreader/actions/workflows/build.yml/badge.svg)](https://github.com/pinkreader/pinkreader/actions/workflows/build.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Qt 6](https://img.shields.io/badge/Qt-6.7-green.svg)](https://www.qt.io/)
[![Android](https://img.shields.io/badge/Android-5.0%2B-brightgreen.svg)](https://developer.android.com/)

PinkReader is an open source Reddit client for Android, written entirely in C++ using the Qt 6 framework. It is a complete rewrite/port of the popular [RedReader](https://github.com/QuantumBadger/RedReader) Android app, preserving every feature while leveraging C++ for maximum performance and efficiency.

## Features

### Core Reddit Browsing
- Browse front page, /r/all, /r/popular, and custom subreddits
- Multireddit viewing support
- Subreddit search with autocomplete
- Multiple post sorting options (Hot, New, Top, Rising, Controversial, Best)
- Date range filtering for top/controversial
- NSFW content support with configurable preferences
- Post flair display and spoiler tag support
- Sticky post highlighting
- Crosspost detection and navigation
- Inline image previews in post list

### Comments & Interaction
- Full comment thread viewing with indented threading
- Multiple comment sorting (Best, Top, New, Controversial, Old, Q&A)
- Collapse/expand comments with tap handling
- Skip between top-level comments
- Load more comments on demand
- Comment context viewing
- In-comment searching
- Comment swipe actions
- Upvote/downvote with score display
- Save, hide, report, edit, and delete actions
- Reply to comments with image upload support
- Distinguished user highlighting (moderator, admin, submitter)

### Inbox & Messaging
- Inbox with unread/replied/sent filtering
- Private message composition and replies
- Modmail viewing
- Configurable new message notifications
- Mark all as read / individual read/unread

### Media Support
- Built-in OpenGL image viewer with pinch-to-zoom and drag pan
- Double-tap-to-zoom with full resolution loading
- Image gallery/album viewer (card, list, grid modes)
- Built-in video player with playback controls
- GIF player with speed control and frame stepping
- Reddit gallery (multi-image) support
- Imgur API v3 integration (albums, images, GIFs)
- Reddit-hosted images (i.redd.it)
- Reddit video (v.redd.it) with audio support
- Streamable.com integration
- DeviantArt support
- Save and share images/videos

### Customization
- 6 built-in themes (Light, Dark, Night, Ultra Black, Blue, Low Contrast Night)
- Material Design 3 interface
- Font scaling for multiple text categories
- Two-column tablet mode
- Bottom toolbar option
- Left-handed mode
- Customizable swipe actions
- Customizable toolbar buttons and menu items
- Navigation bar color customization
- Screen orientation preferences

### Performance
- Advanced cache management with configurable storage
- Image and comment precaching (always, never, WiFi only)
- Cache compression using Zstd
- Automatic cache pruning
- Efficient C++20 memory management
- Hardware-accelerated rendering via OpenGL ES

### Accessibility
- Screen reader optimization
- Concise mode for accessibility
- Comment indent level announcements
- Minimum comment height settings
- Configurable font sizes
- 31 language translations

### Account Management
- Multiple account support
- Quick account switching
- OAuth 2.0 authentication
- Secure token storage

### Privacy & Security
- No ads, no tracking
- No analytics or telemetry
- Tor/Orbot support for anonymous browsing
- Configurable HTTPS preference
- Custom Reddit client ID for self-hosted API access
- Block screenshots option

## Building

### Prerequisites
- Qt 6.7+ with Android support
- Android NDK 27+
- CMake 3.21+
- JDK 17
- Android SDK (API level 35)

### Build Commands

```bash
# Clone the repository
git clone https://github.com/pinkreader/pinkreader.git
cd pinkreader

# Configure for Android
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-35 \
  -DCMAKE_PREFIX_PATH=$QT_DIR/android_arm64_v8a

# Build
cmake --build build --parallel $(nproc)

# The APK will be in build/android-build/
```

### Desktop Build (for development)

```bash
cmake -B build-desktop
cmake --build build-desktop --parallel $(nproc)
./build-desktop/PinkReader
```

## Architecture

PinkReader is structured into these layers:

```
src/
├── core/         - Application lifecycle, constants, versioning
├── models/       - Data models (Post, Comment, Subreddit, User, Message, etc.)
├── network/      - Reddit API client, OAuth, HTTP, image host APIs
├── cache/        - SQLite + disk cache, compression, prefetching
├── ui/           - Views (Post list, Comments, Image viewer, Inbox, etc.)
├── widgets/      - Custom UI widgets (Post card, Comment, Swipe, Markdown, Video, etc.)
├── adapters/     - List adapters and pagination controllers
├── accounts/     - Multi-account management and OAuth tokens
├── settings/     - Preferences, themes, fonts, layout configuration
└── utils/        - Logging, time, markdown, HTML, encryption, etc.
```

## Testing

```bash
# Build and run tests
cmake -B build-tests -DBUILD_TESTS=ON
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

PinkReader is licensed under the GNU General Public License v3.0 - see [LICENSE](LICENSE) for details.

## Acknowledgments

PinkReader is a C++ port of [RedReader](https://github.com/QuantumBadger/RedReader) by QuantumBadger. We gratefully acknowledge the original project's design and feature set that made this port possible.
