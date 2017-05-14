#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include "Core.h"
#include "ObjectRenderer.h"
#include "GameObject.h"
#include "TheRenderer.h"

class CubeRenderer : public ObjectRenderer {
public:
    
    void init() override {
        
        if (TheRenderer::Instance()->renderer->getObjects()->getModel("cube") == nullptr) {
            
            // Load the shader to render the cube for the skybox
            ShaderPtr skyShader = TheRenderer::Instance()->renderer->getObjects()->loadShaderFile("sky",0,false,false,false,false,false);
            
            // Load cube model
            TheRenderer::Instance()->renderer->getObjects()->loadObjModel("cube.obj", true, false, skyShader);
            
            // TODO: where should the depth test be disabled??
            // Load the images for the cubemap
            TextureData skyL = TextureData("ame_nebula/purplenebula_lf.tga");
            TextureData skyR = TextureData("ame_nebula/purplenebula_rt.tga");
            TextureData skyB = TextureData("ame_nebula/purplenebula_dn.tga");
            TextureData skyT = TextureData("ame_nebula/purplenebula_up.tga");
            TextureData skyF = TextureData("ame_nebula/purplenebula_ft.tga");
            TextureData skyBack = TextureData("ame_nebula/purplenebula_bk.tga");
            
            // Put the 6 textures into a vector
            std::vector<TextureData> skyImages;
            skyImages.push_back(skyL);
            skyImages.push_back(skyR);
            skyImages.push_back(skyB);
            skyImages.push_back(skyT);
            skyImages.push_back(skyF);
            skyImages.push_back(skyBack);
            
            // Create the cupemap
            CubeMapPtr skybox = TheRenderer::Instance()->renderer->getObjects()->createCubeMap("skybox", skyImages);
            
        }
    }
    
    void render() override {
    
        vmml::Matrix4f modelMatrix = gameObject->getComponent<Transform>()->getTransformationMatrix();
        
        // Update position of skybox as camera moves
        gameObject->getComponent<Transform>()->position = TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->getPosition() * -1;

        ShaderPtr shader = TheRenderer::Instance()->renderer->getObjects()->getShader("sky");
        
        // Keep the camera centered in the skybox by setting the last column of the view matrix to zero so that
        // the skybox can still rotate with the camera but it is not traslated anymore
        vmml::Matrix4f view = TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->getViewMatrix();
        view[0][3] = 0;
        view[1][3] = 0;
        view[2][3] = 0;
        
        vmml::Matrix4f projection = TheRenderer::Instance()->renderer->getObjects()->getCamera("camera")->getProjectionMatrix();

        CubeMapPtr skybox = TheRenderer::Instance()->renderer->getObjects()->getCubeMap("skybox");
        
        if (shader.get()){
            
            // .vert uniforms
            shader->setUniform("ModelMatrix", modelMatrix);
            shader->setUniform("ViewMatrix", view);
            shader->setUniform("ProjectionMatrix", projection);
            
            // .frag uniforms
            shader->setUniform("CubeMap", skybox);
        }
        
        // Put an instance of the skybox in the render queue
        TheRenderer::Instance()->renderer->getModelRenderer()->queueModelInstance("cube", "cube_instance" + std::to_string(gameObject->getId()), "camera", modelMatrix, std::vector<std::string>({ "firstLight" }), true, false, false);
    }
    
    void destroy() override {
        
    }
};

#endif /* defined(CUBE_RENDERER_H) */