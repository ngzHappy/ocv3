#ifndef _m_STATIC_STACK__
#define _m_STATIC_STACK__

#include<memory>
#include<type_traits>

template<typename _T_,std::size_t _N_>
class StaticStack {
private:
    /*is was begin-1*/
    unsigned char _m_never_used_space[
        sizeof(std::aligned_storage_t< sizeof(_T_),alignof(_T_) >)];
protected:
    union DataWithoutConstruct {
    public:
        _T_ data[_N_];
        DataWithoutConstruct() {}
        ~DataWithoutConstruct() {}
    };
    constexpr const void * never_used_space() const { return _m_never_used_space; }
    DataWithoutConstruct _memData;
    _T_*_memTop;
    constexpr auto begin() const { return (_T_*)(_memData.data); }
    constexpr auto before_begin()const { return begin()-1; }
public:
    StaticStack() { _memTop=before_begin(); }
    ~StaticStack() {
        for (_T_*p=begin(); p<=(_memTop); ++p) { p->~_T_(); }
    }
public:
    constexpr static auto max_size() { return _N_; }
    bool empty()const { return _memTop<begin(); }
    auto size()const { if (empty()) { return 0; }return 1+(_memTop-begin()); }
    auto & back() { return *_memTop; }
    const auto & back() const { return *_memTop; }
    template<typename ..._U_>
    void push_back(_U_&&...arg) {
        try {
            ++_memTop; ::new(_memTop) _T_(std::forward<_U_>(arg)...);
        }
        catch (...) { --_memTop; throw; }
    }
    void pop_back() {
        if (empty()) { return; }
        auto _about_to_delete=--_memTop; _about_to_delete->~_T_();
    }
public:
    StaticStack(const StaticStack&)=delete;
    StaticStack(StaticStack&&)=delete;
    StaticStack&operator=(const StaticStack&)=delete;
    StaticStack&operator=(StaticStack&&)=delete;
};

#endif

