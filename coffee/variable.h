#ifndef __variable_h_
#define __variable_h_
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
}
#endif