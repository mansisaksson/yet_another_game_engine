#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/tavlawindow.h"

int main()
{
	const auto application = std::make_shared<tavla_application>();
	application
		->add_slot()
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
		);

	tavla::build_tavla_tree(application);

    return application->run();
}