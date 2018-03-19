#ifndef MRT_CONTAINERS_CIRCULAR_LIST_HPP_
#define MRT_CONTAINERS_CIRCULAR_LIST_HPP_

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace mrt { namespace containers {

    namespace  {
        template<typename T, typename size_type = std::size_t>
        T* next(T* buffer,  size_type max_size, T* position) noexcept {
	        if (position == buffer + max_size) {
                return buffer;
            } else {
                return position + 1;
            }
        }

        template<typename T, typename size_type = std::size_t>
        T* previous(T* buffer,  size_type max_size, T* position) noexcept {
            if (position == buffer) {
                return buffer + max_size;
            } else {
                return position - 1;
            }
        }
    }

    template<typename U>
    class circular_iterator {
    public:
        using value_type = U;
        using pointer = U*;
        using const_pointer = const U*;
        using reference = U&;
        using const_reference = const U&;
        using iterator_category = std::random_access_iterator_tag;
        using size_type = std::size_t;
        using difference_type = int;
        using my_it = circular_iterator<value_type>;

    public:
        using _Unchecked_type = circular_iterator<U>; // msvc C4996.

    private:
        pointer value;
        pointer base;
        size_type max_size;

    public:
        circular_iterator() = delete;
        explicit circular_iterator(pointer val, pointer buffer, size_type max_size) : value{val}, base{buffer}, max_size{max_size} {}
        circular_iterator(const circular_iterator<value_type>& other) : value{other.value}, base{other.base}, max_size{other.max_size} {}
        
        my_it& operator=(const my_it& other) {
            value = other.value;
            base = other.base;
            max_size = other.max_size;
            return *this;
        }

        reference operator*() noexcept { return *value; }
        const_reference operator*() const noexcept { return *value; }
        bool operator==(const my_it& other) { return value == other.value; }
        bool operator!=(const my_it& other) { return !(*this == other); }
        
        my_it& operator+=(int n) noexcept {
            if (n >= 0) {
                while(n--) value = previous(base, max_size, value);    
            } else {
                while(n++) value = next(base, max_size, value);
            }

            return *this;
        }

        my_it operator+(int n) const noexcept {
            pointer tmp = value;

            if (n >= 0) {
                while(n--) tmp = previous(base, max_size, tmp);    
            } else {
                while(n++) tmp = next(base, max_size, tmp);
            }

            return my_it{ tmp, base, max_size };
        }

        my_it operator-(int n) const noexcept {
            return operator+(-n);
        }

        difference_type operator-(my_it n) const noexcept {
            if (value >= n.value) {
                return value - n.value;
            } else {
                return (value + (max_size + 1) - n.value);
            }
        }

        my_it& operator-=(int n) noexcept {
            return operator+=(-n);
        }

        my_it& operator++(int) {
            value = previous(base, max_size, value);
            return *this;
        }

        my_it& operator++() {
            return operator++(1);
        }

        my_it& operator--(int) {
            value = next(base, max_size, value);
            return *this;
        }

        my_it& operator--() {
            return operator--(1);
        }

        reference operator[](std::size_t n) {
            return *(operator+(n));
        }

        bool operator<(my_it b) const noexcept {
            auto n = (b - *this);
            return n < 0;
        }

        bool operator>(my_it b) const noexcept {
            return b < *this;
        }

        bool operator>=(my_it b) const noexcept {
            return !(*this < b);
        }

        bool operator<=(my_it b) const noexcept {
            return !(*this > b);
        }
    };

    template<typename T>
    class circular_list {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using pointer = value_type*;
        using const_pointer = const pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = circular_iterator<value_type>;
        using const_iterator = const circular_iterator<value_type>;
        using reverse_iterator = std::reverse_iterator<circular_iterator<value_type>>;
        using const_reverse_iterator = const reverse_iterator;

    private:
        size_type max_size;        
        pointer buffer;
        pointer head;
        pointer tail;

    public:
        circular_list() = delete;

        explicit circular_list(size_type max_size) 
            : max_size{max_size}, 
            buffer{new value_type[max_size + 1]},
            head{buffer},
            tail{buffer} 
        {
        }
        
        explicit circular_list(std::initializer_list<value_type> list) 
            : max_size{list.size()},
            buffer{new value_type[list.size() + 1]},
            head{buffer + list.size()},
            tail{buffer}
        {
            try {
                std::copy(std::rbegin(list), std::rend(list), begin());
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        template<typename It>
        circular_list(It first, It last)
            : max_size{std::distance(first, last)} ,
              buffer{new value_type[max_size + 1]},
              head{buffer},
              tail{buffer}
        {
            try {
                std::copy(first, last, std::begin(buffer));
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        circular_list(circular_list&& other) noexcept
            : max_size{other.max_size},
            buffer{other.buffer},
            head{buffer},
            tail{buffer}
        {
            other.buffer = {};
            other.head = {};
            other.tail = {};
            other.max_size = {};
        }

        circular_list(circular_list& other) 
            : max_size{other.max_size},
            buffer{new value_type[max_size + 1]},
            head{buffer + 1},
            tail{buffer}
        {
            try {
                std::copy(std::rbegin(other), std::rend(other), begin());
                head = buffer + max_size;
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        circular_list<value_type>& operator=(circular_list<value_type>& other) {
            if (this == &other) return *this;

            head = buffer + 1;
            std::copy(std::rbegin(other), std::rend(other), begin());
            head = buffer + static_cast<int>((other.head - other.buffer));
            tail = buffer + static_cast<int>((other.tail - other.buffer));

            return *this;
        }
        
        circular_list<value_type>& operator=(circular_list<value_type>&& other) {
            delete [] buffer;
            buffer = other.buffer;
            head = other.head;
            tail = other.tail;
            other.buffer = {};
            other.head = {};
            other.tail = {};
            other.max_size = {};

            return *this;
        }

        ~circular_list() {
            delete [] buffer;
        }

        reference front() noexcept {
            return *previous(buffer, max_size, head);
        }

        const_reference front() const noexcept {
            return *previous(buffer, max_size, head);
        }

        reference back() noexcept {
            return *tail;
        } 

        const_reference back() const noexcept {
            return *tail;
        }

        void pop() {
            tail = next(buffer, max_size, tail);
        }

        void push(const value_type& element) {
            *head = element;
            head = next(buffer, max_size, head);

            if(head == tail) {
		        tail = next(buffer, max_size, tail);
	        }
        }

        void push(value_type&& element) {
            *head = element;
            head = next(buffer, max_size, head);
            
            if(head == tail) {
		        tail = next(buffer, max_size, tail);
	        }
        }

        bool empty() const noexcept {
            return head == tail;
        }

        bool full() const noexcept {
            return next(buffer, max_size, head) == tail;
        }

        void clear() {
            head = buffer;
            tail = buffer;
        }

        size_type size() const noexcept {
            if (head >= tail) { 
                return static_cast<size_type>(head - tail);
            } else {
                return static_cast<size_type>((head + (max_size + 1) - tail));
            }
        }

        iterator begin() noexcept {
            return iterator{ previous(buffer, max_size, head), buffer, max_size };
        }

        reverse_iterator rbegin() noexcept {
            return reverse_iterator{ iterator{ previous(buffer, max_size, tail), buffer, max_size } };
        }

        const_iterator cbegin() const noexcept {
            return const_iterator{ previous(buffer, max_size, head), buffer, max_size };
        }

        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator{ const_iterator{ previous(buffer, max_size, tail), buffer, max_size } };
        }

        iterator end() noexcept {
            return iterator{ previous(buffer, max_size, tail), buffer, max_size };
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator{ iterator{ previous(buffer, max_size, head), buffer, max_size } };
        }

        const_iterator cend() const noexcept {
            return const_iterator{ previous(buffer, max_size, tail), buffer, max_size };
        }

        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator{ const_iterator{ previous(buffer, max_size, head), buffer, max_size } };
        }
    };
}}

#endif
