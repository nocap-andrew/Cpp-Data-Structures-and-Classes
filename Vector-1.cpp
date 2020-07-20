#include <utility>
#include <algorithm>
#include <cstdint>

class Vector {
    using i32 = std::int32_t;

private:
    size_t capacity_{0};
    i32 *data_{nullptr};
    size_t size_{0};

public:
    Vector() = default;
    Vector(const Vector& temp) : size_{temp.size()},
                                 capacity_{temp.size()}, data_{new i32[capacity()]} {
        for (size_t i = 0; i != size(); i++) {
            data_[i] = temp[i];
        }
    }
    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = new i32[other.size()];
        size_ = other.size();
        capacity_ = other.size();
        for (size_t i = 0; i != other.size(); ++i) {
            data_[i] = other[i];
        }
        return *this;
    }
    ~Vector() {
        delete [] data_;
    }
    size_t size() const noexcept {
        return size_;
    }
    size_t capacity() const noexcept {
        return capacity_;
    }
    i32& operator[](size_t pos) {
        return data_[pos];
    }
    const i32& operator[] (size_t pos) const {
        return data_[pos];
    }
    void pop_back() {
        --size_;
    }
    void push_back(const i32& value) {
        if (capacity() == size()) {
            capacity_ = std::max(static_cast<size_t>(1), capacity() * 2);
            i32 *new_data = new i32[capacity()];
            std::copy(data_, data_ + size(), new_data);
            std::swap(data_, new_data);
            delete [] new_data;
        }
        data_[size_++] = value;
    }
};