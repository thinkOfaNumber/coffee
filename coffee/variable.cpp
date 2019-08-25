namespace idb {
    template <class T>
    class Variable {
        private:
        bool _forced;
        T _value;
        T _forcedValue;

        public:
        T Get() const;
        void Set(T value);
        void Manual(T value);
        void Auto() const;
        bool IsAuto() const;
    };

    // can't split template definition from declaration for templates
    template <class T>
    T Variable<T>::Get() const {
        if (_forced) {
            return _forcedValue;
        }
        else {
            return _value;
        }
    }

    template <class T>
    void Variable<T>::Set(T value) {
        _value = value;
    }

    template <class T>
    void Variable<T>::Manual(T value) {
        _forcedValue = value;
        _forced = true;
    }

    template <class T>
    void Variable<T>::Auto() const {
        _forcedValue = _value;
        _forced = false;
    }

    template <class T>
    bool Variable<T>::IsAuto() const {
        return _forced;
    }
}
