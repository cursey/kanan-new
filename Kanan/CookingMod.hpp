#pragma once
#include "FunctionHook.hpp"
#include "Mod.hpp"
namespace kanan {
	class CookingMod :
		public Mod
	{
	public:
		CookingMod();		

		void onUI() override;

		void cookingMath(int cookingTemp[3]);


		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

		//void onKeyUp(DWORD key);

		//void onKeyDown(DWORD key);

	private:
        std::unique_ptr<FunctionHook> m_hook;
		bool m_is_enabled{};
        static void __fastcall HookForCooking(int* _this, int ingredientIdx, int amount);
	};
}
