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

class TavlaWindow : public Tavla
{

public:
	TavlaWindow(int width, int height, std::initializer_list<std::shared_ptr<Tavla>> l)
		: Tavla(l)
	{

	}

	virtual void Tick(float DeltaTime) override
	{
		
	}
};

class TavlaApplication : public Tavla, std::enable_shared_from_this<TavlaApplication>
{
public:
	TavlaApplication(std::initializer_list<std::shared_ptr<Tavla>> l)
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