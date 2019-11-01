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

#include "test.hpp"

int main()
{
	run_tests();

	std::shared_ptr<scene> game_scene = std::make_shared<scene>();

	const auto game_viewport = std::make_shared<tavla_viewport>()
		->set_property(&tavla_viewport::scene, std::weak_ptr<scene>(game_scene));

	const auto game_window = std::make_shared<tavla_window>()
		->set_property(&tavla_window::width, 800)
		->set_property(&tavla_window::height, 1000)
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
		game_scene->tick_scene(t_delta_time);
	}));

	// TODO: Input should probably be received by the game_viewport
	application->on_receive_input.bind(delegate<void, const input_event&>::create_function([&](const input_event& t_input_event) {
		game.on_input_event(t_input_event);
	}));

    return application->run();
}
