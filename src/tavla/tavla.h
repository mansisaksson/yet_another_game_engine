#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>

class tavla : public std::enable_shared_from_this<tavla>
{
public:
	class slot : public std::enable_shared_from_this<slot>
	{
		friend tavla;

	private:
		std::shared_ptr<tavla> m_parent;
		std::shared_ptr<tavla> m_content;

	public:
		int width;
		int height;

	public:
		slot(std::shared_ptr<tavla> t_parent)
			: m_parent(t_parent)
			, width(100)
			, height(100)
		{}
		slot(const slot& o) = default;

		inline virtual std::shared_ptr<tavla> get_content() const { return m_content; };

		std::shared_ptr<tavla> set_content(std::shared_ptr<tavla> t_content)
		{
			m_content = t_content;
			return m_parent;
		}

		template<typename Type, class Class>
		std::shared_ptr<Class> set_property(Type Class::* MemPtr, const Type& Value)
		{
			static_cast<Class* const>(this)->*MemPtr = Value;
			return std::static_pointer_cast<Class>(shared_from_this());
		}
	};

protected:
	std::vector<std::shared_ptr<tavla::slot>> m_child_slots;

public:
	tavla()
		: m_child_slots({})
	{}

	template<typename Type>
	std::shared_ptr<Type> add_slot()
	{
		const auto slot = std::make_shared<Type>(shared_from_this());
		m_child_slots.push_back(slot);
		return slot;
	}

	template<class Class, typename Type>
	std::shared_ptr<Class> set_property(Type Class::* MemPtr, Type Value)
	{
		static_cast<Class*const>(this)->*MemPtr = Value;
		return std::static_pointer_cast<Class>(shared_from_this());
	}

	virtual void tick(float t_delta_time) {};

	inline const std::vector<std::shared_ptr<tavla::slot>> &get_child_slots() const { return m_child_slots; }

	static void tick_tavla_tree(const std::shared_ptr<tavla> &t_root_tavla, const float t_delta_time)
	{
		struct local
		{
			static void tick_slots_recursive(const std::shared_ptr<tavla::slot> &t_slot, const float t_delta_time)
			{
				const auto slotContent = t_slot ? t_slot->get_content() : nullptr;
				if (slotContent)
				{
					slotContent->tick(t_delta_time);

					for (const auto& child_slot : slotContent->get_child_slots())
					{
						if (child_slot)
							tick_slots_recursive(child_slot, t_delta_time);
					}
				}
			}
		};
		
		t_root_tavla->tick(t_delta_time);

		for (const auto & slot : t_root_tavla->get_child_slots())
		{
			local::tick_slots_recursive(slot, t_delta_time);
		}
	}
};