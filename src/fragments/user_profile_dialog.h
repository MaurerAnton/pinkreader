// Origin: org/quantumbadger/redreader/fragments/UserProfileDialog.kt
#pragma once

#include <memory>
#include <string>
#include <functional>

namespace PinkReader {

class AppCompatActivity;
class ImageView;
class UriString;

class UserProfileDialog {
public:
    static void show(AppCompatActivity& activity, const std::string& username);

private:
    static void unblockUser(
            AppCompatActivity& activity,
            const std::string& username,
            void* chipBlock,
            void* chipBlocked,
            void* chipUnblock);

    static void unblockUserApiCall(
            AppCompatActivity& activity,
            const std::string& usernameToUnblock,
            const std::string& currentUserFullname,
            void* chipBlock,
            void* chipBlocked,
            void* chipUnblock);

    static void blockUser(
            AppCompatActivity& activity,
            const std::string& username,
            void* chipBlock,
            void* chipBlocked,
            void* chipUnblock,
            AppCompatActivity& context);

    static void launchAndCompleteLogin(
            AppCompatActivity& activity,
            AppCompatActivity& context);

    static void assignUserAvatar(
            const UriString& url,
            ImageView& imageOutput,
            AppCompatActivity& context);
};

} // namespace PinkReader
