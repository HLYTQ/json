// This file used to parse the text of a json file

#ifndef PARSER_H_
#define PARSER_H_

#include "json.h"
#include <string>


namespace myJson
{

class Parser{
public:

    Parser() = default;

    void load(const std::string& str)
    { m_str = str; }

    Json parse();

private:
    void __skip_white_space();
    char __get_next_token();

    Json            __parse_null();
    Json            __parse_bool();
    Json            __parse_number();
    std::string     __parse_string();
    Json            __parse_array();
    Json            __parse_object();

private:
    std::string m_str = "";
    // type is int, wouldn't too short? Maybe I should use int_64?
    // (2023-1-10) change it !!!
    u_int64_t m_idx = 0;
};

} // end namespace myJson

#endif // end PAESER_H_
