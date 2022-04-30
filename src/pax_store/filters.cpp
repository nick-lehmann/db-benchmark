#pragma once
#include "types.h"

class Filter {
 public:
  Filter(ColumnIndex index, Data value) {
    this->index = index;
    this->value = value;
  };
  virtual bool match(Data *cell) = 0;

  ColumnIndex index;
  Data value;
};

class Equal : public Filter {
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell == value; }
};
class LessThan : public Filter {
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell < value; }
};
class GreaterThan : public Filter {
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell > value; }
};
