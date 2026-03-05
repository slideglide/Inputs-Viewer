#include "IVEvent.hpp"

using namespace geode::prelude;

GEODE_NS_IV_BEGIN

IVSettingEvent::IVSettingEvent(SettingEventType type)
    : GlobalEvent(type), m_type(type) {}

IVSettingEvent::IVSettingEvent()
    : GlobalEvent(), m_type(SettingEventType::General) {}

SettingEventType IVSettingEvent::getType() const noexcept {
    return m_type;
}

GEODE_NS_IV_END