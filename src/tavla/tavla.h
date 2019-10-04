#pragma once
#include <vector>
#include <stdlib.h>
#include <memory>
#include <iostream>

class Tavla : public std::enable_shared_from_this<Tavla>
{
public:
	class Slot : public std::enable_shared_from_this<Tavla::Slot>
	{
		friend Tavla;

	protected:
		std::shared_ptr<Tavla> Content;

	public:
		int Width;

	private:
		Slot() = default;
		Slot(const Slot& o) = default;

	public:
		inline virtual std::shared_ptr<Tavla> GetContent() const { return Content; };

		std::shared_ptr<Tavla::Slot> SetContent(const std::shared_ptr<Tavla> InContent) { Content = InContent; return std::shared_ptr<Tavla::Slot>(this); }

		template<class T>
		std::shared_ptr<Tavla::Slot> SetProperty(T Tavla::Slot::* MemPtr, const T &Value)
		{
			(*this).*MemPtr = Value;
			return std::shared_ptr<Tavla::Slot>(this);
		}
	};

	static std::shared_ptr<Tavla::Slot> CreateSlot()
	{
		return std::shared_ptr<Tavla::Slot>(new Tavla::Slot);
	}

protected:
	std::vector<std::shared_ptr<Tavla::Slot>> ChildSlots;

public:
	Tavla(std::initializer_list<std::shared_ptr<Tavla::Slot>> l)
	{
		ChildSlots = l;
	}

	virtual void Tick(float DeltaTime) {};

	inline const std::vector<std::shared_ptr<Tavla::Slot>> &GetChildSlots() const { return ChildSlots; }

	static void TickTavlaTree(const std::shared_ptr<Tavla> &RootTavla, const float DeltaTime)
	{
		struct Local
		{
			static void TickSlotsRecursive(const std::shared_ptr<Tavla::Slot> &Slot, const float DeltaTime)
			{
				const auto SlotContent = Slot ? Slot->GetContent() : nullptr;
				if (SlotContent)
				{
					SlotContent->Tick(DeltaTime);

					for (const auto& ChildSlot : SlotContent->GetChildSlots())
						if (ChildSlot)
							TickSlotsRecursive(ChildSlot, DeltaTime);
				}
			}
		};

		for (const auto & Slot : RootTavla->GetChildSlots())
		{
			Local::TickSlotsRecursive(Slot, DeltaTime);
		}
	}
};