local entity = {}

function entity:OnStart()
    local testEntity = Entity()
    Debug.Log("Entity '", testEntity:GetName(), "' isValid=", testEntity:IsValid())
end

return entity