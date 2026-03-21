#pragma once
#include "../includes.hpp"

class SaveMacroLayer : public geode::Popup {
    
    TextInput* authorInput = nullptr;
    TextInput* descInput = nullptr;
    TextInput* nameInput = nullptr;
    
    CCMenuItemSpriteExtra* leftArrow = nullptr;
    CCMenuItemSpriteExtra* rightArrow = nullptr;
    CCLabelBMFont* formatLabel = nullptr;
    
    SaveFormat selectedFormat = SaveFormat::GDR2;
    SaveFormat defaultFormat = SaveFormat::GDR2;
    
    private:
    
    bool init() override {
        std::string defaultFormatStr = Mod::get()->getSettingValue<std::string>("default_save_format");
        if (defaultFormatStr == "GDR") {
            defaultFormat = SaveFormat::GDR1;
            selectedFormat = SaveFormat::GDR1;
        } else if (defaultFormatStr == "JSON") {
            defaultFormat = SaveFormat::JSON;
            selectedFormat = SaveFormat::JSON;
        } else {
            defaultFormat = SaveFormat::GDR2;
            selectedFormat = SaveFormat::GDR2;
        }
        
        if (!Popup::init(285, 210, Utils::getTexture().c_str())) return false;
        Utils::setBackgroundColor(m_bgSprite);
        
        setTitle("Save Macro");
        
        cocos2d::CCPoint offset = (CCDirector::sharedDirector()->getWinSize() - m_mainLayer->getContentSize()) / 2;
        m_mainLayer->setPosition(m_mainLayer->getPosition() - offset);
        m_closeBtn->setPosition(m_closeBtn->getPosition() + offset);
        m_bgSprite->setPosition(m_bgSprite->getPosition() + offset);
        m_title->setPosition(m_title->getPosition() + offset);
        
        CCMenu* menu = CCMenu::create();
        m_mainLayer->addChild(menu);
        
        authorInput = TextInput::create(104, "Author", "chatFont.fnt");
        authorInput->setPosition({ 61, 50 });
        authorInput->setString(GJAccountManager::sharedState()->m_username.c_str());
        menu->addChild(authorInput);
        
        CCLabelBMFont* lbl = CCLabelBMFont::create("(optional)", "chatFont.fnt");
        lbl->setPosition({ 61, 28 });
        lbl->setOpacity(73);
        lbl->setScale(0.575);
        menu->addChild(lbl);
        
        nameInput = TextInput::create(104, "Name", "chatFont.fnt");
        nameInput->setPosition({ -61, 50 });
        
        nameInput->setString(Global::get().macro.levelInfo.name);
        
        menu->addChild(nameInput);
        
        descInput = TextInput::create(226, "Description (optional)", "chatFont.fnt");
        descInput->setPositionY(0);
        menu->addChild(descInput);
        
        ButtonSprite* spr = ButtonSprite::create("Save");
        spr->setScale(0.725f);
        CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SaveMacroLayer::onSave));
        btn->setPositionY(-48);
        menu->addChild(btn);
        
        CCSprite* leftSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        leftSpr->setFlipX(true);
        leftSpr->setScale(0.4f);
        leftArrow = CCMenuItemSpriteExtra::create(leftSpr, this, menu_selector(SaveMacroLayer::onLeftArrow));
        leftArrow->setPosition({ -128, -86 });
        menu->addChild(leftArrow);
        
        formatLabel = CCLabelBMFont::create("", "bigFont.fnt");
        formatLabel->setPosition({ -100, -85.5f });
        formatLabel->setScale(0.5f);
        menu->addChild(formatLabel);
        
        CCSprite* rightSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        rightSpr->setScale(0.4f);
        rightArrow = CCMenuItemSpriteExtra::create(rightSpr, this, menu_selector(SaveMacroLayer::onRightArrow));
        rightArrow->setPosition({ -48, -86 });
        menu->addChild(rightArrow);
        
        switch (defaultFormat) {
            case SaveFormat::GDR2: lbl = CCLabelBMFont::create("Default: GDR2", "chatFont.fnt"); break;
            case SaveFormat::GDR1: lbl = CCLabelBMFont::create("Default: GDR", "chatFont.fnt"); break;
            case SaveFormat::JSON: lbl = CCLabelBMFont::create("Default: JSON", "chatFont.fnt"); break;
        }
        lbl->setPosition({ -88, -85.5 });
        lbl->setScale(0.5);
        lbl->setOpacity(100);
        menu->addChild(lbl);
        
        updateFormatLabel();
        
        return true;
    }
    
    void updateFormatLabel() {
        switch (selectedFormat) {
            case SaveFormat::GDR2: formatLabel->setString("GDR2"); break;
            case SaveFormat::GDR1: formatLabel->setString("GDR"); break;
            case SaveFormat::JSON: formatLabel->setString("JSON"); break;
        }
    }
    
    void onLeftArrow(CCObject*) {
        int current = static_cast<int>(selectedFormat);
        current = (current - 1 + 3) % 3;
        selectedFormat = static_cast<SaveFormat>(current);
        updateFormatLabel();
    }
    
    void onRightArrow(CCObject*) {
        int current = static_cast<int>(selectedFormat);
        current = (current + 1) % 3;
        selectedFormat = static_cast<SaveFormat>(current);
        updateFormatLabel();
    }
    
    public:
    
    STATIC_CREATE(SaveMacroLayer)
    
    static void open() {
        if (Global::get().macro.inputs.empty())
        return FLAlertLayer::create("Save Macro", "You can't save an <cl>empty</c> macro.", "OK")->show();
        
        #ifdef GEODE_IS_IOS
        std::filesystem::path path = Mod::get()->getSaveDir() / "macros";
        #else
        std::filesystem::path path = Mod::get()->getSettingValue<std::filesystem::path>("macros_folder");
        #endif
        
        if (!std::filesystem::exists(path)) {
            if (!utils::file::createDirectoryAll(path).isOk())
            return FLAlertLayer::create("Error", ("There was an error getting the folder \"" + geode::utils::string::pathToString(path) + "\". ID: 10").c_str(), "OK")->show();
        }
        
        SaveMacroLayer* layerReal = create();
        layerReal->m_noElasticity = true;
        layerReal->show();
    }
    
    void onSave(CCObject*) {
        std::string macroName = nameInput->getString();
        if (macroName == "")
        return FLAlertLayer::create("Save Macro", "Give a <cl>name</c> to the macro.", "OK")->show();
        
        #ifdef GEODE_IS_IOS
        std::filesystem::path path = Mod::get()->getSaveDir() / "macros" / macroName;
        #else
        std::filesystem::path path = Mod::get()->getSettingValue<std::filesystem::path>("macros_folder") / macroName;
        #endif
        std::string author = authorInput->getString();
        std::string desc = descInput->getString();
        
        int result = Macro::save(author, desc, geode::utils::string::pathToString(path), selectedFormat);
        
        if (result != 0)
        return FLAlertLayer::create("Error", "There was an error saving the macro. ID: " + geode::utils::numToString(result), "OK")->show();
        
        this->keyBackClicked();
        Notification::create("Macro Saved", NotificationIcon::Success)->show();
    }
    
};
