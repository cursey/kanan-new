#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "TTFFontSize.hpp"

using namespace std;

namespace kanan {
	TTFFontSize::TTFFontSize()
		: PatchMod{ "TTF Font Size", "" },
		m_choice{ 0 },
		m_patch{},
		m_originalByte{}

	{
		// Pattern for locating location of the byte that controls text font size
		auto address = scan("client.exe", "41 C7 86 D0 00 00 00 ? ? ? ? 41 C7 86 D4 00 00 00");
		if (address) {
			log("[TTF Font Size] Found address TTF Font Size %p", *address);

			m_patch.address = *address + 7;
			m_originalByte = { *reinterpret_cast<unsigned char*>(m_patch.address) };
                } else {
                    log("[TTF Font Size] Failed to find TTFFontSize address.");
                }
	}
	void TTFFontSize::onPatchUI() {
		if (m_patch.address == 0) {
			return;
		}

		ImGui::Text("TTF Original Font Size: %d", m_originalByte);
		if (ImGui::InputInt("TTF Font Size", &m_choice)) {
			if (m_choice < 0) {
				m_choice = 0;
			}
			else if (m_choice > 255) {
				m_choice = 255;
			}
			apply();
		}
	}

	void TTFFontSize::onConfigLoad(const Config& cfg) {
		m_choice = cfg.get<int>("TTFFontSize.Choice").value_or(0);

		if (m_choice != 0) {
			apply();
		}
		else {
			m_choice = m_originalByte;
		}
	}

	void TTFFontSize::onConfigSave(Config& cfg) {
		cfg.set<int>("TTFFontSize.Choice", m_choice);
	}


	void TTFFontSize::apply() {
		if (m_patch.address == 0) {
			return;
		}
		// Set back to default size if 0 is selected
		if (m_choice == 0) {
			m_choice = m_originalByte;
			short choice = m_choice;
			m_patch.bytes = { choice };
			patch(m_patch);
		}
		else{
			log("[TTF Font Size] Applying TTFFontSize...");
			short choice = m_choice;
			m_patch.bytes = { choice };
			patch(m_patch);
		}
	}
}