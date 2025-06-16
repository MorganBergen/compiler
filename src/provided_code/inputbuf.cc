/**
 * @file        inputbuf.cc
 * @date        sun june 15 2025
 * @author      morgan bergen
 * @brief
 */

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cstudio>

#include "inputbuf.h"

bool InputBuffer::EndOfInput() {
    if (!input_buffer.empty())
        return false;
    else
        return std::cin.eof();
}

char InputBuffer::UngetChar(char c) {
    if (c != EOF)
        input_buffer.push_back(c);
    return c;
}

void InputBuffer::GetChar(char& c) {
    if (!input_buffer.empty()) {
        c = input_buffer.back();
        input_buffer.pop_back();
    } else {
        std::cin.get(c);
    }
}

std::string InputBuffer::UngetString(std::string s) {
    for (int i = 0; i < s.size(); i++) {
        input_buffer.push_back(s[s.size()- i - 1]);
    }
    return s;
}
