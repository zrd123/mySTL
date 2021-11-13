/*
 *hash_setʵ��
 *
 *hash_setʹ��hashtable��Ϊ�ײ�������hashtable�ṩ������hash_set��Ҫ�Ĳ���
 *hash_setʹ��hashtable��insert_unique������Ԫ�أ�hast_multisetʹ�� insert_equal
 */

#ifndef _HASHSET_H_
#define _HASHSET_H_

#include "Allocator.h"
#include "AlgoBase.h"
#include "Hashtable.h"

namespace TinySTL {

	/*	Value:		����ֵ�ĺ������Value
	 *	HashFunc:	hash����
	 *	ExtractKey:	��δ�Value������ȡ��Key(�������߷º���)
	 *	EqualKey:	��αȽ�key(�������߷º���)
	 *	Alloc:		�ڴ������
	 */
	template<class Value,
		class HashFunc = std::hash<Value>,
		class EqualKey = equal_to<Value>,
		class Alloc = alloc>
	struct hash_set {
	private:
		typedef typename detail::hashtable<Value, Value, HashFunc, identity<Value>, EqualKey, Alloc> hashtable;

		hashtable htb;

	public:
		typedef typename hashtable::key_value				key_type;
		typedef typename hashtable::value_type				value_type;
		typedef typename hashtable::size_type				size_type;
		typedef typename hashtable::const_iterator			iterator;
		typedef typename hashtable::const_iterator			const_iterator;
		typedef typename hashtable::const_pointer			pointer;
		typedef typename hashtable::const_pointer			const_pointer;
		typedef typename hashtable::const_reference			reference;
		typedef typename hashtable::const_reference			const_reference;
		typedef typename hashtable::difference_type			difference_type;

		HashFunc hash_funct()const { return htb.hash(); }
		EqualKey key_eq() const { return htb.equals(); }

	public:
		hash_set() :htb(50, HashFunc(), EqualKey()) {}	//ȱʡʹ�ñ���СΪ50
		explicit hash_set(size_type n) : htb(n, HashFunc(), EqualKey()) {}
		hash_set(size_type n, const HashFunc& hf) : htb(n, hf, EqualKey()) {}
		hash_set(size_type n, const HashFunc& hf, const EqualKey& eql) :htb(n, hf, eql) {}

		//ע��: һ��ȫ��ʹ�� insert_unique
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last) : htb(50, HashFunc(), EqualKey()) {
			htb.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last, size_type n) {

		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last,
			size_type n, const HashFunc& hf) : htb(n, hf, EqualKey())
		{
			htb.insert_unique(first, last);
		}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last,
			size_type n, const HashFunc& hf, const EqualKey& eql) : htb(n, hf, eql)
		{
			htb.insert_unique(first, last);
		}

	public:
		size_type size()const { return htb.size(); }
		size_type max_size() const { return htb.max_bucket_count(); }
		bool empty()const { return htb.swap(rhs.htb); }
		void swap(hash_set& rhs) { htb.swap(rhs.htb); }
		iterator begin() const { return htb.begin(); }
		iterator end() const { return htb.end(); }

	public:
		std::pair<iterator, bool> insert(const value_type& obj) {
			std::pair<typename hashtable::iterator, bool> tmp = htb.insert_unique(obj);
			return std::pair<iterator, bool>(tmp.first, tmp.second);
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			htb.insert_unique(first, last);
		}
		std::pair<iterator, bool> insert_noresize(const value_type& obj) {
			pair<typename hashtable::iterator, bool> tmp = htb.insert_unique_noresize(obj);
			return pair<iterator, bool>(tmp.first, tmp.second);
		}

		void erase(const key_type& key) { htb.erase(key); }
		void erase(iterator it) { htb.erase(it); }
		void erase(iterator first, iterator last) { htb.erase(first, last); }

		void clear() { htb.clear(); }

		iterator find(const key_type& key)const { return htb.find(key); }
		size_type count(const key_type& key)const { return htb.bucket_count(key); }
		void resize(size_type new_sz) { return htb.resize(new_sz); }
		size_type bucket_count() const { return htb.bucket_count(); }
		size_type max_bucket_size()const { return htb.max_bucket_count(); }
		size_type elems_in_bucket(size_type n)const { return htb.elemts_in_buckets(n); }
	
		friend bool operator== <>(const hash_set&, const hash_set&);
	};
	template<class Value, class HashFunc, class EqualKey, class Alloc>
	inline bool operator==(const hash_set<Value, HashFunc, EqualKey, Alloc>& hs1,
		const hash_set<Value, HashFunc, EqualKey, Alloc>& hs2)
	{
		return hs1.htb == hs2.htb;
	}




}//end of namespace TinySTL


#endif // !_HASHSET_H_
