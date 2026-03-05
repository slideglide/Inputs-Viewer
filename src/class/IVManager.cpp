#include "IVManager.hpp"
#include <IVEvent.hpp>

using namespace geode::prelude;

GEODE_NS_IV_BEGIN

IVManager::IVManager()
: m_isInSetting(false)
, m_backgroundPressColor(Mod::get()->getSettingValue<ccColor4B>("background-press-color"))
, m_backgroundReleaseColor(Mod::get()->getSettingValue<ccColor4B>("background-release-color"))
, m_outlinePressColor(Mod::get()->getSettingValue<ccColor4B>("outline-press-color"))
, m_outlineReleaseColor(Mod::get()->getSettingValue<ccColor4B>("outline-release-color"))
, m_textPressColor(Mod::get()->getSettingValue<ccColor4B>("text-press-color"))
, m_textReleaseColor(Mod::get()->getSettingValue<ccColor4B>("text-release-color"))
, m_settingClassic(
    Mod::get()->getSavedValue<LevelSettings>("classic", {
        .p1Transform = IVManager::getDefaultP1Transform(),
        .p2Transform = IVManager::getDefaultP2Transform()
    })
)
, m_settingPlatformer(
    Mod::get()->getSavedValue<LevelSettings>("platformer", {
        .p1Transform = IVManager::getDefaultP1Transform(),
        .p2Transform = IVManager::getDefaultP2Transform()
    })
)
{
    geode::listenForSettingChanges<ccColor4B>("background-press-color", [this](ccColor4B color) {
        m_backgroundPressColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
    
    geode::listenForSettingChanges<ccColor4B>("background-release-color", [this](ccColor4B color) {
        m_backgroundReleaseColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
    
    geode::listenForSettingChanges<ccColor4B>("outline-press-color", [this](ccColor4B color) {
        m_outlinePressColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
    
    geode::listenForSettingChanges<ccColor4B>("outline-release-color", [this](ccColor4B color) {
        m_outlineReleaseColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
    
    geode::listenForSettingChanges<ccColor4B>("text-press-color", [this](ccColor4B color) {
        m_textPressColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
    
    geode::listenForSettingChanges<ccColor4B>("text-release-color", [this](ccColor4B color) {
        m_textReleaseColor = color;
        IVSettingEvent(SettingEventType::Color).send();
    });
}

IVManager& IVManager::get() {
    static auto inst = std::make_unique<IVManager>();
    return *inst;
}

NodeTransform IVManager::getDefaultP1Transform() {
    auto winSize = CCDirector::get()->getWinSize();
    auto maxWidth = winSize.width * 0.5f;
    auto maxHeight = winSize.height * 0.5f;
    
    return {
        .position = {-maxWidth * 0.5f, -maxHeight + 4.f},
        .scale = 1.f
    };
}

NodeTransform IVManager::getDefaultP2Transform() {
    auto winSize = CCDirector::get()->getWinSize();
    auto maxWidth = winSize.width * 0.5f;
    auto maxHeight = winSize.height * 0.5f;
    
    return {
        .position = {maxWidth * 0.5f, -maxHeight + 4.f},
        .scale = 1.f
    };
}

LevelSettings& IVManager::getLevelSettings(LevelSettingsType type) noexcept {
    switch (type) {
        case LevelSettingsType::Classic: return m_settingClassic;
        case LevelSettingsType::Platformer: return m_settingPlatformer;
    }
}

$on_mod(DataSaved) {
    Mod::get()->setSavedValue("classic", IVManager::get().m_settingClassic);
    Mod::get()->setSavedValue("platformer", IVManager::get().m_settingPlatformer);
    Mod::get()->setSavedValue("version", 1);
}

$on_mod(Loaded) {
    int version = Mod::get()->getSavedValue("version", 0);
    if (version == 0) {
        IVManager::get().m_settingClassic.p1Transform = Mod::get()->getSavedValue<NodeTransform>("p1-display");
        IVManager::get().m_settingPlatformer.p1Transform = Mod::get()->getSavedValue<NodeTransform>("p1-display");
        
        IVManager::get().m_settingClassic.p2Transform = Mod::get()->getSavedValue<NodeTransform>("p2-display");
        IVManager::get().m_settingPlatformer.p2Transform = Mod::get()->getSavedValue<NodeTransform>("p2-display");
        
        IVManager::get().m_settingClassic.showTotalInputs = Mod::get()->getSavedValue<bool>("show-total-inputs");
        IVManager::get().m_settingPlatformer.showTotalInputs = Mod::get()->getSavedValue<bool>("show-total-inputs");
        
        IVManager::get().m_settingClassic.showCPS = Mod::get()->getSavedValue<bool>("show-cps");
        IVManager::get().m_settingPlatformer.showCPS = Mod::get()->getSavedValue<bool>("show-cps");
        
        if (Mod::get()->getSavedValue<bool>("minimal-if-non-platformer")) {
            IVManager::get().m_settingClassic.hideLeftRight = true;
            IVManager::get().m_settingPlatformer.hideLeftRight = false;
        }
    }
}

GEODE_NS_IV_END

matjson::Value matjson::Serialize<inputs_viewer::LevelSettings>::toJson(inputs_viewer::LevelSettings const& value) {
    return matjson::makeObject({
        {"p1-display", value.p1Transform},
        {"p2-display", value.p2Transform},
        {"show-total-inputs", value.showTotalInputs},
        {"show-cps", value.showCPS},
        {"hide-left-right", value.hideLeftRight}
    });
}

Result<inputs_viewer::LevelSettings> matjson::Serialize<inputs_viewer::LevelSettings>::fromJson(matjson::Value const& value) {
    return Ok(inputs_viewer::LevelSettings{
        .p1Transform = GEODE_UNWRAP(value["p1-display"].as<NodeTransform>()),
        .p2Transform = GEODE_UNWRAP(value["p2-display"].as<NodeTransform>()),
        .showTotalInputs = GEODE_UNWRAP(value["show-total-inputs"].asBool()),
        .showCPS = GEODE_UNWRAP(value["show-cps"].asBool()),
        .hideLeftRight = GEODE_UNWRAP(value["hide-left-right"].asBool())
    });
}