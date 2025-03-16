local Entity = {}

function tprint(table)
    for k, v in pairs(table) do
        if type(v) == "table" then
            tprint(v)
        elseif type(v) == "function" then
            print(k .. ": <function>")
        else
            print(k .. ": " .. v)
        end
    end
end

function Entity:OnStart()
    local res = CustomUtils.TestFunction(1, 2)
    print("Entity:OnStart "..res)

    local res = CustomUtils.TestOtherFunction()
    print("Entity:OnStart "..res)

    Debug.Log("getting actor")
    local actor = Debug.GetActor()
    local isValid = actor:IsValid()

    print(isValid)

    local second = Actor()
    print(second:IsValid())
end

return Entity