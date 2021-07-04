/*
 * File: MenuRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 21 avril 2020, 14:50
 */
#include "MenuRendering.h"

MenuRendering::MenuRendering(const Menu& menu, const FontTexturesGenerator::FTContent& ftContent):
    _menu(menu),
    _graphicAlphabet(FontTexturesGenerator::genGraphicAlphabet(_menu, ftContent)),
    _pagesRenderPassesLetters(createPagesRenderPassesLetters()),
    _pagesRenderPassesOthers(createPagesRenderPassesOthers()),
    _pagesRenderPasses(createPagesRenderPasses()),
    _pagesRenderProcess(createPagesRenderProcess()) {
}

void MenuRendering::update() {

    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }

    for (const auto& renderPass : _pagesRenderPasses.at(currentPage)) {
        renderPass->update();
    }
    _pagesRenderProcess.at(currentPage)->updateUniforms();
}

void MenuRendering::render() const {
    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    _pagesRenderProcess.at(currentPage)->render();
}

MenuRendering::PagesRenderPassesLetters MenuRendering::createPagesRenderPassesLetters() const {
    MenuRendering::PagesRenderPassesLetters pageRenderPassesLetters;

    for (const auto& page : _menu.pages()) {
        MenuRendering::RenderPassesLetters renderPassesLetters;

        const auto lettersMeshes =  MeshGenerator::genLettersLabel(page, _graphicAlphabet);

        for (const auto& letterMeshes : lettersMeshes) {
            const unsigned char letter = letterMeshes.first;
            const vecMesh_sptr meshes = letterMeshes.second;
            renderPassesLetters[std::make_shared <RenderPass>(meshes)] = letter;
        }

        pageRenderPassesLetters[page] = renderPassesLetters;
    }

    return pageRenderPassesLetters;
}

MenuRendering::PagesRenderPasses MenuRendering::createPagesRenderPassesOthers() const {
    MenuRendering::PagesRenderPasses pageRenderPassesOthers;

    for (const auto& page : _menu.pages()) {
        const auto meshes = MeshGenerator::genOthersLabel(page);
        pageRenderPassesOthers[page] = { std::make_shared <RenderPass>(meshes) };
    }

    return pageRenderPassesOthers;
}

MenuRendering::PagesRenderPasses MenuRendering::createPagesRenderPasses() const {
    MenuRendering::PagesRenderPasses pagesRenderPasses;

    for (const auto& pageRenderPassesLetters : _pagesRenderPassesLetters) {
        const auto& page = pageRenderPassesLetters.first;
        const auto& renderPassesLetters = pageRenderPassesLetters.second;

        vecRenderPass_sptr renderPasses;
        for (const auto& letterRenderPass : renderPassesLetters) {
            renderPasses.push_back(letterRenderPass.first);
        }
        pagesRenderPasses[page] = renderPasses;
    }

    for (const auto& pageRenderPassesOthers : _pagesRenderPassesOthers) {
        const auto& page = pageRenderPassesOthers.first;
        const auto& renderPasses = pageRenderPassesOthers.second;

        if (pagesRenderPasses.find(page) == pagesRenderPasses.end()) {
            pagesRenderPasses[page] = renderPasses;
        } else {
            vecRenderPass_sptr& pageRenderPasses = pagesRenderPasses.at(page);
            pageRenderPasses.insert(pageRenderPasses.end(), renderPasses.begin(), renderPasses.end());
        }
    }

    return pagesRenderPasses;
}

RenderProcess_sptr MenuRendering::createRenderProcess (const CstPage_sptr& page) const {

    const auto getRenderPassesLetters =
        [this, &page] ()->MenuRendering::RenderPassesLetters {
            const auto& pageRenderPassesLetters = _pagesRenderPassesLetters.find(page);
            if (pageRenderPassesLetters == _pagesRenderPassesLetters.end()) {
                return {};
            }
            const RenderPassesLetters& renderPassesLetters = pageRenderPassesLetters->second;
            return renderPassesLetters;
        };
    const MenuRendering::RenderPassesLetters renderPassesLetters = getRenderPassesLetters();

    const auto getRenderPassesUsingLetters =
        [&renderPassesLetters] ()->vecRenderPass_sptr {
            vecRenderPass_sptr renderPassesUsingLetters;
            for (const auto& renderPassLetter : renderPassesLetters) {
                renderPassesUsingLetters.push_back(renderPassLetter.first);
            }
            return renderPassesUsingLetters;
        };
    const vecRenderPass_sptr renderPassesUsingLetters = getRenderPassesUsingLetters();

    const auto getRenderPassesOthers = [this, &page] ()->vecRenderPass_sptr {
                                           if (
                                               _pagesRenderPassesOthers.find(page) ==
                                               _pagesRenderPassesOthers.end()
                                               ) {
                                               return {};
                                           }
                                           return _pagesRenderPassesOthers.at(page);
                                       };
    const vecRenderPass_sptr renderPassesOthers = getRenderPassesOthers();

    const auto createShaders =
        [this, &page, &renderPassesUsingLetters, &renderPassesOthers] ()->RenderProcess::PassShaderMap {

            const CstShaderProgram_sptr lettersShaderPrograms = ShaderProgram::createShaderProgram(
                "fontVs.vs",
                "fontFs.fs"
                );
            const CstShaderProgram_sptr othersShaderPrograms = ShaderProgram::createShaderProgram(
                "labelVs.vs",
                "labelFs.fs"
                );

            RenderProcess::PassShaderMap passShaderMap;

            for (const auto& renderPass : renderPassesUsingLetters) {
                passShaderMap[renderPass] = lettersShaderPrograms;
            }
            for (const auto& renderPass : renderPassesOthers) {
                passShaderMap[renderPass] = othersShaderPrograms;
            }
            return passShaderMap;
        };

    const auto renderingLettersUpdateFct =
        [this, &page] (const RenderPass_sptr& letterRenderPass, GLuint shaderProgramID)->void {

            // TODO: use a object "page state" and access the values
            // TODO 2: Should we use an external uniform ?
            letterRenderPass->upsertUniform(shaderProgramID, "positionY", page->localPosY());

            const auto& renderPassesLetters = _pagesRenderPassesLetters.at(page);
            const unsigned char character = renderPassesLetters.at(letterRenderPass);

            letterRenderPass->upsertUniformTexture(
                shaderProgramID,
                "characterTexture",
                _graphicAlphabet.at(character).textureID
                );
        };
    const auto renderingOthersUpdateFct =
        [this, &page] (const RenderPass_sptr& othersRenderPass, GLuint shaderProgramID)->void {

            // TODO: use a object "page state" and access the values
            // TODO 2: Should we use an external uniform ? Yes because it's common with renderingLetUpFct
            othersRenderPass->upsertUniform(shaderProgramID, "positionY", page->localPosY());
        };
    const auto createRenderPassRenderingFctAssociation =
        [
            &renderPassesUsingLetters,
            &renderPassesOthers,
            &renderingLettersUpdateFct,
            &renderingOthersUpdateFct
        ] ()->RenderProcess::PassUniformUpdateMap {
            RenderProcess::PassUniformUpdateMap passUniformUpdateMap;
            for (const auto& renderPassLetter : renderPassesUsingLetters) {
                passUniformUpdateMap[renderPassLetter] = renderingLettersUpdateFct;
            }
            for (const auto& renderPassOthers : renderPassesOthers) {
                passUniformUpdateMap[renderPassOthers] = renderingOthersUpdateFct;
            }
            return passUniformUpdateMap;
        };

    vecRenderPass_sptr renderPasses =  renderPassesUsingLetters;
    renderPasses.insert(renderPasses.end(), renderPassesOthers.begin(), renderPassesOthers.end());

    return std::make_shared <RenderProcess>(
        renderPasses,
        createShaders(),
        createRenderPassRenderingFctAssociation(),
        nullptr,
        false
        );
}

MenuRendering::PagesRenderProcess MenuRendering::createPagesRenderProcess() const {
    MenuRendering::PagesRenderProcess pagesRenderProcess;
    for (const auto& page : _menu.pages()) {
        pagesRenderProcess[page] = createRenderProcess(page);
    }
    return pagesRenderProcess;
}

Rendering::ExternalUniformVariables <GLuint> MenuRendering::createExternalUniformTexturesVariables() const {
    return {};
}
