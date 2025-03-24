local entity = {}

function entity:OnStart()
    local testEntity = BaseEntity()
    Debug.Log("Entity '", testEntity:GetName(), "' isValid=", testEntity:IsValid())
end

return entity