#pragma once

#include "tavla.h"

class tavla_application : public tavla, std::enable_shared_from_this<tavla_application>
{
public:
	tavla_application()
	{
	}

	virtual void tick(float DeltaTime) override
	{
		std::cout << "Hello World" << "\n";
	}

	int run()
	{
		//auto test = shared_from_this();
		const auto sharedThis = std::shared_ptr<tavla_application>(this);
		float time = 0;
		while (true)
		{
			tavla::tick_tavla_tree(sharedThis, time);
			time += 1;
		}
	}
};