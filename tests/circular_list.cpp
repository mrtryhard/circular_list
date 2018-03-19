#include <iostream>
#include <iterator>
#include "circular_list.hpp"
#include "../../containers/circular_list.hpp"

using namespace mrt::containers;

namespace {
    bool test_front() {
        circular_list<int> list(15); // 15 ints
        list.push(23);
        list.push(41);
        int front = list.front();

        if (front != 41) {
            std::clog << "Front does not return top number" << std::endl;
            return false;
        }

        return true;
    }

    bool test_front_when_popped() {
        circular_list<int> list(4);
        list.push(5);
        list.push(9);
        list.push(3);
        list.pop();

        if (list.front() != 3) {
            std::clog << "Front is not working after pop." << std::endl;
            return false;
        }

        return true;
    }

    bool test_back() {
        circular_list<int> list(15); // 15 ints
        list.push(23);
        list.push(41);
        int back = list.back();

        if (back != 23) {
            std::clog << "Back does not return back number" << std::endl;
            return false;
        }

        return true;
    }

    bool test_back_when_popped() {
        circular_list<int> list(4);
        list.push(5);
        list.push(9);
        list.push(3);
        list.pop();

        if (list.back() != 9) {
            std::clog << "Back does not work when popped" << std::endl;
            return false;
        }

        list.push(8);
        list.push(4);
        list.pop();

        if(list.back() != 3) {
            std::clog << "Back does not work when popped" << std::endl;
            return false;
        }

        return true;
    }

    bool test_empty_init() {
        circular_list<int> list(4);
        
        if (!list.empty()) {
            std::clog << "Circular list is not empty" << std::endl;
            return false;
        }

        return true;
    }

    bool test_empty_after_clear() {
        circular_list<int> list(5);
        list.push(1);
        list.push(2);
        list.push(3);
        list.clear();

        if (!list.empty()) {
            std::clog << "Circular list is not empty after clear" << std::endl;
            return false;
        }

        return true;
    }

    bool test_overwrite() {
        circular_list<int> list(3);
        list.push(1);
        list.push(2);
        list.push(3);
        list.push(4);
        int back = list.back();

        if (back != 2) {
            std::clog << "Circular list back() is not overwritten when exceeded." << std::endl;
            return false;
        }

        return true;
    }

    bool test_full_init_empty() {
        circular_list<int> list(4);
        
        if(list.full()) {
            std::clog << "Circular list is full when just initialized." << std::endl;
            return false;
        }

        return true;
    }

    bool test_full_when_full() {
        circular_list<int> list(4);
        list.push(1);
        list.push(2);
        list.push(3);
        list.push(4);

        if (!list.full()) {
            std::clog << "Circular list is not full when it should be." << std::endl;
            return false;
        }

        return true;
    }

    bool test_full_when_missing_one()
    {
        circular_list<int> list(4);
        list.push(1);
        list.push(2);
        list.push(3);

        if (list.full()) {
            std::clog << "Circular list is full when it should miss one." << std::endl;
            return false;
        }

        return true;
    }

    bool test_size_is_good() {
        circular_list<int> list(4);
        size_t size = list.size();

        if (list.size() != 0) {
            std::clog << "Size is incorrect for list." << std::endl;
            return false;
        }

        list.push(1);
        list.push(2);
        list.push(3);
        size = list.size();

        if (size != 3) {
            std::clog << "Size is incorrect for list." << std::endl;
            return false;
        }
        
        list.push(4);
        list.push(5);
        size = list.size();

        if (size != 4) {
            std::clog << "Size is incorrect for list." << std::endl;
            return false;
        }

        list.pop();
        list.pop();
        size = list.size();

        if (size != 2) {
            std::clog << "Size is incorrect for list." << std::endl;
            return false;
        }

        return true;
    }

    bool test_initializer_list() {
        circular_list<int> list{ 2, 4, 6, 9 };

        if (!list.full()) {
            std::clog << "Circular list initializer list not full" << std::endl;
            return false;
        }

        if (list.size() != 4) {
            std::clog << "Incorrect size for initializer list initialized circular list" << std::endl;
            return false;
        }

        if (list.back() != 2) {
            std::clog << "Incorrect back for circular list initialized with initialized list" << std::endl;
            return false;
        }

        if (list.front() != 9) {
            std::clog << "Incorrect front for circular list initialized with initialized list" << std::endl;
            return false;
        }

        list.push(78);
        auto front = list.front();
        if (front != 78) {
            std::clog << "Incorrect front after overwrite in initializer_list ctor." << std::endl;
            return false;
        }

        if (list.back() != 4) {
            std::clog << "Incorrect back after overwrite in initializer_list ctor." << std::endl;
            return false;
        }

        return true;
    }

    bool test_iterator() {
        circular_list<int> list(3);
        list.push(5);
        list.push(6);
        list.push(8);

        circular_list<int>::iterator it = list.begin();
        if ((*it) != 8) {
            std::clog << "Begin is not returning right value" << std::endl;
            return false;
        }

        it = std::next(it);
        if ((*it) != 6) {
            std::clog << "std::next is not returning right value" << std::endl;
            return false;
        }

        std::advance(it, 1);
        if ((*it) != 5) {
            std::clog << "std::advance is not returning right value" << std::endl;
            return false;
        }

        it = std::next(it);
        circular_list<int>::iterator it_end = list.end();
        if (it != it_end) {
            std::clog << "End iterator does not equal list.end()" << std::endl;
            return false;
        }

        return true;
    }

    bool test_reverse_iterator() {
    circular_list<int> list(3);
        list.push(5);
        list.push(6);
        list.push(8);

        circular_list<int>::reverse_iterator it = list.rbegin();
        if ((*it) != 5) {
            std::clog << "rbegin is not returning right value" << std::endl;
            return false;
        }

        it = std::next(it);
        if ((*it) != 6) {
            std::clog << "std::next is not returning right value" << std::endl;
            return false;
        }

        std::advance(it, 1);
        if ((*it) != 8) {
            std::clog << "std::advance is not returning right value" << std::endl;
            return false;
        }

        it = std::next(it);
        circular_list<int>::reverse_iterator it_end = list.rend();
        if (it != it_end) {
            std::clog << "End iterator does not equal list.rend()" << std::endl;
            return false;
        }

        return true;
    }

    bool test_range() {
        circular_list<int> list(3);

        list.push(1);
        list.push(2);
        list.push(3);

        for (int i : list) {
            if ( i != 1 && i != 2 && i != 3) {
                std::clog << "Forward range for(...) doesn't work properly." << std::endl;
                return false;
            }
        }

        for(auto it = list.crbegin(); it != list.crend(); ++it) {
            if ( *it != 1 && *it != 2 && *it != 3) {
                std::clog << "Reverse iteration doesn't work properly." << std::endl;
                return false;
            }            
        }

        return true;
    }

    bool test_copies() {
        circular_list<int> initial(3);
        initial.push(18);
        initial.push(19);
        initial.push(20);

        circular_list<int> copy_ctor{initial};
        if (!copy_ctor.full()) {
            std::clog << "Copy ctor doesn't work." << std::endl;
            return false;
        }

        if (copy_ctor.front() != initial.front()) {
            std::clog << "Copy ctor doesn't work." << std::endl;
            return false;
        }

        if (copy_ctor.back() != initial.back()) {
            std::clog << "Copy ctor doesn't work." << std::endl;
            return false;
        }

        circular_list<int> copy_op(3);
        copy_op = initial;

        if (!copy_op.full()) {
            std::clog << "Copy operator doesn't work." << std::endl;
            return false;
        }

        if (copy_op.front() != initial.front()) {
            std::clog << "Copy operator doesn't work." << std::endl;
            return false;
        }

        if (copy_op.back() != initial.back()) {
            std::clog << "Copy operator doesn't work." << std::endl;
            return false;
        }

        return true;
    }
}

namespace mrt { namespace tests { namespace circular_list {
    bool execute() noexcept {
        bool success{ true };

        success = success & test_front();
        success = success & test_front_when_popped();
        success = success & test_back();
        success = success & test_back_when_popped();
        success = success & test_empty_init();
        success = success & test_empty_after_clear();
        success = success & test_overwrite();
        success = success & test_full_init_empty();
        success = success & test_full_when_full();
        success = success & test_full_when_missing_one();
        success = success & test_size_is_good();
        success = success & test_initializer_list();
        success = success & test_iterator();
        success = success & test_reverse_iterator();
        success = success & test_range();
        success = success & test_copies();

        return success;
    }
}}}