#pragma once

namespace Ruler {
    struct MeasurementColor {
        int main = 0;
        int chroma = 0;
    };

    struct Measurement {
        cocos2d::CCPoint start = cocos2d::CCPointZero;
        cocos2d::CCPoint end = cocos2d::CCPointZero;

        MeasurementColor color;

        geode::Label* xLabel = nullptr;
        geode::Label* yLabel = nullptr;
        
        void cleanup() {
            if (xLabel) {
                xLabel->removeMeAndCleanup();
            }
            if (yLabel) {
                yLabel->removeMeAndCleanup();
            }
        }
    };

    // catpuccin mocha :3
    constexpr std::array<cocos2d::ccColor4F, 14> MEASUREMENT_COLOR{
        cocos2d::ccColor4F{0.96f, 0.88f, 0.86f, 1.0f},
        cocos2d::ccColor4F{0.95f, 0.8f, 0.8f, 1.0f},
        cocos2d::ccColor4F{0.96f, 0.76f, 0.91f, 1.0f},
        cocos2d::ccColor4F{0.8f, 0.65f, 0.97f, 1.0f},
        cocos2d::ccColor4F{0.95f, 0.55f, 0.66f, 1.0f},
        cocos2d::ccColor4F{0.92f, 0.63f, 0.67f, 1.0f},
        cocos2d::ccColor4F{0.98f, 0.7f, 0.53f, 1.0f},
        cocos2d::ccColor4F{0.98f, 0.89f, 0.69f, 1.0f},
        cocos2d::ccColor4F{0.65f, 0.89f, 0.63f, 1.0f},
        cocos2d::ccColor4F{0.58f, 0.89f, 0.84f, 1.0f},
        cocos2d::ccColor4F{0.54f, 0.86f, 0.92f, 1.0f},
        cocos2d::ccColor4F{0.45f, 0.78f, 0.93f, 1.0f},
        cocos2d::ccColor4F{0.54f, 0.71f, 0.98f, 1.0f},
        cocos2d::ccColor4F{0.71f, 0.75f, 1.0f, 1.0f}
    };
}