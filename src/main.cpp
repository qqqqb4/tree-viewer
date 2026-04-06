#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

auto run(fs::path def) -> void {
    std::cout << ("\033[2J\033[H");
    int depth = 0;
    std::string filename{def};
    for (fs::recursive_directory_iterator it{def}, end; it != end; it++) {
        if (it->path() == it->path().parent_path() / ".git") {
            it.disable_recursion_pending();
            continue;
        }

        for (int i = 0; i < depth; i++) {
            std::cout << "│   ";
        }

        if (filename == def) {
            std::cout << filename << std::endl;
            filename = it->path().filename().string();
            depth = it.depth();
            continue;
        }

        if (depth > it.depth()) {
            std::cout << "└───" + filename << std::endl;
        } else {
            std::cout << "├───" + filename << std::endl;
        }

        filename = it->path().filename().string();
        depth = it.depth();
    }
    for (int i = 0; i < depth; i++) {
        std::cout << ".   ";
    }
    std::cout << "└───" + filename << std::endl;
}

auto main(int argc, char* argv[]) -> int {
    fs::path def = ".";
    if (argc > 1) {
        def = argv[1];
    }
    run(def);

    while (1) {
        if (std::cin.get()) {
            run(def);
        }
    }
    return 0;
}

// Super minimal
// TODO
// ?[-] Storing data
// ?[-] Sorting (by name? by type?)
// [?] Proper drawing
// [-] Auto update
// [-] Better input handling
// [-] Ignore options (.gitignore and "." files)
