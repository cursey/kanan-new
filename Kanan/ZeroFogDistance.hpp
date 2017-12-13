#pragma once

#include <string>
#include <vector>

#include <Patch.hpp>

#include "Mod.hpp"

namespace kanan {
	// Easier to use Mod that works for any mods that are just simple toggleable patches.
	class ZeroFogDistance : public Mod {
	public:
		ZeroFogDistance();

		void onPatchUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

	private:
		bool m_isEnabled;
		Patch m_Patch1;
		Patch m_Patch2;

		void apply();
	};
}
