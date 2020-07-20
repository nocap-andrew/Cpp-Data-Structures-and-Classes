#include <cstddef>
#include <memory>
#include <utility>
#include <new>

using namespace std;
template<typename T>
struct RawMemory {
    T *buf_ = nullptr;
    size_t capacity_ = 0;

    RawMemory() = default;

    RawMemory(size_t n) {
        buf_ = Allocate(n);
        capacity_ = n;
    }

    RawMemory(const RawMemory &) = delete;

    RawMemory(RawMemory &&other) noexcept {
        Swap(other);
    }

    void Swap(RawMemory &other) noexcept {
        swap(buf_, other.buf_);
        swap(capacity_, other.capacity_);
    }

    static T *Allocate(size_t n) {
        return static_cast<T *>(operator new(n * sizeof(T)));
    }

    static void Deallocate(T *buf) {
        operator delete(buf);
    }

    T *operator+(size_t i) {
        return buf_ + i;
    }

    const T *operator+(size_t i) const {
        return buf_ + i;
    }

    T &operator[](size_t i) {
        return buf_[i];
    }

    const T &operator[](size_t i) const {
        return buf_[i];
    }

    RawMemory &operator=(const RawMemory &) = delete;

    RawMemory &operator=(RawMemory &&other) {
        Swap(other);
        return *this;
    }

    ~RawMemory() {
        Deallocate(buf_);
    }
};

template<typename T>
class Vector {
public:
    Vector() = default;

    Vector(size_t n) : _vect{n} {
        uninitialized_value_construct_n(_vect.buf_, n);
        _size = n;
    }

    Vector(const Vector &other) : _vect{other._size} {
        uninitialized_copy_n(other._vect.buf_, other._size, _vect.buf_);
        _size = other._size;
    }

    void swap(Vector &other) {
        _vect.Swap(other._vect);
        std::swap(_size, other._size);
    }

    Vector(Vector &&other) noexcept {
        Swap(other);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _vect.capacity_;
    }

    const T &operator[](size_t i) const {
        return _vect[i];
    }

    T &operator[](size_t i) {
        return _vect[i];
    }

    Vector &operator=(const Vector &other) {
        if (other._size > _vect.capacity_) {
            Vector tmp(other);
            swap(tmp);
        } else {
            for (size_t i = 0; i < _size && i < other._size; ++i) {
                _vect[i] = other[i];
            }
            if (_size < other._size) {
                uninitialized_copy_n(other._vect.buf_ + _size,
                                     other._size - _size, _vect.buf_ + _size);
            } else if (_size > other._size) {
                destroy_n(_vect.buf_ + other._size, _size - other._size);
            }
            _size = other._size;
        }
        return *this;
    }

    Vector &operator=(Vector &&other) noexcept {
        Swap(other);
        return *this;
    }

    void reserve(size_t n) {
        if (n > _vect.capacity_) {
            RawMemory<T> data2(n);
            uninitialized_move_n(_vect.buf_, _size, data2.buf_);
            destroy_n(_vect.buf_, _size);
            _vect.Swap(data2);
        }
    }

    void resize(size_t n) {
        reserve(n);
        if (_size < n) {
            uninitialized_value_construct_n(_vect + _size, n - _size);
        } else if (_size > n) {
            destroy_n(_vect + n, _size - n);
        }
        _size = n;
    }

    void push_back(const T &elem) {
        if (_size == _vect.capacity_) {
            reserve(_size == 0 ? 1 : _size * 2);
        }
        new(_vect + _size) T(elem);
        ++_size;
    }

    void push_back(T &&elem) {
        if (_size == _vect.capacity_) {
            reserve(_size == 0 ? 1 : _size * 2);
        }
        new(_vect + _size) T(move(elem));
        ++_size;
    }

    void pop_back() {
        destroy_at(_vect + _size - 1);
        --_size;
    }


    template<typename ... Args>
    T &EmplaceBack(Args &&... args) {
        if (_size == _vect.capacity_) {
            reserve(_size == 0 ? 1 : _size * 2);
        }
        auto elem = new(_vect + _size) T(forward<Args>(args)...);
        ++_size;
        return *elem;
    }

    T *data() noexcept {
        return _vect.buf_;
    };

    const T *data() const noexcept {
        return _vect.buf_;
    };

    T *begin() noexcept {
        return _vect.buf_;
    };

    const T *begin() const noexcept {
        return _vect.buf_;
    };

    T *end() noexcept {
        return _vect.buf_ + _size;
    };

    const T *end() const noexcept {
        return _vect.buf_ + _size;
    };

    void clear() {
        std::destroy_n(_vect.buf_, _size);
        _size = 0;
    }

    ~Vector() {
        destroy_n(_vect.buf_, _size);
    }

private:
    static void Construct(void *buf) {
        new(buf) T();
    }

    static void Construct(void *buf, const T &elem) {
        new(buf) T(elem);
    }

    static void Construct(void *buf, T && elem) {
        new(buf) T(move(elem));
    }

    static void Destroy(T *buf) {
        buf->~T();
    }

    RawMemory<T> _vect;
    size_t _size = 0;
};