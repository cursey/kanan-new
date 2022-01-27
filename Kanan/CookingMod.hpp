#pragma once
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include "Mod.hpp"
namespace kanan {
	class CookingMod :
		public Mod
	{
	public:
		CookingMod();
		void applycook(bool m_cooking_is_enabled);

		void onUI() override;

		void cookingMath(int cookingTemp[3]);


		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

		//void onKeyUp(DWORD key);

		//void onKeyDown(DWORD key);

	private:
		bool m_is_enabled{};
	};
}
