#include "PageRendering.h"
#include "TextRendering.h"
#include "BoxRendering.h"

PageRendering::PageRendering(const Page &page,
                  const ShaderProgram& spFont,
                  const ShaderProgram& spBox):
_page(page),
_spFont(spFont),
_spBox(spBox),
_textRenderings(createTextRenderings(page)),
_boxRenderings(createBoxRenderings(page)),
_arrowRenderings(createArrowRenderings(page)),
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

            if (currentQuadVao != textRendering->getQuadVAO())
            {
              const Quad &displayQuad = textRendering->getDisplayQuad();
              displayQuad.bind();
              currentQuadVao = textRendering->getQuadVAO();
            }

            _spFont.bindUniform("fontColor", textRendering->getTextColor());
            const std::vector<size_t>& indices = lookupTable.indices;
            for (const size_t& index : indices ) {
                textRendering->render(index);
            }
        }
    }

    _spBox.use();
    for (const BoxRendering_sptr& boxRendering : _boxRenderings) {
        boxRendering->render();
    }
    for (const ArrowRendering_sptr& arrowRendering : _arrowRenderings) {
        arrowRendering->render();
    }
}

vecTextRendering_sptr PageRendering::createTextRenderings(const Page& page)
const {
    vecTextRendering_sptr textRenderings;
    for (const CstLabel_sptr& cstLabel : page.labels()) {
        if (page.type(cstLabel) == Page::TypeOfLabel::Message) {
            textRenderings.push_back(
                std::make_shared<TextRendering>(*cstLabel, _spFont));
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

vecArrowRendering_sptr PageRendering::createArrowRenderings(const Page& page)
const {
    vecArrowRendering_sptr arrowRenderings;
    for (const CstLabel_sptr& cstLabel : page.labels()) {
        if (page.type(cstLabel) == Page::TypeOfLabel::Arrow) {
            arrowRenderings.push_back(
                std::make_shared<ArrowRendering>(*cstLabel, _spBox));
        }
    }
    return arrowRenderings;
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


