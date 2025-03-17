
table.removeByValue = function(t, v)
    for key, value in pairs(t) do
        if value == v then
            table.remove(t, key)
            break
        end
    end
end

table.count = function(t)
    local count = 0
    for _ in pairs(t) do
        count = count + 1
    end
    return count
end

table.isEmpty = function(t)
    return table.count(t) == 0
end

table.findKey = function(t, v)
    for key, value in pairs(t) do
        if value == v then
            return key
        end
    end
    return nil
end

table.findLastElement = function(t)
    return t[t.count]
end

table.simpleCopy = function(t)
	local copy = {}
	for key, value in pairs(t) do
		copy[key] = value
	end
	return copy
end

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

math.clamp = function(num, lower, upper)
    if num > upper then
        return upper
    end
    if num < lower then
        return lower
    end
    return num
end

string.startsWith = function(s, subString)
    return string.sub(s, 1, s.len(subString)) == subString
end