if not connected then
    connected = false
end

function Run()
    print("Running test.lua");
    -- print("GamemodeLogicSubsystem.GetState() = " .. GamemodeLogicSubsystem.GetState())
    print("Input.IsKeyDown(32) = " .. tostring(Input.IsKeyDown(32)))
    
    if not connected then
        System.ConnectToNotification("TestNotification", MyCallback)
        System.ConnectToNotification("OnKeyPressed", OnKeyPressed)
        connected = true
    end

    local v = Vector()
    local v2 = Vector(1.5, 2)
    print(v)
    print(v2)
    -- TestError()
end

function MyCallback(...)
    print(...)
end

function OnKeyPressed(key)
    print("OnKeyPressed: " .. key)
end

function TestError()
    print("TestError()")
    nonexistentfunction()
end

Run()