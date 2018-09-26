_G.___logfilename = nil
_print = print
os._execute = os.execute
function UpdatePyInFile(Key,Value)
	if type(Key) == type(Value) and type(Key) == "string" and Key ~= "" and Value ~= "" then
		if _G.Py[Key] then
			print("拼音只能添加不能修改到文件")
			Py[Key] = Value
			return
		end
		local f = io.open("C:/Lua/PinYin.Py","a")
		if f then
			f:write(Key.."="..Value.."\n")
			f:close()
		end
		--os.execute("echo "..Key.."="..Value.." >> PinYin.Py")
	else
		print("错误的类型或者错误的值")
	end
end
function initLoadPy()
	if _G.Py == nil then
		local f = io.open("C:/Lua/PinYin.Py","r")
		local s = ""
		local index = nil
		_G.Py = {}
		for line in f:lines() do
			s = line
			index = string.find(s,"=")
			if index then
				local k = string.sub(s,1,index-1)
				local v = string.sub(s,index+1,-1)
				if _G.Py[k] == nil then
					_G.Py[k] = v
				end
			end
		end
		f:close()
		setmetatable(Py,{__call = function (t,Key,back)
			print(t,Key,back)
			if Key == nil or Key == "" then
				return nil
			end
			if t[Key] then
				return t[Key]
			end
			back = back or function ()end
			local v = back(Key)
			if type(v) == "string" and v ~= "" then
				UpdatePyInFile(Key,v)
			end
			return v
		end})
	end
end
initLoadPy()
function print(...)
	local str = ""
	local tab = {...}
	for i,v in ipairs(tab) do
		str = str..tostring(v)..","
	end
	if string.len(str) > 0 then
		str = string.sub(str,1,-2)
	end
	if _G.___logfilename ~= nil and _G.___logfilename ~= "" then
		local f = io.open(_G.___logfilename,"a+")
		if f == nil then
			_G.___logfilename = ""
			_print("传入的log日志文件不能使用")
			_print(str)
			return
		end
		f:write(str.."\n")
		f:close()
	else
		_print(str)
	end
end
--禁止使用导致死循环的表如:单例模式 self.a = self self.a = b b.z = self
function GetTableString(param)
	local _fun = nil
	local str = ""
	local _type = type(param)
	if _type == "string" then
		str = param
	elseif _type == "number" then
		str = ""..param
	elseif _type == "userdata" then
		str = "userdata:"..tostring(param)
	elseif _type == "function" then
		str = "function:"..tostring(param)
	elseif _type == type(true) then
		str = tostring(param)
	elseif _type == type(nil) then
		str = "{\n param = nil \n}"
	elseif _type == "table" then
		local Prve = ""
		_fun = function (tab)
			if type(tab) == "table" then
				for k,v in pairs(tab) do
					local ktype = type(k)
					local vtype = type(v)
					str = str..Prve
					if ktype == "string" then
						str = str.."[\""..k.."\"] = "
					elseif ktype == "number" then
						str = str.."["..k.."] = "
					elseif ktype == "function" then
						str = str.."[\"func:"..tostring(k).."\"] = "
					elseif ktype == "userdata" then
						str = str.."[\"user:"..tostring(k).."\"] = "
					elseif ktype == "table" then
						str = str.."[\"table:"..tostring(k).."\"] = "
					else--其他未知类型，不支持
						assert(false,"Unknown key type, temporarily unsupported "..ktype)
					end
					if vtype == "string" then
						str = str.."[["..v.."]],\n"
					elseif vtype == "number" then
						str = str..v..",\n"
					elseif vtype == "function" then
						str = str.."func:"..tostring(v)..",\n"
					elseif vtype == "userdata" then
						str = str.."user:"..tostring(v)..",\n"
					elseif vtype == "table" then
						local OldPrve = Prve
						str = str..Prve.."{\n"
						Prve = Prve.."  "
						_fun(v)
						Prve = OldPrve
						str = str..Prve.."},\n"
					elseif vtype == type(true) then
						str = str..tostring(v).."\n"
					else
						assert(false,"Unknown value type, temporarily unsupported "..vtype)
					end
				end
			end
		end
		str = str.."{\n"
		Prve = "  "..Prve
		_fun(param)
		if string.byte(str,string.len(str)-1) == string.byte(",",1) then
			str = string.sub(str,1,-3).."\n"
		end
		str = str.."}\n"
	else
		assert(false,"Unknown param type, temporarily unsupported "..vtype)
	end
	return str
end
--禁止使用导致死循环的表如:单例模式 self.a = self self.a = b b.z = self
function dump(param,filepath)
	local str = GetTableString(param)
	if type(filepath) == "string" then
		local f = io.open(filepath,"a+")
		if f then
			str = str.."\n"
			f:write(str)
			f:close()
			return
		end
	end
	print(str)
	return
end
function sleep(time)
	time = tonumber(time or "")
	if time == nil then
		return false
	end
	-->0s and < 10小时
	if time > 0 and time < 36000 then
		local t = os.clock()
		while os.clock() - t < time do end
		return true
	else
		return false
	end
end
function os.execute(...)
	print("[["..os.date([[%Y/%m/%d %H:%M]],os.time()).."]] 命令行")
	print(...)
	os._execute(...)
end
function DelPrveNullChat(s)
	if s == "" then
		return s
	end
	local i = string.find(s,"[^%s+]")
	if i then
		s = string.sub(s,i,-1)
	else
		s = ""
	end
	return s
end
function DelTailNullChat(s)
	if s == "" then return s end
	s = string.reverse(s)
	s = DelPrveNullChat(s)
	if s == "" then return s end
	return string.reverse(s)
end
function Str2ByteTable(str)
	if str == "" then
		return {}
	end
	local count = string.len(str)
	local t = {}
	for i = 1,count do
		t[i] = string.byte(str,i)
	end
	return t
end
function ByteTable2Str(bytes)
	local s = ""
	if #bytes == 0 then
		return s
	end
	for i,v in pairs(bytes) do
		s = s..string.char(v)
	end
	return s
end
function ByteTableFrstSame(t1,t2)
	local t3 = {}
	for k,v in pairs(t1) do
		if t2[k] == v then
			t3[k] = v
		else
			return t3
		end
	end
	return t3
end
function ByteTableFindTable(t1,t2)
	local s1 = ByteTable2Str(t1)
	local s2 = ByteTable2Str(t2)
	return string.find(s1,s2)
end
function ByteTableCommand(t1,t2)
	if #t1 ~= #t2 then
		return false
	end
	for k,v in pairs(t1) do
		if v ~= t2[k] then
			return false
		end
	end
	return true
end
if string.split == nil then
	function string.split(str, delimiter)  
	    if (delimiter=='') then return false end  
	    local pos,arr = 0, {}  
	    for st,sp in function() if(str == nil) then return nil end return  string.find(str, delimiter, pos, true) end do  
	        table.insert(arr, string.sub(str, pos, st - 1))  
	        pos = sp + 1  
	    end  
	    if str then
		    table.insert(arr, string.sub(str, pos))  
		end
    	return arr
	end  
end
function TabLoopBackNewTab(srctab,back)
	back = type(back) == "function" and back or function (k,v)
		return "xinkey","xinvalue"
	end
	local ret = {}
	local key,value = nil,nil
	for k,v in pairs(srctab) do
		key,value = back(k,v)
		if key ~= nil then
			ret[key] = value
		end
	end
	return ret
end