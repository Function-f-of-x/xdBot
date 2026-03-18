#pragma once
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/PlayerCheckpoint.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/binding/GameObject.hpp>

using namespace geode::prelude;

class FixPlayerCheckpoint {
public:
    FixPlayerCheckpoint() = default;

    FixPlayerCheckpoint(PlayerObject* player) {
        if (!player) return;
        
        auto cp = PlayerCheckpoint::create();
        if (!cp) return;
        
        m_checkpoint = cp;
        m_isValid = true;
        player->saveToCheckpoint(m_checkpoint.data());

        m_yVelocityRelated = player->m_yVelocityRelated;
        m_groundYVelocity = player->m_groundYVelocity;
        m_scaleXRelated2 = player->m_scaleXRelated2;
        m_scaleXRelated3 = player->m_scaleXRelated3;
        m_scaleXRelated4 = player->m_scaleXRelated4;
        m_scaleXRelated5 = player->m_scaleXRelated5;
        m_currentSlope = player->m_currentSlope;
        m_currentSlope2 = player->m_currentSlope2;
        m_slopeAngle = player->m_slopeAngle;
        m_slopeAngleRadians = player->m_slopeAngleRadians;
        m_slopeFlipGravityRelated = player->m_slopeFlipGravityRelated;
        m_slopeSlidingMaybeRotated = player->m_slopeSlidingMaybeRotated;
        m_currentSlopeYVelocity = player->m_currentSlopeYVelocity;
        m_slopeRotation = player->m_slopeRotation;
        m_isCollidingWithSlope = player->m_isCollidingWithSlope;
        m_collidingWithSlopeId = player->m_collidingWithSlopeId;
        m_maybeUpsideDownSlope = player->m_maybeUpsideDownSlope;
        m_isOnSlope = player->m_isOnSlope;
        m_wasOnSlope = player->m_wasOnSlope;
        m_physDeltaRelated = player->m_physDeltaRelated;
    }

    void apply(PlayerObject* player) {
        if (!player || !m_checkpoint || !m_isValid) return;
        
        player->loadFromCheckpoint(m_checkpoint.data());

        player->m_yVelocityRelated = m_yVelocityRelated;
        player->m_groundYVelocity = m_groundYVelocity;
        player->m_scaleXRelated2 = m_scaleXRelated2;
        player->m_scaleXRelated3 = m_scaleXRelated3;
        player->m_scaleXRelated4 = m_scaleXRelated4;
        player->m_scaleXRelated5 = m_scaleXRelated5;
        player->m_currentSlope = m_currentSlope;
        player->m_currentSlope2 = m_currentSlope2;
        player->m_slopeAngle = m_slopeAngle;
        player->m_slopeAngleRadians = m_slopeAngleRadians;
        player->m_slopeFlipGravityRelated = m_slopeFlipGravityRelated;
        player->m_slopeSlidingMaybeRotated = m_slopeSlidingMaybeRotated;
        player->m_currentSlopeYVelocity = m_currentSlopeYVelocity;
        player->m_slopeRotation = m_slopeRotation;
        player->m_isCollidingWithSlope = m_isCollidingWithSlope;
        player->m_collidingWithSlopeId = m_collidingWithSlopeId;
        player->m_maybeUpsideDownSlope = m_maybeUpsideDownSlope;
        player->m_isOnSlope = m_isOnSlope;
        player->m_wasOnSlope = m_wasOnSlope;
        player->m_physDeltaRelated = m_physDeltaRelated;
    }

private:
    bool m_isValid = false;
    Ref<PlayerCheckpoint> m_checkpoint = nullptr;
    
    double m_yVelocityRelated, m_groundYVelocity, m_scaleXRelated2, m_scaleXRelated3;
    double m_scaleXRelated4, m_scaleXRelated5, m_currentSlopeYVelocity, m_slopeRotation, m_physDeltaRelated;
    GameObject *m_currentSlope, *m_currentSlope2;
    float m_slopeAngle, m_slopeAngleRadians;
    bool m_slopeFlipGravityRelated, m_slopeSlidingMaybeRotated, m_isCollidingWithSlope;
    bool m_maybeUpsideDownSlope, m_isOnSlope, m_wasOnSlope;
    int m_collidingWithSlopeId;
};

class FixPlayLayerCheckpoint {
public:
    FixPlayLayerCheckpoint() = default;
    FixPlayLayerCheckpoint(PlayLayer* playLayer) {
        if (!playLayer) return;
        m_player1CollisionBlock = playLayer->m_player1CollisionBlock;
        m_player2CollisionBlock = playLayer->m_player2CollisionBlock;
        m_extraDelta = playLayer->m_extraDelta;
        m_currentStep = playLayer->m_currentStep;
        m_unk3380 = playLayer->m_unk3380;
    }

    void apply(PlayLayer* playLayer) {
        if (!playLayer) return;
        playLayer->m_player1CollisionBlock = m_player1CollisionBlock;
        playLayer->m_player2CollisionBlock = m_player2CollisionBlock;
        playLayer->m_extraDelta = m_extraDelta;
        playLayer->m_currentStep = m_currentStep;
        playLayer->m_unk3380 = m_unk3380;
    }

private:
    GameObject *m_player1CollisionBlock, *m_player2CollisionBlock;
    double m_extraDelta;
    int m_currentStep;
    float m_unk3380;
};