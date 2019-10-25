#include "core/core.h"
#include "tavla/tavla.h"
#include "tavla/tavlaapplication.h"
#include "tavla/tavlawindow.h"
#include "tavla/tavlaviewport.h"
#include "tavla/primitives/verticalbox.h"
#include "tavla/primitives/horizontalbox.h"
#include "assetmanager/assetptr.h"
#include "rendercore/rendercore.h"

#include "gameframework/scene.h"
#include "gameframework/entity.h"

int main()
{
	std::shared_ptr<scene> test_scene = std::make_shared<scene>();
	test_scene->spawn_entity<entity>(transform::identity);
	test_scene->spawn_entity<entity>(transform(vector3(0, 5, 0), quaternion::identity, 1.f));
	test_scene->spawn_entity<entity>(transform(vector3(0, -5, 0), quaternion::identity, 2.f));

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
						->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(test_scene))
					)
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
						->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(test_scene))
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
						->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(test_scene))
					)
					->add_slot()
					->set_content
					(
						std::make_shared<tavla_viewport>()
						->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(test_scene))
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

    return application->run();
}
