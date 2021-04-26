/*#ifndef PAGERENDERING_H
#define PAGERENDERING_H
#include <gameMenu/Page.h>
#include <system/ParallelTask.h>
#include "TextRendering.h"
#include "BoxRendering.h"
#include "ArrowRendering.h"
#include "ObjectRendering.h"
#include "Utility.h"

class PageRendering;
using PageRendering_sptr = std::shared_ptr <PageRendering>;
using CstPageRendering_sptr = std::shared_ptr <const PageRendering>;
using vecCstPageRendering_sptr = std::vector <CstPageRendering_sptr>;
using vecPageRendering_sptr = std::vector <PageRendering_sptr>;

class PageRendering {
public:
PageRendering(const Page& page);
void update();
void render() const;

private:


struct LettersLookupTable { CstTextRendering_sptr textRendering;
                            std::vector <size_t> indices; };

const Page& _page;

vecTextRendering_sptr _textRenderings;
vecBoxRendering_sptr _boxRenderings;
vecArrowRendering_sptr _arrowRenderings;
vecObjectRendering_sptr _objectRenderings;
vecLabelRendering_sptr _labelRenderings;
ParallelTask <void> _labelRenderingsUpdate;

const std::map <GLuint, std::vector <LettersLookupTable> >
_charactersLookUpTable;

vecTextRendering_sptr createTextRenderings(const Page& page) const;
vecBoxRendering_sptr createBoxRenderings(const Page& page) const;
vecArrowRendering_sptr createArrowRenderings(const Page& page) const;
vecObjectRendering_sptr createObjectRenderings(const Page& page) const;
vecLabelRendering_sptr createLabelRenderings() const;
std::map <GLuint, std::vector <LettersLookupTable> >
createCharactersLookUpTable() const;

};

#endif // PAGERENDERING_H
*/