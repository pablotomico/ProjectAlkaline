return {
    InputActions = {
        move_up = { name = "Move Up", key = 265 },
        move_down = { name = "Move Down", key = 264 },
        move_left = { name = "Move Left", key = 263 },
        move_right = { name = "Move Right", key = 262 },
        spawn_sigil = { name = "Spawn Sigil", key = 257 },
    },
    InputContexts = {
        PlayerMovement = {
            actions = {
                "move_up",
                "move_down",
                "move_left",
                "move_right",
                "spawn_sigil",
            }
        }
    }
}
