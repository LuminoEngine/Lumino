#pragma once

class VisualizedMeshComponent
    : public ln::StaticMeshComponent
{
public:

protected:
    void onRenderGizmo(ln::RenderingContext* context) override;

private:
};

