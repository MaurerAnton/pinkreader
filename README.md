# PinkReader

Android Reddit client written in C++20 / Qt6. A native, privacy-respecting alternative to the official Reddit app, inspired by [RedReader](https://github.com/QuantumBadger/RedReader).

<p align="center">
  <img src="https://img.shields.io/badge/language-C%2B%2B20-blue" alt="C++20">
  <img src="https://img.shields.io/badge/framework-Qt%206.6-red" alt="Qt 6.6">
  <img src="https://img.shields.io/badge/platform-Android-green" alt="Android">
  <img src="https://github.com/MaurerAnton/pinkreader/actions/workflows/build-android.yml/badge.svg" alt="Android Build">
</p>

## Features

- **Multi-strategy API** — OAuth JSON, anonymous `.json`, RSS feeds, alternative frontends (17 Teddit/Libreddit/Redlib instances), HTML scraping
- **Full offline support** — SQLite cache, auto-saves your feed, offline action queue (votes, saves)
- **Media** — image caching with `image://pink/` provider, video player (MediaPlayer + VideoOutput), gallery viewer with swipe & pinch-to-zoom
- **OAuth PKCE login** — secure login via Reddit, vote, save, comment, submit posts
- **Discover subreddits** — search with sorting by subscribers, active users, posts/day
- **Markdown rendering** — bold, italic, code, links, spoilers, tables, blockquotes in comments and self-text
- **Dark / Light theme** — full palette with font size control (S/M/L/XL)
- **Content settings** — NSFW filter, auto-hide read posts, swipe to dismiss
- **Context menus** — long-press on posts/comments: vote, save, hide, copy link, share, report, view profile
- **Infinite scroll + pull-to-refresh**
- **Comment collapse** — tap to collapse/expand comment trees
- **Subreddit sidebar** — about info, rules, subscriber count
- **Multireddit support**
- **Unit tests** — 22 tests covering Markdown parser, JSON parser, models

## Architecture

```
src/
├── main.cpp
├── core/                          # API and business logic
│   ├── models.hpp/cpp             # Post, Comment, Subreddit, Account structs
│   ├── api_routes.hpp            # Reddit API endpoint builders
│   ├── api_strategy.hpp/cpp      # Strategy pattern with fallback chain
│   ├── reddit_client.hpp/cpp     # Main API client
│   ├── json_parser.hpp/cpp       # Reddit JSON deserializer
│   ├── cache_manager.hpp/cpp     # SQLite offline cache
│   ├── image_cache.hpp/cpp       # Image file cache + QQuickImageProvider
│   ├── content_resolver.hpp/cpp  # Media URL resolver (v.redd.it, Imgur, etc.)
│   ├── media_loader.hpp/cpp      # Media type detection and URL extraction
│   ├── markdown_parser.hpp/cpp   # Reddit Markdown → HTML converter
│   ├── oauth_flow.hpp/cpp        # PKCE OAuth2 authorization flow
│   ├── offline_detector.hpp/cpp  # Network connectivity monitor
│   ├── offline_queue.hpp/cpp     # Offline action queue (vote/save/hide)
│   ├── account_manager.hpp/cpp   # Multi-account storage
│   ├── theme_manager.hpp/cpp     # Dark/light palette + font scale
│   └── strategies/               # API strategy implementations
│       ├── oauth_json_strategy   # Authenticated OAuth API
│       ├── anonymous_json_strategy # old.reddit.com/.json
│       ├── rss_strategy          # RSS feed parser
│       ├── alt_frontend_strategy # Teddit/Libreddit proxy
│       └── scrape_strategy       # HTML scraping fallback
├── ui/                            # Qt/C++ models for QML
│   ├── app_controller.hpp/cpp    # Main controller exposed to QML
│   ├── post_list_model.hpp/cpp   # QAbstractListModel for posts
│   ├── comment_tree_model.hpp/cpp # Flattened comment tree model
│   ├── subreddit_list_model.hpp/cpp # Subreddit search with sorting
│   └── ...                       # Account model, etc.
└── android/                       # Android-specific utilities
```

## Building

### Desktop (Linux)

```bash
# Install Qt6
sudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev \
  qt6-multimedia-dev libqt6sql6-sqlite cmake ninja-build

# Build
cmake -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build --parallel $(nproc)

# Run tests
ctest --test-dir build --output-on-failure

# Run the app
./build/pinkreader
```

### Android APK

```bash
# Requirements: Qt 6.6+ for Android, Android NDK 26+, SDK 34

python3 build.py build    # Build for arm64-v8a
python3 build.py deploy   # Install to connected device via ADB
```

Or use the pre-built APK from [GitHub Releases](https://github.com/MaurerAnton/pinkreader/releases).

## API Strategy Chain

When fetching content, PinkReader tries strategies in order until one succeeds:

| Priority | Strategy | Auth | Description |
|----------|----------|------|-------------|
| 10 | OAuth JSON | Yes | Full API access: voting, saving, commenting |
| 20 | Anonymous JSON | No | `old.reddit.com/.json` — most reliable |
| 30 | RSS Feeds | No | `reddit.com/.rss` — fast but limited |
| 40 | Alt Frontends | No | 17 Teddit/Libreddit/Redlib instances |
| 50 | HTML Scraping | No | Parse old.reddit.com HTML (last resort) |

For authenticated actions (vote, save, report), only the OAuth strategy is used.

## License

This project is in early development. License TBD.
