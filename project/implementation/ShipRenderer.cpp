#ifndef SHIP_RENDERER_H
#define SHIP_RENDERER_H

#include "Core.h"
#include "ObjectRenderer.h"
#include "GameObject.h"
#include "TheRenderer.h"

class ShipRenderer : public ObjectRenderer {
public:
    vmml::Vector3f color;
    
    void init() override {
        color = vmml::Vector3f(0.1f, 1.0f, 1.0f);
        if (TheRenderer::Instance()->renderer->getObjects()->getModel("ship") == nullptr) {
            ShaderPtr customShader = TheRenderer::Instance()->renderer->getObjects()->generateShader("customShader", { 2, true, true, true, true, true, true, false, false, false, false, false, false });	// automatically generates a shader with a maximum of 2 lights
            TheRenderer::Instance()->renderer->getObjects()->loadObjModel("ship.obj", false, true, customShader);
        }
    }
    
    void render() override {
        vmml::Matrix4f modelMatrix = gameObject->getComponent<Transform>()->getTransformationMatrix();
        TheRenderer::Instance()->renderer->getModelRenderer()->queueModelInstance("ship", "ship" + std::to_string(gameObject->getId()), "camera", modelMatrix, std::vector<std::string>({ "firstLight" }), true, false, false);
    }
    
    void destroy() override {
        
    }
};

#endif /* defined(SHIP_RENDERER_H) */
