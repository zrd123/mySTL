/*
 *set实现
 *
 *set使用rb-tree作为底层容器，rb-tree提供了所有set需要的操作
 *set使用rb-tree的insert_unique来插入元素，multiset使用 insert_equal
 */

#ifndef _SET_H_
#define _SET_H_

#include "Allocator.h"
#include "AlgoBase.h"		//identity less
#include "Rbtree.h"

namespace TinySTL {
	//省缺使用递增排序(less)
	template<class Key, class Compare = less<Key>, class Alloc = alloc>
	class set {
	public:
		typedef Key			key_type;
		typedef Key			value_type;
		typedef Compare		key_compare;
		typedef Compare		value_compare;

	private:
		//注意：以下第一、第二参数分别为键值和实值型别，对于set键值和实值型别相同
		typedef detail::rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
		typedef typename rep_type::iterator rep_iterator;

		rep_type t;			//底层容器采用 RB-tree

	public:
		/* 注意：以下将set的iterator定义为rb-tree的const_iterator，因为set的键值不可以改变 */
		typedef typename rep_type::const_iterator			iterator;
		typedef typename rep_type::const_iterator			const_iterator;
		typedef typename rep_type::const_pointer			pointer;
		typedef typename rep_type::const_pointer			const_pointer;
		typedef typename rep_type::const_reference			reference;
		typedef typename rep_type::const_reference			const_reference;
		typedef typename rep_type::size_type				size_type;
		typedef typename rep_type::difference_type			difference_type;
		//typedef typename rep_type::const_reverse_iterator	reverse_iterator;

		set() : t(Compare()){}
		explicit set(const Compare& comp) : t(comp){}
		template <class InputIterator>
		set(InputIterator first, InputIterator last) : t(Compare()) {
			t.insert_unique(first, last);
		}
		set(const set<Key, Compare, Alloc>& x) : t(x.t){}

		set<Key, Compare, Alloc>& operator =(const set<Key, Compare, Alloc>& x) {
			t = x.t;
			return *this;
		}

		/* 以下的所有操作RB-tree已经提供，只需要传递调用即可 */
		//accessors:
		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		key_compare key_comp()const { return t.key_comp(); }
		value_compare value_comp()const { return t.key_comp(); }
		//reverse_iterator rbegin() const { return t.rbegin(); }
		//reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

		//insert
		std::pair<iterator, bool> insert(const value_type& x) {
			std::pair<rep_iterator, bool> p = t.insert_unique(x);
			return std::pair<iterator, bool>(p.first, p.second);
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}

		//erase
		void erase(iterator pos) {
			t.erase(static_cast<rep_iterator&>(pos));
		}
		size_type erase(const key_type& x) {
			return t.erase(x);
		}
		void erase(iterator first, iterator last) {
			t.erase(reinterpret_cast<rep_iterator&>(first), reinterpret_cast<rep_iterator&>(last));
		}
		void clear() { t.clear(); }

		//set operations
		iterator find(const key_type& x) const { return t.find(x); }
		size_type count(const key_type& x) const { return t.count(x); }
		iterator lower_bound(const key_type& x) const { return static_cast<iterator>(t.lower_bound(x)); }
		iterator upper_bound(const key_type& x) const { return static_cast<iterator>(t.upper_bound(x)); }

		/* 返回元素 x 的区间 */
		std::pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x); }

		/*friend bool operator==(const set& x, const set& y);
		friend bool operator< (const set& x, const set& y);*/

	};//end of class set

	/*template<class Key, class Compare, class Alloc>
	inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.t == y.t);
	}
	template<class Key, class Compare, class Alloc>
	inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.t < y.t);
	}*/


}//end of namespace TinySTL





#endif // !_SET_H_
