// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ConfigProviders.kt (lines 1-38)
// Ported from Kotlin object to C++ class with static members

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <mutex>

namespace PinkReader {

// ConfigProviders.kt line 22: object ConfigProviders
class ConfigProviders {
public:
    // line 24-26: fun interface ConfigProvider { fun config(): String }
    using ConfigProvider = std::function<std::string()>;

    // line 28: private val providers: MutableList<ConfigProvider> = mutableListOf()
    // line 31: fun register(provider: ConfigProvider) = providers.add(provider)
    static void registerProvider(ConfigProvider provider) {
        std::lock_guard<std::mutex> lock(sProvidersMutex);
        sProviders.push_back(std::move(provider));
    }

    // line 33: fun apply(action: (ConfigProvider) -> Unit) = providers.forEach(action)
    static void apply(const std::function<void(const ConfigProvider&)>& action) {
        std::lock_guard<std::mutex> lock(sProvidersMutex);
        for (const auto& provider : sProviders) {
            action(provider);
        }
    }

    // line 35-37: fun read(action: (ByteArray) -> Unit) = apply { action(Base64.decode(it.config(), 0)) }
    static void read(const std::function<void(const std::vector<uint8_t>&)>& action) {
        apply([&action](const ConfigProvider& provider) {
            std::string config = provider();
            // action(Base64.decode(it.config(), 0))
            std::vector<uint8_t> decoded = base64Decode(config);
            action(decoded);
        });
    }

private:
    // line 28
    static inline std::vector<ConfigProvider> sProviders;
    static inline std::mutex sProvidersMutex;

    static std::vector<uint8_t> base64Decode(const std::string& data);
};

} // namespace PinkReader
