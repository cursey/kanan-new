#include "BitmapFont.hpp"
#include "ShowCombatPower.hpp"
#include "ElfLagFix.hpp"
#include "WindowsAppearFaster.hpp"
#include "OneClickRevive.hpp"

#include "AutoSetMTU.hpp"
#include "DisableNagle.hpp"
#include "BorderlessWindow.hpp"
#include "EntityViewer.hpp"

#include "Log.hpp"
#include "Mods.hpp"

using namespace std;

namespace kanan {
    Mods::Mods()
        : m_mods{}
    {
        g_log << "Entering Mods cosntructor." << endl;

        m_mods.emplace_back(make_unique<BitmapFont>());
        m_mods.emplace_back(make_unique<ShowCombatPower>());
        m_mods.emplace_back(make_unique<ElfLagFix>());
        m_mods.emplace_back(make_unique<WindowsAppearFaster>());
        m_mods.emplace_back(make_unique<OneClickRevive>());

        m_mods.emplace_back(make_unique<AutoSetMTU>());
        m_mods.emplace_back(make_unique<DisableNagle>());
        m_mods.emplace_back(make_unique<BorderlessWindow>());
        m_mods.emplace_back(make_unique<EntityViewer>());

        g_log << "Leaving Mods constructor." << endl;
    }
}