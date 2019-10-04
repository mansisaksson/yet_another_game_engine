#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>

class tavla
{
protected:
	std::vector<std::shared_ptr<tavla>> Children;

public:
	tavla(std::initializer_list<std::shared_ptr<tavla>> l)
	{
		Children = l;
	}

	virtual void Tick(float DeltaTime) {};

	static void TicktavlaTree(const std::shared_ptr<tavla> &Root, const float DeltaTime)
	{
		Root->Tick(DeltaTime);

		for (const auto& child : Root->Children)
			if (child)
				TicktavlaTree(child, DeltaTime);
	}
};

class tavlaWindow : public tavla
{

public:
	tavlaWindow(int width, int height, std::initializer_list<std::shared_ptr<tavla>> l)
		: tavla(l)
	{

	}

	virtual void Tick(float DeltaTime) override
	{
		
	}
};

class tavlaApplication : public tavla, std::enable_shared_from_this<tavlaApplication>
{
public:
	tavlaApplication(std::initializer_list<std::shared_ptr<tavla>> l)
		: tavla(l)
	{

	}

	virtual void Tick(float DeltaTime) override
	{
		std::cout << "Hello World" << "\n";
	}

	int Run()
	{
		const auto sharedThis = std::shared_ptr<tavlaApplication>(this);
		float time = 0;
		while (true)
		{
			tavla::TicktavlaTree(sharedThis, time);
			time += 1;
		}
	}
};