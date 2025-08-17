---@diagnostic disable //TODO: enable when the globals generation is fixed

local entity = {}

function entity:OnStart(owner)
    print(Game.GetEntityPosition(self.owner))
end

function entity:OnUpdate(deltaTime)
    -- print("Entity:OnUpdate: " .. deltaTime)
end

return entity