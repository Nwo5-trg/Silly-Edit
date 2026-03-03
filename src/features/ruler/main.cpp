#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include <features/shared.hpp>
#include "settings.hpp"

using namespace geode::prelude; 
using namespace Utils::Aliases;

struct MeasurementColor {
    int main;
    int chroma;
};

struct Measurement {
    CCPoint start;
    CCPoint end;

    MeasurementColor color;

    CCLabelBMFont* xLabel = nullptr;
    CCLabelBMFont* yLabel = nullptr;
};

// catpuccin mocha :3
static constexpr std::array<ccColor4F, 14> s_colors{
    ccColor4F{0.96f, 0.88f, 0.86f, 1.0f},
    ccColor4F{0.95f, 0.8f, 0.8f, 1.0f},
    ccColor4F{0.96f, 0.76f, 0.91f, 1.0f},
    ccColor4F{0.8f, 0.65f, 0.97f, 1.0f},
    ccColor4F{0.95f, 0.55f, 0.66f, 1.0f},
    ccColor4F{0.92f, 0.63f, 0.67f, 1.0f},
    ccColor4F{0.98f, 0.7f, 0.53f, 1.0f},
    ccColor4F{0.98f, 0.89f, 0.69f, 1.0f},
    ccColor4F{0.65f, 0.89f, 0.63f, 1.0f},
    ccColor4F{0.58f, 0.89f, 0.84f, 1.0f},
    ccColor4F{0.54f, 0.86f, 0.92f, 1.0f},
    ccColor4F{0.45f, 0.78f, 0.93f, 1.0f},
    ccColor4F{0.54f, 0.71f, 0.98f, 1.0f},
    ccColor4F{0.71f, 0.75f, 1.0f, 1.0f}
};

class $modify(RulerEditorUI, EditorUI) {
    struct Fields {
        std::vector<Measurement> measurements;
    };

    MeasurementColor getMeasurementColor() {
        const auto& measurements = m_fields->measurements;

        int main = measurements.empty() ? -1 : measurements.back().color.main;

        while (true) {
            const auto random = Utils::random(0, static_cast<int>(s_colors.size()) - 1);

            if (random != main) {
                main = random;

                break;
            }
        }

        int chroma = measurements.empty() ? -1 : measurements.back().color.chroma;

        while (true) {
            const auto random = Utils::random(0, 5);

            if (random != chroma) {
                chroma = random;

                break;
            }
        }

        return {main, chroma};
    }

    std::string getMeasurementString(float pMeasure) {
        if (Settings::Ruler::useGDUnits.get()) {
            // i think the correct way to do the math is slightly different but this works so
            const auto measure = pMeasure / (Settings::Ruler::useGDUnits.get() ? Editor::GRID_SIZE : 1);
            const auto units = std::floor(measure);

            const auto str =  Utils::numToString(units);

            // floating point trust issues
            if (std::abs(measure - units) > std::numeric_limits<float>::epsilon()) {
                return fmt::format("{}, {}", str, Utils::numToString((measure - units) * Editor::GRID_SIZE_OBJECT));
            }

            return str;
        }
        else {
            return Utils::numToString(pMeasure);
        }
    }

    CCLabelBMFont* createMeasurementLabel(float pMeasure) {
        auto label = CCLabelBMFont::create(getMeasurementString(pMeasure).c_str(), "chatFont.fnt");

        Shared::getOverlayLayer()->addChild(label);

        return label;
    }

    void createMeasurement() {
        auto objs = Editor::Selection::get();
        const auto bounds = Editor::Object::bounds(objs, true);

        Measurement measurement = {
            bounds.origin, bounds.origin + bounds.size, getMeasurementColor(),
            createMeasurementLabel(bounds.size.width), createMeasurementLabel(bounds.size.height)
        };

        m_fields->measurements.push_back(std::move(measurement));
    }

    void deleteMeasurement(bool pDeleteAll) {
        auto& measurements = m_fields->measurements;

        if (measurements.empty()) {
            return;
        }

        measurements.back().xLabel->removeMeAndCleanup();
        measurements.back().yLabel->removeMeAndCleanup();

        measurements.pop_back();

        if (pDeleteAll) {
            for (auto& measurement : measurements) {
                measurement.xLabel->removeMeAndCleanup();
                measurement.yLabel->removeMeAndCleanup();
            }

            measurements.clear();
        }
    }

    void onCreateMeasurement(CCObject* pSender) {
        if (Editor::Selection::empty()) {
            return deleteMeasurement(false);
        }
        
        createMeasurement();
    }

    void updateMeasurements() {
        // border alignment no workie :fire:
        const auto padding = CCPoint{Settings::Ruler::padding.get(), Settings::Ruler::padding.get()} / 2 
            + CCPoint{Settings::Ruler::thickness.get(), Settings::Ruler::thickness.get()} / 2;

        for (const auto& measurement : m_fields->measurements) {
            const auto start = measurement.start - padding;
            const auto end = measurement.end + padding;

            const auto col = Settings::Ruler::chroma.get() ? Shared::getChroma(measurement.color.chroma) : s_colors[measurement.color.main];

            Shared::getOverlayDraw()->drawRect(
                start, end, {col.r, col.g, col.b, Settings::Ruler::fillOpacity.get()}, Settings::Ruler::thickness.get(), col
            );

            for (auto label : {measurement.xLabel, measurement.yLabel}) {
                const auto y = (label == measurement.yLabel);

                Utils::setupNode(
                    label,

                    SetNodePosition{ // not even worth trying to turn into a ternary :pray:
                        y
                            ? (Settings::Ruler::labelOnRight.get()
                                ? ccp(measurement.end.x + Settings::Ruler::thickness.get() + Settings::Ruler::labelDistance.get(),
                                    (measurement.start.y + measurement.end.y) / 2)
                                : ccp(measurement.start.x - Settings::Ruler::thickness.get() - Settings::Ruler::labelDistance.get(),
                                    (measurement.start.y + measurement.end.y) / 2)
                            )
                            : (Settings::Ruler::labelOnBottom.get()
                                ? ccp((measurement.start.x + measurement.end.x) / 2,
                                    measurement.start.y - Settings::Ruler::thickness.get() - Settings::Ruler::labelDistance.get())
                                : ccp((measurement.start.x + measurement.end.x) / 2,
                                    measurement.end.y + Settings::Ruler::thickness.get() + Settings::Ruler::labelDistance.get())
                            )
                    },
                    SetNodeScale{Settings::Ruler::labelSize.get()},
                    SetNodeAnchor{ // this is prolly a war crime icl but atleast its better than my old ruler impl
                        y 
                            ? (Settings::Ruler::dontRotateLabel.get() 
                                ? (Settings::Ruler::labelOnRight.get() 
                                    ? LEFT_CENTER_ANCHOR 
                                    : RIGHT_CENTER_ANCHOR
                                    ) 
                                : BOTTOM_CENTER_ANCHOR
                              )
                            : (Settings::Ruler::labelOnBottom.get() 
                                ? TOP_CENTER_ANCHOR 
                                : BOTTOM_CENTER_ANCHOR
                              )
                    },
                    SetNodeRotation{
                        Settings::Ruler::dontRotateLabel.get() 
                            ? 0.0f 
                            : (y 
                                ? (Settings::Ruler::labelOnRight.get() 
                                    ? 90.0f 
                                    : 270.0f
                                  ) 
                                : (Settings::Ruler::labelOnBottom.get() 
                                    ? 180.0f 
                                    : 0.0f
                                  )
                              )
                    },
                    SetNodeColor{ccc3(col.r * 255, col.g * 255, col.b * 255)}
                );
            }
        } 
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }
    
        // i dont want the same colors in the same order every time (or mayb it doesnt do that and i js got *very* lucky in my testing idk)
        // btw ik theres some time utils w/ asp but i actually cant figure them out and i dont wanna go thru the source so wtv man
        random::_getGenerator().seed(time(nullptr));
        Shared::addUpdateFunc(SE_UPDATE_FUNC(updateMeasurements));

        Utils::setupKeybind(this, "ruler-create-measurement-key", [this] (const Keybind&, bool pDown, bool, double) {
            if (Settings::Ruler::enabled.get() && pDown) {
                createMeasurement();
            }
        });
        Utils::setupKeybind(this, "ruler-delete-last-measurement-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::Ruler::enabled.get() && pDown) {
                deleteMeasurement(pRepeat);
            }
        });

        return true;
    }

    void createMoveMenu() {
		EditorUI::createMoveMenu();

        if (!Settings::Ruler::enabled.get() || !Settings::Ruler::editorTabButton.get()) {
            return;
        }

        // @geode-ignore(unknown-resource)
        auto button = this->getSpriteButton("ruler.png"_spr, menu_selector(RulerEditorUI::onCreateMeasurement), nullptr, 1.0f);

        m_editButtonBar->m_buttonArray->addObject(button);
        const auto rows = GameManager::sharedState()->getIntGameVariable("0049");
        const auto cols = GameManager::sharedState()->getIntGameVariable("0050");
        m_editButtonBar->reloadItems(rows, cols);
	}
};