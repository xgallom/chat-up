//
// Created by xgallom on 1/28/19.
//

#ifndef CHAT_UP_UI_H
#define CHAT_UP_UI_H

#include <string>
#include <sstream>

struct _win_st;

namespace Ui
{
    enum MessageLevel {
        MessageInfo = 0,
        MessageSuccess,
        MessageError,
        MessagePrimary,

        MessageSize = 4
    };

    enum Alignment {
        AlignStart = 0,
        AlignCenter,
        AlignEnd
    };

    struct Ui {
        Ui();
        ~Ui();
    };

    struct Window {
        _win_st *handle;
        const MessageLevel messageLevel;
        const int width, height;

        Window(int x, int y, int w, int h, const std::string_view &title, MessageLevel level);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        Window(Window &&o) noexcept;
        Window &operator=(Window &&) noexcept = default;

        static Window Center(int w, int h, const std::string_view &title, MessageLevel level);
        static Window FlatHorizontal(Alignment vertical, int h, const std::string_view &title, MessageLevel level);
        static Window MessagePrompt(
                const std::string_view &prompt,
                const std::string_view &title,
                MessageLevel level = MessageInfo);


        void move(int x, int y) noexcept;
        void rmove(int dx, int dy) noexcept;

        void print(const std::string_view &str) noexcept;

        void setTextLevel(MessageLevel level) noexcept;

        void refresh() noexcept;

        bool readStringAsync(std::string &string, size_t maxLen, bool secure = false) noexcept;
        std::string readString(size_t maxLen, bool secure = false) noexcept;
    };

    std::string PopupInput(
            const std::string_view &prompt,
            size_t maxLen,
            const std::string_view &title,
            bool secure = false,
            MessageLevel level = MessageInfo
    );

    void MessagePrompt(
            const std::string_view &prompt,
            const std::string_view &title,
            MessageLevel level = MessageInfo
    );

    void refresh() noexcept;

    void setCursor(bool visible) noexcept;

    struct OutStream {
        OutStream &print(const char str[]) noexcept;

        template<typename T>
        friend OutStream &operator<<(OutStream &os, const T &t)
        {
            std::stringstream ss;
            ss << t;

            return os.print(ss.str().c_str());
        }
    };

    OutStream &Log() noexcept;
}


#endif //CHAT_UP_UI_H
