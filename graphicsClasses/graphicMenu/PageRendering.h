#ifndef PAGERENDERING_H
#define PAGERENDERING_H
#include <gameMenu/Page.h>
#include <system/ParallelTask.h>
#include "LabelRendering.h"
#include "Utility.h"

class PageRendering;
using PageRendering_sptr = std::shared_ptr<PageRendering>;
using CstPageRendering_sptr = std::shared_ptr<const PageRendering>;
using vecCstPageRendering_sptr = std::vector<CstPageRendering_sptr>;
using vecPageRendering_sptr = std::vector<PageRendering_sptr>;

class PageRendering
{
public:
    PageRendering(const Page& page,
                  float maxHeight,
                  const ShaderProgram& spFont,
                  const ShaderProgram& spBox
                  );
    void update();
    void render() const;

private:
    const Page& _page;
    const ShaderProgram& _spFont;
    const ShaderProgram& _spBox;
    vecLabelRendering_sptr _labelRenderings;
    ParallelTask<void> _labelRenderingsUpdate;

    vecLabelRendering_sptr createLabelRenderings(
            const Page& page, float maxHeight) const;

    static void updateCharactersList(const Page& page);
};

#endif // PAGERENDERING_H
