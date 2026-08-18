#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude; 
using namespace nwo5::ui::prelude;

namespace Ruler {
    MeasurementColor EditorUI::getMeasurementColor() {
        const auto& measurements = m_fields->measurements;

        int main = measurements.empty() ? -1 : measurements.back().color.main;

        while (true) {
            const auto random = nwo5::utils::random(0, static_cast<int>(MEASUREMENT_COLOR.size()) - 1);

            if (random != main) {
                main = random;

                break;
            }
        }

        int chroma = measurements.empty() ? -1 : measurements.back().color.chroma;

        while (true) {
            const auto random = nwo5::utils::random(0, 360);

            if (random != chroma) {
                chroma = random;

                break;
            }
        }

        return {main, chroma};
    }

    std::string EditorUI::getMeasurementString(float pMeasure) {
        if (Ruler::useGDUnits) {
            // i think the correct way to do the math is slightly different but this works so
            const auto measure = pMeasure / (Ruler::useGDUnits ? editor::constants::GRID_SIZE : 1);
            const auto units = std::floor(measure);

            const auto str =  nwo5::utils::numToString(units);

            // floating point trust issues
            if (std::abs(measure - units) > std::numeric_limits<float>::epsilon()) {
                return fmt::format("{}, {}", str, nwo5::utils::numToString((measure - units) * editor::constants::GRID_SIZE_OBJECT));
            }

            return str;
        }
        else {
            return nwo5::utils::numToString(pMeasure);
        }
    }

    CCLabelBMFont* EditorUI::createMeasurementLabel(float pMeasure) {
        auto label = ui::label(this->getMeasurementString(pMeasure), ui::Font::ChatFont);

        Shared::getOverlayLayer()->addChild(label);

        return label;
    }

    void EditorUI::createMeasurement() {
        auto objs = editor::selection::get();
        const auto bounds = editor::object::bounds(objs, true);

        Measurement measurement = {
            bounds.origin, bounds.origin + bounds.size, this->getMeasurementColor(),
            this->createMeasurementLabel(bounds.size.width), this->createMeasurementLabel(bounds.size.height)
        };

        m_fields->measurements.push_back(std::move(measurement));
    }

    void EditorUI::deleteMeasurement(bool pDeleteAll) {
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





    void Feature::onEditor() {
        auto self = editor::ui<Ruler::EditorUI>();

        editor::conditionallyRegisterEditTabButtonFrame(
            Ruler::enabled() && Ruler::editorTabButton,
            "ruler.png"_spr, "create-measurement-button"_spr, 1, [self] (auto) {
                if (!Ruler::enabled()) {
                    return;
                }

                if (editor::selection::empty()) {
                    self->deleteMeasurement(false);
                }
                else {
                    self->createMeasurement();
                }
            }
        );

        // i dont want the same colors in the same order every time (or mayb it doesnt do that and i js got *very* lucky in my testing idk)
        random::_getGenerator().seed(asp::SystemTime::now().timeSinceEpoch().seconds());

        nwo5::utils::setupKeybind(self, "ruler-create-measurement-key", [self] (const Keybind&, bool pDown, bool, double) {
            if (Ruler::enabled() && pDown) {
                self->createMeasurement();
            }
        });
        nwo5::utils::setupKeybind(self, "ruler-delete-last-measurement-key", [self] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Ruler::enabled() && pDown) {
                self->deleteMeasurement(pRepeat);
            }
        });
    }

    void Feature::onUpdate() {
        auto self = editor::ui<Ruler::EditorUI>();

        // border alignment no workie :fire: - update to this comment like months later, now i use my own drawnode so it shoudl work but i havent implemented it yet so it still doesnt and im now too scared to touch this code soooo
        const auto padding = CCPoint{Ruler::padding, Ruler::padding} / 2 
            + CCPoint{Ruler::thickness, Ruler::thickness} / 2;

        for (const auto& measurement : self->m_fields->measurements) {
            const auto start = measurement.start - padding;
            const auto end = measurement.end + padding;

            const auto col = Ruler::chroma 
                ? nwo5::utils::getChroma(measurement.color.chroma) 
                : MEASUREMENT_COLOR[measurement.color.main];

            Shared::getOverlayDraw()->drawRect(
                start, end, nwo5::utils::setOpacity(col, Ruler::fillOpacity.get()), 
                Ruler::thickness / (Ruler::scaleWithZoom ? editor::zoom() : 1.0f), col
            );

            for (auto label : {measurement.xLabel, measurement.yLabel}) {
                const auto y = (label == measurement.yLabel);

                Setup(label)
                    .scale(Ruler::labelSize)
                    .anchor( // this is prolly a war crime icl but atleast its better than my old ruler impl
                        y 
                            ? (Ruler::dontRotateLabel 
                                ? (Ruler::labelOnRight 
                                    ? Anchor::Left 
                                    : Anchor::Right
                                ) 
                                : Anchor::Bottom
                            )
                            : (Ruler::labelOnBottom 
                                ? Anchor::Top 
                                : Anchor::Bottom
                            )
                    )
                    .pos(
                        y
                            ? (Ruler::labelOnRight
                                ? ccp(
                                    measurement.end.x + Ruler::thickness + Ruler::labelDistance,
                                    (measurement.start.y + measurement.end.y) / 2
                                )
                                : ccp(
                                    measurement.start.x - Ruler::thickness - Ruler::labelDistance,
                                    (measurement.start.y + measurement.end.y) / 2
                                )
                            )
                            : (Ruler::labelOnBottom
                                ? ccp(
                                    (measurement.start.x + measurement.end.x) / 2,
                                    measurement.start.y - Ruler::thickness - Ruler::labelDistance
                                )
                                : ccp(
                                    (measurement.start.x + measurement.end.x) / 2,
                                    measurement.end.y + Ruler::thickness + Ruler::labelDistance
                                )
                            )
                    )
                    .rotation(
                            Ruler::dontRotateLabel 
                                ? 0.0f 
                                : (y 
                                    ? (Ruler::labelOnRight 
                                        ? 90.0f 
                                        : 270.0f
                                    ) 
                                    : (Ruler::labelOnBottom 
                                        ? 180.0f 
                                        : 0.0f
                                    )
                                )
                        )
                    .color(ccc3(col.r * 255, col.g * 255, col.b * 255));
            }
        }
    }
}