#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <type_traits>
using std::remove_reference_t;
#include <exception>
using std::exception;
#include <cstddef>
using std::nullptr_t;

#ifndef funcptr_t
#define funcptr_t uint64_t
#endif

#ifndef LAKTYPE_H
#define LAKTYPE_H

class lak_type_exception : public exception { using exception::exception; };

struct void_t {};

template<typename T>
void typeof_f(T t){}

template<typename T>
struct typeof 
{
    funcptr_t type = (funcptr_t)(void(*)(T))typeof_f<T>;
    template<typename OT> bool operator==(const OT& other){ return type == other.type; }
    template<typename OT> bool operator==(OT&& other){ return type == other.type; }
    template<funcptr_t> bool operator==(const funcptr_t& other){ return type == other; }
    template<funcptr_t> bool operator==(funcptr_t&& other){ return type == other; }
    template<typename OT> bool operator!=(const OT& other){ return type != other.type; }
    template<typename OT> bool operator!=(OT&& other){ return type != other.type; }
    template<funcptr_t> bool operator!=(const funcptr_t& other){ return type != other; }
    template<funcptr_t> bool operator!=(funcptr_t&& other){ return type != other; }
};

template<typename T>
void nulltypeof_f(){}

template<>
struct typeof<nullptr_t>
{
    funcptr_t type = (funcptr_t)(void(*)())nulltypeof_f<nullptr_t>;
    template<typename OT> bool operator==(const OT& other){ return type == other.type; }
    template<typename OT> bool operator==(OT&& other){ return type == other.type; }
    template<funcptr_t> bool operator==(const funcptr_t& other){ return type == other; }
    template<funcptr_t> bool operator==(funcptr_t&& other){ return type == other; }
    template<typename OT> bool operator!=(const OT& other){ return type != other.type; }
    template<typename OT> bool operator!=(OT&& other){ return type != other.type; }
    template<funcptr_t> bool operator!=(const funcptr_t& other){ return type != other; }
    template<funcptr_t> bool operator!=(funcptr_t&& other){ return type != other; }
};

struct lakType
{
    funcptr_t type;
    shared_ptr<void> value;
    lakType(){}
    lakType(const lakType& other)
    {
        type = other.type;
        value = other.value;
    }
    lakType(lakType&& other)
    {
        type = other.type;
        value = other.value;
    }
    template<typename T>
    lakType(const T& other)
    {
        type = typeof<remove_reference_t<T>>().type;
        value = make_shared<remove_reference_t<T>>();
        remove_reference_t<T>& ot = *(remove_reference_t<T>*)value.get();
        ot = other;
    }
    template<typename T>
    lakType(T&& other)
    {
        type = typeof<remove_reference_t<T>>().type;
        value = make_shared<remove_reference_t<T>>();
        remove_reference_t<T>& ot = *(remove_reference_t<T>*)value.get();
        ot = other;
    }
    lakType& operator=(const lakType& other)
    {
        type = other.type;
        value = other.value;
        return *this;
    }
    lakType& operator=(lakType&& other)
    {
        type = other.type;
        value = other.value;
        return *this;
    }
    template<typename T>
    lakType& set(const T& t)
    {
        type = typeof<remove_reference_t<T>>().type;
        value = make_shared<remove_reference_t<T>>();
        remove_reference_t<T>& ot = *(remove_reference_t<T>*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    lakType& set(T&& t)
    {
        type = typeof<remove_reference_t<T>>().type;
        value = make_shared<remove_reference_t<T>>();
        remove_reference_t<T>& ot = *(remove_reference_t<T>*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    T& get()
    {
        return *(remove_reference_t<T>*)value.get();
    }
    template<typename T>
    T& getStrict()
    {
        if (type == typeof<remove_reference_t<T>>().type) return get<T>();
        else throw lak_type_exception();
    }
    template<typename T> bool operator==(const typeof<T>& tpo){ return type == tpo.type; }
    template<typename T> bool operator==(typeof<T>&& tpo){ return type == tpo.type; }
    template<typename T> bool operator!=(const typeof<T>& tpo){ return type != tpo.type; }
    template<typename T> bool operator!=(typeof<T>&& tpo){ return type != tpo.type; }
    bool operator==(const lakType& other){ return type == other.type; }
    bool operator==(lakType&& other){ return type == other.type; }
    bool operator!=(const lakType& other){ return type != other.type; }
    bool operator!=(lakType&& other){ return type != other.type; }
};

#endif