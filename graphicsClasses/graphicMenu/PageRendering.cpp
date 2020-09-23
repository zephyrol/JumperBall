#include "PageRendering.h"
#include "TextRendering.h"
#include "BoxRendering.h"

PageRendering::PageRendering(const Page &page, float maxHeight,
                  const ShaderProgram& spFont,
                  const ShaderProgram& spBox):
_page(page),
_spFont(spFont),
_spBox(spBox),
_labelRenderings(createLabelRenderings(page,maxHeight)),
_labelRenderingsUpdate( [this](size_t renderingLabelNumber){
    _labelRenderings.at(renderingLabelNumber)->update(_page.localPosY());
}, _labelRenderings.size())
{
}

void PageRendering::update()
{
    _labelRenderingsUpdate.runTasks();
    _labelRenderingsUpdate.waitTasks();
}

void PageRendering::render() const
{
    for ( const LabelRendering_sptr& labelRendering : _labelRenderings) {
        labelRendering->render();
    }
}

vecLabelRendering_sptr PageRendering::createLabelRenderings(
        const Page &page, float maxHeight) const
{
    vecLabelRendering_sptr labelRenderings;
    for (const CstLabel_sptr& cstLabel : page.labels()) {
        LabelRendering_sptr labelRendering = nullptr;
        switch (page.type(cstLabel)) {
            case Page::TypeOfLabel::Unknown :
            break;
            case Page::TypeOfLabel::Object :
            break;
            case Page::TypeOfLabel::Message:
            labelRendering = std::make_shared<TextRendering>(*cstLabel,
                                                             maxHeight,
                                                             _spFont);
            break;
            case Page::TypeOfLabel::Switch:
            break;
            case Page::TypeOfLabel::Box:
            labelRendering = std::make_shared<BoxRendering>(
                        *cstLabel,
                        glm::vec3(0.f,0.f,0.f),
                        glm::vec3(0.f,0.f,0.f),
                        _spBox);
            break;
            default :
            break;
        }
        labelRenderings.push_back(labelRendering);
    }

    return labelRenderings;
}


