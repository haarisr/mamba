#pragma once

#include <glad/glad.h>

#include "app.hpp"
#include "layer.hpp"

class RedLayer : public mamba::Layer {
  public:
    void onRender() override {
        auto& renderer = getApp()->getRenderer();
        renderer.setClearColor({1.0f, 0.0f, 0.0f, 1.0f});
        renderer.clear();
    }
};

class GreenLayer : public mamba::Layer {
  public:
    void onRender() override {
        auto& renderer = getApp()->getRenderer();
        renderer.setClearColor({0.0f, 1.0f, 0.0f, 1.0f});
        renderer.clear();
    }
};

class BlueLayer : public mamba::Layer {
  public:
    void onRender() override {
        auto& renderer = getApp()->getRenderer();
        renderer.setClearColor({0.0f, 0.0f, 1.0f, 1.0f});
        renderer.clear();
    }
};
