#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/tavlawindow.h"

void testfunc()
{
	std::cout << "Hello\n";
}

int main()
{
	const auto application = std::make_shared<tavla_application>();
	application
		/*->add_slot()
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
			->add_slot()
			->set_content
			(
				nullptr
			)
		)
		->add_slot()
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		)*/;

	tavla::build_tavla_tree(application);

	auto test = delegate<void>::create_lambda(testfunc);
	test.execute_if_bound();

	multicast_delegate<void> multicast_delegates;
	multicast_delegates.bind(test);

	multicast_delegates.broadcast();

    return application->run();
}