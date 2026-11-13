#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static auto& getObjectArray() {
    static std::unordered_set<GroupLabelShenanigans::EffectGameObject*> val;
    return val;
}

static bool canHaveLabel(GameObject* obj) {
    // second one for robtops player collision objects
    return trigger::type(obj) != trigger::ObjectType::Normal && !obj->m_bDontDraw;
}

namespace GroupLabelShenanigans {
    LevelEditorLayer::Fields::~Fields() {
        getObjectArray().clear();
    }



    void LevelEditorLayer::updateLabelsInSection(bool positionsOnly) {
        if (!GroupLabelShenanigans::enabled()) {
            return;
        }

        auto options = m_fields->options.get();

        if (!options) {
            return;
        }

        object::forEachInSection([&] (GameObject* obj) {
            if (!canHaveLabel(obj)) {
                return;
            }

            auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(obj);

            if (!obj->isVisible()) {
                return;
            }

            if (getObjectArray().find(obj) == getObjectArray().end()) {
                obj->addShenanigans(options);
            }

            if (positionsOnly) {
                auto fields = obj->m_fields.self();

                obj->updateShenaniganPositions(fields->label, fields->sprite);
            }
            else {
                obj->updateShenanigans(options);
            }
        });
    }



    void LevelEditorLayer::updateObjectLabel(GameObject* object) {
        if (!GroupLabelShenanigans::enabled() || !canHaveLabel(object)) {
            return GD::LevelEditorLayer::updateObjectLabel(object);
        }

        auto options = editor::layer<GroupLabelShenanigans::LevelEditorLayer>()->m_fields->options.get();

        if (!options) {
            return;
        }

        auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(object);

        if (getObjectArray().find(obj) == getObjectArray().end()) {
            obj->addShenanigans(options);
        }

        obj->updateShenanigans(options);
    }

    



    EffectGameObject::Fields::~Fields() {
        getObjectArray().erase(obj);
    }



    void EffectGameObject::removeShenanigans() {
        auto fields = m_fields.self();

        if (fields->label) {
            fields->label->removeMeAndCleanup();
            fields->label = nullptr;
        }
        if (fields->sprite) {
            fields->sprite->removeMeAndCleanup();
            fields->sprite = nullptr;
        }

        if (m_objectLabel) {
            m_objectLabel->setVisible(true);
        }
    }

    void EffectGameObject::updateShenaniganPositions(geode::Label* label, CCSprite* sprite) {
        if (label) {
            Setup(label)
                .pos(CCPoint{0.0f, sillyedit::utils::triggerHasBodyOffset(m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET.y : 0.0f} + ui::size(this) / 2)
                .opacity(this)
                .rotation(GroupLabelShenanigans::dontRotateLabel ? -this->getRotation() : 0.0f);
        }

        if (sprite) {
            Setup(sprite)
                .pos(ccAdd(ui::size(this) / 2, 8.5f))
                .opacity(sillyedit::utils::modifyOpacity(sprite->getTag(), this->getOpacity()));
        }
    }

    void EffectGameObject::updateShenanigans(LabelOptions* options) {
        auto fields = m_fields.self();

        if (!fields->label || !fields->sprite) {
            return;
        }

        this->updateShenaniganPositions(fields->label, fields->sprite);
        
        char buf[LabelOptions::BUF_SIZE];
        options->stringForObject(buf, this);

        Setup(fields->label)
            .opacity(this)
            .visible(buf[0] != '\0');

        if (std::strcmp(fields->label->getString(), buf)) {
            fields->label->setText(buf);
            fields->label->validate();
        }

        const auto col = options->extraForObject(this);
        if (col.has_value() && GroupLabelShenanigans::extras) {
            Setup(fields->sprite)
                .color(color_cast<ccColor3B>(col.value()))
                .tag(col.value().a)
                .visible(true);
        }
        else {
            fields->sprite->setVisible(false);
        }
    }

    void EffectGameObject::addShenanigans(LabelOptions* options) {
        auto fields = m_fields.self();

        getObjectArray().insert(this);

        if (!fields->label) {
            fields->label = ui::label(Font::Default)
                .id("custom-label"_spr)
                .scale(0.5f)
                .maxWidth(50.0f)
                .parent(this)
                .alignment(geode::Label::Alignment::Center)
                .hide();
        }

        if (!fields->sprite) {
            fields->sprite = ui::spr("extra-dot.png"_spr)
                .id("extra"_spr)
                .scale(0.75f)
                .tag(255)
                .parent(this)
                .hide();
        }

        if (m_objectLabel) {
            m_objectLabel->setVisible(false);
        }

        this->updateShenanigans(options);
    }
    


    void EffectGameObject::customSetup() {
        GD::EffectGameObject::customSetup();

        if (editor::layer() && canHaveLabel(this)) {
            m_fields->obj = this;
            m_addToNodeContainer = true;
        }
    }





    void Feature::onEditor() {
        auto self = editor::layer<GroupLabelShenanigans::LevelEditorLayer>();

        self->m_fields->options = std::make_unique<LabelOptions>();

        parseLabels(*(self->m_fields->options.get()));

        self->addEventListener(ObjectsChangedEvent(), [self] {
            self->updateLabelsInSection();
        });
    }

    void Feature::onUpdate() {
        if (GroupLabelShenanigans::enabled()) {
            editor::layer<GroupLabelShenanigans::LevelEditorLayer>()->updateLabelsInSection(true);
        }
    }

    void Feature::onSettingChanged(std::string name, GenericSetting*) {
        auto self = editor::layer<GroupLabelShenanigans::LevelEditorLayer>();

        auto ptr = self->m_fields->options.get();

        if (ptr) {
            getObjectArray().clear();

            object::forEachInSection([&, self] (GameObject* obj) {
                if (!canHaveLabel(obj)) {
                    return;
                }

                auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(obj);

                obj->removeShenanigans();
                self->updateObjectLabel(obj);
            });

            parseLabels(*ptr);

            self->updateLabelsInSection(false);
        }
    }
}