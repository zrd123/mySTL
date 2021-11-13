#ifndef __REVERSR_ITERATOR_H_
#define __REVERSR_ITERATOR_H_

#include "Iterator.h"

namespace TinySTL {
	template<class Iterator>
	class reverse_iterator_t {
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category	iterator;
		typedef typename iterator_traits<Iterator>::value_type			value_type;
		typedef typename iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename iterator_traits<Iterator>::pointer				pointer;
		typedef typename const pointer									const_pointer;
		typedef typename iterator_traits<Iterator>::reference			reference;
		typedef typename const reference								const_reference;

	private:
		Iterator base_;
		Iterator cur_;

	public:
		//构造。复制，析构相关
		reverse_iterator_t() : base_(0), cur_(0){}
		//explicit reverse_iterator_t(const iterator_type& it):base_(it),cur_(it - 1){}
		explicit reverse_iterator_t(const iterator& it) :base_(it) {
			auto temp = it;
			cur_ = --temp;
		}
		template <class Iter>
		reverse_iterator_t(const reverse_iterator_t<Iter>& rev_it) {
			base_ = (iterator_type)rev_it.base();
			auto temp = base_;
			cur_ = --temp;

		}

		//其他成员函数
		iterator_type base() { return base_; }
		reference operator*() { return (*cur_); }
		const_reference operator*()const { return (*cur_); }
		pointer operator->() { return&(operator*()); }
		const_pointer operator->()const { return &(operator*()); }
		reverse_iterator_t& operator ++() {
			base_;
			cur_;
			return *this;
		}
		reverse_iterator_t& operator ++(int) {
			reverse_iterator_t tmp = *this;
			++(*this);
			return tmp;
		}
		reverse_iterator_t& operator --() {
			base_;
			cur_;
			return *this;
		}
		reverse_iterator_t operator --() {
			reverse_iterator_t tmp = *this;
			--(*this);
			return tmp;
		}
		reference operator[](difference_type n) {
			return base()[-n - 1];
		}
		reverse_iterator_t operator + (difference_type n)const;
		reverse_iterator_t& operator += (difference_type n);
		reverse_iterator_t operator - (difference_type n)const;
		reverse_iterator_t& operator -= (difference_type n);
	private:
		//���õĵ������ƶ�������forward_list����ʵ�ַ��������
		Iterator advance_n(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			random_access_iterator_tag) {
			if (right) {
				it += n;
			}
			else {
				it -= n;
			}
			return it;
		}
		Iterator advance_n(Iterator it,
			difference_type n,
			bool right,//true -> \ false <-
			bidirectional_iterator_tag) {
			difference_type i;
			difference_type absN = n >= 0 ? n : -n;
			if ((right && n > 0) || (!right && n < 0)) {// ->
				for (i = 0; i != absN; ++i) {
					it = it + 1;
				}
			}
			else if ((!right && n > 0) || (right && n < 0)) {// <-
				for (i = 0; i != absN; ++i) {
					it = it - 1;
				}
			}
			return it;
		}

	public:
		template<class Iterator>
		friend bool operator == (const reverse_iterator_t<Iterator>& lhs, 
			const reverse_iterator_t<Iterator>& rhs);
		friend bool operator != (const reverse_iterator_t<Iterator>& lhs, 
			const reverse_iterator_t<Iterator>& rhs);
		friend bool operator < (const reverse_iterator_t<Iterator>& lhs, 
			const reverse_iterator_t<Iterator>& rhs);
		friend bool operator <= (const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);
		friend bool operator > (const reverse_iterator_t<Iterator>& lhs, 
			const reverse_iterator_t<Iterator>& rhs);
		friend bool operator >= (const reverse_iterator_t<Iterator>& lhs, 
			const reverse_iterator_t<Iterator>& rhs);
		//��������ǰǰ��n������
		friend reverse_iterator_t<Iterator> operator + (
			typename reverse_iterator_t<Iterator>::difference_type n,
			const reverse_iterator_t<Iterator>& rev_it);
		//�����������ľ���n
		friend typename reverse_iterator_t<Iterator>::difference_type operator-(
			const reverse_iterator_t<Iterator>& lhs,
			const reverse_iterator_t<Iterator>& rhs);

	};//end of reverse_iterator_t

	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator+=(difference_type n) {
		base_ = advance_n(base_, n, false, iterator_category(Iterator));
		cur_ = advance_n(cur_, n, false, iterator_category(Iterator));
		return *this;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator-=(difference_type n) {
		base_ = advance_n(base_, n, true, iterator_category(*this));
		cur_ = advance_n(cur_, n, true, iterator_category(*this));
		return *this;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator+(difference_type n)const {
		reverse_iterator_t<Iterator> res = *this;
		res += n;
		return res;
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator-(difference_type n)const {
		reverse_iterator_t<Iterator> res = *this;
		res -= n;
		return res;
	}

	template<class Iterator>
	bool operator == (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ == rhs.cur_;
	}
	template <class Iterator>
	bool operator != (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return !(lhs == rhs);
	}
	template <class Iterator>
	bool operator < (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ < rhs.cur_;
	}
	template <class Iterator>
	bool operator > (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ > rhs.cur_;
	}
	template <class Iterator>
	bool operator >= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return !(lhs < rhs);
	}
	template <class Iterator>
	bool operator <= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs) {
		return !(lhs > rhs);
	}

	template<class Iterator>
	reverse_iterator_t<Iterator> operator+(
		typename reverse_iterator_t<Iterator>::difference_type n,
		const reverse_iterator_t<Iterator>& rev_it) {
		return rev_it + n;
	}
	template<class Iterator>
	typename reverse_iterator_t<Iterator>::difference_type operator - (
		const reverse_iterator_t<Iterator>& lhs,
		const reverse_iterator_t<Iterator>& rhs) {
		return lhs.cur_ - rhs.cur_;
	}
	

}//end of namespace TinySTL


#endif // !_REVERSR_ITERATOR_H_
