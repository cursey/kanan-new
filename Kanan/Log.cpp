#include <cstdarg>
#include <memory>
#include <vector>

#include <Windows.h>

#include <imgui.h>
#include <String.hpp>

#include "Kanan.hpp"
#include "Log.hpp"

using namespace std;

namespace kanan {
    struct Log {
    public:
        Log(const string& filepath) 
            : m_buf{},
            m_filter{},
            m_lineOffsets{},
            m_scrollToBottom{ false },
            m_file{}
        {
            m_file.open(filepath);

            if (!m_file.is_open()) {
                error("Failed to open log file: %s!", filepath.c_str());
            }
        }

        void clear() { 
            m_buf.clear(); 
            m_lineOffsets.clear(); 
        }

        void addLog(const string& msg)  {
            auto oldSize = m_buf.size();

            m_buf.appendf("%s\n", msg.c_str());

            for (auto newSize = m_buf.size(); oldSize < newSize; ++oldSize) {
                if (m_buf[oldSize] == '\n') {
                    m_lineOffsets.push_back(oldSize);
                }
            }

            m_scrollToBottom = true;

            if (m_file.is_open()) {
                m_file << msg << endl;
            }
        }

        void draw(const string& title, bool* isOpen) {
            ImGui::SetNextWindowSize(ImVec2{ 500.0f, 400.0f }, ImGuiCond_FirstUseEver);

            if (!ImGui::Begin(title.c_str(), isOpen)) {
                ImGui::End();
                return;
            }

            if (ImGui::Button("Clear")) {
                clear();
            }

            ImGui::SameLine();

            auto copy = ImGui::Button("Copy");

            ImGui::SameLine();
            m_filter.Draw("Filter", -100.0f);
            ImGui::Separator();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (copy) {
                ImGui::LogToClipboard();
            }

            if (m_filter.IsActive()) {
                auto bufBegin = m_buf.begin();
                auto line = bufBegin;

                for (auto lineNum = 0; line != nullptr; ++lineNum) {
                    auto lineEnd = (lineNum < m_lineOffsets.Size) ? bufBegin + m_lineOffsets[lineNum] : nullptr;

                    if (m_filter.PassFilter(line, lineEnd)) {
                        ImGui::TextUnformatted(line, lineEnd);
                    }

                    line = lineEnd && lineEnd[1] ? lineEnd + 1 : nullptr;
                }
            }
            else {
                ImGui::TextUnformatted(m_buf.begin());
            }

            if (m_scrollToBottom) {
                ImGui::SetScrollHereY(1.0f);
            }

            m_scrollToBottom = false;

            ImGui::EndChild();
            ImGui::End();
        }

    private:
        ImGuiTextBuffer m_buf;
        ImGuiTextFilter m_filter;
        ImVector<int> m_lineOffsets; // Index to lines offset
        bool m_scrollToBottom;
        ofstream m_file;
    };

    unique_ptr<Log> g_log{};

    void startLog(const string& filepath) {
        g_log = make_unique<Log>(filepath);
    }

    void log(const string& msg) {
        g_log->addLog(msg);
    }

    void msg(const string& msg) {
        log(msg);

        // Use the real window if we have it.
        HWND wnd{ nullptr };

        if (g_kanan) {
            wnd = g_kanan->getWindow();
        }
        else {
            wnd = GetDesktopWindow();
        }

        MessageBox(wnd, widen(msg).c_str(), L"Kanan", MB_ICONINFORMATION | MB_OK);
    }

    void error(const string& msg) {
        log(msg);

        // Use the real window if we have it.
        HWND wnd{ nullptr };

        if (g_kanan) {
            wnd = g_kanan->getWindow();
        }
        else {
            wnd = GetDesktopWindow();
        }

        MessageBox(wnd, widen(msg).c_str(), L"Kanan Error!", MB_ICONERROR | MB_OK);
    }

    void log(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        log(formatString(format, args));
        va_end(args);
    }

    void msg(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        msg(formatString(format, args));
        va_end(args);
    }

    void error(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        error(formatString(format, args));
        va_end(args);
    }

    void drawLog(bool* isOpen) {
        if (g_log) {
            g_log->draw("Log", isOpen);
        }
    }
}
