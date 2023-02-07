// This file implements the functions 

#include "json.h"
#include "parser.h"

using JSON = myJson::Json;


// OK, I should use release() and reset() instread of 
// the badly `*smartpoint`

JSON::Json()
    :m_type(json_null)
    { }

JSON::Json(bool value)
    :m_type(json_bool)
    { m_value.m_bool = value; }

JSON::Json(int value)
    :m_type(json_int)
    { m_value.m_int = value; }

JSON::Json(double value)
    :m_type(json_double)
    { m_value.m_double = value; }

JSON::Json(const std::string& value)
    :m_type(json_string)
    { m_value.m_string = std::make_unique<std::string>(value); }

JSON::Json(std::string&& value) noexcept
    :m_type(json_string)
    { m_value.m_string = std::make_unique<std::string>(std::move(value)); }

JSON::Json(const char* value)
    :m_type(json_string)
    { m_value.m_string = std::make_unique<std::string>(value); }

JSON::Json(const arrayType& value)
    :m_type(json_array)
    {
        m_value.m_array = std::make_unique<arrayType>(value) ;
    }

JSON::Json(arrayType&& value) noexcept
    :m_type(json_array)
    {
        m_value.m_array = std::make_unique<arrayType>(std::move(value));
    }

JSON::Json(const objectType& value)
    :m_type(json_object)
    {
        m_value.m_object = std::make_unique<objectType>(value);
    }

JSON::Json(objectType&& value) noexcept
    :m_type(json_object)
    {
        m_value.m_object = std::make_unique<objectType>(std::move(value));
    }

// this can use Json::copy() to make the code shorter
JSON::Json(const Json& other)
    :m_type(other.m_type)
    {
        switch (other.m_type) {
        
        case json_null: 
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            {
                m_value.m_string = std::make_unique<std::string>(
                                std::string(*other.m_value.m_string)
                        );
                break;
            }
        case json_array:
            m_value.m_array = std::make_unique<arrayType>(
                        arrayType(*other.m_value.m_array)
                    );
            break;
        case json_object:
            m_value.m_object = std::make_unique<objectType>(
                        objectType(*other.m_value.m_object)
                    );
        default:
            break;
        }     
    }

JSON::Json(Json&& other) noexcept
     :m_type(other.m_type)
    {
        switch (other.m_type) {
        
        case json_null: 
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = std::move(other.m_value.m_string);
        case json_array:
            m_value.m_array = std::move(other.m_value.m_array);
            break;
        case json_object:
            m_value.m_object = std::move(other.m_value.m_object);
        default:
            break;
        }     
    }

JSON::operator int()
{
    assert(m_type == json_int);
    return m_value.m_int;
}

JSON::operator double()
{
    assert(m_type == json_double);
    return m_value.m_double;
}

JSON::operator std::string()
{
    assert(m_type == json_string);
    return *(m_value.m_string);
}

JSON::operator arrayType()
{
    assert(m_type == json_array);
    return *(m_value.m_array);
}

JSON::operator objectType()
{
    assert(m_type == json_object);
    return *(m_value.m_object);
}

myJson::Json& 
JSON::operator=(const bool value)
{
    m_type = json_bool;
    m_value.m_int = value;
    return *this;
} 

myJson::Json& 
JSON::operator=(const int value)
{
    m_type = json_int;
    m_value.m_int = value;
    return *this;
}

myJson::Json& 
JSON::operator=(const double value){
    m_type = json_double;
    m_value.m_double = value;
    return *this;
}

myJson::Json& 
JSON::operator=(const char* value)
{
    m_type = json_string;
    m_value.m_string.release();
    m_value.m_string = std::make_unique<std::string>(value);
    return *this;
}

myJson::Json& 
JSON::operator=(const std::string& value)
{
    m_type = json_string;
    m_value.m_string.release();
    m_value.m_string = std::make_unique<std::string>(value);
    return *this;
}

myJson::Json& 
JSON::operator=(std::string&& value) noexcept
{
    m_type = json_string;
    m_value.m_string.release();
    m_value.m_string = std::make_unique<std::string>(std::move(value));
    return *this;
}

myJson::Json& 
JSON::operator=(const arrayType& value)
{
    m_type = json_array;
    m_value.m_array.release();
    m_value.m_array = std::make_unique<arrayType>(value);
    return *this;
}

myJson::Json& 
JSON::operator=(arrayType&& value) noexcept
{
    m_type = json_array;
    m_value.m_array.release();
    m_value.m_array = std::make_unique<arrayType>(std::move(value));
    return *this;
}

myJson::Json& 
JSON::operator=(const objectType& value)
{
    m_type = json_object;
    m_value.m_object.release();
    m_value.m_object = std::make_unique<objectType>(value);
    return *this;
}

myJson::Json& 
JSON::operator=(objectType&& value) noexcept
{
    m_type = json_object;
    m_value.m_object.release();
    m_value.m_object = std::make_unique<objectType>(std::move(value));
    return *this;
}

myJson::Json& 
JSON::operator=(const Json& other)
{
    if(&other == this)
       return *this;
    clear();
    this->copy(other);
    return *this;
}

myJson::Json& 
JSON::operator=(Json&& other) noexcept
{
    if (&other == this)
        return *this;
    clear();
    m_type = other.m_type;
    switch (m_type) {
        
        case json_null: 
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = std::move(other.m_value.m_string);
        case json_array:
            m_value.m_array = std::move(other.m_value.m_array);
            break;
        case json_object:
            m_value.m_object = std::move(other.m_value.m_object);
        default:
            break;
        }     
    return *this; 
}

myJson::Json& 
JSON::operator[](const int index)
{
    if(m_type != json_array){
        m_type = json_array;
        m_value.m_array = std::make_unique<arrayType>();
    }
    int size = m_value.m_array->size();
    if(index >= size){
        for(int i = size;i <= index;i++)
            m_value.m_array->emplace_back(Json());
    }
    return m_value.m_array->at(index);
}

JSON& JSON::operator[](const char* key) 
{
    std::string name = key;
    return (*this)[name];
}

JSON& JSON::operator[](const std::string& key)
{
    if(m_type != json_object){
        m_type = json_object;
        m_value.m_object.release();
        m_value.m_object = std::make_unique<objectType>();
    }
    return (*m_value.m_object)[key];
}

const bool JSON::operator==(const Json& other) const
{
    return this == &other;
}

const bool JSON::operator!=(const Json& other) const
{
    return !(*this == other);
}

bool JSON::has(const int index)
{
    if(m_type != json_array) return false;
    int size = m_value.m_array->size();
    return index >= 0 && index < size;
}

bool JSON::has(const char* key)
{
    return has(std::string(key));
}

bool JSON::has(const std::string& key)
{
    if(m_type != json_object) return false;
    return m_value.m_object->find(key) != 
           m_value.m_object->end();
}

void JSON::append(const Json& other)
{
    if(m_type != json_array){
        m_type = json_array;
        m_value.m_array = std::make_unique<arrayType>();
    }
    (m_value.m_array)->emplace_back(other);
}

void JSON::append(Json&& other)
{
    if(m_type != json_array){
        m_type = json_array;
        m_value.m_array = std::make_unique<arrayType>();
    }
    (m_value.m_array)->emplace_back(std::move(other));
}

// without a check, if the resource was moved than 
// the programmer will wrong
std::string JSON::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
    case json_null:
        ss << "null";
        break;
    case json_bool:
        if (!m_value.m_bool) ss << "false";
        else ss << "true";
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << '\"' << *m_value.m_string << '\"';
        break;
    case json_array:
        ss << '[';
        for(auto it = m_value.m_array->begin();
                    it != m_value.m_array->end();it++)
        {
            if(it != m_value.m_array->begin())
                ss << ',';
            ss << it->str();
        }
        ss << ']';
        break;
    case json_object:
        ss << '{';
        for(auto it = m_value.m_object->begin();
                    it != m_value.m_object->end();it++)
        {
            if(it != m_value.m_object->begin())
                ss << ',';
            ss << '\"' << it->first << '\"' << ':' << it->second.str();
        }
        ss << '}';
        break;
    default:
        break;
    }   
    return ss.str();
}

void JSON::clear()
{
    switch (m_type)
    {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0f;
        break;
    case json_string:
        m_value.m_string.release();
        break;
    case json_array:
        m_value.m_array.release();
        break;
    case json_object:
        m_value.m_object.release();
        break;
    default:
        break;
    }
}

JSON JSON::copy(const Json& other)
{
    clear();
    m_type = other.m_type;
    switch (m_type) {
        
        case json_null: 
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            {
                m_value.m_string = std::make_unique<std::string>(
                                std::string(*other.m_value.m_string)
                        );
                break;
            }
        case json_array:
            m_value.m_array = std::make_unique<arrayType>(
                        arrayType(*other.m_value.m_array)
                    );
            break;
        case json_object:
            m_value.m_object = std::make_unique<objectType>(
                        objectType(*other.m_value.m_object)
                    );
        default:
            break;
    }     
    return *this;
}

void JSON::remove(const size_t index)
{
    if(m_type != json_array) return;
    if(index >= m_value.m_array->size()) 
        return;
    m_value.m_array->at(index).clear();
    m_value.m_array->erase(m_value.m_array->begin() + index);
}

void JSON::remove(const char* key)
{
    remove(std::string(key));
}

void JSON::remove(const std::string& key)
{
    auto it = m_value.m_object->find(key);
    if(it == m_value.m_object->end()) 
        return;
    (*m_value.m_object)[key].clear();
    m_value.m_object->erase(key);
}

void JSON::parse(const std::string& str)
{
    Parser p;
    p.load(str);
    *this = p.parse();
}
