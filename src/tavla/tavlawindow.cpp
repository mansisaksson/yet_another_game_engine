#include "tavlawindow.h"
#include "platform/platform.h"
#include "platform/platformwindow.h"

void tavla_window::construct()
{
	m_window = platform::create_window();
	m_window->init(width, height, title);
	m_window->on_window_closed.bind(delegate<void, platform_window&>::create_weak_function(weak_from_this(), [&](platform_window&) {
		m_window.reset();
	}));
}

void tavla_window::destruct()
{
	m_window->close_window();
	m_window.reset();
}

void tavla_window::tick(float t_delta_time)
{
	
}

void tavla_window::draw()
{
	if (!m_window)
		return;

	m_window->make_current();
	m_window->clear(color::blue);
}

void tavla_window::post_draw()
{
	if (!m_window)
		return;

	m_window->swap_buffers();
}

dimensions tavla_window::get_tavla_dimentions() const
{
	const auto window_size = get_window_size();
	return { std::get<0>(window_size), std::get<1>(window_size), 0, 0 };
}

std::tuple<int, int> tavla_window::get_window_size() const
{
	return m_window ? m_window->get_window_size() : std::tuple<int, int>{ 0, 0 };
}

bool tavla_window::has_valid_context() const
{
	return m_window ? true : false;
}

std::weak_ptr<tavla_window> tavla_window::find_parent_window(const std::weak_ptr<tavla> &t_tavla)
{
	struct local
	{
		static std::weak_ptr<tavla_window> find_window_recursive(const std::weak_ptr<tavla>& t_tavla)
		{
			if (t_tavla.expired())
				return std::weak_ptr<tavla_window>();

			const auto tavla_widow = std::dynamic_pointer_cast<tavla_window>(t_tavla.lock());
			if (tavla_widow)
			{
				return tavla_widow;
			}

			const auto parent_slot = t_tavla.lock()->get_parent_slot();
			if (!parent_slot.expired())
			{
				const auto parent_tavla = parent_slot.lock()->get_parent_tavla();
				if (!parent_tavla.expired())
					return find_window_recursive(parent_tavla.lock());
			}

			return std::weak_ptr<tavla_window>();
		}
	};
	
	return local::find_window_recursive(t_tavla);
}