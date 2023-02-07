#include "json.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace myJson;

int main(int argc, char *argv[]){
    std::string file_name; 

    for(int i = 0; i < argc ; ++i){
        bool print_flag = false;
        if(strcmp(argv[i], "--print") == 0){
            print_flag = true;
        }else if (strcmp(argv[i], "-p") == 0){
            print_flag = true;
        }else{
            file_name(argv[i])
            std::ifstream fin("./test.json");

            std::stringstream ss;
            ss << fin.rdbuf();

            std::string str = ss.str();

            Json v;
            v.parse(str);
            if(print_flag == true){
                std::cout<<v.str()<<std::endl;
            }
        }
    }

    //std::ifstream fin("./test.json");

    //std::stringstream ss;
    //ss << fin.rdbuf();

    //std::string str = ss.str();

    //Json v;
    //v.parse(str);

    return 0;
}

