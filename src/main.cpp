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
        std::string new_prefix;
        if (i == dir.dirs.size() - 1 && dir.files.empty()) {
            new_prefix = "    ";
            std::cout << prefix << "└───\uf4d4"
                      << dir.dirs[i].path.filename().string() << std::endl;
        } else {
            new_prefix = "│   ";
            std::cout << prefix << "├───\uf4d4"
                      << dir.dirs[i].path.filename().string() << std::endl;
        }
        draw(dir.dirs[i], prefix + new_prefix);
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
    std::system("clear");
    std::cout << ("\033[2J\033[H");
    Dir base = {def, 1, {}, {}};

    walk_dir(base, exclude);
    sort(base);
    std::cout << "\uf4d4" << def.filename().string() << std::endl;
    draw(base, "");
}

auto main(int argc, char* argv[]) -> int {
    fs::path def = fs::current_path();
    std::vector<std::string> exclude{".git"};
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
// [x] Proper drawing ( └─├│ )
// [-] Interactive TUI (close/open dirs)
// [-] Search option
