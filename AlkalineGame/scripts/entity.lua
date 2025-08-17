---@diagnostic disable //TODO: enable when the globals generation is fixed

local entity = {}

function entity:OnStart()
    local testEntity = Entity()
    Debug.Log("Entity '", testEntity:GetName(), "' isValid=", testEntity:IsValid())
end

function entity:OnUpdate(deltaTime)
    print("Entity:OnUpdate: " .. deltaTime)
end

return entity