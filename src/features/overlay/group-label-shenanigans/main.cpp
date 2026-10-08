#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static auto& getObjectArray() {
    static std::unordered_set<GroupLabelShenanigans::EffectGameObject*> val;
    return val;
}

static bool canHaveLabel(GameObject* pObj) {
    // second one for robtops player collision objects
    return trigger::type(pObj) != trigger::ObjectType::Normal && !pObj->m_bDontDraw;
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

        object::forEachInSection([&] (GameObject* pObj) {
            if (!canHaveLabel(pObj)) {
                return;
            }

            auto obj = reinterpret_cast<GroupLabelShenanigans::EffectGameObject*>(pObj);

            if (!obj->isVisible()) {
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
        if (!GroupLabelShenanigans::enabled() || !GroupLabelShenanigans::optimize || !canHaveLabel(object)) {
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

    void EffectGameObject::updateShenaniganPositions(geode::Label* pLabel, CCSprite* pSprite) {
        if (pLabel) {
            Setup(pLabel)
                .pos(CCPoint{0.0f, sillyedit::utils::triggerHasBodyOffset(m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET.y : 0.0f} + ui::size(this) / 2)
                .opacity(this)
                .rotation(GroupLabelShenanigans::dontRotateLabel ? -this->getRotation() : 0.0f);
        }

        if (pSprite) {
            Setup(pSprite)
                .pos(ccAdd(ui::size(this) / 2, 8.5f))
                .opacity(sillyedit::utils::modifyOpacity(pSprite->getTag(), this->getOpacity()));
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

        self->addEventListener(ObjectsChangedEvent(), [self] {
            self->updateLabelsInSection();
        });
    }

    void Feature::onUpdate() {
        if (GroupLabelShenanigans::enabled()) {
            editor::layer<GroupLabelShenanigans::LevelEditorLayer>()->updateLabelsInSection(true);
        }
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::layer<GroupLabelShenanigans::LevelEditorLayer>();

        auto ptr = self->m_fields->options.get();

        if (ptr) {
            getObjectArray().clear();

            object::forEachInSection([&, self] (GameObject* pObj) {
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