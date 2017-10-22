require('ext.printlib')
core = require('core.core')
netcode=require('ext.netcode')

core.json = require("3rd.json")
core.app = netcode.new('ppp')
core.app:start()

core:register_entities()
local conn = core.app:create_connection()
if conn ~= nil then
    conn:bind(8964)
    local callbacks = {}
    conn:set_callbacks(callbacks)

    callbacks.on_connect = function (conn, fd) 
        print('on_connect: '.. dump(self) .. ',' .. tostring(fd))
    end
end
print(dump(conn))

while true do
    core.app:update()
    -- print('hello: ' .. tostring(core.app:time()))
    core.app:sleep(100)
end