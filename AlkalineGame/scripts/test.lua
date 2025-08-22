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

    Input.LoadInputContext("PlayerMovement")
end

function MyCallback(...)
    print(...)
end

e = Game.GetRandomEntity()

function OnKeyPressed(action)
    print("OnKeyPressed: " .. action)
    local pos = Game.GetEntityPosition(e)
    local updatePos = false
    if action == "move_right" then -- right
        updatePos = true
        pos.x = pos.x + 10
    elseif action == "move_left" then -- left
        updatePos = true
        pos.x = pos.x - 10
    elseif action == "move_down" then -- down
        updatePos = true
        pos.y = pos.y + 10
    elseif action == "move_up" then -- up
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