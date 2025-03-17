
--- Remove a single element from a table by the value
--- @param t table
--- @param v any
--- @return any
table.removeByValue = function(t, v)
    for key, value in pairs(t) do
        if value == v then
            return table.remove(t, key)
        end
    end
end

--- Return the number of elements in a table
--- @param t table
--- @return number
table.count = function(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end

--- Check if a table is empty (contains 0 elements)
--- @param t table
--- @return boolean
table.isEmpty = function(t)
    return table.count(t) == 0
end

--- Returns the key for a given element in a table
--- @param t table
--- @param v any
--- @return string|number|nil
table.findKey = function(t, v)
    for key, value in pairs(t) do
        if value == v then
            return key
        end
    end
    return nil
end

--- Returns the last element of a table (only works for numeric tables)
--- @param t table
--- @return any
table.findLastElement = function(t)
    return t[t.count]
end

--- Shallow copy a table
--- @param t table
--- @return table
table.simpleCopy = function(t)
	local copy = {}
	for key, value in pairs(t) do
		copy[key] = value
	end
	return copy
end

--- Deep copy a table (includes nested tables)
--- @param t table
--- @return table
table.deepCopy = function(t)
    local copy = {}
    for key, value in pairs(t) do
        if type(value) == "table" then
            copy[key] = table.deepCopy(value) -- Recursively copy nested tables
        else
            copy[key] = value -- Directly copy non-table values
        end
    end
    return copy
end

--- Deep copy a table, preserving metatables
--- @param t table
--- @return table
table.deepCopyWithMetaTable = function(t)
    local copy = {}
    for key, value in pairs(t) do
        if type(value) == "table" then
            copy[key] = table.deepCopyWithMetaTable(value) -- Recursively copy nested tables
        else
            copy[key] = value -- Directly copy non-table values
        end
    end
    -- Copy the metatable of the original table
    setmetatable(copy, getmetatable(t))
    return copy
end

--- Returns a value clamped between the given lower and upper values
--- @param num number
--- @param lower number
--- @param upper number
--- @return number
math.clamp = function(num, lower, upper)
    if num > upper then
        return upper
    end
    if num < lower then
        return lower
    end
    return num
end

--- Returns boolean whether a string starts with a given sub-string
--- @param s string
--- @param subString string
--- @return boolean
string.startsWith = function(s, subString)
    return string.sub(s, 1, s.len(subString)) == subString
end