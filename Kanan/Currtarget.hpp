#pragma once

#include <forward_list>

#include "Mod.hpp"
#include "Mabinogi.hpp"
#include "KCharacter.hpp"

namespace kanan {
	class Currtarget : public Mod
	{
	public:
		Currtarget();

		void onFrame() override;
		void onUI() override;
		void createtargetwindow();
		float ffdiv(float x, float y);
		void progressBar(float fraction, const ImVec2& sizeArg, ImU32 color, const char* overlay);
		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;
		void onKeyUp(DWORD key);
		void onKeyDown(DWORD key);

	private:
		bool m_is_enabled{};
		bool hastarget{};
		bool showpos{};
		bool showothertarget{};
		bool showtargethp{};
		std::forward_list<KCharacter*> m_characters;
		std::string m_hp;
	};
}