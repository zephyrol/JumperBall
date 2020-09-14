#ifndef PAGERENDERING_H
#define PAGERENDERING_H
#include <gameMenu/Page.h>
#include <ParallelTask.h>
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
    PageRendering(const Page& page);
    void update();
    void render() const;

private:
    const Page& _page;
    vecLabelRendering_sptr _labelRenderings;
    ParallelTask<void> _labelRenderingsUpdate;

    vecLabelRendering_sptr createLabelRenderings(const Page& page) const;
};

#endif // PAGERENDERING_H
