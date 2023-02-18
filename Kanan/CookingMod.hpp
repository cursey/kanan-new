#pragma once
#include "Kanan.hpp"
#include <Scan.hpp>
#include <Utility.hpp>
#include <imgui.h>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <Utility.hpp>
#include <iostream>
#include "Mod.hpp"
namespace kanan {
	class CookingMod :
		public Mod
	{
	public:
		CookingMod();
		void applycook(bool m_cooking_is_enabled);

		void onUI() override;


		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;
	private:
		bool m_is_enabled{};

		int cookingOne = 0;
		int cookingTwo = 0;
		int cookingThree = 0;
	};
}
