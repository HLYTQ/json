// tang zheng
// This file implements the functions 

#include "parser.h"
#include <exception>

using myJson::Json;
using parser = myJson::Parser;

void parser::__skip_white_space()
{
    while (m_str[m_idx] == ' '  ||
           m_str[m_idx] == '\n' ||
           m_str[m_idx] == '\r' ||
           m_str[m_idx] == '\t')
    { ++m_idx; }
}

char parser::__get_next_token()
{
    __skip_white_space();
    return m_str[m_idx++];
}

Json parser::parse()
{
    char ch = __get_next_token();
    switch (ch)
    {
    case 'n':
        m_idx--;
        return __parse_null();
    case 't':
    case 'f':
        m_idx--;
        return __parse_bool();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        m_idx--;
        return __parse_number();
    case '"':
        return Json(__parse_string());
    case '[':
        return __parse_array();
    case '{':
        return __parse_object();
    default:
        break;
    }
    throw new std::logic_error("unexpected char!");
}

Json parser::__parse_null()
{
    if(m_str.compare(m_idx,4,"null") == 0){
        m_idx += 4;
    }
    return Json();
}

Json parser::__parse_bool()
{
    if(m_str.compare(m_idx,4,"true") == 0) {
        m_idx += 4;
        return Json(true);

    }else if(m_str.compare(m_idx,5,"false") == 0){
        m_idx += 5;
        return Json(false);
    }
    throw new std::logic_error("parse bool error!");
}

// can't handle number error，I think no one is that stupid
Json parser::__parse_number()
{
    size_t position = m_idx;
    if(m_str[m_idx] == '-')
       { m_idx++; }
    while(m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
       { m_idx++; }
    if(m_str[m_idx] != '.'){
        int i = std::atoi(m_str.c_str() + position);
        return Json(i);
    } 
    m_idx++;
    while(m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
        { m_idx++; }
    
    double f = std::atof(m_str.c_str() + position);
    return Json(f);
}

std::string parser::__parse_string()
{
    std::string out;
    while(true){
        char ch = m_str[m_idx++];
        if(ch == '"')
            break;
        if(ch == '\\'){
            ch = m_str[m_idx++];
            switch (ch) {
                case '\n':
                    out += '\n';
                    break;
                case '\r':
                    out += '\r';
                    break;
                case '\t':
                    out += '\t';
                    break;
                case '\b':
                    out += '\b';
                    break;
                case '\f':
                    out += '\f';
                    break;
                case '"':
                    out += "\\\"";
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case 'u':
                    out += "\\u";
                    for (int i = 0; i < 4; ++i) {
                        out += m_str[m_idx++];
                    }
                    break;
            default:
                break;
            }
        }else out += ch;
    }
    return out;
}

Json parser::__parse_array()
{
    Json arr(Json::json_array);
    char ch = __get_next_token();
    if(ch == ']')
        return arr;
    m_idx--;
    while (true) {
        arr.append(parse());
        ch = __get_next_token();
        if(ch == ']')
            break;
        if(ch != ',')
            throw new std::logic_error(
                    "parser array error!"); 
        // ( m_idx++ ) <= without this my programma works well,but 
        // I don't no where I added one more,which bothers me!!!FUCK!!!
    }
    return arr;
}

Json parser::__parse_object()
{
    Json obj(Json::json_object);
    char ch = __get_next_token();
    if(ch == '}')
        return obj;
    m_idx--;
    while (true) {
        ch = __get_next_token();
        if(ch != '"')
            throw new std::logic_error(
                    "object parse error!");
        std::string key = __parse_string();
        ch = __get_next_token();
        if(ch != ':')
            throw new std::logic_error(
                    "object parse error!");
        obj[key] = parse();
        ch = __get_next_token();
        if(ch == '}') 
            break;
        if(ch != ',')
            throw new std::logic_error(
                    "object parse error!");
        m_idx++;
    }
    return obj;
}
