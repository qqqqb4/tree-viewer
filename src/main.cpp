#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Dir {
    fs::path path;
    bool is_shown = 1;
    std::vector<Dir> dirs;
    std::vector<std::string> files;
};

auto walk_dir(Dir& dir, std::vector<std::string>& exclude) -> void {
    for (fs::directory_iterator it{dir.path}, end; it != end; it++) {
        bool skip = false;
        for (size_t i = 0; i < exclude.size(); i++) {
            if (it->path().filename().string() == exclude[i]) {
                exclude.erase(exclude.begin() + i);
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

auto draw_spaces(int depth) -> void {
    for (size_t i = 0; i < depth; i++) {
        std::cout << "│   ";
    }
}

auto draw_spaces_last(int depth) -> void {
    std::cout << "    ";
    for (size_t i = 1; i < depth; i++) {
        std::cout << "│   ";
    }
}

auto (*draw_spaces_func)(int depth) -> void = draw_spaces;

auto draw(Dir& dir, int& depth) -> void {
    for (size_t i = 0; i < dir.dirs.size(); i++) {
        draw_spaces_func(depth);
        if (i == dir.dirs.size() - 1 && depth == 0 && dir.files.empty()) {
            std::cout << "└───" << dir.dirs[i].path.filename().string()
                      << std::endl;
            draw_spaces_func = draw_spaces_last;
        } else {
            std::cout << "├───" << dir.dirs[i].path.filename().string()
                      << std::endl;
        }

        draw(dir.dirs[i], ++depth);
        depth--;
    }
    for (size_t i = 0; i < dir.files.size(); i++) {
        draw_spaces_func(depth);
        if (i == dir.files.size() - 1) {
            std::cout << "└───" << dir.files[i] << std::endl;
        } else {
            std::cout << "├───" << dir.files[i] << std::endl;
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

auto run(fs::path def, std::vector<std::string> exclude) -> void {
    std::cout << ("\033[2J\033[H");
    Dir base = {def, 1, {}, {}};
    int depth = 0;

    walk_dir(base, exclude);
    sort(base);
    draw(base, depth);
}

auto main(int argc, char* argv[]) -> int {
    fs::path def = ".";
    std::vector<std::string> exclude{".git"};
    if (argc > 1) {
        def = argv[1];  // TODO remove
    }
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            exclude.push_back(argv[i]);
        }
    }
    run(def, exclude);

    while (1) {
        if (std::cin.get()) {
            run(def, exclude);
        }
    }
    return 0;
}

// TODO
// [x] Storing data
// [x] Sorting (by name)
// [x] Ignore options (input args)
//     [-] .gitignore support
// [x?] Proper drawing ( └─├│ )
// [-] Interactive TUI (close/open dirs)
