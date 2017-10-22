
modules= {
    {
        name='luafilesystem',
        require_name='lfs',
        version=''
    }
}


function module_exists(name)
  if package.loaded[name] then
    return true
  else
    for _, searcher in ipairs(package.searchers or package.loaders) do
      local loader = searcher(name)
      if type(loader) == 'function' then
        package.preload[name] = loader
        return true
      end
    end
    return false
  end
end

not_exist_module = {}
for i,v in ipairs(modules) do
    if module_exists(v.require_name) == false then
        print(v.name .. ' => [' .. v.require_name .. '] not exist')
        not_exist_module[#not_exist_module+1] = v
    end
end