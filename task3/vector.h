#ifndef EPC_VECTOR
#define EPC_VECTOR

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
            v_data = nullptr;
        }

        vector(const vector& vec) {
            v_data = static_cast<T*> (operator new (vec.v_capacity* sizeof(T)));
            for ( int i = 0; i < vec.v_size; ++i) new (v_data + i) T (vec.v_data[i]);
            v_capacity = vec.v_capacity;
            v_size = vec.v_size;
        }

        vector& operator=(const vector& vec) {
            if ( this == &vec ) return *this;
            vector tmpVec (vec);
            swap (tmpVec);
            return *this;
        }

        ~vector() {
            clear();
            operator delete (v_data);
        }

        T* data() {
            return v_data;
        }

        const T* data() const {
            return v_data;
        }

        T& operator[](size_t i) {
            return *(v_data + i);
        }
        const T& operator[](size_t i) const {
            return *(v_data + i);
        }

        void push_back(const T& value) {
            if ( v_capacity > 0)
            {
                if ( v_size == v_capacity )
                {
                    reserve(2*v_capacity);
                }
                new (v_data + v_size++ ) T (value);
            }
            else
            {
                v_data = static_cast<T*> (operator new ( sizeof(T)));
                new (v_data) T (value);
                v_size = 1;
                v_capacity = 1;
            }
        }

        void swap(vector& other) noexcept {
            T* tmpData = v_data;
            size_t tmpSize = v_size;
            size_t tmpCapacity = v_capacity;

            v_data = other.v_data;
            other.v_data = tmpData;
            v_size = other.v_size;
            v_capacity = other.v_capacity;
            other.v_size = tmpSize;
            other.v_capacity = tmpCapacity;
        }

        size_t capacity() const {
            return v_capacity;
        }

        size_t size() const {
            return v_size;
        }

        void reserve(size_t new_capacity) {
            if ( new_capacity > v_capacity )
            {
                T* tmp = static_cast<T*> (operator new (new_capacity * sizeof(T)));
                size_t i = 0;
                for ( ; i < v_size; ++i) new (tmp + i) T (*(v_data + i));
                clear();
                operator delete(v_data);
                v_data = tmp;
                v_size = i;
                v_capacity = new_capacity;
            }
        }

        void pop_back() {
            if ( v_size > 0 ) v_data[--v_size].~T();
        }

        void clear() {
            while ( v_size > 0 ) pop_back();
        }


        private:
            size_t v_capacity;
            size_t v_size;
            T* v_data;
    };
}

#endif