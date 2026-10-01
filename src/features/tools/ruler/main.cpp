#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude; 
using namespace nwo5::ui::prelude;

namespace Ruler {
    MeasurementColor EditorUI::getMeasurementColor() {
        const auto& measurements = m_fields->measurements;

        int main = measurements.empty() ? -1 : measurements.back().color.main;

        while (true) {
            const auto random = misc::random(0, static_cast<int>(MEASUREMENT_COLOR.size()) - 1);

            if (random != main) {
                main = random;

                break;
            }
        }

        int chroma = measurements.empty() ? -1 : measurements.back().color.chroma;

        while (true) {
            const auto random = misc::random(0, 360);

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

            const auto str =  misc::numToString(units);

            // floating point trust issues
            if (std::abs(measure - units) > std::numeric_limits<float>::epsilon()) {
                return fmt::format("{}, {}", str, misc::numToString((measure - units) * editor::constants::GRID_SIZE_OBJECT));
            }

            return str;
        }
        else {
            return misc::numToString(pMeasure);
        }
    }

    Label* EditorUI::createMeasurementLabel(float pMeasure) {
        auto label = ui::label(this->getMeasurementString(pMeasure), Font::Chat);

        sillyedit::utils::getOverlayLayer()->addChild(label);

        return label;
    }

    void EditorUI::createMeasurement() {
        auto objs = selection::get();
        const auto bounds = object::bounds(objs, true);

        auto& measurements = m_fields->measurements;

        if (const auto it = std::ranges::find_if(measurements, [bounds] (const Measurement& pMeasurement) {
            return pMeasurement.start == bounds.origin && pMeasurement.end == bounds.origin + bounds.size;
        }); it != measurements.end()) {
            it->cleanup();
            measurements.erase(it);

            return;
        }

        Measurement measurement{
            bounds.origin, bounds.origin + bounds.size, this->getMeasurementColor(),
            this->createMeasurementLabel(bounds.size.width), this->createMeasurementLabel(bounds.size.height)
        };


        measurements.push_back(std::move(measurement));
    }

    void EditorUI::deleteMeasurement(bool pDeleteAll) {
        auto& measurements = m_fields->measurements;

        if (measurements.empty()) {
            return;
        }

        measurements.back().cleanup();
        measurements.pop_back();

        if (pDeleteAll) {
            for (auto& measurement : measurements) {
                measurement.cleanup();
            }

            measurements.clear();
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<Ruler::EditorUI>();

        // i dont want the same colors in the same order every time (or mayb it doesnt do that and i js got *very* lucky in my testing idk)
        random::_getGenerator().seed(asp::SystemTime::now().timeSinceEpoch().seconds());

        feature.registerKeybind<"create-measurement-key">([self] (bool pDown, bool) {
            if (pDown) {
                self->createMeasurement();
            }
        });
        feature.registerKeybind<"delete-last-measurement-key">([self] (bool pDown, bool pRepeat) {
            if (pDown) {
                self->deleteMeasurement(pRepeat);
            }
        });
    }

    void Feature::onToggled(bool pEnabled) {
        auto self = editor::ui<Ruler::EditorUI>();

        editor::conditionallyRegisterEditTabButtonFrame(
            pEnabled && Ruler::editorTabButton,
            "ruler.png"_spr, "create-measurement-button"_spr, 1, [self] (auto) {
                if (selection::empty()) {
                    self->deleteMeasurement(false);
                }
                else {
                    self->createMeasurement();
                }
            }
        );
    }

    void Feature::onUpdate() {
        auto self = editor::ui<Ruler::EditorUI>();

        const auto thicknessFactor = Ruler::scaleWithZoom ? 1.0f / editor::zoom() : 1.0f;
        // border alignment no workie :fire: - update to this comment like months later, now i use my own drawnode so it shoudl work but i havent implemented it yet so it still doesnt and im now too scared to touch this code soooo
        const auto padding = ccAdd(CCPoint{Ruler::padding, Ruler::padding} / 2, Ruler::thickness * thicknessFactor / 2);

        for (const auto& measurement : self->m_fields->measurements) {
            const auto start = measurement.start - padding;
            const auto end = measurement.end + padding;

            const auto col = Ruler::chroma 
                ? sillyedit::utils::getChroma(measurement.color.chroma) 
                : MEASUREMENT_COLOR[measurement.color.main];

            sillyedit::utils::getOverlayDraw()->drawRect(
                start, end, misc::setOpacity(col, Ruler::fillOpacity.get() / 255.0f), 
                Ruler::thickness * thicknessFactor, col
            );

            if (Ruler::showCenter) {
                sillyedit::utils::getOverlayDraw()->drawDot(
                    (start + end) / 2, Ruler::centerSize * thicknessFactor, col
                );
            }

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
                                ? CCPoint{
                                    measurement.end.x + Ruler::thickness + Ruler::labelDistance,
                                    (measurement.start.y + measurement.end.y) / 2
                                }
                                : CCPoint{
                                    measurement.start.x - Ruler::thickness - Ruler::labelDistance,
                                    (measurement.start.y + measurement.end.y) / 2
                                }
                            )
                            : (Ruler::labelOnBottom
                                ? CCPoint{
                                    (measurement.start.x + measurement.end.x) / 2,
                                    measurement.start.y - Ruler::thickness - Ruler::labelDistance
                                }
                                : CCPoint{
                                    (measurement.start.x + measurement.end.x) / 2,
                                    measurement.end.y + Ruler::thickness + Ruler::labelDistance
                                }
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
                    .color(color_cast<ccColor3B>(col));
            }
        }
    }
}