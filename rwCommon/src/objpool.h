/**
* \author       unknown
* \copyright    https://opensource.org/licenses/MIT License
*/
#pragma once

#include <string>
#include <vector>

//Klasa do pool'ingu obiektów
//obiekty powinny dziedziczyæ po tej klasie

#define ___USE_MT___

template <class T > class ObjectPool
{
public:

	ObjectPool() {}
	virtual ~ObjectPool() {}

	#ifdef ___USE_MT___
	inline static CRITICAL_SECTION* get_cs()
	{
		static CRITICAL_SECTION __m_cs;
		return &__m_cs;
	}
	#endif

	static std::vector<T* >& get_list()
	{
		static std::vector<T* > m_free;
		return m_free;
	}

	static void __op_initialize()
	{
		#ifdef ___USE_MT___
		InitializeCriticalSection(get_cs());
		#endif
		init();
	}

	static void __op_finalize()
	{
		#ifdef ___USE_MT___
		DeleteCriticalSection(get_cs());
		#endif
		destroy();
	}

	
	static void init()
	{
		for (int i = 0; i < 1; ++i)
		{
			T* p = ::new T;
			T::get_list().push_back(p);
		}
	}

	inline void *operator new( size_t stAllocateBlock)
	{
		#ifdef ___USE_MT___
		EnterCriticalSection(get_cs());
		#endif
		if (T::get_list().size() <= 0)
		{
			T::init();
		}
		T* p = T::get_list().back();
		T::get_list().pop_back();
		#ifdef ___USE_MT___
		LeaveCriticalSection(get_cs());
		#endif
		return p;
	}

	inline void operator delete( void *p )
	{
		#ifdef ___USE_MT___
		EnterCriticalSection(get_cs());
		#endif
		T::get_list().push_back((T*)p);
		#ifdef ___USE_MT___
		LeaveCriticalSection(get_cs());
		#endif
	}

	static void destroy()
	{
		std::vector<T* >::iterator first = T::get_list().begin();
		std::vector<T* >::iterator last = T::get_list().end();
		while (first != last)
		{
			T* p = *first; ++first;
			::delete p;
		}
		T::get_list().erase(T::get_list().begin(), T::get_list().end());
	}
};
