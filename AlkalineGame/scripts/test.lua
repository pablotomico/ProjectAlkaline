function Run()
    print("Running test.lua");
    print("GamemodeLogicSubsystem.GetState() = " .. GamemodeLogicSubsystem.GetState())
    print("Input.IsKeyDown(32) = " .. tostring(Input.IsKeyDown(32)))
    
    System.ConnectToNotification("TestNotification", MyCallback)
    System.ConnectToNotification("OnKeyPressed", OnKeyPressed)
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