
#include "ViewModel.hpp"
#include "Application.hpp"
#include "MainWindow.hpp"
#include "../../../build/BuildCache/imgui/imgui.h"
#include "../../LuminoEngine/src/Mesh/MeshModelInstance.hpp"


MainWindow::MainWindow()
{
}

void MainWindow::onLoaded()
{
	setImGuiLayerEnabled(true);
	m_onImGuiLayer.connect(ln::makeDelegate(this, &MainWindow::handleImGUI));
}

void MainWindow::handleImGUI(ln::UIEventArgs* e)
{
    const auto& meshComponent = ViewModel::instance()->meshComponent();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Statistics");


    //if (meshComponent) {

    //    if (const auto& modelInstance = meshComponent->modelInstance()) {
    //        const auto& skeletons = modelInstance->skeletons();
    //        for (int iSkeleton = 0; iSkeleton < modelInstance->skeletons().size(); iSkeleton++) {
    //            const auto& skeleton = skeletons[iSkeleton];
    //            if (ImGui::TreeNodeEx(skeleton, ImGuiTreeNodeFlags_DefaultOpen, "Skeleton[%d]", iSkeleton)) {

    //                const auto& bones = skeleton->bones();
    //                for (int iBone = 0; iBone < bones.size(); iBone++) {
    //                    const auto& bone = bones[iBone];

    //                    ImGui::BulletText("Bone[%d] %s", iBone, bone->node()->name().toStdString().c_str());
    //                }

    //                ImGui::TreePop();
    //            }
    //        }
    //    }

    //}

    ImGui::End();


    ImGui::SetNextWindowPos(ImVec2(200, 10));
    ImGui::Begin("Animations");

    //if (meshComponent) {
    //    if (ln::AnimationController* ac = meshComponent->model()->animationController()) {
    //        const auto& layers = ac->core()->layers();
    //        for (const auto& layer : layers) {
    //            for (const auto& state : layer->animationStatus()) {
    //                ImGui::BulletText("%s", state->name().toStdString().c_str());
    //            }
    //        }
    //    }

    //}

    ImGui::End();
}
