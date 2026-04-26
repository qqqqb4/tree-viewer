#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Dir {
    fs::path path;
    bool is_shown = 1;  // TODO
    std::vector<Dir> dirs;
    std::vector<std::string> files;
};

auto parse_gitignore(std::vector<std::string>& exclude) -> void {
    exclude.clear();
    std::ifstream file(".gitignore");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }
            std::erase_if(
                line, [](char c) { return c == '*' || c == '/' || c == '\\'; });
            exclude.push_back(line);
        }
    }
    file.close();
    exclude.push_back(".git");
}

auto walk_dir(Dir& dir, std::vector<std::string>& exclude) -> void {
    for (fs::directory_iterator it{dir.path}, end; it != end; it++) {
        bool skip = false;
        for (size_t i = 0; i < exclude.size(); i++) {
            if (it->path().filename().string() == exclude[i]) {
                skip = true;
                break;
            }
        }
        if (skip) {
            continue;
        }
        if (it->is_directory()) {
            dir.dirs.push_back({it->path(), 1, {}, {}});
            walk_dir(dir.dirs.at(dir.dirs.size() - 1), exclude);
        } else {
            dir.files.push_back(it->path().filename().string());
        }
    }
}

auto draw(Dir& dir, std::string prefix) -> void {
    for (size_t i = 0; i < dir.dirs.size(); i++) {
        if (dir.is_shown) {
            if (i == dir.dirs.size() - 1 && dir.files.empty()) {
                std::cout << prefix << "└───\uf4d4"
                          << dir.dirs[i].path.filename().string() << std::endl;
                draw(dir.dirs[i], prefix + "    ");
            } else {
                std::cout << prefix << "├───\uf4d4"
                          << dir.dirs[i].path.filename().string() << std::endl;
                draw(dir.dirs[i], prefix + "│   ");
            }
        } else {
            if (i == dir.dirs.size() - 1 && dir.files.empty()) {
                std::cout << prefix << "└───\uf4d4"
                          << dir.dirs[i].path.filename().string() << std::endl;
            } else {
                std::cout << prefix << "├───\uf4d4"
                          << dir.dirs[i].path.filename().string() << std::endl;
            }
        }
    }
    for (size_t i = 0; i < dir.files.size(); i++) {
        if (i == dir.files.size() - 1) {
            std::cout << prefix << "└───" << dir.files[i] << std::endl;
        } else {
            std::cout << prefix << "├───" << dir.files[i] << std::endl;
        }
    }
}

auto sort(Dir& dir) -> void {
    std::sort(dir.dirs.begin(), dir.dirs.end(),
              [](Dir a, Dir b) { return a.path.string() < b.path.string(); });
    std::sort(dir.files.begin(), dir.files.end());
    for (auto& i : dir.dirs) {
        sort(i);
    }
}

auto run(fs::path& def, std::vector<std::string>& exclude) -> void {
    Dir base = {def, 1, {}, {}};
    system("clear");
    parse_gitignore(exclude);
    walk_dir(base, exclude);
    sort(base);
    std::cout << "\uf4d4" << def.filename().string() << std::endl;
    draw(base, "");
    std::cout << ": " << std::flush;
}

auto main(int argc, char* argv[]) -> int {
    fs::path def = fs::current_path();
    std::vector<std::string> exclude{};
    if (argc > 1) {
        def = argv[1];
    }
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            exclude.push_back(argv[i]);
        }
    }
    run(def, exclude);
    while (1) {
        std::string input;
        if (std::getline(std::cin, input)) {
            for (auto word_it : std::ranges::views::split(input, ' ')) {
                auto word = std::string_view(word_it.begin(), word_it.end());
                if (word == "exit" || word == "quit") {
                    system("clear");
                    return 0;
                }
            }
        }
        run(def, exclude);
    }
    return 0;
}

// TODO
// [-] Interactive TUI? (close/open dirs)
// [-] Search
