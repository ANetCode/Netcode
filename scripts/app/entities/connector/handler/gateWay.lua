local M = {}

function M:new()
    local u = { }
    for k, v in pairs(M) do u[k] = v end
    local obj = setmetatable(u, getmetatable(M))
    return obj
end

function M:login( )
    print('do login')
end

print('111')
return M