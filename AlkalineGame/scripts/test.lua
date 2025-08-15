function Run()
    print("Running test.lua");
    print("GamemodeLogicSubsystem.GetState() = " .. GamemodeLogicSubsystem.GetState())
    TestError()
end

function TestError()
    print("TestError()")
    nonexistentfunction()
end

Run()