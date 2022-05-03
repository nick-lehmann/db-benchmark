#pragma once

template <typename Data>
class Filter {
 public:
  Filter(int index, Data value) {
    this->index = index;
    this->value = value;
  };
  virtual bool match(Data *cell) = 0;

  int index;
  Data value;
};

template <typename Data>
class Equal : public Filter<Data> {
  using Filter<Data>::Filter;

 public:
  bool match(Data *cell) { return *cell == this.value; }
};

template <typename Data>
class LessThan : public Filter<Data> {
  using Filter<Data>::Filter;

 public:
  bool match(Data *cell) { return *cell < this.value; }
};

template <typename Data>
class LessEqual : public Filter<Data> {
  using Filter<Data>::Filter;

 public:
  bool match(Data *cell) { return *cell <= this.value; }
};

template <typename Data>
class GreaterThan : public Filter<Data> {
  using Filter<Data>::Filter;

 public:
  bool match(Data *cell) { return *cell > this.value; }
};

template <typename Data>
class GreaterEqual : public Filter<Data> {
  using Filter<Data>::Filter;

 public:
  bool match(Data *cell) { return *cell >= this.value; }
};
