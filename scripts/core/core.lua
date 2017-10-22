require('os')
require('lfs')
require('core/utils')

local M = {
    entities_path='app/entities/',
    entities={}
}

function M:new()
    local u = { }
    for k, v in pairs(M) do u[k] = v end
    local obj = setmetatable(u, getmetatable(M))
    return obj
end

function M:copy(obj)
  if type(obj) ~= 'table' then return obj end
  local res = {}
  for k, v in pairs(obj) do res[copy1(k)] = copy1(v) end
  return res
end

function M:serialize(t)
    if type(t) ~= 'table' then
        if type(t) == 'string' then
            return 'do local ret = "' .. t .. '" return ret end'
        elseif type(t) == 'number' then
            return 'do local ret = ' .. t .. ' return ret end'
        else
            return 'return nil'
        end
    end

    local mark={}
    local assign={}

    local function ser_table(tbl,parent)
        mark[tbl]=parent
        local tmp={}
        for k,v in pairs(tbl) do
            if type(v) == 'table' or type(v) == "string" or type(v) == 'number' then
                local key= type(k)=="number" and "["..k.."]" or k
                if type(v)=="table" then
                    local dotkey= parent..(type(k)=="number" and key or "."..key)
                    if mark[v] then
                        table.insert(assign,dotkey.."="..mark[v])
                    else
                        table.insert(tmp, key.."="..ser_table(v,dotkey))
                    end
                elseif type(v)=="string" then
                    table.insert(tmp, key.."="..'"'..v..'"')
                else
                    table.insert(tmp, key.."="..v)
                end
            end
        end
        return "{"..table.concat(tmp,",").."}"
    end

    return "do local ret="..ser_table(t,"ret")..table.concat(assign," ").." return ret end"
end

function M:req(name)
    if module_exists(name) then
        return require(name)
    else
        print('no such module: ' .. name .. '\n' .. debug.traceback())
    end
    return nil
end

function M:register_entities()
    for dir in lfs.dir(self.entities_path) do
        if dir ~= '.' and dir ~= '..' then
            local name = self.entities_path .. dir
            if dir_exists(name .. '/handler/') then
                core:register_handler(dir)
            end
            if dir_exists(name .. '/rpc/') then
                core:register_rpc(dir)
            end
        end
    end
end

function M:register_handler( name )
    print('register_handler: ' .. name)
    local path = self.entities_path .. name .. '/handler/'
    for dir in lfs.dir(path) do
        if dir ~= '.' and dir ~= '..' then
            if string.ends(dir, '.lua') then
                local filename = string.sub(dir, 1, string.len(dir)-4)
                local route_name = name .. '.' .. filename
                local module_path = '' .. path .. filename
                self.entities[route_name] = require(module_path)
                print('file:' .. module_path)
            end
        end
    end
end

function M:register_rpc( name )
    print('register_rpc: ' .. name)
end


return M
