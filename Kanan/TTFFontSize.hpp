#pragma once

#include <Patch.hpp>

#include "PatchMod.hpp"

namespace kanan {
	class TTFFontSize : public PatchMod {
	public:
		TTFFontSize();

		void onPatchUI() override;

		void onConfigLoad(const Config& cfg) override;
		void onConfigSave(Config& cfg) override;

	private:
		Patch m_patch;
		int m_size;
		unsigned char m_originalByte;

		void applyPatch();
	};
}