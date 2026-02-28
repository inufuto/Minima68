#pragma once
#include <cassert>
#include "ApiException.h"

template<class T>
class ComPtr
{
private:
	T* p;
public:
	ComPtr() : p{ nullptr } {}
	ComPtr(ComPtr&) = delete;
	ComPtr(ComPtr<T>&& comPtr) noexcept : p{ comPtr.Detach() } {}
	template<class S> ComPtr(ComPtr<S>&& comPtr) noexcept : p{ comPtr.Detach() } {}
	~ComPtr() { Release(); }
protected:
	ComPtr(T* p) :p{ p } {}
public:
	static ComPtr<T> FromInterface(T* p) { return p; }
	operator T* () const { return p; }
	auto operator->() const { return p; }
	ComPtr& operator =(ComPtr<T>&& comPtr)
	{
		Release();
		this->p = comPtr.p;
		comPtr.p = nullptr;
		return *this;
	}
	T** operator&()
	{
		assert(p == nullptr);
		return &p;
	}
	void Release()
	{
		if (p != nullptr) {
			p->Release();
			p = nullptr;
		}
	}

	void CoCreateInstance(REFCLSID rclsid)
	{
		assert(p == NULL);
		Call(::CoCreateInstance(rclsid, nullptr, CLSCTX_ALL, __uuidof(T), reinterpret_cast<void**>(&p)));
	}
	static auto Create(REFCLSID rclsid)
	{
		T* p;
		Call(::CoCreateInstance(rclsid, nullptr, CLSCTX_ALL, __uuidof(T), reinterpret_cast<void**>(&p)));
		return ComPtr<T>{p};
	}
	template<class T2>
	ComPtr<T2> QueryInterface(REFCLSID rclsid)
	{
		ComPtr<T2> p2;
		Call(p->QueryInterface(rclsid, reinterpret_cast<LPVOID*>(&p2)));
		return p2;
	}

	void Attach(T* p)
	{
		Release();
		this->p = p;
	}

	T* Detach()
	{
		auto p = this->p;
		this->p = nullptr;
		return p;
	}
};
