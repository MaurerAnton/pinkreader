/*******************************************************************************
 * This file is part of RedReader.
 *
 * RedReader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RedReader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RedReader.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "reddit_post.h"

namespace PinkReader {

// RedditPost and its inner classes have all methods inline in the header.
//
// Port of the init block (RedditGalleryAPI.addToCache) - this is called
// externally after construction:
//
// void initRedditPost(RedditPost &post) {
//     try {
//         RedditGalleryAPI::addToCache(post);
//     } catch (const std::exception &e) {
//         // Log.e("RedditPost", "Got error when adding post to cache", e)
//     }
// }

} // namespace PinkReader
