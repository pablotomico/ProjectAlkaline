
local Constants = 
{
	OverrideFunctionPrefix = "Override_"
}

local System = {
	--- Creates a merged copy of the given tables
	--- Replaces all functions with chained functions which are called in the order of the inheritance chain
	---@param self any
	---@param ... table
	---@return table
	CreateChainedInheritanceScript = function(self, ...)
		local scripts = {...}
		local extractedFunctionLists = {}
		local createdScript = {}

		for i = 1, #scripts do
			for key, value in pairs(scripts[i]) do
				if type(value) == "function" then
					local functionName = key
					if not extractedFunctionLists[functionName] then
						extractedFunctionLists[functionName] = {}
					end

					if string.startsWith(functionName, Constants.OverrideFunctionPrefix) then
						functionName = string.gsub(functionName, Constants.OverrideFunctionPrefix, "")
						extractedFunctionLists[functionName] = { value }
					else
						table.insert(extractedFunctionLists[functionName], value)
					end
				else
					createdScript[key] = value
				end
			end
		end

		for functionName, functionList in pairs(extractedFunctionLists) do
			createdScript[functionName] = self:CreateChainedFunction(functionList)
		end

		return createdScript
	end,

	--- Creates a single function which calls every function in the given function list in order
	---@param self any
	---@param functionList table
	---@return function
	CreateChainedFunction = function(self, functionList)
		return function(...)
			for index, func in ipairs(functionList) do
				-- We can only return the value of the last function in the list
				if index == table.count(functionList) then
					return func(...)
				else
					func(...)
				end
			end
		end
	end,
}
return System