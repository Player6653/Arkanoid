#pragma once
#include <SFML/System/Utf.hpp>
#include <string>
#include <iterator>

// Общая логика ввода имени для таблицы рекордов.
namespace NameEntry {
    const sf::Uint32 BACKSPACE = 8;
    const int MAX_NAME_LENGTH = 12; // в символах.

    // Можно писать по-русски.
    inline bool isAllowedChar(sf::Uint32 unicode)
    {
        bool isAsciiPrintable = unicode > 32 && unicode < 127;
        bool isCyrillic = unicode >= 0x0400 && unicode <= 0x04FF;
        return isAsciiPrintable || isCyrillic;
    }

    inline void popLastUtf8Char(std::string& s)
    {
        if (s.empty()) {
            return;
        }
        std::size_t i = s.size() - 1;
        while (i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) {
            --i;
        }
        s.erase(i);
    }

    // Обрабатывает один sf::Event::TextEntered для поля ввода имени.
    inline void handleTextEntered(sf::Uint32 unicode, std::string& name, int& nameLength)
    {
        if (unicode == BACKSPACE) {
            if (!name.empty()) {
                popLastUtf8Char(name);
                --nameLength;
            }
        }
        else if (isAllowedChar(unicode)) {
            if (nameLength < MAX_NAME_LENGTH) {
                sf::Utf8::encode(unicode, std::back_inserter(name));
                ++nameLength;
            }
        }
    }
}
