#pragma once
#include <Geode/loader/Event.hpp>
#include "macros.hpp"

GEODE_NS_IV_BEGIN

enum class SettingEventType {
    Color,
    General,
    KeyAppearance,
    RefreshView
};

class IVSettingEvent : public geode::GlobalEvent<
    IVSettingEvent, 
    bool(),
    SettingEventType
> {
protected:
    SettingEventType m_type;

public:
    IVSettingEvent(SettingEventType type);
    IVSettingEvent();
    SettingEventType getType() const noexcept;
};

GEODE_NS_IV_END