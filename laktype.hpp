#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <type_traits>
using std::remove_reference;
#include <exception>
using std::exception;

#ifndef funcptr_t
#define funcptr_t uint64_t
#endif

#ifndef LAKTYPE_H
#define LAKTYPE_H

class lak_type_exception : public exception { using exception::exception; };

template<typename T>
T* typeof_f(T t) { return &t; }

template<typename T>
struct typeof 
{
    funcptr_t type = (funcptr_t)(T*(*)(T t))typeof_f<T>;
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
    template<typename T> bool operator==(const typeof<T>& tpo){ return type == tpo.type; }
    template<typename T> bool operator==(typeof<T>&& tpo){ return type == tpo.type; }
    template<typename T> bool operator!=(const typeof<T>& tpo){ return type != tpo.type; }
    template<typename T> bool operator!=(typeof<T>&& tpo){ return type != tpo.type; }
    bool operator==(const lakType& other){ return type == other.type; }
    bool operator==(lakType&& other){ return type == other.type; }
    bool operator!=(const lakType& other){ return type != other.type; }
    bool operator!=(lakType&& other){ return type != other.type; }
    template<typename T>
    lakType& operator=(const T& t)
    {
        type = (funcptr_t)typeof<remove_reference<T>::type>().type;
        value = make_shared<remove_reference<T>::type>();
        remove_reference<T>::type& ot = *(remove_reference<T>::type*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    lakType& operator=(T&& t)
    {
        type = (funcptr_t)typeof<remove_reference<T>::type>().type;
        value = make_shared<remove_reference<T>::type>();
        remove_reference<T>::type& ot = *(remove_reference<T>::type*)value.get();
        ot = t;
        return *this;
    }
    template<typename T>
    T get()
    {
        return *(remove_reference<T>::type*)value.get();
    }
    template<typename T>
    T getStrict()
    {
        if (type == (funcptr_t)typeof<remove_reference<T>::type>().type) return get<T>();
        else throw lak_type_exception();
    }
};

#endif