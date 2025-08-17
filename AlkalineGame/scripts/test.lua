if not connected then
    connected = false
end

function Run()
    print("Running test.lua");
    -- print("GamemodeLogicSubsystem.GetState() = " .. GamemodeLogicSubsystem.GetState())
    -- print("Input.IsKeyDown(32) = " .. tostring(Input.IsKeyDown(32)))
    
    if not connected then
        System.ConnectToNotification("TestNotification", MyCallback)
        System.ConnectToNotification("OnKeyPressed", OnKeyPressed)
        connected = true
    end

    -- local v = Vector()
    -- local v2 = Vector(1.5, 2)
    -- print(v)
    -- print(v2)

    -- local e = Game.GetRandomEntity()
    -- print("Game.GetRandomEntity()=" ..e .. type(e))
    -- local pos = Game.GetEntityPosition(e)
    -- print("pos=" .. tostring(pos))
    -- TestError()
end

function MyCallback(...)
    print(...)
end

e = Game.GetRandomEntity()

function OnKeyPressed(key)
    -- print("OnKeyPressed: " .. key)
    local pos = Game.GetEntityPosition(e)
    local updatePos = false
    if key == 262 then -- right
        updatePos = true
        pos.x = pos.x + 10
    elseif key == 263 then -- left
        updatePos = true
        pos.x = pos.x - 10
    elseif key == 264 then -- down
        updatePos = true
        pos.y = pos.y + 10
    elseif key == 265 then -- up
        updatePos = true
        pos.y = pos.y - 10
    end

    if updatePos then
        Game.SetEntityPosition(e, pos)
    end
end

function TestError()
    print("TestError()")
    nonexistentfunction()
end

Run()