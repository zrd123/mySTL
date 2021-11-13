#ifndef _AUTOPTR_H_
#define _AUTOPTR_H_


namespace JJ {

	template<class T> 
	class auto_ptr {
	public:
		explicit auto_ptr(T* p = nullptr)
			:pointer(p) {}
		template<class U>
		auto_ptr(auto_ptr<U>& rhs) : pointer(rhs.release()) {}

		template<class U>
		auto_ptr<T>& operator = (auto_ptr<U>& rhs) {
			if (this != &rhs) {
				reset(rhs.release());
			}
			return *this;
		}

		T& operator*()const { return *pointer; }
		T* operator -> ()const { return pointer; }
		T* get()const { return pointer; }
		T* release() {
			T* tmp = pointer;
			pointer = nullptr;
			return tmp;
		}

	private:
		T* pointer;

	};

}//the end of namespace mystl;


#endif // !_MY_AUTOPTR_H_
