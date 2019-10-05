#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/tavlawindow.h"

int main()
{
	const auto application = std::make_shared<tavla_application>();
	application
		->add_slot<tavla::slot>()
		->set_property(&tavla::slot::width, 10)
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		)
		->add_slot<tavla::slot>()
		->set_property(&tavla::slot::width, 10)
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		);

    return application->run();
}