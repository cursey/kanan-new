#include <imgui.h>

#include "Kanan.hpp"
#include "FieldOfView.hpp"

namespace kanan {
    FieldOfView::FieldOfView()
        : m_fov{ 45.0f }
    { }

    void FieldOfView::onFrame() {
        auto game = g_kanan->getGame();
        auto renderer = game->getRenderer();

        if (renderer == nullptr) {
            return;
        }

        auto camera = renderer->camera;

        if (camera == nullptr) {
            return;
        }

        auto cameraState = camera->state;

        if (cameraState == nullptr) {
            return;
        }

        if (m_isEnabled) {
            cameraState->fov = m_fov;
        }
    }

    void FieldOfView::onUI() {
        if (ImGui::CollapsingHeader("Field Of View")) {
            ImGui::Checkbox("Enable Field Of View", &m_isEnabled);
            ImGui::SliderFloat("FOV", &m_fov, 10.0f, 120.0f, "%.0f");
            if (ImGui::Button("Reset")) {
                m_fov = 45.0f;
            }
        }
    }

    void FieldOfView::onConfigLoad(const Config& cfg) {
        m_fov = cfg.get<float>("FieldOfView.FOV").value_or(45.0f);
        m_isEnabled = cfg.get<bool>("FieldOfView.Enabled").value_or(m_isEnabled);
    }

    void FieldOfView::onConfigSave(Config& cfg) {
        cfg.set<float>("FieldOfView.FOV", m_fov);
        cfg.set<bool>("FieldOfView.Enabled", m_isEnabled);
    }
}
