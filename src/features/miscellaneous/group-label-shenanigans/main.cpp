#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static auto& getObjectArray() {
    static std::unordered_set<GroupLabelShenanigans::EffectGameObject*> val;
    return val;
}

static bool canHaveLabel(GameObject* pObj) {
    return editor::trigger::type(pObj) != editor::trigger::ObjectType::Normal;
}

namespace GroupLabelShenanigans {
    LevelEditorLayer::Fields::~Fields() {
        getObjectArray().clear();
    }



    void LevelEditorLayer::updateLabelsInSection(bool pPositionsOnly) {
        if (!GroupLabelShenanigans::enabled()) {
            return;
        }

        auto options = m_fields->options.get();

        if (!options) {
            return;
        }

        editor::object::forEachInSection([&] (GameObject* pObj) {
            if (!canHaveLabel(pObj)) {
                return;
            }

            auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(pObj);

            if (!obj->isVisible() || !obj->getOpacity()) {
                return;
            }

            if (getObjectArray().find(obj) == getObjectArray().end()) {
                obj->addShenanigans(options);
            }

            if (pPositionsOnly) {
                auto fields = obj->m_fields.self();

                obj->updateShenaniganPositions(fields->label, fields->sprite);
            }
            else {
                obj->updateShenanigans(options);
            }
        });
    }



    void LevelEditorLayer::updateObjectLabel(GameObject* object) {
        if (!GroupLabelShenanigans::enabled()) {
            return GD::LevelEditorLayer::updateObjectLabel(object);
        }
        
        if (!GroupLabelShenanigans::optimize || !canHaveLabel(object)) {
            return;
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

    void LevelEditorLayer::updateDebugDraw() {
        GD::LevelEditorLayer::updateDebugDraw();

        if (!GroupLabelShenanigans::enabled()) {
            return;
        }

        if (!GroupLabelShenanigans::optimize || m_fields->lastObjectCount != m_objects->count()) {
            this->updateLabelsInSection(false);

            m_fields->lastObjectCount = m_objects->count();
        }
        else {
            this->updateLabelsInSection(true);
        }
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

    void EffectGameObject::updateShenaniganPositions(geode::Label* pLabel, CCSprite* pSprite) {
        auto centered = false;
        switch (m_objectID) {
            case 22: [[fallthrough]];
            case 24: [[fallthrough]];
            case 23: [[fallthrough]];
            case 25: [[fallthrough]];
            case 26: [[fallthrough]];
            case 27: [[fallthrough]];
            case 28: [[fallthrough]];
            case 55: [[fallthrough]];
            case 56: [[fallthrough]];
            case 57: [[fallthrough]];
            case 58: [[fallthrough]];
            case 59: [[fallthrough]];
            case 1816: [[fallthrough]];
            case 1915: [[fallthrough]];
            case 3640: {
                centered = true;
            break; }
            default: {};
        }

        if (pLabel) {
            Setup(pLabel)
                .pos(CCPoint{0.0f, centered ? 0.0f : -4.0f} + this->getContentSize() / 2)
                .opacity(this)
                .rotation(GroupLabelShenanigans::dontRotateLabel ? -this->getRotation() : 0.0f);
        }

        if (pSprite) {
            Setup(pSprite)
                .pos(CCPoint{8.5f, 8.5f} + this->getContentSize() / 2)
                .opacity(Sillyedit::modifyOpacity(pSprite->getTag(), this->getOpacity()));
        }
    }

    void EffectGameObject::updateShenanigans(LabelOptions* pOptions) {
        auto fields = m_fields.self();

        if (!fields->label || !fields->sprite) {
            return;
        }

        this->updateShenaniganPositions(fields->label, fields->sprite);
        
        char buf[LabelOptions::BUF_SIZE];
        pOptions->stringForObject(buf, this);

        Setup(fields->label)
            .opacity(this)
            .visible(buf[0] != '\0');

        if (std::strcmp(fields->label->getString(), buf)) {
            fields->label->setText(buf);
            fields->label->validate();
        }

        const auto col = pOptions->extraForObject(this);
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

    void EffectGameObject::addShenanigans(LabelOptions* pOptions) {
        auto fields = m_fields.self();

        getObjectArray().insert(this);

        if (!fields->label) {
            fields->label = Setup(geode::Label::create(Font::Default))
                .id("custom-label"_spr)
                .scale(0.5f)
                .maxWidth(50.0f)
                .parent(this)
                .hide();
            fields->label->setAlignment(geode::Label::Alignment::Center);
        }

        if (!fields->sprite) {
            fields->sprite = Setup(CCSprite::create("extra-dot.png"_spr))
                .id("extra"_spr)
                .scale(0.75f)
                .tag(255)
                .parent(this)
                .hide();
        }

        if (m_objectLabel) {
            m_objectLabel->setVisible(false);
        }

        this->updateShenanigans(pOptions);
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
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::layer<GroupLabelShenanigans::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        auto ptr = fields->options.get();

        if (ptr) {
            getObjectArray().clear();
            fields->lastObjectCount = -1;

            editor::object::forEachInSection([&, self] (GameObject* pObj) {
                if (!canHaveLabel(pObj)) {
                    return;
                }

                auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(pObj);

                obj->removeShenanigans();
                self->updateObjectLabel(pObj);
            });

            parseLabels(*ptr);

            self->updateLabelsInSection(false);
        }
    }
}