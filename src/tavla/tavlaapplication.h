#pragma once

#include "tavla.h"

class TavlaApplication : public Tavla, std::enable_shared_from_this<TavlaApplication>
{
public:
	TavlaApplication(std::initializer_list<std::shared_ptr<Tavla::Slot>> l)
		: Tavla(l)
	{

	}

	virtual void Tick(float DeltaTime) override
	{
		std::cout << "Hello World" << "\n";
	}

	int Run()
	{
		const auto sharedThis = std::shared_ptr<TavlaApplication>(this);
		float time = 0;
		while (true)
		{
			Tavla::TickTavlaTree(sharedThis, time);
			time += 1;
		}
	}
};