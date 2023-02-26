#ifndef EPC_VECTOR
#define EPC_VECTOR

#include <cstdlib>
#include <iostream>

namespace epc
{
   template <typename T, size_t N>
   class vector
   {
      public:
       vector() noexcept {

       }

       vector(const vector& vec) {
           v_capacity = vec.v_capacity;
           v_size = vec.v_size;
           try {
               if (v_size > N) {
                   isSmall = false;
                   v_data = static_cast<T *> (operator new(v_capacity * sizeof(T)));
                   for (int i = 0; i < v_size; ++i) new(v_data + i) T(vec.v_data[i]);
               } else for (int i = 0; i < v_size; ++i) new(&v_buffer[i]) T(vec[i]);
           } catch (...) {
               clear();
               throw;
           }
       }

       vector(vector&& other) {
           swap (other);
       }

       vector& operator=(const vector& vec) {
           if ( this == &vec ) return *this;
           vector tmpVec (vec);
           swap (tmpVec);
           return *this;
       }

       vector& operator=(vector&& other) {
            clear();
            swap(other);
       }

       ~vector() {
           clear();
           operator delete (v_data);
       }

       T* data() {
           if(isSmall) return buffer_ptr();
           return v_data;
       }

       const T* data() const {
           if(isSmall) return buffer_ptr();
           return v_data;
       }

       T& operator[](size_t i) {
           if(isSmall) return *reinterpret_cast<T*>(&v_buffer[i]);
           return *(v_data + i);
       }

       const T& operator[](size_t i) const {
           if(isSmall) return *reinterpret_cast<const T*>(&v_buffer[i]);
           return *(v_data + i);
       }

       void push_back(const T& value) {
           if ( v_size == v_capacity )
           {
               reallocate();
           }
           try {
               if (isSmall) {
                   new(&v_buffer[v_size++]) T(value);
                   return;
               }
               new(v_data + v_size++) T(value);
           } catch (...) {
               pop_back();
               throw;
           }
       }

       void swap(vector& other) {
           if (!isSmall && !other.isSmall) {
               std::swap(v_data, other.v_data);
           } else if (isSmall && other.isSmall) {
               swapSmallBuffers(other);
           } else {
               std::swap(v_data, other.v_data);
               size_t smaller = (isSmall ? v_size : other.v_size );
               for (size_t i = 0; i < smaller; ++i) std::swap (*(buffer_ptr() + i), *(other.buffer_ptr() + i));
               std::swap(isSmall, other.isSmall);
           }
           std::swap(v_size, other.v_size);
           std::swap(v_capacity, other.v_capacity);
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
               T *tmp = nullptr;
               size_t i = 0;
               try {
                   tmp = static_cast<T *> (operator new(new_capacity * sizeof(T)));
                   for (; i < v_size; ++i) new(tmp + i) T(std::move(v_data[i]));
                   operator delete(v_data);
                   v_data = tmp;
                   v_size = i;
               } catch (...) {
                   for (int l = 0; l < i; ++l) {
                       tmp[l].~T();
                   }
                   operator delete (tmp);
                   throw;
               }
               v_capacity = new_capacity;
           }
       }

       void pop_back() {
               --v_size;
               if ((v_size + 1) > N) {
                   v_data[v_size].~T();
                   return;
               }
               if (v_data != nullptr) v_data[v_size].~T();
               if (isSmall) std::destroy_at(buffer_ptr() + v_size);
       }

       void clear() {
           while ( v_size > 0 ) pop_back();
       }

       void clearBuffer() {
           size_t tmpSize = v_size;
           while (tmpSize > 0) std::destroy_at(buffer_ptr() + tmpSize--);
       }

       T* buffer_ptr() {
           return reinterpret_cast<T*>(v_buffer);
       }


       void push_back(T&& value) {
           emplace_back(std::move(value));
       }

       template <typename... Ts>
       void emplace_back(Ts&&... args) {
           if ( v_size == v_capacity )
           {
               reallocate();
           }
           try {
               if (isSmall) {
                   new(&v_buffer[v_size++]) T(std::forward<Ts>(args)...);
                   return;
               }
               new(v_data + v_size++) T(std::forward<Ts>(args)...);
           } catch (...) {
               pop_back();
               throw;
           }
       }

   private:
       void reallocate () {
           if (isSmall)
           {
               v_capacity *= 2;
               size_t i = 0;
               try {
                   v_data = static_cast<T *> (operator new(v_capacity * sizeof(T)));
                   for (; i < v_size; ++i) new(v_data + i) T(std::move(*(buffer_ptr() + i)));
               } catch (...) {
                   for (int l = 0; l < i; ++l) {
                       v_data[l].~T();
                   }
                   throw;
               }
               isSmall = false;
               clearBuffer();
           } else reserve(2 * v_capacity);
       }

       void swapSmallBuffers (vector & other) {
               size_t common = (other.v_size > v_size ? v_size : other.v_size);
               for (size_t i = 0; i < common; ++i) std::swap(*(buffer_ptr() + i), *(other.buffer_ptr() + i));
               if (other.v_size > v_size) {
                   for (int i = other.v_size; i >= (int) v_size; --i) {
                       new(&v_buffer[i]) T(*(other.buffer_ptr() + i));
                       std::destroy_at(other.buffer_ptr() + i);
                   }
               } else if (v_size > other.v_size) {
                   for (int i = v_size - 1; i >= (int) other.v_size; --i) {
                       new(&other.v_buffer[i]) T(*(buffer_ptr() + i));
                       std::destroy_at(buffer_ptr() + i);
                   }
               }
       }

       size_t v_capacity = N;
       size_t v_size = 0;
       T* v_data = nullptr;
       bool isSmall = true;
       std::aligned_storage_t<sizeof(T), alignof(T)> v_buffer [N];
   };
}

#endif
