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
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell == value; }
};

template <typename Data>
class LessThan : public Filter<Data> {
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell < value; }
};

template <typename Data>
class GreaterThan : public Filter<Data> {
  using Filter::Filter;

 public:
  bool match(Data *cell) { return *cell > value; }
};