#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <functional>

class tavla : public std::enable_shared_from_this<tavla>
{
protected:
	class slot : public std::enable_shared_from_this<slot>
	{
	private:
		std::weak_ptr<tavla> m_parent;
		std::shared_ptr<tavla> m_content;

	protected:
		template<class Class>
		std::shared_ptr<Class> set_content(std::shared_ptr<tavla> t_content)
		{
			m_content = t_content;
			return std::static_pointer_cast<Class>(m_parent.lock());
		}

	public:
		slot(std::shared_ptr<tavla> t_parent)
			: m_parent(t_parent)
		{}
		slot(const slot& o) = delete;

		inline virtual std::shared_ptr<tavla> get_content() const { return m_content; };

		template<class Class, typename Type>
		std::shared_ptr<Class> set_property(Type Class::* MemPtr, const Type& Value)
		{
			static_cast<Class* const>(this)->*MemPtr = Value;
			return std::static_pointer_cast<Class>(shared_from_this());
		}
	};

protected:

	template<class Class, typename Type>
	std::shared_ptr<Type> add_slot()
	{
		const auto slot = std::make_shared<Type>(std::static_pointer_cast<Class>(shared_from_this()));
		m_child_slots.push_back(slot);
		return slot;
	}

protected:

	std::vector<std::shared_ptr<tavla::slot>> m_child_slots;

	bool m_constructed;

public:

	tavla()
		: m_child_slots({})
		, m_constructed(false)
	{}
	~tavla() { destruct(); }
	  
	// ~begin tavla interface
	virtual void construct() {};
	virtual void destruct() {};
	virtual void tick(float t_delta_time) {};
	// ~end tavla interface

	static void tick_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla, const float t_delta_time);
	
	static void build_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla);

	static void traverse_tree(
		const std::shared_ptr<tavla>& t_root_tavla, 
		const std::function<void(const std::shared_ptr<tavla>&)>& callback
	);

	template<class Class, typename Type>
	std::shared_ptr<Class> set_property(Type Class::* MemPtr, Type Value)
	{
		static_cast<Class* const>(this)->*MemPtr = Value;
		return std::static_pointer_cast<Class>(shared_from_this());
	}

	inline const std::vector<std::shared_ptr<tavla::slot>> &get_child_slots() const { return m_child_slots; }
	
};