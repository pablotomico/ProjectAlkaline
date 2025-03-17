local Entity = {}

function Entity:OnStart()
    local actor = Actor()
    Debug.Log("Actor '", actor:GetName(), "' isValid=", actor:IsValid())
end

return Entity