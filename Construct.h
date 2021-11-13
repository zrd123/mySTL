/*
 *对象构造、析构器实现：
 *
 *定义了全局函数construct和destroy，分别负责对象的构造和析构
 */
#pragma once
#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include<new>
#include"Iterator.h"
#include"Type_Traits.h"


namespace TinySTL {

	//在已申请的内存p处构造对象
	template<typename T1, typename T2>
	inline void construct(T1* p, const T2& value) {
		new (p) T1(value);	//placement new (在已申请的内存p处构造对象),调用T1::T1(value)
	}

	//以下是destroy的第一版本,接受一个指针
	template<typename T>
	inline void destroy(T* p) {
		p->~T();
	}


	namespace detail {
		//判断元素的数值类别(value type)是否有trivial destructor
		template<typename ForwaedIterator, typename T>
		inline void __destroy(ForwaedIterator first, ForwaedIterator last, T*) {
			typedef typename __type_traits<T>::has_trivial_destructor trivail_destructor;
			__destroy_aux(first, last, trivail_destructor());
		}

		/* 这样设计的目的是：[first, last)区间可能很大，而元素的析构函数可能是trivial destructor(没什么作用的)，
		 此时没有必要去遍历这个区间调用析构函数，造成不必要的效率损失。
		 详见 STL源码剖析 P52最后
		*/

		//如果元素数值型别有non-trivial destructor，则依次调用每个元素的destructor
		template<typename ForwardIterator>
		inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
			for (; first != last; ++first) {
				destroy(&*first);
			}
		}

		//如果元素数值类型别(value type)有trivial destructor,则什么也不做
		template<typename ForwardIterator>
		inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type){}

	}//end of namespace detail

	//以下是destroy第二版本，接受两个迭代器，此函数设法找出元素的数值型别，
	//进而利用__type_traits<>求取最恰当的措施
	template<typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		detail::__destroy(first, last, value_type(first));//value_type函数实现于Iterator.h
	}

	//以下是destroy第二版本针对迭代器为char*和wchar_t*的特化版（他们什么也不用做）
	inline void destroy(char*, char*) { }
	inline void destroy(wchar_t*, wchar_t*) { }

}// end of namespace TinySTL


#endif // !_MYCONSTRUCT_H_
