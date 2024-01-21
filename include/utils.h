#pragma once

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>

#define STD_HASH_SHARED_PTR(T)                                   \
    namespace std                                                \
    {                                                            \
        template <>                                              \
        struct hash<std::shared_ptr<T>>                          \
        {                                                        \
            size_t operator()(const std::shared_ptr<T> &e) const \
            {                                                    \
                return std::hash<T>{}(*e);                       \
            }                                                    \
        };                                                       \
    }

#define STD_EQUAL_TO_SHARED_PTR(T)                                                            \
    namespace std                                                                             \
    {                                                                                         \
        template <>                                                                           \
        struct equal_to<std::shared_ptr<T>>                                                   \
        {                                                                                     \
            size_t operator()(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) const \
            {                                                                                 \
                return *l == *r;                                                              \
            }                                                                                 \
        };                                                                                    \
    }

#define STD_LESS_SHARED_PTR(T)                                                                \
    namespace std                                                                             \
    {                                                                                         \
        template <>                                                                           \
        struct less<std::shared_ptr<T>>                                                       \
        {                                                                                     \
            size_t operator()(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) const \
            {                                                                                 \
                return *l < *r;                                                               \
            }                                                                                 \
        };                                                                                    \
    }

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
inline void hash_combine(size_t &seed, const T &v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class UTF8StringReverser
{
public:
    static std::string reverse(const std::string &str)
    {
        std::vector<uint32_t> codePoints = utf8ToCodePoints(str);
        std::reverse(codePoints.begin(), codePoints.end());
        return codePointsToUTF8(codePoints);
    }

private:
    static std::vector<uint32_t> utf8ToCodePoints(const std::string &str)
    {
        std::vector<uint32_t> codePoints;
        for (size_t i = 0; i < str.size();)
        {
            uint32_t codePoint;
            size_t numBytes = decodeUTF8(str, i, codePoint);
            if (numBytes == 0)
            {
                throw std::runtime_error("Invalid UTF-8 encoding");
            }
            codePoints.push_back(codePoint);
            i += numBytes;
        }
        return codePoints;
    }

    static size_t decodeUTF8(const std::string &str, size_t index, uint32_t &codePoint)
    {
        if (index >= str.size())
        {
            return 0;
        }

        unsigned char lead = str[index];
        if (lead < 0x80)
        {
            codePoint = lead;
            return 1;
        }
        else if ((lead >> 5) == 0x06)
        { // 110xxxxx
            if (index + 1 >= str.size())
                return 0;
            codePoint = ((lead & 0x1F) << 6) | (str[index + 1] & 0x3F);
            return 2;
        }
        else if ((lead >> 4) == 0x0E)
        { // 1110xxxx
            if (index + 2 >= str.size())
                return 0;
            codePoint = ((lead & 0x0F) << 12) | ((str[index + 1] & 0x3F) << 6) | (str[index + 2] & 0x3F);
            return 3;
        }
        else if ((lead >> 3) == 0x1E)
        { // 11110xxx
            if (index + 3 >= str.size())
                return 0;
            codePoint = ((lead & 0x07) << 18) | ((str[index + 1] & 0x3F) << 12) | ((str[index + 2] & 0x3F) << 6) | (str[index + 3] & 0x3F);
            return 4;
        }

        return 0; // Invalid UTF-8 lead byte
    }

    static std::string codePointsToUTF8(const std::vector<uint32_t> &codePoints)
    {
        std::string utf8;
        for (uint32_t cp : codePoints)
        {
            if (cp < 0x80)
            {
                utf8.push_back(static_cast<char>(cp));
            }
            else if (cp < 0x800)
            {
                utf8.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
                utf8.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            }
            else if (cp < 0x10000)
            {
                utf8.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
                utf8.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            }
            else
            {
                utf8.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
                utf8.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                utf8.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            }
        }
        return utf8;
    }
};
