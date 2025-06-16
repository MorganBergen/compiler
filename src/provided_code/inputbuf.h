/**
 * @file        inputbuf.h
 * @date        sun june 15 2025
 * @author      morgan bergen
 * @brief
 */

#ifndef __INPUT_BUFFER__H__
#define __INPUT_BUFFER__H__

#include <string>
#include <vector>

class InputBuffer {
    public:
        bool EndOfInput();
        void GetChar(char&);
        char UngetChar(char);
        std::string UngetString(std::string);

    private:
        std::vector<char> input_buffer;
};

#endif //  __INPUT_BUFFER__H__
