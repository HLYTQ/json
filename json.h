/* (2023-1-8) C++ tangzheng
 * A tiny C++ JSON interpreter using modern C++
 * Cpp YES!!!
 */

// This file used to define Json's API
#ifndef JSON_H_
#define JSON_H_

// ( #include "parser.h" ) <== if I do this,complie will give a 
// 'xxclass is not a name type`. Because it cause a loop nesting
// define, and finally lead to the `json.h` can't be truly declared 
// in `parser.h` maybe `#pragma once` can make you life easy

#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sstream>

namespace myJson{

class Json{
public:
    using arrayType     = std::vector<Json>;
    using objectType    = std::map<std::string,Json>;
    using iterator      = arrayType::iterator;
    
public:

    enum Type{
        json_null,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    Json();
    Json(bool value);
    Json(int  value);
    Json(double value);
    Json(const std::string& value);
    Json(std::string&& value) noexcept;
    Json(const char* value);
    Json(const arrayType& value);
    Json(arrayType&& value) noexcept;
    Json(const objectType& value);
    Json(objectType&& value) noexcept;

    // default using deep copy
    // if you want more faster, use std::move()
    Json(const Json& other);
    Json(Json&& other) noexcept;

    operator int();
    operator double();
    operator std::string();
    operator arrayType();
    operator objectType();

    // this function can't use size_t as index type
    // GCC can't ditinguish size_t with char*
    // I don't know why!!!
    Json& operator[](const int index);

    Json& operator[](const std::string& key);
    Json& operator[](const char* key);

    // I give every complex type both copy operation and move operation
    // I don't know whether it is necessary or not, this design make my
    // target file more big. I try through template and perfect forwarding
    // to simplify my code, but I fail.

    Json& operator=(const bool value); 
    Json& operator=(const int value);
    Json& operator=(const double value);

    Json& operator=(const char* value);
    Json& operator=(const std::string& value);
    Json& operator=(std::string&& value) noexcept;

    Json& operator=(const arrayType& value);
    Json& operator=(arrayType&& value) noexcept;

    Json& operator=(const objectType& value);
    Json& operator=(objectType&& value) noexcept;

    Json& operator=(const Json& other);
    Json& operator=(Json&& other) noexcept;

    /* You always need to remember using `const member function`
     * when operator some like `==`, `*`. If someone use you class 
     * type if he make error like this: `if(a*b = c)`,if there is 
     * no const,complie will do nothing, but this error will destory you code.
     */

    const bool operator==(const Json& other) const;
    const bool operator!=(const Json& other) const;

    iterator begin() const
    { return m_value.m_array->begin(); }

    iterator end() const
    { return m_value.m_array->end(); }

    bool isNull() const
    { return m_type == json_null; }

    bool isBool() const
    { return m_type == json_bool; }

    bool isInt() const 
    { return m_type == json_int; }

    bool isDouble() const
    { return m_type == json_double; }

    bool isString() const 
    { return m_type == json_string; }

    bool isArray() const
    { return m_type == json_array; }

    bool isObject() const
    { return m_type == json_object; }

    bool asBool() const {   
        assert(m_type == json_bool); 
        return m_value.m_bool; 
    }

    int asInt() const {
        assert(m_type == json_int);
        return m_value.m_int;
    }

    double asDouble() const {
        assert(m_type == json_double);
        return m_value.m_double;
    }

    std::string asString() const {
        assert(m_type == json_string);
        return *m_value.m_string;
    }

    bool has(const int index);
    bool has(const char* key);
    bool has(const std::string& key);

    void append(const Json& other);
    void append(Json&& other);
    std::string str() const;
    Json copy(const Json& other);
    void clear();

    void remove(const size_t index);
    void remove(const char* key);
    void remove(const std::string& key);

    void parse(const std::string& str);

private:

    // I want use smart pointer to manage memory automaticly
    // but it seem like I should use struct instead of union
    // I have to waste some memories, and I have no idea to improve
    struct value{
        bool                            m_bool = false;
        int                             m_int = 0;
        double                          m_double = 0.0f;
        std::unique_ptr<std::string>    m_string;
        std::unique_ptr<arrayType>      m_array;
        std::unique_ptr<objectType>     m_object;
    };

private:
    Type m_type;
    value m_value;
};

} // end mstd
#endif // _JSON_H_

