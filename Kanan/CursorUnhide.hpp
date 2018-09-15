#pragma once

#include <cstdint>

#include "Mod.hpp"
#include "Patch.hpp"

namespace kanan {
	class CursorUnhide : public Mod {
	public:
		CursorUnhide();

		void onUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

	private:
		bool m_isEnabled;
		Patch m_patch;

		void apply();
	};
}
