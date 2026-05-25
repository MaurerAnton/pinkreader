/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: search_listing_header.h - Port of SearchListingHeader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SearchListingHeader.java
 *
 * Original: final class SearchListingHeader extends FrameLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <string>

namespace PinkReader {

// Forward declarations
class Context;
class SearchPostListURL;

// ============================================================================
// SearchListingHeader — port of Java class (Java lines 35-105)
//
// Port of: org.quantumbadger.redreader.views.SearchListingHeader
// Orig extends: FrameLayout
//
// Search form with query + location fields and a search button.
// Every field, method, constant ported exactly.
// ============================================================================

class SearchListingHeader {
public:
    // ========================================================================
    // Constructor (Java lines 42-83)
    //
    // Port of: public SearchListingHeader(final Activity parentActivity,
    //     final SearchPostListURL url)
    // ========================================================================

    SearchListingHeader(Context* parentActivity,
                        const SearchPostListURL& url);

    virtual ~SearchListingHeader();

private:
    // ========================================================================
    // performSearch — port of static method (Java lines 85-104)
    //
    // Port of: private static void performSearch(final Activity parentActivity,
    //     final EditText mLocation, final EditText mQuery)
    // ========================================================================

    static void performSearch(Context* parentActivity,
                              const std::string& location,
                              const std::string& query);

    // ========================================================================
    // Fields — exact match (Java lines 37-40)
    // ========================================================================

    // Port of: final SearchPostListURL mUrl; (Java line 37)
    // Port of: final EditText mQuery; (Java line 38)
    // Port of: final EditText mLocation; (Java line 39)
    // Port of: final Button mSearchButton; (Java line 40)

    SearchPostListURL m_url;
    std::string m_query;
    std::string m_location;
};

} // namespace PinkReader
