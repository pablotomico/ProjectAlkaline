#include "systems/Scene.h"
#include "systems/World.h"
#include "GridSystem.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/GridPreviewComponent.h"
#include "components/GridEntityComponent.h"
#include "Debug/DebugUI.h"
#include "misc/GridHelpers.h"

namespace alk
{
    namespace GameLogic
    {
        void Scene::Initialize()
        {
            ALK_LOG("Initializing Scene subsystem");

            gridSystem = new GridSystem();
            gridSystem->Initialize();

            // TODO: Figure out a better way to render the grid on screen
            gridRenderEntity = world.CreateEntity();
            world.AddComponent<RenderComponent>(gridRenderEntity, RenderSystem::RenderType::Grid, TILE_WIDTH_HALF, TILE_HEIGHT_HALF, "assets/sprites/test_validGrid.png", "assets/sprites/test_invalidGrid.png");
            world.AddComponent<TransformComponent>(gridRenderEntity, gridSystem->GetGridArray().size());
            world.GetComponent<TransformComponent>(gridRenderEntity)->GetPositionArray() = gridSystem->GetGridArray();
            alk::RenderSystem::AddToScreen(gridRenderEntity);

            gameState.RegisterOnStateChangedCallback([this](EGameState oldState, EGameState newState) {
                OnGameStateChanged(oldState, newState);
            });

            gameState.SetState(EGameState::BUILD);
        }

        void Scene::Update()
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if(world.IsValid(gridPlacementEntity))
                {
                    auto gridPreviewComponent = world.GetComponent<GridPreviewComponent>(gridPlacementEntity);
                    if(!gridPreviewComponent->QueryValidPlacement())
                    {
                        ALK_LOG("Invalid placement");
                        return;
                    }
                    
                    Entity newBuilding = world.CreateEntity();
                    buildings.push_back(newBuilding);
                    world.AddComponent<RenderComponent>(newBuilding, RenderSystem::RenderType::Sprite, "assets/sprites/test_castle.png", Color{ 255, 255, 255, 255 });

                    Vector2 position = world.GetComponent<TransformComponent>(gridPlacementEntity)->GetPosition();
                    Vector2 gridPosition = GridHelpers::WorldToGridPosition(position);
                    world.AddComponent<TransformComponent>(newBuilding, position);
                    world.AddComponent<GridEntityComponent>(newBuilding, gridPosition);
                    world.GetComponent<GridEntityComponent>(newBuilding)->ConvertPreviewToGridMap(gridPreviewComponent->GetValidMap());

                    alk::RenderSystem::AddToScreen(newBuilding);

                    world.DestroyEntity(gridPlacementEntity);
                }
            }
            if(IsKeyPressed(KEY_SPACE))
            {
                gameState.TransitionToNextState();
            }
        }
        void Scene::OnGameStateChanged(EGameState oldState, EGameState newState)
        {
            alk::Debug::UI::RegisterText("GameState", gameState.GetStateString());

            if (newState == EGameState::BUILD)
            {
                alk::Debug::UI::RegisterButton("Spawn Castle", [this]() {
                    if(!world.IsValid(gridPlacementEntity))
                    {
                        gridPlacementEntity = world.CreateEntity();
                        world.AddComponent<RenderComponent>(gridPlacementEntity, RenderSystem::RenderType::Sprite, "assets/sprites/test_castle.png", Color{ 255, 255, 255, 100 });
                        world.AddComponent<TransformComponent>(gridPlacementEntity, Vector2{0, 0});
                        world.AddComponent<GridPreviewComponent>(gridPlacementEntity, Vector2{3, 3});
                        world.GetComponent<RenderComponent>(gridPlacementEntity)->SetDrawLayer(1000);
                        alk::RenderSystem::AddToScreen(gridPlacementEntity);
                    }
                });
            }
            else if (newState == EGameState::BATTLE)
            {
                alk::Debug::UI::UnregisterButton("Spawn Castle");
                if (world.IsValid(gridPlacementEntity))
                {
                    world.DestroyEntity(gridPlacementEntity);
                }
            }
            else if (newState == EGameState::REWARDS)
            {

            }
        }
    }
}