require('ext.printlib')
core = require('core.core')
netcode=require('ext.netcode')

core.json = require("3rd.json")
core.app = netcode.new('ppp')
core.app:start()
core.connections = {}

core:register_entities()
local server = core.app:create_connection()

local count = 0;
local tcount = 0;
if server ~= nil then
    server:bind(8964)
    local callbacks = {}
    server:set_callbacks(callbacks)

    callbacks.on_connect = function (cb, client, fd)
        core.connections[fd] = client
        local new_client = core.connections[fd];
    end
    callbacks.on_disconnect = function(cb, client, fd)
        core.connections[fd] = nil
    end

    callbacks.on_message = function(cb, client, fd, msg)
        local json_str = msg:content()
        -- print(tostring(count) .. 'cli: '.. dump(client) .. ',' .. tostring(fd) .. " msg:" .. dump(msg) .. ' json: ' .. json_str)
        -- client:send(json_str)
        count = count + 1
        tcount = tcount + 1
        local req = core.json.decode(json_str)
        if req ~= nil then
            local resp = {
                _type = "resp",
                route=req.route,
                msg='echo hello'
            }
            local resp_content = core.json.encode(resp)
            client:send(resp_content)
        end
    end
end
local last_time = core.app:time()

function update(delta)
    core.app:update()
    local now = core.app:time()
    if core.app:time() - last_time > 3 then
        last_time = core.app:time()
        print('count:' .. tostring(count) .. ' tcount: ' .. tostring(tcount) .. ' delta time:' .. tostring(delta))
        count=0
    end
end

 -- 16ms
local tick_time = 16
local start_time = core.app:time()
local end_time = core.app:time()
local delta = end_time - start_time
while true do
    start_time = core.app:time()
    update(delta)
    end_time = core.app:time()
    delta = end_time - start_time
    if delta <= 0 or delta >= tick_time then 
        delta = 0 
    else
        delta = tick_time - delta
    end
    
    core.app:sleep(delta)
end