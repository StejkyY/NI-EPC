#ifndef EPC_VECTOR_H
#define EPC_VECTOR_H

#include <cstdlib>
#include <iostream>

namespace epc
{
    template <typename T>
    class vector
    {
    public:
        vector() noexcept {
            v_capacity = 0;
            v_size = 0;
            array = nullptr;
        }

        vector(const vector&) = delete;
        vector& operator=(const vector&) = delete;

        ~vector() {
            delete[] array;
        }

        T* data() {
            if ( v_size == 0 ) return nullptr;
            return &array[0];
        }

        const T* data() const {
            if ( v_size == 0 ) return nullptr;
            return &array[0];
        }

        T& operator[](size_t i) {
            return array[i];
        }
        const T& operator[](size_t i) const {
            return array[i];
        }

        void push_back(const T& value) {
            if ( v_capacity > 0)
            {
                if ( v_size == v_capacity )
                {
                    v_capacity *= 2;
                    T* tmp = new T[v_capacity];
                    std::copy ( array, array + v_size, tmp);
                    delete[] array;
                    array = tmp;
                }
                array[v_size++] = value;
            }
            else
            {
                array = new T[1];
                array[0] = value;
                v_size = 1;
                v_capacity = 1;
            }

        }

        size_t capacity() const {
            return v_capacity;
        }

        size_t size() const {
            return v_size;
        }

    private:
        size_t v_capacity;
        size_t v_size;
        T* array;
    };
}

#endif