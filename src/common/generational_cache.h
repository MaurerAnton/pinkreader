// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GenerationalCache.java (lines 1-63)
// Ported from Java to C++ template

#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <concepts>

namespace PinkReader {

// GenerationalCache.java line 24: public class GenerationalCache<In extends HasUniqueId, Out>
// Requires In to have a getUniqueId() method returning std::string
template<typename In, typename Out>
class GenerationalCache {
public:
    // line 26: @NonNull private final FunctionOneArgWithReturn<In, Out> mCreator;
    using Creator = std::function<Out(const In&)>;

    // line 32-35: constructor
    explicit GenerationalCache(Creator creator)
        : mCreator(std::move(creator)) {}

    // line 37-57: @NonNull public Out get(@NonNull final In in)
    Out get(const In& in) {
        // line 40: final String uniqueId = in.getUniqueId();
        const std::string& uniqueId = in.getUniqueId();

        // line 42: Out result = mThisGen.get(uniqueId);
        auto it = mThisGen.find(uniqueId);

        // line 44-46: if(result != null) return result;
        if (it != mThisGen.end()) {
            return it->second;
        }

        // line 48: result = mPreviousGen.get(uniqueId);
        it = mPreviousGen.find(uniqueId);

        // line 50-53: if(result == null) { result = mCreator.apply(in); }
        Out result;
        if (it == mPreviousGen.end()) {
            // line 51: result = mCreator.apply(in);
            result = mCreator(in);
        } else {
            result = it->second;
        }

        // line 55: mThisGen.put(uniqueId, result);
        mThisGen[uniqueId] = result;

        // line 56: return result;
        return result;
    }

    // line 59-62: public void nextGeneration()
    void nextGeneration() {
        // line 60: mPreviousGen = mThisGen;
        mPreviousGen = std::move(mThisGen);
        // line 61: mThisGen = new HashMap<>();
        mThisGen.clear();
    }

private:
    // line 26
    Creator mCreator;

    // line 28: @NonNull private HashMap<String, Out> mPreviousGen = new HashMap<>();
    std::unordered_map<std::string, Out> mPreviousGen;

    // line 29: @NonNull private HashMap<String, Out> mThisGen = new HashMap<>();
    std::unordered_map<std::string, Out> mThisGen;
};

} // namespace PinkReader
