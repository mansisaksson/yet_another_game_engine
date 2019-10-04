#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/twindow.h"

int main()
{
	const auto application = std::shared_ptr<tavla_application>(new tavla_application);
	application
		->add_slot<tavla::slot>()
		->set_property(&tavla::slot::width, 10)
		->set_content
		(
			std::shared_ptr<tavla_window>(new tavla_window)
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		);

    return application->run();
}