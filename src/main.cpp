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

#include "game/game.hpp"

int main()
{
	// test matrix 3x3
	// test compared to: https://www.wikihow.com/Find-the-Inverse-of-a-3x3-Matrix
	/*{
		log::info("main", "***** matrix3x3 *****");
		matrix3x3 test3x3 = {
			{ 1, 2, 3 },
			{ 0, 1, 4 },
			{ 5, 6, 0 },
		};

		test3x3.inverse();
		log::info("main", "matrix: %s", test3x3.to_string().c_str());

		log::info("main", "matrix determinant: %f", test3x3.determinant());

		log::info("main", "inv_matrix: %s\n\n", test3x3.inverse().to_string().c_str());
	}*/

	// test matrix 4x4
	// compared to: https://semath.info/src/inverse-cofactor-ex4.html
	/*{
		log::info("main", "***** matrix4x4 *****");
		matrix4x4 test4x4 = {
			{  1,  1,  1, -1 },
			{  1,  1, -1,  1 },
			{  1, -1,  1,  1 },
			{ -1,  1,  1,  1 },
		};

		log::info("main", "matrix determinant: %f", test4x4.determinant());

		log::info("main", "inv_matrix: %s\n\n", test4x4.inverse().to_string().c_str());
	}*/

	std::shared_ptr<scene> game_scene = std::make_shared<scene>();

	const auto game_viewport = std::make_shared<tavla_viewport>()
		->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(game_scene));

	const auto game_window = std::make_shared<tavla_window>()
		->set_property(&tavla_window::width, 1280)
		->set_property(&tavla_window::height, 720)
		->add_slot()
		->set_content
		(
			game_viewport
		);

	const auto application = std::make_shared<tavla_application>();
	application
		->add_slot()
		->set_content
		(
			game_window
		)
		/*->add_slot()
		->set_content
		(
			std::make_shared<tavla_window>()
			->set_property(&tavla_window::width, 1280)
			->set_property(&tavla_window::height, 720)
		)*/;

	tavla::construct_tavla_tree(application);

	game game;
	game.start_game(game_scene.get(), game_viewport.get());

	application->on_tick_application.bind(delegate<void, float>::create_function([&](float t_delta_time) {
		game.tick_game(t_delta_time);
	}));

	// TODO: Input should probably be received by the game_viewport
	application->on_receive_input.bind(delegate<void, const input_event&>::create_function([&](const input_event& t_input_event) {
		game.on_input_event(t_input_event);
	}));

    return application->run();
}
