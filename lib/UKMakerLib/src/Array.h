#ifndef UKMAKER_ARRAY_H
#define UKMAKER_ARRAY_H
/*
* Simple wrapper for C arrays to make life easier without having to import all of std:collections 
* or whatever
*/
template<typename T>
class Array {

    public:
    
    Array(const T *arrayValues, const size_t arraySize) : values(arrayValues), size(arraySize) {
    }

    T &operator [](size_t index) {
        return values[index];
    }

    const T *values;
    const size_t size;

};
#endif