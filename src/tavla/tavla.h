#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>

class Tavla
{
protected:
	std::vector<std::shared_ptr<Tavla>> Children;

public:
	Tavla(std::initializer_list<std::shared_ptr<Tavla>> l)
	{
		Children = l;
	}

	virtual void Tick(float DeltaTime) {};

	static void TickTavlaTree(const std::shared_ptr<Tavla> &Root, const float DeltaTime)
	{
		Root->Tick(DeltaTime);

		for (const auto& child : Root->Children)
			if (child)
				TickTavlaTree(child, DeltaTime);
	}
};