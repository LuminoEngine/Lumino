#pragma once

class VisualizedMeshComponent
    : public ln::MeshComponent
{
public:

protected:
    void onRenderGizmo(ln::RenderingContext* context) override;

private:
};

