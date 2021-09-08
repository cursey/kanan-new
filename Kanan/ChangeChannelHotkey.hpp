#pragma once

#include <FunctionHook.hpp>

#include "Mod.hpp"



namespace kanan {
	class ChangeChannelHotkey : public Mod
	{
	public:
		ChangeChannelHotkey();

		void onFrame() override;
		void onUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

		void onKeyUp(DWORD key);

		void onKeyDown(DWORD key);

		void ChangeCh(int ch);

	private:
		bool m_is_enabled{};

		bool modiDown{};




	};
}
