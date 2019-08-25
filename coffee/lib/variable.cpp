#include "variable.h"

using namespace idb;

template <class T>
T Variable<T>::Get() const {
    if (_forced) {
        return _value;
    }
    else {
        return _forcedValue;
    }
}

template <class T>
void Variable<T>::Set(T value) {
    _value = value;
}

template <class T>
void Variable<T>::Manual(T value) {
    _forcedValue = value;
    _forced = false;
}

template <class T>
void Variable<T>::Auto() const {
    _forcedValue = _value;
    _forced = true;
}

template <class T>
bool Variable<T>::IsAuto() const {
    return _forced;
}