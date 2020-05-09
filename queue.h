// queue.h (Queue)

template <typename T> class Queue {  // T is a type to be specified by user
public:
    ~Queue()                        // destructor
    {
        clear();
    }

    bool is_empty() const
    {
        return _front == nullptr;
    }

    void clear()
    {
        while (not is_empty()) {
            pop_front();
        }
    }

    void push_back(const T & object)   // insert object at end of queue
    {
        Item * cur = new Item(object); // get new memory for Item at address cur,
                                       // initialize with object and nullptr
        if (is_empty()) {
            _front = cur;
        }
        else {
            _back->_next = cur;     // p->n is abbreviation for (*p).n
        }
        _back = cur;
    }

    T pop_front()                   // delete and return first object of queue
    {                               // ATTENTION: queue must not be empty!
        Item * cur = _front;
        if (_back == _front) {
            _front = nullptr;
            _back  = nullptr;
        }
        else {
            _front = _front->_next;
        }
        T object = cur->_object;
        delete cur;                 // free memory for 1 Item at address cur
        return object;
    }

private:
    struct Item {                                   // struct is a class where by
        Item(const T & object) : _object(object) {} // default everything is public

        T _object;
        Item * _next = nullptr;    // pointer to the next Item (or nullptr)
    };

    Item * _front = nullptr;       // _front and _back are pointers to
    Item * _back = nullptr;        // variables of type Item, or the
};                                 // nullptr if queue is empty
