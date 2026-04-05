#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

auto run(fs::path def) {
    std::cout << ("\033[2J\033[H");
    for (fs::recursive_directory_iterator it{def}, end; it != end; it++) {
        if (it->path() == it->path().parent_path() / ".git") {
            it.disable_recursion_pending();
            continue;
        }

        for (int i = 0; i < it.depth(); ++i) {
            std::cout << "│    ";
        }

        if (it->is_directory()) {
            std::cout << "├───" + it->path().stem().string() << std::endl;
        } else {
            std::cout << "├───" + it->path().filename().string() << std::endl;
        }
    }
}

auto main() -> int {
    fs::path def = ".";
    run(def);
    while (1) {
        if (std::cin.get()) {
            run(def);
        }
    }
    return 0;
}

// Super minimal (and ugly)
// TODO
// [-] Auto update
// [-] Storing data
// [-] Sorting (by name? by type?)
// [-] Proper drawing
// [-] Ignore options (.gitignore and "." files)
