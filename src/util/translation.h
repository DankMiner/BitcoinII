// Copyright (c) 2019-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOINII_UTIL_TRANSLATION_H
#define BITCOINII_UTIL_TRANSLATION_H

#include <tinyformat.h>

#include <functional>
#include <string>

/**
 * Bilingual messages:
 *   - in GUI: user's native language + untranslated (i.e. English)
 *   - in log and stderr: untranslated only
 */
struct bilingual_str {
    std::string original;
    std::string translated;

    bilingual_str& operator+=(const bilingual_str& rhs)
    {
        original += rhs.original;
        translated += rhs.translated;
        return *this;
    }

    bool empty() const
    {
        return original.empty();
    }

    void clear()
    {
        original.clear();
        translated.clear();
    }
};

inline bilingual_str operator+(bilingual_str lhs, const bilingual_str& rhs)
{
    lhs += rhs;
    return lhs;
}

/** Mark a bilingual_str as untranslated */
inline bilingual_str Untranslated(std::string original) { return {original, original}; }

// Provide an overload of tinyformat::format which can take bilingual_str arguments.
namespace tinyformat {
template <typename... Args>
bilingual_str format(const bilingual_str& fmt, const Args&... args)
{
    const auto translate_arg{[](const auto& arg, bool translated) -> const auto& {
        if constexpr (std::is_same_v<decltype(arg), const bilingual_str&>) {
            return translated ? arg.translated : arg.original;
        } else {
            return arg;
        }
    }};
    return bilingual_str{tfm::format(fmt.original, translate_arg(args, false)...),
                         tfm::format(fmt.translated, translate_arg(args, true)...)};
}
} // namespace tinyformat

/** Translate a message to the native language of the user. */
const extern std::function<std::string(const char*)> G_TRANSLATION_FUN;

/**
 * Translation function.
 * If no translation function is set, simply return the input.
 */
inline bilingual_str _(const char* psz)
{
    return bilingual_str{psz, G_TRANSLATION_FUN ? (G_TRANSLATION_FUN)(psz) : psz};
}

#endif // BITCOINII_UTIL_TRANSLATION_H
