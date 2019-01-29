//
// Created by xgallom on 1/28/19.
//

#include "Ui.h"
#include <ncurses.h>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <thread>

namespace Ui
{
    static const int offW = 1, offH = 1;
/*
    static const char borders[3][3][4] = {{"\u250c", "\u2500", "\u2510"},
                                          {"\u2502", " ",      "\u2502"},
                                          {"\u2514", "\u2500", "\u2518"}};
*/
    static const char borders[3][3][4] = {{"\u2590", "\u2588", "\u258b"},
                                          {"\u2502", " ",      "\u2502"},
                                          {"\u2570", "\u2500", "\u256f"}};

    static const char titles[3][4] = {"", "\u2026", ""};
    static int s_normalCursor = 0;

    Ui::Ui()
    {
        setlocale(LC_ALL, "");

        if(!initscr())
            throw std::runtime_error("Failed to initialize Ui");

        if(!has_colors() || start_color() == ERR) {
            endwin();

            throw std::runtime_error("Failed to initialize colors");
        }

        noecho();
        s_normalCursor = curs_set(0);

        init_pair(MessageInfo, COLOR_WHITE, COLOR_BLACK);
        init_pair(MessageSuccess, COLOR_GREEN, COLOR_BLACK);
        init_pair(MessageError, COLOR_RED, COLOR_BLACK);
        init_pair(MessagePrimary, COLOR_BLUE, COLOR_BLACK);

        init_pair(MessageSize | MessageInfo, COLOR_BLACK, COLOR_WHITE);
        init_pair(MessageSize | MessageSuccess, COLOR_BLACK, COLOR_GREEN);
        init_pair(MessageSize | MessageError, COLOR_BLACK, COLOR_RED);
        init_pair(MessageSize | MessagePrimary, COLOR_BLACK, COLOR_BLUE);
    }

    Ui::~Ui()
    {
        nl();
        echo();

        endwin();
    }

    Window::Window(int x, int y, int w, int h, const std::string_view &title, MessageLevel level) :
            handle(newwin(h + (offH << 1), w + (offH << 1), y - offH, x - offW)), messageLevel(level)
            , width(w), height(h)
    {
        if(!handle)
            throw std::runtime_error("Failed to create window");

        keypad(handle, true);
        nodelay(handle, true);

        wattrset(handle, COLOR_PAIR(messageLevel));

        mvwprintw(handle, 0, 0, borders[0][0]);
        for(auto n = 0; n < w; ++n)
            wprintw(handle, borders[0][1]);
        wprintw(handle, borders[0][2]);

        for(auto m = 0; m < h; ++m) {
            mvwprintw(handle, m + offH, 0, borders[1][0]);
            for(auto n = 0; n < w; ++n)
                wprintw(handle, borders[1][1]);
            wprintw(handle, borders[1][2]);
        }

        mvwprintw(handle, h + offH, 0, borders[2][0]);
        for(auto n = 0; n < w; ++n)
            wprintw(handle, borders[2][1]);
        wprintw(handle, borders[2][2]);


        wattron(handle, COLOR_PAIR(messageLevel | MessageSize));

        const auto
                maxWidth = w - 2,
                titleLength = static_cast<int>(title.length()),
                titleWidth = std::min(maxWidth, titleLength);

        mvwprintw(handle, 0, offW, titles[0]);
        wprintw(handle, "%.*s%s", titleWidth, title.cbegin(), titleWidth < titleLength ? titles[1] : "");
        wprintw(handle, titles[2]);

        wattrset(handle, A_NORMAL);

        move(0, 0);

        wrefresh(handle);
    }
    Window::~Window()
    {
        delwin(handle);
    }

    Window::Window(Window &&o) noexcept : handle(nullptr), messageLevel(o.messageLevel)
                                          , width(o.width), height(o.height)
    {
        std::swap(handle, o.handle);
    }

    Window Window::Center(int w, int h, const std::string_view &title, MessageLevel level)
    {
        return Window((COLS - w) >> 1, (LINES - h) >> 1, w, h, title, level);
    }

    Window Window::FlatHorizontal(Alignment vertical, int h, const std::string_view &title, MessageLevel level)
    {
        const auto w = COLS - (offW << 1);
        int y;

        if(h < 0)
            h = LINES + h - (offH << 1);

        switch(vertical) {
            case AlignCenter:
                return Center(w, h, title, level);

            case AlignEnd:
                y = LINES - h - offH;
                break;

            case AlignStart:
            default:
                y = offH;
                break;
        }

        return Window(offW, y, w, h, title, level);
    }

    Window Window::MessagePrompt(const std::string_view &prompt, const std::string_view &title, MessageLevel level)
    {
        const auto w = static_cast<int>(prompt.length() + 4), h = 3;

        auto window = Window::Center(w, h, title, level);

        window.move(2, 1);
        window.setTextLevel(level);
        window.print(prompt);

        return window;
    }

    void Window::move(int x, int y) noexcept
    {
        wmove(handle, y + offH, x + offW);
    }

    void Window::rmove(int dx, int dy) noexcept
    {
        wmove(handle, getcury(handle) + dy, getcurx(handle) + dx);
    }

    void Window::print(const std::string_view &str) noexcept
    {
        wprintw(handle, str.cbegin());
    }

    void Window::setTextLevel(MessageLevel level) noexcept
    {
        wattrset(handle, COLOR_PAIR(level) | A_BOLD);
    }

    void Window::refresh() noexcept
    {
        wrefresh(handle);
    }

    bool Window::readStringAsync(std::string &string, size_t maxLen, bool secure) noexcept
    {
        const int input = wgetch(handle);

        switch(input) {
            case ERR:
                break;

            case KEY_ENTER:
            case KEY_SEND:
            case '\n':
                return true;

            case KEY_BACKSPACE:
                if(!string.empty()) {
                    rmove(-1, 0);
                    waddch(handle, static_cast<chtype>(' '));
                    rmove(-1, 0);

                    string.pop_back();
                }
                break;

            default:
                if(input >= 32 && input < 127 && string.size() < maxLen) {
                    if(secure)
                        wprintw(handle, "\u2022");
                    else
                        waddch(handle, static_cast<chtype>(input));

                    string.push_back(static_cast<char>(input));
                }
                break;
        }

        return false;
    }

    std::string Window::readString(size_t maxLen, bool secure) noexcept
    {
        std::string string;
        string.reserve(maxLen);

        while(!readStringAsync(string, maxLen, secure))
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

        return string;
    }

    std::string PopupInput(
            const std::string_view &prompt,
            size_t maxLen,
            const std::string_view &title,
            bool secure,
            MessageLevel level
    )
    {
        const auto returnValue = ([&prompt, maxLen, &title, secure, level]()->std::string {
            setCursor(true);

            const auto w = static_cast<int>(prompt.length() + maxLen + 4), h = 3;

            auto window = Window::Center(w, h, title, level);

            window.setTextLevel(MessageInfo);

            window.move(2, 1);
            window.print(prompt);
            window.setTextLevel(level);

            return window.readString(maxLen, secure);
        })();

        setCursor(false);

        return returnValue;
    }

    void MessagePrompt(
            const std::string_view &prompt,
            const std::string_view &title,
            MessageLevel level
    )
    {
        auto window = Window::MessagePrompt(prompt, title, level);

        window.readString(0);
    }

    void refresh() noexcept
    {
        wrefresh(stdscr);
    }

    void setCursor(bool visible) noexcept
    {
        curs_set(visible ? s_normalCursor : 0);
    }

    OutStream &OutStream::print(const char str[]) noexcept
    {
        wprintw(stdscr, str);
        refresh();

        return *this;
    }

    OutStream &Log() noexcept
    {
        static OutStream instance = OutStream();

        return instance;
    }
}
