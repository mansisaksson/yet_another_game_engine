#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <functional>
#include <algorithm>
#include "guid.h"

template<typename RetV, typename ... Types>
class delegate
{
	typedef delegate<RetV, Types ...> DelegateType;

private:
	bool m_is_weak_delegate;
	std::weak_ptr<void> m_object_ptr;
	std::function<RetV(Types ...)> m_func;

public:

	delegate()
		: m_is_weak_delegate(false)
		, m_object_ptr(nullptr)
	{}

	delegate(const delegate&) = default;
	delegate(delegate&&) = default;

	delegate(const std::function<RetV(Types ...)>& t_func)
		: m_is_weak_delegate(false)
		, m_object_ptr(std::weak_ptr<void>())
		, m_func(t_func)
	{}

	delegate(const std::weak_ptr<void> &t_object_ptr, const std::function<RetV(Types ...)>& t_func)
		: m_is_weak_delegate(true)
		, m_object_ptr(t_object_ptr)
		, m_func(t_func)
	{}

	static delegate<RetV, Types ...> create_lambda(const std::function<RetV(Types ...)> &func)
	{
		return delegate<RetV, Types ...>(func);
	}

	static delegate<RetV, Types ...> create_weak_lambda(const std::weak_ptr<void> &object_ptr, const std::function<RetV(Types ...)>& func)
	{
		return delegate<RetV, Types ...>(object_ptr, func);
	}

	/*static delegate<RetV, Types ...> create_static(const std::function<RetV(Types ...)> &func)
	{
		return delegate<RetV, Types ...>();
	}*/

	bool is_bound() const
	{
		if (m_is_weak_delegate)
			return !m_object_ptr.expired() && (m_func ? true : false);

		return m_func ? true : false;
	}

	void execute()
	{
		m_func();
	}

	void execute_if_bound()
	{
		if (is_bound())
			m_func();
	}
};

template<typename RetV, typename ... Types>
class multicast_delegate
{
	typedef delegate<RetV, Types ...> DelegateType;

private:
	std::vector<DelegateType> delegates;
	std::vector<guid> delegate_ids;

public:

	guid bind(const DelegateType& delegate)
	{
		delegates.push_back(delegate);
		const auto guid = guid::new_guid();
		delegate_ids.push_back(guid);
		return guid;
	}

	void remove(const guid& delegate_id)
	{
		const auto result = std::find(delegate_ids.begin(), delegate_ids.end(), delegate_id);
		if (result != std::end(delegate_ids))
		{
			const int index = *result;
			delegate_ids.erase(vec.begin() + 1);
		}
	}

	void broadcast()
	{
		/*delegate_ids.erase(
			std::remove_if(delegate_ids.begin(), delegate_ids.end(), [&](const auto& delegate)->bool { return !delegate.is_bound(); }),
			delegate_ids.end()
		);*/

		for (auto &delegate : delegates)
		{
			delegate.execute_if_bound();
		}
	}
};