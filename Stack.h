/*
 *stack的实现
 *
 *stack实际不是容器而是一个配接器，其内部使用list作为底层容器（也可使用deque、vector等），因而实现非常简单
 */


#ifndef _STACK_H_
#define _STACK_H_

#include "List.h"			//以list作为其底层容器

namespace TinySTL {

	//省缺以list作为底层容器，用户也可以自己指定
	template<class T, class Container = list<T>>
	class stack {
	public:
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		Container cont;		//底层容器

		/* 以下利用底层容器的操作实现stack的外接口 */
	public:
		explicit stack(const Container& ctnr = Container()) :cont(ctnr) {}
		bool empty()const { return cont.empty(); }
		size_type size() const { return cont.size(); }
		reference top() { return cont.back(); }
		const_reference top()const { return cont.back(); }
		void push(const value_type& x) { cont.push_back(x); }
		void pop() { cont.pop_back(); }
		void swap(stack& x) { TinySTL::swap(cont, x.cont); }
	
		template <class T, class Container>
		friend bool operator== (const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
			return lhs.cont == rhs.cont;
		}
		template <class T, class Container>
		friend bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
			return lhs.cont != rhs.cont;
		}
		template <class T, class Container>
		friend void swap(stack<T, Container>& x, stack<T, Container>& y) {
			x.swap(y);
		}
	};	
	



}//end of namespace TinySTL


#endif // !_STACK_H_
