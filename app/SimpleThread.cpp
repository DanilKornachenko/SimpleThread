#include <ncurses.h>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <cstdlib>
#include <clocale>

std::mutex mtx;

void full_done(int id, int start, int end) {
    for (int i = start; i != end; i++) {
        attron(COLOR_PAIR(2));
        mvprintw(id, i, "@");
        attroff(COLOR_PAIR(2));
    }
}

void worker(int id, int &progress) {
    auto start = std::chrono::system_clock::now(); // Получаем текущее время
auto s_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count(); // Преобразуем в миллисекунды

    int print_progress = 0;
    for (int i = 0; i < 100; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
        mtx.lock();
        progress++;
        // Обновляем progress bar через ncurses
        print_progress = (i / 5) + 7;
        mvprintw(0, 0, "Progress: %d", progress);
        mvprintw(id + 1, 0, "id: %d [", id);
        attron(COLOR_PAIR(1));
        mvprintw(id + 1, print_progress, "@");
        attroff(COLOR_PAIR(1));
        mvprintw(id + 1, 28, "]");
        if (i >= 99) {
            full_done(id + 1, 7, 28);
        }

        auto now = std::chrono::system_clock::now(); // Получаем текущее время
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        mvprintw(id + 1, 30, "[Time: %d ms]", (int)(milliseconds - s_milliseconds));

        refresh();
        mtx.unlock();
    }
}

int main(void) {
    #ifdef _WIN32
        // Windows
        setlocale(LC_ALL, "Russian");
    #else
        // Linux
        setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Progress
    init_pair(2, COLOR_GREEN, COLOR_GREEN); // Done
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    // ... инициализация ncurses ...
    std::vector<std::thread> threads;
    int progress = 0;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(worker, i, std::ref(progress));
    }
    for (auto &t : threads) {
        t.join();
    }

    mvprintw(12, 2, "Нажмите Enter, чтобы закрыть программу...");
    mvprintw(14, 2, "Powered by ");
    attron(COLOR_PAIR(3));
    mvprintw(14, 13, "Arch linux");
    attroff(COLOR_PAIR(3));

    getch();

    endwin();
    return 0;
}
