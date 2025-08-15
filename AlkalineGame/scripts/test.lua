function Run()
    print("Running test.lua");
    print("GamemodeLogicSystem.GetState() = " .. GamemodeLogicSystem.GetState())
    TestError()
end

function TestError()
    print("TestError()")
    nonexistentfunction()
end

Run()