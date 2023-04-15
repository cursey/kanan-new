#include <imgui.h>

#include "Kanan.hpp"
#include "DrawDistance.hpp"

namespace kanan {

	DrawDistance::DrawDistance() 
		: m_distance { 25000 }
		, m_isEnabled { false } 
	{ }

	void DrawDistance::onFrame() {
        auto game = g_kanan->getGame();

        //get render, if no render return
		auto renderer = game->getRenderer();	
        if (renderer == nullptr) {
            return;
        }
        //get camera, if no camera return
        auto camera = renderer->camera;
        if (camera == nullptr) {
            return;
        }
        //get cameraState, if no cameraState return
        auto cameraState = camera->state;
        if (cameraState == nullptr) {
            return;
        }
        //if enabled set to wanted draw distance
        if (m_isEnabled) {
            cameraState->drawDistance = m_distance;
        }
	}

    void DrawDistance::onUI() {
        if (ImGui::CollapsingHeader("Draw Distance")) {
            ImGui::Checkbox("Enable Draw Distance", &m_isEnabled);
            ImGui::Text("This is likely to cause lag if too high");
            ImGui::SliderFloat("Distance", &m_distance, 0.0f, 500000.0f, "%.0f");
            if (ImGui::Button("Reset")) {
                m_distance = 25000.0f;
            }
        }
    }

    void DrawDistance::onConfigLoad(const Config& cfg) {
        m_distance = cfg.get<float>("DrawDistance.Vaule").value_or(25000.0f);
        m_isEnabled = cfg.get<bool>("DrawDistance.Enabled").value_or(m_isEnabled);
    }

    void DrawDistance::onConfigSave(Config& cfg) {
        cfg.set<float>("DrawDistance.Vaule", m_distance);
        cfg.set<bool>("DrawDistance.Enabled", m_isEnabled);
    }

}