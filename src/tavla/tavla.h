#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <functional>

struct dimensions
{
	int width	= 100;
	int height	= 100;
	int x_pos	= 0;
	int y_pos	= 0;
};

class tavla : public std::enable_shared_from_this<tavla>
{
protected:
	class slot : public std::enable_shared_from_this<slot>
	{
		friend class tavla;
	protected:
		std::weak_ptr<tavla> m_parent;
		std::shared_ptr<tavla> m_content;
		dimensions m_dimensions;

	public:
		slot(std::shared_ptr<tavla> t_parent)
			: m_parent(t_parent)
		{}
		slot(const slot& o) = delete;
		slot(slot* const o) = delete;

		inline const std::shared_ptr<tavla> &get_content() const { return m_content; };
		inline const std::weak_ptr<tavla> &get_parent_tavla() const { return m_parent; };
		inline const dimensions& get_dimensions() const { return m_dimensions; };

		inline void set_parent_tavla(const std::weak_ptr<tavla>& t_parent) { m_parent = t_parent; };
	};

protected:

	std::weak_ptr<tavla::slot> m_parent_slot;

	std::vector<std::shared_ptr<tavla::slot>> m_child_slots;

	bool m_constructed;

public:

	tavla()
		: m_child_slots({})
		, m_constructed(false)
	{}
	~tavla() { destruct(); }

	tavla(const tavla&) = delete;
	tavla(tavla&&) = delete;
	void operator=(const tavla&) {}

	// ~begin tavla interface
	virtual void construct() {};
	virtual void destruct() {};
	virtual void tick(float t_delta_time) {};
	virtual void draw() {};
	virtual void post_draw() {};
	virtual dimensions calculate_slot_dimentions(const std::shared_ptr<tavla::slot>& t_slot) const { return get_tavla_dimentions(); };
	// ~end tavla interface

	inline const std::vector<std::shared_ptr<tavla::slot>>& get_child_slots() const { return m_child_slots; }
	inline const std::weak_ptr<tavla::slot>& get_parent_slot() const { return m_parent_slot; }
	
	virtual dimensions get_tavla_dimentions() const;

	inline void set_parent_slot(const std::weak_ptr<tavla::slot>& t_parent_slot) { m_parent_slot = t_parent_slot; };

	static void tick_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla, const float t_delta_time);
	static void draw_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla);
	static void construct_tavla_tree(const std::shared_ptr<tavla>& t_root_tavla);
	static void calculate_tavla_tree_dimensions(const std::shared_ptr<tavla>& t_root_tavla);

	static void traverse_tree(
		const std::shared_ptr<tavla>& t_root_tavla,
		const std::function<void(const std::shared_ptr<tavla>&)>& callback
	);
};

template<class tavla_class>
class tavla_base : public tavla
{
protected:
	template<class slot_class>
	class slot : public tavla::slot
	{
	public:
		slot(const std::shared_ptr<tavla_base> &t_parent)
			: tavla::slot(t_parent)
		{}

		template<typename Type>
		std::shared_ptr<slot_class> set_property(Type slot_class::* MemPtr, const Type& Value)
		{
			static_cast<slot_class* const>(this)->*MemPtr = Value;
			return std::static_pointer_cast<slot_class>(shared_from_this());
		}

		std::shared_ptr<tavla_class> set_content(const std::shared_ptr<tavla>& t_content)
		{
			m_content = t_content;
			t_content->set_parent_slot(weak_from_this());
			return std::static_pointer_cast<tavla_class>(m_parent.lock());
		}
	};

	template<typename slot_class>
	std::shared_ptr<slot_class> add_slot_of_type()
	{
		const auto slot = std::make_shared<slot_class>(std::static_pointer_cast<tavla_class>(shared_from_this()));
		slot->set_parent_tavla(weak_from_this());
		m_child_slots.push_back(slot);
		return slot;
	}

public:

	template<typename Type>
	std::shared_ptr<tavla_class> set_property(Type tavla_class::* MemPtr, Type Value)
	{
		static_cast<tavla_class* const>(this)->*MemPtr = Value;
		return std::static_pointer_cast<tavla_class>(shared_from_this());
	}
	
};