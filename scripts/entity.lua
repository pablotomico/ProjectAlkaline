local Entity = {}

function Entity:OnStart()
    local actor = Actor()
    print(actor:IsValid())
end

return Entity