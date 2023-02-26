#ifndef EPC_VECTOR
#define EPC_VECTOR

#include <cstdlib>

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

        vector(const vector& vec) {
            try {
                array = new T[vec.v_capacity];
                std::copy ( vec.array, vec.array + vec.v_size, array);
            } catch (...)
            {
                delete [] array;
                throw;
            }
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
            size_t tmpSize = v_size;
            try {
                if ( v_capacity > 0)
                {
                    if ( v_size == v_capacity )
                    {
                        reserve(2*v_capacity);
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
            } catch ( ... )
            {
                v_size = tmpSize;
                throw;
            }


        }

        void swap(vector& other) noexcept {
            T* tmpArray = array;
            size_t tmpSize = v_size;
            size_t tmpCapacity = v_capacity;

            array = other.array;
            other.array = tmpArray;
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
                T * tmp = nullptr;
                try {
                    tmp = new T[new_capacity];
                    std::copy(array, array + v_size, tmp);
                    delete[] array;
                    array = tmp;
                } catch ( ... )
                {
                    delete[] tmp;
                    throw;
                }
                v_capacity = new_capacity;
            }
        }

        void pop_back() {
            if ( v_size > 0 ) v_size--;
        }


    private:
        size_t v_capacity;
        size_t v_size;
        T* array;
    };
}

#endif