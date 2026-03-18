#include "GameCheckpoint.hpp"
#include "practice_fixes.hpp"
#include <Geode/modify/PlayLayer.hpp>

class PracticeCheckpointData {
public:
    PracticeCheckpointData() = default;
    PracticeCheckpointData(PlayerObject* p1, PlayerObject* p2, PlayLayer* pl) {
        if (!pl || !p1) return;
        m_checkpointPlayLayer = FixPlayLayerCheckpoint(pl);
        m_checkpointPlayer1 = FixPlayerCheckpoint(p1);
        if (p2) m_checkpointPlayer2 = FixPlayerCheckpoint(p2);
    }

    void apply(PlayerObject* p1, PlayerObject* p2, PlayLayer* pl) {
        if (!pl) return;
        m_checkpointPlayLayer.apply(pl);
        if (p1) m_checkpointPlayer1.apply(p1);
        if (p2) m_checkpointPlayer2.apply(p2);
    }

private:
    FixPlayerCheckpoint m_checkpointPlayer1, m_checkpointPlayer2;
    FixPlayLayerCheckpoint m_checkpointPlayLayer;
};

class $modify(FixPlayLayer, PlayLayer) {
    struct Fields {
        std::unordered_map<CheckpointObject*, PracticeCheckpointData> m_checkpoints;
        std::unordered_map<CheckpointObject*, std::vector<input>> m_checkpointInputs;
        std::unordered_map<CheckpointObject*, std::vector<gdr_legacy::FrameFix>> m_checkpointFrameFixes;
        std::unordered_map<CheckpointObject*, int> m_checkpointFrames;
    };
    
    void loadFromCheckpoint(CheckpointObject* checkpoint) {
        PlayLayer::loadFromCheckpoint(checkpoint);

        if (PracticeFix::shouldEnable()) {
            auto fields = m_fields.self();
            if (fields->m_checkpoints.contains(checkpoint)) {
                auto& data = fields->m_checkpoints[checkpoint];
                data.apply(m_player1, m_gameState.m_isDualMode ? m_player2 : nullptr, this);
            }

            auto& g = Global::get();
            if (g.state == state::recording && fields->m_checkpointInputs.contains(checkpoint)) {
                g.ignoreRecordAction = true;
                g.macro.inputs = fields->m_checkpointInputs[checkpoint];
                g.macro.frameFixes = fields->m_checkpointFrameFixes[checkpoint];
                g.ignoreRecordAction = false;
            }
        }
    }
    
    CheckpointObject* createCheckpoint() {
        auto checkpoint = PlayLayer::createCheckpoint();
        if (!checkpoint || !PracticeFix::shouldEnable()) return checkpoint;
        
        PracticeCheckpointData data(m_player1, m_gameState.m_isDualMode ? m_player2 : nullptr, this);
        m_fields->m_checkpoints[checkpoint] = data;

        auto& g = Global::get();
        if (g.state == state::recording) {
            g.ignoreRecordAction = true;
            m_fields->m_checkpointInputs[checkpoint] = g.macro.inputs;
            m_fields->m_checkpointFrameFixes[checkpoint] = g.macro.frameFixes;
            m_fields->m_checkpointFrames[checkpoint] = Global::getCurrentFrame();
            g.ignoreRecordAction = false;
        }
        
        return checkpoint;
    }

    void resetLevel() {
        if (m_checkpointArray->count() <= 0) {
            m_fields->m_checkpoints.clear();
            m_fields->m_checkpointInputs.clear();
            m_fields->m_checkpointFrameFixes.clear();
            m_fields->m_checkpointFrames.clear();
        }
        PlayLayer::resetLevel();
        if (m_checkpointArray->count() > 0) m_resumeTimer = 0;
    }
};