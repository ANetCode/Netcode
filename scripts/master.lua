require('ext.printlib')
core = require('core.core')
netcode=require('ext.netcode')

core.json = require("3rd.json")
core.app = netcode.new('ppp')
core.app:start()

core:register_entities()
local server = core.app:create_connection()
local connections = {}
server:echo()

if server ~= nil then
    server:bind(8964)
    local callbacks = {}
    server:set_callbacks(callbacks)

    callbacks.on_connect = function (cb, client, fd)
        connections[fd] = client
        local new_client = connections[fd];
        new_client:echo()
    end
    callbacks.on_disconnect = function(cb, client, fd)
        connections[fd] = nil
    end

    callbacks.on_message = function(cb, client, fd, msg)
        local json_str = msg:content()
        print('cli: '.. dump(client) .. ',' .. tostring(fd) .. " msg:" .. dump(msg) .. ' json: ' .. json_str)
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

while true do
    core.app:update()
    -- print('hello: ' .. tostring(core.app:time()))
    core.app:sleep(100)
end