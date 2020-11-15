#include "PageRendering.h"
#include "TextRendering.h"
#include "BoxRendering.h"

PageRendering::PageRendering(const Page &page, float maxHeight,
                  const ShaderProgram& spFont,
                  const ShaderProgram& spBox):
_page(page),
_spFont(spFont),
_spBox(spBox),
_textRenderings(createTextRenderings(page,maxHeight)),
_boxRenderings(createBoxRenderings(page)),
_labelRenderings(createLabelRenderings()),
_labelRenderingsUpdate( [this](size_t renderingLabelNumber){
    _labelRenderings.at(renderingLabelNumber)->update(_page.localPosY());
}, _labelRenderings.size()),
_charactersLookUpTable(createCharactersLookUpTable())
{
}

void PageRendering::update()
{
    _labelRenderingsUpdate.runTasks();
    _labelRenderingsUpdate.waitTasks();
}

void PageRendering::render() const
{
    GLuint currentQuadVao = 0;
    const auto updateQuadVao = [&currentQuadVao]
                               (const CstLabelRendering_sptr &labelRendering) {
        if (currentQuadVao != labelRendering->getQuadVAO()) {
            const Quad &displayQuad = labelRendering->getDisplayQuad();
            displayQuad.bind();
            currentQuadVao = labelRendering->getQuadVAO();
        }
    };

    _spFont.use();
    for (std::map<GLuint, std::vector<LettersLookupTable>>::const_iterator
             it = _charactersLookUpTable.begin();
         it != _charactersLookUpTable.end();
         ++it)
    {
        const GLuint& characterID = it->first;
        _spFont.bindUniformTexture("characterTexture", 0,
                                    characterID);
        const std::vector<PageRendering::LettersLookupTable> &lookupTables =
            it->second;
        for (const PageRendering::LettersLookupTable &lookupTable :
             lookupTables)
        {
            const CstTextRendering_sptr& textRendering = 
                lookupTable.textRendering;
            updateQuadVao(textRendering);
            _spFont.bindUniform("fontColor", textRendering->getTextColor());
            const std::vector<size_t>& indices = lookupTable.indices;
            for (const size_t& index : indices ) {
                textRendering->render(index);
            }
        }
    }

    _spBox.use();
    for (const BoxRendering_sptr& boxRendering : _boxRenderings)
    {
        updateQuadVao(boxRendering);
        boxRendering->render();
    }
}

vecTextRendering_sptr PageRendering::createTextRenderings(
    const Page& page, float maxHeight
) const {
    vecTextRendering_sptr textRenderings;
    for (const CstLabel_sptr& cstLabel : page.labels()) {
        if (page.type(cstLabel) == Page::TypeOfLabel::Message) {
            textRenderings.push_back(
                std::make_shared<TextRendering>(*cstLabel, maxHeight, _spFont));
        }
    }
    return textRenderings;
}

vecBoxRendering_sptr PageRendering::createBoxRenderings(const Page& page)
const {
    vecBoxRendering_sptr boxRenderings;
    for (const CstLabel_sptr& cstLabel : page.labels()) {
        if (page.type(cstLabel) == Page::TypeOfLabel::Box) {
            boxRenderings.push_back(
                std::make_shared<BoxRendering>(
                    *cstLabel,
                    glm::vec3(0.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, 0.f),
                    _spBox)
            );
        }
    }
    return boxRenderings;
}

vecLabelRendering_sptr PageRendering::createLabelRenderings() const
{
    vecLabelRendering_sptr labelRenderings;
    for (TextRendering_sptr textRendering : _textRenderings) {
        labelRenderings.push_back(textRendering);
    }
    for (BoxRendering_sptr boxRendering : _boxRenderings) {
        labelRenderings.push_back(boxRendering);
    }
    return labelRenderings;
}

std::map<GLuint, std::vector<PageRendering::LettersLookupTable> >
    PageRendering::createCharactersLookUpTable() const
{
    std::map<GLuint, std::vector<PageRendering::LettersLookupTable> >
        charactersLookUpTable;

    const std::vector<GLuint>& alphabetCharactersIds = 
        TextRendering::getAlphabetCharactersIds();
    
    for (const GLuint alphabetCharacterId : alphabetCharactersIds) {
        std::vector<PageRendering::LettersLookupTable> lettersLookupTables;
        for (const TextRendering_sptr &textRendering : _textRenderings) {
            const std::vector<size_t> indicesWithID = 
                textRendering->getIndicesWithID(alphabetCharacterId);
            if (indicesWithID.size() > 0) {
                const PageRendering::LettersLookupTable lettersLookupTable 
                    { textRendering, indicesWithID };
                lettersLookupTables.push_back(lettersLookupTable);
            }
        }
        if (lettersLookupTables.size() > 0) {
            charactersLookUpTable[alphabetCharacterId] = lettersLookupTables;
        }
    }

    return charactersLookUpTable;
}


