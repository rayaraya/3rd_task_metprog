#include "typelist.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

#define FILE_NAME "input.txt"

std::ifstream _file;

template <class TList>
struct FullSize {
	enum {value = sizeof(typename TList::head) + FullSize<typename TList::tail>::value};
};

template <>
struct FullSize<NullType> {
	enum { value = 0 };
};
template <>
struct FullSize<TypeList<NullType>> {
	enum { value = 0 };
};


//Dont know how to make them class' methods (explicit specialization of non-specialized class doesnt work)
	
template<class TList_another>
void ReadValue(uint8_t* mem, size_t ptr) {
	typename TList_another::head var;

	_file >> var;

	size_t typeSize = sizeof(typename TList_another::head);
	std::memcpy(mem+ptr, &var, typeSize);

	ReadValue<typename TList_another::tail>(mem, ptr + typeSize);
}

template<>
void ReadValue<TypeList<NullType>>(uint8_t* mem, size_t ptr) {
}
template<>
void ReadValue<NullType>(uint8_t* mem, size_t ptr) {
}



template<class TList>
class Reader {
public:
	Reader() {
			_file = std::ifstream(FILE_NAME);
	}
	void printSize () {
		std::cout << FullSize<TList>::value << std::endl;
	}
	uint8_t* ReadLine() {
		uint8_t* mem = (uint8_t*)malloc(FullSize<TList>::value);
	
		ReadValue<TList>(mem, 0);

		return mem;
	}
};


int main() {

	Reader<TypeList<int, char, double, float>> b;
	uint8_t* mem = b.ReadLine();
	std::cout << *((int*)mem) << std::endl;
	std::cout << *((char*)(mem + sizeof(int))) << std::endl;
	std::cout << *((double*)(mem + sizeof(int) +sizeof(char))) << std::endl;
	std::cout << *((float*)(mem + sizeof(int) +sizeof(char) + sizeof(double))) << std::endl;
	
	return 0;



}