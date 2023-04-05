#define DJUTIL_NEEDS_ezstr
#define DJUTIL_NEEDS_pathman
#include "./gl45rw/djutil.hpp"


int main(int argc, const char* argv[]){
    std::string cwd = djutil::pathman::get_cwd();
    std::cout << "cwd = " << cwd << "\n";

    std::string searchpath = "base/machines/";

    std::vector<std::string> entries; djutil::pathman::listdir(entries, searchpath);

    for (auto& ent : entries) {
        std::cout << " -> " << ent << "\n";
    }

    return 0;
}
