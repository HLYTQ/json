#include "json.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace myJson;

int main(int argc, char *argv[]) {
  std::ifstream fin("/home/durandal/Programmer/compile/json/test.json");
  if (!fin.is_open()) {
    std::cout << "not open file.\n";
    return 0;
  }
  std::stringstream ss;
  ss << fin.rdbuf();

  std::string str = ss.str();

  Json v;
  v.parse(str);
  std::string ver = v["version"];
  std::cout << ver << '\n';
  for (auto it = v["myarr"].begin(); it != v["myarr"].end(); ++it) {
    std::cout << it->asInt() << '\n';
  }
  return 0;
}
