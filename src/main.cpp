#include "core/core.h"
#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/tavlawindow.h"
#include "tavla/tavlaviewport.h"
#include "tavla/primitives/verticalbox.h"
#include "tavla/primitives/horizontalbox.h"
#include "assetmanager/assetptr.h"
#include "rendercore/rendercore.h"

void testfunc()
{
	int i = 1337;
	float f = 1.1234f;
	std::string s = "this is a string";
	log::info("test_log", "log int: %d, log float: %f, log string: %s", i, f, s.c_str());
}

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
				std::make_shared<vertical_box>()
				->add_slot()
				->set_content
				(
					std::make_shared<horizontal_box>()
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
					)
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
					)
				)
				->add_slot()
				->set_content
				(
					std::make_shared<horizontal_box>()
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
					)
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
					)
				)
			)
		)
		/*->add_slot()
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		)*/;

	tavla::construct_tavla_tree(application);

	//auto test = delegate<void>::create_lambda(testfunc);
	//test.execute_if_bound();
	//multicast_delegate<void> multicast_delegates;
	//multicast_delegates.bind(test);
	//multicast_delegates.broadcast();

	//asset_ptr<static_mesh> static_mesh_ptr("basic_mesh");
	//asset_ptr<texture> texture_ptr("basic_texture");
	//asset_ptr<material> my_material("basic_mat");

    return application->run();
}
