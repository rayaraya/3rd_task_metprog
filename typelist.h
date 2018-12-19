class NullType {};
template<typename T=NullType, typename ... U>
struct TypeList {
	using head = T;
	using tail = TypeList<U ...>;
};
template<typename T>
struct TypeList<T> {
	using head = T;
	using tail = NullType;
};
using CharList = TypeList<char, signed char, unsigned char>;
using EmptyList = TypeList<>;

template <typename TList>
struct Length {
	enum { 
		value = Length<typename TList::tail>::value + 1 
	};
};
template <typename TList> struct Length;
template <>
struct Length<NullType> {
	enum { value = 0 };
};
template <>
struct Length<TypeList<NullType>> {
	enum { value = 0 };
};

template<class TList, unsigned int pos> struct TypeAt;
template<class TList, unsigned int pos> struct TypeAt {
	using type = typename TypeAt<typename TList::tail, pos-1>::type;
};

template<unsigned int pos> struct TypeAt<TypeList<NullType>, pos>{
	using type = NullType;
};
template<class TList> struct TypeAt<TList, 0u> {
	using type = typename TList::head;
};



template <class TList, template <class RealParameter, class InheritParameter> class Unit, class Root = NullType>
class GenLinearHierarchy : public Unit<typename TList::head, 
		GenLinearHierarchy<typename TList::tail, Unit, Root>> {};

template <typename T, template <class, class> class Unit, class Root>
class GenLinearHierarchy<TypeList<T>, Unit, Root> : public Unit<T, Root> {};

template <template<class> class Unit, typename ... Types>
class GenScatterHierarchy;

template <template <class RealParameter> class Unit, class ... Types>
class GenScatterHierarchy<Unit, TypeList<Types ...>> : public Unit<Types> ... {}; 

template <unsigned int N>
struct Fibonacci {
	enum {value = Fibonacci<N-1>::value + Fibonacci<N-2>::value};
};
template <>
struct Fibonacci<0> {
	enum {value = 1};
};
template <>
struct Fibonacci<1> {
	enum {value = 1};
};


template<typename T, class TList>
struct Append;

template<typename T, class ... Types>
struct Append<T, TypeList<Types ...>>
{
	using toBegin = TypeList<T, Types ...>;
	using toEnd   = TypeList<Types ..., T>;
};

template <unsigned int pos, class TList>
struct Slice {
	using front = typename Append<typename TList::head, typename Slice<pos-1, typename TList::tail>::front>::toBegin;
	using back  = typename Slice<pos-1, typename TList::tail>::back;
};
template <class TList>
struct Slice<0, TList> {
	using front = TypeList<>;
	using back  = TList;
};
template <unsigned int pos>
struct Slice<pos, TypeList<>> {
	using front = TypeList<>;
	using back  = TypeList<>;
};
template <unsigned int pos>
struct Slice<pos, NullType> {
	using front = TypeList<>;
	using back  = TypeList<>;
};

template<class TList, unsigned int pos = 0>
struct SliceForBranches {
	using sliced = Slice<Fibonacci<pos>::value+1, TList>;
	using result = typename Append<typename sliced::front, typename SliceForBranches<
			typename sliced::back, pos+1>::result>::toBegin;
};
template<unsigned int pos>
struct SliceForBranches<TypeList<>, pos> {
	using result = EmptyList;
};

template<class TTList, template <class RealParameter, class InheritParameter> class Unit>
struct MakeTypesFromBranchSlices {
	using result = typename Append<GenLinearHierarchy<typename TTList::head, Unit>,
			typename MakeTypesFromBranchSlices<typename TTList::tail, Unit>::result>::toBegin;
};
template<template <class, class> class Unit>
struct MakeTypesFromBranchSlices<TypeList<EmptyList>, Unit> {
	using result = NullType;
};
// For explanations see #208
template<class TList, template <class, class> class LinearUnit, template <class> class ScatterUnit>
class GenerateFibonacciHierarchy 
{
	using result = GenScatterHierarchy<ScatterUnit, MakeTypesFromBranchSlices<SliceForBranches<TList>, LinearUnit>>;
};


template <typename T, class Inherit>
class IHolder : public Inherit {
public:
	IHolder() {

	}
	T data;
};

template <typename T>
class Holder  {
public:
	Holder() {

	}
	T data;
};
