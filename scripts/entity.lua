local Entity = {}

function Entity:OnStart()
    local entity = Entity()
    Debug.Log("Entity '", entity:GetName(), "' isValid=", entity:IsValid())
end

return Entity