function string.starts(String,Start)
   return string.sub(String,1,string.len(Start))==Start
end

function string.ends(String,End)
   return End=='' or string.sub(String,-string.len(End))==End
end

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

function file_exists(file)
   local ok, err, code = os.rename(file, file)
   if not ok then
      if code == 13 then
         -- Permission denied, but it exists
         return true
      end
   end
   return ok, err
end

function dir_exists(path)
   return file_exists(path.."/")
end

function table.dump ( t )
    local print_r_cache={}
    local s=''
    local function sub_print_r(t,indent)
        local ls=''
        if (print_r_cache[tostring(t)]) then
            ls = ls .. (indent.."*"..tostring(t)) .. '\n'
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        ls = ls .. (indent.."["..pos.."] => "..tostring(t).." {") .. '\n'
                        ls = ls .. sub_print_r(val,indent..string.rep(" ",string.len(pos)+8)) .. '\n'
                        ls = ls .. (indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        ls = ls .. (indent.."["..pos..'] => "'..val..'"') .. '\n'
                    else
                        ls = ls .. (indent.."["..pos.."] => "..tostring(val)) .. '\n'
                    end
                end
            else
                ls = ls .. (indent..tostring(t))
            end
        end
        return ls
    end
    if (type(t)=="table") then
        s = s .. (tostring(t).." {") .. '\n'
        s = s .. sub_print_r(t,"  ")
        s = s .. ("}") .. '\n'
    else
        sub_print_r(t,"  ")
    end
    return s
end

function dump( v )
    if type(v) == 'table' then
        return table.dump(v)
    else
        return  '(' .. type(v) .. ')=' .. tostring(v)
    end
end