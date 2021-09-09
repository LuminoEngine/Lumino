
#include "ViewModel.hpp"
#include "Application.hpp"
#include "MainWindow.hpp"
#include <imgui.h>
#include "../../Engine/src/Mesh/MeshModelInstance.hpp"


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

    //ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Nodes");
    if (meshComponent) {
        const auto& model = meshComponent->model();
        const auto& nodes = model->m_nodes;

        for (int iNode = 0; iNode < nodes.size(); iNode++) {
            ImGui::BulletText("Node[%d] %s", iNode, nodes[iNode]->name().toStdString().c_str());
        }
    }
    ImGui::End();


    ImGui::Begin("Skeletons");


    if (meshComponent) {

        if (const auto& modelInstance = meshComponent->modelInstance()) {
            const auto& skeletons = modelInstance->skeletons();
            for (int iSkeleton = 0; iSkeleton < modelInstance->skeletons().size(); iSkeleton++) {
                const auto& skeleton = skeletons[iSkeleton];
                if (ImGui::TreeNodeEx(skeleton, ImGuiTreeNodeFlags_DefaultOpen, "Skeleton[%d]", iSkeleton)) {

                    const auto& bones = skeleton->bones();
                    for (int iBone = 0; iBone < bones.size(); iBone++) {
                        const auto& bone = bones[iBone];

                        ImGui::BulletText("Bone[%d] %s", iBone, bone->node()->name().toStdString().c_str());
                    }

                    ImGui::TreePop();
                }
            }
        }

    }

    ImGui::End();


    //ImGui::SetNextWindowPos(ImVec2(200, 10));
    ImGui::Begin("Animations");

    if (meshComponent) {
        if (ln::AnimationController* ac = meshComponent->model()->animationController()) {
            const auto& layers = ac->core()->layers();
            for (const auto& layer : layers) {
                for (const auto& state : layer->animationStatus()) {

                    ImGui::Text("%s", state->name().toStdString().c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton(("Play##" + std::to_string(reinterpret_cast<intptr_t>(state.get()))).c_str())) {
                        ac->play(state);
                    }
                    //ImGui::TreePop();

                    //ImGui::BulletText("%s", state->name().toStdString().c_str());
                }
            }
        }

    }

    ImGui::End();
}
