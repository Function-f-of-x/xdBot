#pragma once
#include "../includes.hpp"
#include "../macro.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class PracticeFix {
public:
    static bool shouldEnable() {
        PlayLayer* pl = PlayLayer::get();
        if (!pl || !pl->m_isPracticeMode) return false;
        
        auto& g = Global::get();
        if (g.state != state::none) return true;
        
        if (g.alwaysPracticeFixes && pl->m_isPlatformer) return true;
        
        return false;
    }
};

class PlayerPracticeFixes {
public:
    static PlayerData saveData(PlayerObject* player) {
        PlayerData data;
        if (!player) return data;
        
        data.position = player->getPosition();
        data.rotation = player->getRotation();
        data.m_holdingRight = player->m_holdingRight;
        data.m_holdingLeft = player->m_holdingLeft;
        data.m_dashX = player->m_dashX;
        data.m_dashY = player->m_dashY;
        data.m_dashAngle = player->m_dashAngle;
        data.m_dashStartTime = player->m_dashStartTime;
        data.m_slopeStartTime = player->m_slopeStartTime;
        data.m_justPlacedStreak = player->m_justPlacedStreak;
         
        data.m_playerFollowFloats = player->m_playerFollowFloats;
        
        return data;
    }
    
    static void applyData(PlayerObject* player, PlayerData& data, bool player2, bool applyPos) {
        if (!player) return;
        
        if (applyPos) {
            player->setPosition(data.position);
            player->setRotation(data.rotation);
        }
        
        player->m_holdingRight = data.m_holdingRight;
        player->m_holdingLeft = data.m_holdingLeft;
        player->m_dashX = data.m_dashX;
        player->m_dashY = data.m_dashY;
        player->m_dashAngle = data.m_dashAngle;
        player->m_dashStartTime = data.m_dashStartTime;
        player->m_slopeStartTime = data.m_slopeStartTime;
        player->m_justPlacedStreak = data.m_justPlacedStreak;
        player->m_playerFollowFloats = data.m_playerFollowFloats;
    }
};