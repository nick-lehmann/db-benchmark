#pragma once

template <typename T>
class Filter {
public:
    Filter(unsigned index, T value) : index(index), value(value) {}

    virtual bool match(T cell) const = 0;

public:
    unsigned index;
    T value;
};

template <typename T>
class Equal : public Filter<T> {
    using Filter<T>::Filter;

public:
    bool match(T cell) const override { return cell == this->value; }
};

template <typename T>
class LessThan : public Filter<T> {
    using Filter<T>::Filter;

public:
    bool match(T cell) const override { return cell < this->value; }
};

template <typename T>
class LessEqual : public Filter<T> {
    using Filter<T>::Filter;

public:
    bool match(T cell) const override { return cell <= this->value; }
};

template <typename T>
class GreaterThan : public Filter<T> {
    using Filter<T>::Filter;

public:
    bool match(T cell) const override { return cell > this->value; }
};

template <typename T>
class GreaterEqual : public Filter<T> {
    using Filter<T>::Filter;

public:
    bool match(T cell) const override { return cell >= this->value; }
};
