/*
 *标准库常用基本算法实现
 *
 */

#ifndef _ALGOBASE_H_
#define _ALGOBASE_H_

#include "Iterator.h"			//for function distance_type、iterator_category、value_type
#include "Type_Traits.h"		//for __type_traits
#include <string.h>				//for memmove

namespace TinySTL {		//这一级namespace下是外部用户使用的标注接口

	/* fill: 将[first, last) 内的所有位置填充为value */
	template<class ForwardIterator, class T>
	inline void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first) {
			*first = value;
		}
	}

	/* fill_n: 将从first开始的n个位置填充为value，返回最后一个填充元素的下一位置 */
	template<class OutputIterator, class Size, class T>
	inline OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}

	/*  max  */
	template <class T>
	inline const T& max(const T& a, const T& b) {
		return (a < b) ? b : a;		//注意：用 < 降低对操作数类型支持操作的要求
	}

	/* min */
	template<class T>
	inline const T& min(const T& a, const T& b) {
		return (a < b) ? a : b;		//注意：用 < 降低对操作数类型支持操作的要求
	}

	/* swap  */
	template<class T>
	void swap(T& lhs, T& rhs) {
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	/* find */
	template <class InputIterator, class T>
	inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
		while (first != last && *first != value)
			++first;
		return first;
	}

	namespace detail {		//这一级namespace下是stl下内部实现细节

		template<class InputIterator, class OutputIterator>
		struct __copy_dispatch {
			OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
				return __copy(first, last, result, iterator_category(first));
				//这里根据迭代器种类的不同调用不同的__copy().
				//为的是不同种类的迭代器使用的循环条件相同,有快慢之分
			}

		};

		/*  __copy_dispatch 针对T*的偏特化版本*/
		template <class T>
		struct __copy_dispatch<T*, T*> {	//两个参数都是T*形式
			T* operator()(T* first, T* last, T* result) {
				typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
				return __copy_t(first, last, result, has_trivial());
				//根据是否有trivial assignment operator使用不同的策略
			}
		};

		/*  __copy_dispatch 针对const T*的特化版本 */
		template <class T>
		struct __copy_dispatch<const T*, T*> {		//第一个参数为 const T*，第二个参数为 T* 形式
			T* operator()(const T* first, const T* last, T* result) {
				typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
				return __copy_t(first, last, result, has_trivial());
				//根据是否有trivial assignment operator使用不同的策略
			}
		};
		
		/*  InputIterator 版本 */
		template<class InputIterator, class OutputIterator>
		inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
			for (; first != last; ++first, ++result) {			//以迭代器作为循环条件,速度慢
				*result = *first;
			}
			return result;
		}

		/*  RandomAccessIterator 版本 */
		template<class RandomAccessIterator, class OutputIterator>
		inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag){
			return __copy_d(first, last, result, distance_type(first));
		}
		//又划分出一个函数,为的是其他地方能够用到
		template<class RandomAccessIterator, class OutputIterator, class Distance>
		inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
			Distance n = last - first;
			for (; n > 0; --n, ++first, ++result) {		//以n作为循环条件,速度快
				*result = *first;
			}
			return result;
		}
		
		//以下版本适合于指针所指对象具备  trivial assignment operator
		template<class T>
		inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
			memmove(result, first, sizeof(T) * (last - first));
			return result + (last - first);
		}

		//以下版本适合于指针所指对象具备 non-trivial assignment operator
		template<class T>
		inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
			//指针是一种RandomAccessIterator, 因此可以调用__copy_d
			return __copy_d(first, last, result, static_cast<ptrdiff_t*>(0));
		}

	}//end of namespace detail


	/*    copy:  将[first, last)内的元素从前往后拷贝到result开始的位置, 返回最后一个目的位置的下一个位置 */
	/*  copy标准接口 */
	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		return detail::__copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}
	/* 针对const char*的全特化版本 */
	inline char* copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}
	/* 针对const wchar_t* 的全特化版本 */
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}


	namespace detail {			//这一级namespace下是stl下内部实现细节

	/* __copy_backward_dispatch完全泛化版本 */
		template <class BidirectionalIterator1, class BidirectionalIterator2>
		struct __copy_backward_dispatch {
			BidirectionalIterator2 operator()(
				BidirectionalIterator1 first,
				BidirectionalIterator1 last,
				BidirectionalIterator2 result) 
			{
				return __copy_backward(first, last, result, iterator_category(first));
				//这里根据迭代器种类的不同调用了不同的__copy_backward()，
				//为的是不同种类的迭代器使用的循环条件不同，有快慢之分。
			}
		};
		/* __copy_backward_dispatch针对 T* 的偏特化版本 */
		template <class T>
		struct __copy_backward_dispatch<T*, T*> {			//两个参数都是 T* 指针形式
			T* operator()(T* first, T* last, T* result) {
				typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivial;
				return __copy_backward_t(first, last, result, has_trivial());
				//根据是否有trivial assignment operator使用不同的策略
			}
		};
		/* __copy_backward_dispatch针对const T* 的偏特化版本 */
		template <class T>
		struct __copy_backward_dispatch<const T*, T*> {			//第一个参数为 const T*，第二个参数为 T* 形式
			T* operator() (const T* first, const T* last, T* result) {
				typedef typename __type_traits<T>::has_trivial_assignment_operator has_trivail;
				return __copy_backward_t(first, last, result, has_trivail());
				//根据是否有trivial assignment operator使用不同的策略
			}
		};

		/*  BidirerctionalIterator 版本  */
		template <class BidirectionalIterator1, class BidirectionalIterator2>
		inline BidirectionalIterator2 __copy_backward(
			BidirectionalIterator1 first,
			BidirectionalIterator1 last,
			BidirectionalIterator2 result,
			bidirectional_iterator_tag
		) {
			while (last != first) {		//以迭代器作为循环条件,速度慢
				*(--result) = *(--last);
			}
			return result;
		}
		/*  RandomAccessIterator 版本  */
		template<class RandomAccessIterator, class BidirectionalIterator>
		inline BidirectionalIterator __copy_backward(
			RandomAccessIterator first,
			RandomAccessIterator last,
			BidirectionalIterator result,
			random_access_iterator_tag
		) {
			return __copy_backward_d(first, last, result, distance_type(first));
		}
		//划分出一个函数,为的是其他地方能用到
		template<class RandomAccessIterator, class BidirectionalIterator, class Distance>
		inline BidirectionalIterator __copy_backward_d(
			RandomAccessIterator first,
			RandomAccessIterator last,
			BidirectionalIterator result,
			Distance*
		) {
			Distance n = last - first;
			for (; n > 0; --n) {		//以n作为循环条件,速度快
				*(--result) = *(--last);
			}
			return result;
		}

		//以下版本适合于指针所指对象具备trivial assignment operatpr
		template <class T>
		inline T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
			result = result - (last - first);
			memmove(result, first, sizeof(T) * (last - first));
			return result;
		}
		//以下版本适合于指针所指对象具备 non-trivial assignment operator
		template <class T>
		inline T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
			//指针是一种RandomAccessIterator,因此可用__copy_d
			return __copy_backward_d(first, last, result, static_cast<ptrdiff_t*>(0));
		}

	}//end of namespace detail

/* copy_backward: 将[first, last)内的元素从后往前拷贝到result往前的位置 */
/* 注意：result不存储元素，并且只有双向迭代器才能调用该函数 */
	/*  copy_backward标准接口  */
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(
		BidirectionalIterator1 first,
		BidirectionalIterator1 last,
		BidirectionalIterator2 result
	) {
		return detail::__copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
	}
	/* 针对const char*的全特化版本 */
	inline char* copy_backward(const char* first, const char* last, char* result) {
		result = result - (last - first);
		memmove(result, first, last - first);
		return result;
	}
	/*  针对const wchar_t* 的全特化版本  */
	inline wchar_t* copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		result = result - (last - first);
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result;
	}

	namespace detail {		//这一级namespace 下是stl下内部实现细节
	/*  loewr_bound版本一辅助函数 二分法 */
		/*  forward_iterator版  */
		template<class ForwardIterator, class T, class Distance>
		ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
			const T& value, Distance*, forward_iterator_tag
		) {
			Distance len = distance(first, last);			//求得整个区间的长度
			Distance half = 0;
			ForwardIterator mid;

			while (len > 0) {
				half = (len >> 1);				//通过位移实现除2
				mid = first;
				advance(mid, half);				//令mid指向区间中部
				if (*mid < value) {
					first = ++mid;
					len = len - half - 1;		//修正len
				}
				else {
					len = half;
				}
			}
			return first;
		}
		/*  random_access_iterator版  */
		template <class RandomAccessIterator, class T, class Distance>
		RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Distance*, random_access_iterator_tag
		) {
			Distance len = last - first;		//求取整个区间的长度
			Distance half = 0;
			RandomAccessIterator mid;

			while (len > 0) {
				half = (len >> 1);				//通过位移实现除2
				mid = first + half;				//注意:随机指针才能这样移动
				if (*mid < value) {
					first = mid + 1;
					len = len - half - 1;
				}
				else {
					len = half;
				}
			}
			return first;
		}

	/*  lower_bound版本二函数   */
		/* forward_iterator版 */
		template<class ForwardIterator, class T, class Compare, class Distance>
		ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
			const T& value, Compare comp, Distance*, forward_iterator_tag
		) {
			Distance len = distance(first, last);		//求得整个区间的长度
			Distance half = 0;
			ForwardIterator mid;

			while (len > 0) {
				half = (len >> 1);						//通过位移实现除2
				mid = first; 
				advance(mid, half);						//令mid指向区间中部
				if (comp(*mid, value)) {
					first = ++mid;
					len = len - half - 1;				//修正len
				}
				else {
					len = half;
				}
			}
			return first;
		}
		/* random_access_iterator */
		template<class RandomAccessIterator, class T, class Compare, class Distance>
		RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Compare comp, Distance*, random_access_iterator_tag
		) {
			Distance len = last - first;
			Distance half = 0;
			RandomAccessIterator mid;

			while (len > 0) {
				half = (len >> 1);						//通过位移实现除2
				mid = first + half;						//注意:随机指针才能这样移动
				if (comp(*mid, value)) {
					first = mid + 1;
					len = len - half - 1;
				}
				else {
					len = half;
				}
			}
			return first;
		}


	/*  upper_bound版本一辅助函数  */
		/* forward_iterator版 */
		template<class ForwardIterator, class T, class Distance>
		ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
			const T& value, Distance*, forward_iterator_tag
		) {
			Distance len = distance(first, last);		//求的整个区间的长度
			Distance half = 0;
			ForwardIterator mid;

			while (len > 0) {
				half = (len >> 1);						//通过位移实现除2
				mid = first;
				advance(mid, half);						//令mid 指向区间中部
				if (value < *mid) {
					len = half;
				}
				else {
					first = ++mid;
					len = len - half - 1;				//修正half
				}
			}
			return first;
		}
		/* random_access_iterator版 */
		template<class RandomAccessIterator, class T, class Distance>
		RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Distance*, random_access_iterator_tag
		) {
			Distance len = last - first;				//求取整个区间长度
			Distance half = 0;
			RandomAccessIterator mid;

			while (len > 0) {
				half = (len >> 1);						//通过位移实现除2
				mid = first + half;						//注意:随机指针才能这样移动
				if (value < *mid) {
					len = half;
				}
				else {
					first = ++mid;
					len = len - half - 1;
				}

			}
			return first;
		}

	/*  upper_bound 版本二函数  */
		/*  forward_iterator版  */
		template <class ForwardIterator, class T, class Compare, class Distance>
		ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
			const T& value, Compare comp, Distance*, forward_iterator_tag
		) {
			Distance len = distance(first, last);			//求的整个区间的长度
			Distance half = 0;
			ForwardIterator mid;
			while (len > 0) {
				half = (len >> 1);							//通过位移实现除2 
				mid = first;
				advance(mid, half);							//令mid 指向区间中部
				if (comp(value, *mid)) {
					len = half;
				}
				else {
					first = ++mid;
					len = len - half - 1;					//修正half
				}
			}
			return first;
		}
		/*  random_access_iterator版  */
		template <class RandomAccessIterator, class T, class Compare, class Distance>
		RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
			const T& value, Compare comp, Distance*, random_access_iterator_tag
		) {
			Distance len = last - first;					//求的整个区间的长度
			Distance half = 0;
			RandomAccessIterator mid;
			while (len > 0) {
				half = (len >> 1);							//通过位移实现除2
				mid = first + half;							//注意:随机指针才能这样移动
				if (comp(value, *mid)) {
					len = half;
				}
				else {
					first = mid + 1;
					len = len - half - 1;
				}
			}
			return first;
		}

	}//end of namespace detail
		
/*  lower_bound 版本1, 使用 < 进行判断  */
	//指定区域内查找不小于目标值的第一个元素
	template <class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
		return detail::__lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}

/*  lower_bound 版本2, 使用仿函数 Compare 进行判断  */
	template <class ForwardIterator, class T, class Compare>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
		return detail::__lower_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}

/*  upper_bound 版本1, 使用 < 进行判断  */
	//指定区域内查找大于目标值的第一个元素
	template <class ForwardIterator, class T>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
		return detail::__upper_bound(first, last, value, distance_type(first), iterator_category(first));
	}

/*  upper_bound 版本2, 使用仿函数 Compare 进行判断   */
	template<class ForwardIterator, class T, class Compare>
	inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
		return detail::__upper_bound(first, last, value, comp, distance_type(first), iterator_category(first));
	}


/*  unary_function 用来呈现一元函数的参数行别和返回值型别  */
	/*  每一个一元函数都应该继承此类别  */
	template<class Arg, class Result>
	struct unary_function {
		typedef Arg						argument_type;
		typedef Result					result_type;
	};

/*  binary_function 用来呈现二元函数的第一参数类型别、第二参数型别和返回值型别  */
	/*  每一个二元函数都应该继承此类别  */
	template <class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1					first_argument_type;
		typedef Arg2					second_argument_type;
		typedef Result					result_type;
	};



/*  以下为 + - * / % < > 运算符的仿函数  */
	//Arithmetic
	template <class T>
	struct plus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x + y;
		}
	};
	template<class T>
	struct minus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x - y;
		}
	};
	template<class T>
	struct multiplies : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x * y;
		}
	};
	template<class T>
	struct divides : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x / y;
		}
	};	
	template<class T>
	struct modulus : public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x % y;
		}
	};
	template <class T>
	struct negate : public unary_function<T, T> {
		T operator()(const T& x)const { return -x; }
	};

	//Relational
	template<class T>
	struct equal_to : public binary_function<T, T, bool>{
		bool operator()(const T& x, const T& y) const {
			return (x == y);
		}
	};
	template<class T>
	struct not_equal_to : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return (x != y);
		}
	};
	template<class T>
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x > y;
		}
	};
	template<class T>
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x < y;
		}
	};
	template<class T>
	struct greater_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x >= y;
		}
	};
	template<class T>
	struct less_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x <= y;
		}
	};

	//Logical
	template<class T>
	struct logical_and : public binary_function <T,T,bool>{
		bool operator()(const T& x, const T& y) const {
			return x && y;
		}
	};
	template<class T>
	struct logical_or : public binary_function <T, T, bool> {
		bool operator()(const T& x, const T& y) const {
			return x || y;
		}
	};
	template<class T>
	struct logical_not : public unary_function<T, bool> {
		bool operator()(const T& x) const {
			return !x;
		}
	};

/*  证同(identity)、选择（select）、投射（project）函数  */
	template <class T>
	struct identity : public unary_function<T, T> {
		const T& operator()(const T& x) const {
			return x;
		}
	};

	template <class Pair>
	struct select1st : public unary_function<Pair, typename Pair::first_type> {
		const typename Pair::first_type& operator()(const Pair& x) const {
			return x.first;
		}
	};

	template <class Pair>
	struct select2nd : public unary_function<Pair, typename Pair::second_type> {
		const typename Pair::second_type& operator()(const Pair& x) const {
			return x.second;
		}
	};

/*  投射函数:传回第一参数, 忽略第二参数  */
	template <class Arg1, class Arg2>
	struct project1st : public binary_function<Arg1, Arg2, Arg1> {
		Arg1 operator()(const Arg1& x, const Arg2&) {
			return x;
		}
	};

/*  投射函数: 传回第二参数, 忽略第一参数  */
	template<class Arg1, class Arg2>
	struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
		Arg2 operator()(const Arg1&, const Arg2& y) {
			return y;
		}
	};

	/*  字符串判等  */
	struct str_equal {
		bool operator ()(const char* s1, const char* s2) const {
			return strcmp(s1, s2) == 0;
		}
	};


}//end of namespace TinySTL


#endif // !_MYALGOBASRE_H_
