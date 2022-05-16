#include<chrono>
#include<vector>
#include<string.h>

template <typename T>
class MTime {

public:

    MTime() = default;

    uint64_t measureTime(Table<T> * table, std::vector<unsigned> &projection, std::vector<Filter<T>*> &filters){

        auto startF=std::chrono::steady_clock::now();
        uint64_t count = table->query_count(projection, filters);
        auto endF=std::chrono::steady_clock::now();
        std::cout<<"query time: "<<std::chrono::duration<double,std::micro>(endF-startF).count()<<"\u03BCs"<<std::endl;
        return count;

    }

};