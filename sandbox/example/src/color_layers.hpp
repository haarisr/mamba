#pragma once

#include "layer.hpp"

#include <glad/glad.h>

class RedLayer : public mamba::Layer {
  public:
    void onRender() override {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class GreenLayer : public mamba::Layer {
  public:
    void onRender() override {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class BlueLayer : public mamba::Layer {
  public:
    void onRender() override {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
