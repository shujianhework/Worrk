local HeadFile = "LuaInterFace.h"
local Write2File = "LuaInterFace.cpp"
local GetLuaManage = "SJH::LuaManage::getInstance()"
local GetDataPrve = GetLuaManage.."->get"
local PutDataPrve = GetLuaManage.."->push"
local BeginStructFlg = "namespace LUANOUSER"
local ModelName = "testModel"
local Server = {WriteList = {}}
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
local function GetTableString(...)
    local s = ""
    local function cout(data,Count)
        if type(data) ~= "nil" then

            if type(data) == "string" then
                s = s.."\""..data.."\""
            elseif type(data) == "table" then
                s = s.." {\n"--table
                for k,v in pairs(data) do
                    if type(v) ~= "nil" then
                        if Count > 0 then
                            for i =1,Count do
                                s = s.."\t"
                            end
                        end
                        if type(k) == "number" then
                            s = s.."["..k.."]".." = "
                        else
                            s = s.."[\""..k.."\"]".." = "
                        end
                        cout(v,Count+1)
                        s = s..",\n"
                    end
                end
                s = string.sub(s,1,-3)
                s = s.."\n"
                if Count-1 > 0 then
                    for i =1,Count-1 do
                        s = s.."\t"
                    end
                end
                s = s.."}"
            elseif type(data) == "boolean" then
                if data == true then
                    s = s..tostring(data)--"真"
                elseif data == false then
                    s = s..tostring(data)--"假"
                end
            else
                s = s..tostring(data)
            end
        end
    end
    local tab = {...}
    for i = 1, #tab do
        cout(tab[i],1)
        if tab[i+1] then
            s = s..","
        end
    end
    return s
end
function Server:DelTailNILChar(s)
	s = s or ""
	s = string.reverse(s)
	s = self:DelHeadNILChar(s)
	if s ~= "" then
		s = string.reverse(s)
	end
	return s
end
function Server:DelHeadNILChar(s)
	s = s or ""
	local index,endindex = string.find(s,"%s+")
	if index == nil or index > 1 then
		return s
	end
	s = string.sub(s,endindex+1,-1)
	if s == nil or s == "" then
		return ""
	else
		return s
	end
end
function Server:DelFirstEndNILChar(s)
	return self:DelTailNILChar(self:DelHeadNILChar(s))
end
function Server:read(back)
	back = type(back) == "function" and back or function (line,i)
		print(line,i)
	end
	local f = io.open(HeadFile,"r")
	if f == nil then
		return false
	end
	local i = 0
	local index,endindex
	for line in f:lines() do
		i = i + 1
		local s = self:DelFirstEndNILChar(line)
		index,endindex = string.find(s,"//")
		if index == nil then
			back(s,i)
		elseif index > 0 then
			back(string.sub(s,1,index),i)
		end
	end
	f:close()
end
function Server:MemHandler(tab)
	local ret = {}
	local index,endindex = nil,nil
	local s = nil
	local Name,Type = nil,nil
	for k,v in ipairs(tab) do
		s = string.sub(v,1,-2)
		index,endindex = string.find(s," ")
		if index then
			Type = string.sub(s,1,index-1)
			Name = string.sub(s,endindex+1,-1)
			index,endindex = string.find(Name,"%(")
			if index then
				Name = string.sub(Name,1,index-1)
				ret[Name] = ret[Name] or {Ret = Type}
				ret[Name][#ret[Name]+1] = string.sub(s,string.len(Type)+string.len(Name)+2,-1)
			else
				ret[Name] = Type
			end
		end
	end
	return ret
end
function Server:RegisterStructGetVal(StructName,Type,Name,FuncName)
	self.WriteList[#self.WriteList + 1] = "static int "..FuncName.."(lua_State* L);"
	local tab = {
		"static int "..FuncName.."(lua_State* L){",
		StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, "..StructName..");",
		"luaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
		PutDataPrve..Type.."((*P)->"..Name..")",
		"}"
	}
	return tab
end
function Server:RegisterStructGetFunc(StructName,Name,List,FuncName)
	self.WriteList[#self.WriteList + 1] = "static int "..FuncName.."(lua_State* L);"
	local s = "static struct "..FuncName.."FuncList {"
	local tab = {
		"static int "..FuncName.."(lua_State* L){",
		StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, "..StructName..");",
		"luaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
	}
	local ParamLists = {}
	local index,endindex = nil,nil
	--去重  函数名 参数列表相同
	for k,v in ipairs(List) do
		index,endindex = string.find(v,"%(")
		if index then
			v = string.sub(v,index+1,-1)
		end
		index,endindex = string.find(v,"%)")
		if index then
			v = string.sub(v,1,index-1)
		end
		ParamLists[v] = 1
	end
	local wrret = "return "
	if List.Ret == nil or List.Ret == "" or List.Ret == "void" then
		wrret = wrret.."0;"
	else
		wrret = wrret.."1;"
	end
	List = {}
	for k,v in pairs(ParamLists) do
		List[#List+1] = k
	end
	ParamLists = {}
	for i,v in ipairs(List) do
		ParamLists[i] = {}
		local tab = string.split(v,',')
		if #tab[i] == 1 and tab[i] == "" then
			tab[i] = {}
		end
		for k,vv in pairs(tab) do
			local ps = self:DelFirstEndNILChar(vv) --string.split(vv)
			if ps ~= "" then
				ParamLists[i][#ParamLists[i] + 1] = ps
			end
		end
	end
	if #ParamLists == 1 then--直接调用
		local s = ""
		if #ParamLists[1] == 0 then
			s = "(*P)->"..Name.."();"
		else
			s = "auto LM = "..GetLuaManage..";(*P)->"..Name.."("
			local Getdataprve = string.sub(GetDataPrve,string.len(GetLuaManage)+1,-1)
			for k,v in pairs(ParamLists[1]) do
				s = s.."LM"..Getdataprve..v.."(),"
			end
			s = string.sub(s,1,-2)
			s = s..");"
		end
		tab[#tab + 1] = s
	else
		local s = "int len = lua_gettop("..GetLuaManage.."->L);\nswitch(len){"
		local max,min = 0,0
		for k,v in pairs(ParamLists) do
			max = math.max(max,#v)
			min = math.min(min,#v)
		end
		for i=min,max do
			s = s.."\ncase "..i..":{\n"
			
			s = s.."\n};break;"
		end
		s = s.."\n default:printf(\"参数数量错误没有该重载函数\");return 0;"
		s = s.."}"
	end
	tab[#tab+1] = wrret
	tab[#tab + 1] = "}"
	--需要搜集参数s
	--首先判断传入的参数列表的类型s
	--return tab
	--print(StructName,Name,Ret,FuncName)
end
function Server:RegisterLua(Name,tab,varname,funcname)
	local vartab = {"static const struct luaL_Reg "..varname.."[] = {"}
	local FuncName = nil
	local Members = {}
	for k,v in pairs(tab) do
		if type(v) == "string" then
			FuncName = "AutoRegister_"..Name.."_Val_"..k
			vartab[#vartab+1] = "{"..k..","..FuncName.."}"
			Members[#Members+1] = self:RegisterStructGetVal(Name,v,k,FuncName)
		else
			FuncName = "AutoRegister_"..Name.."_Func_"..k
			self:RegisterStructGetFunc(Name,k,v,FuncName)
		end
	end
	vartab[#vartab+1] = "{NULL,NULL},"
	vartab[#vartab+1] = "}"
	print(GetTableString(Members))
end
function Server:LoadSrcCode()
	local temptab = {}
	local StructBeginFlg = false
	local StructName = nil
	local si = nil
	local index,endindex = nil,nil
	self:read(function (s,i)
		if StructBeginFlg == false then
			if s == BeginStructFlg or s == (BeginStructFlg.."{") or s == (BeginStructFlg.." {") then
				StructBeginFlg = true
				return
			end
			self.WriteList[#self.WriteList+1] = s
		else
			if StructName == nil then
				local isstruct = true
				index,endindex = string.find(s,"struct ")
				if index == nil then
					index,endindex = string.find(s,"class ")
					isstruct = false
				end
				if index then
					local StructDatas = {}
					s = string.sub(s,endindex+1,-1)
					index,endindex = string.find(s,"{")
					if index then
						s = string.sub(s,1,index-1)
					end
					index,endindex = string.find(s,":")
					if index then
						local strParent = string.sub(s,endindex+1,-1)
						s = string.sub(s,1,index-1)
						local tab = string.split(strParent,",")
						local tabParent = {}
						for k,v in pairs(tab) do
							v = self:DelTailNILChar(v)
							index,endindex = string.find(v," ")
							if index then
								if string.sub(v,1,index-1) == "public" then
									tabParent[#tabParent+1] = string.sub(v,endindex+1,-1)
								end
							elseif isstruct == true then
								tabParent[#tabParent+1] = v
							end
						end
						StructDatas.switch = tabParent
					end
					s = self:DelTailNILChar(s)
					StructName = s
					temptab[StructName] = StructDatas
					si = 1
				end
			else
				if s == "}" or s == "};" then
					StructName = nil
				else
					temptab[StructName][si] = s
					si = si + 1
				end
			end
		end
	end)
	self.temptab = {}
	for k,v in pairs(temptab) do
		self.temptab[k] = self:MemHandler(v)
	end
	local sv = nil
	local count = 0
	while true do
		local l = 0
		count = count + 1
		for k,v in pairs(temptab) do
			if v.switch then
				local flg = false
				sv = self.temptab[k]
				l = l + 1
				for _,vv in ipairs(v.switch) do
					if self.temptab[vv] then
						flg = true
						for kkk,vvv in pairs(self.temptab[vv]) do
							if type(vvv) ~= "string" then
								sv[kkk] = sv[kkk] or {}
								if type(sv[kkk]) == "table" and type(vvv) == "table" then
									for _,fv in ipairs(vvv) do
										sv[kkk][#sv[kkk] + 1] = fv
									end
								end
							else
								sv[kkk] = vvv
							end
						end
					end
				end
				if flg == true then
					temptab[k].switch = nil
					l = l - 1
				end
			end
		end
		if l == 0 then
			break
		end
	end
	self.WriteList[#self.WriteList + 1] = "int AutoRegister_Model(lua_State *L);"
	self.WriteList[#self.WriteList + 1] = "static int testModelOpen(lua_State *L);"
	self.ClassFunctions = {}
	local testModelOpen = {"static int testModelOpen(lua_State *L){"}
	local switch = {"int AutoRegister_Model(lua_State *L){","int top = lua_gettop(L);",
	"luaL_requiref(L, "..ModelName..", testModelOpen, 0);","top=lua_gettop(L);"}
	local count = 0
	local ValUserStructName = ""
	local FucUserStructName = ""
	for k,v in pairs(self.temptab) do
		if next(v) then
			ValUserStructName = "AutoRegister_"..k.."_Struct"
			FucUserStructName = "AutoRegister_"..k.."_Funcs"
			switch[#switch+1] = "luaL_newmetatable(L, "..k..");"
			switch[#switch+1] = "lua_pushvalue(L, -1);"
			switch[#switch+1] = "lua_setfield(L, -2, \"__index\");"
			switch[#switch+1] = "luaL_setfuncs(L, "..ValUserStructName..", 0);"
			switch[#switch+1] = "lua_pop(L,1);"
			switch[#switch+1] = "top=lua_gettop(L);"
			count = count +1
			testModelOpen[#testModelOpen + 1] = "\tluaL_newlib(L, "..FucUserStructName..");"
			self:RegisterLua(k,v,ValUserStructName,FucUserStructName)
		end
	end
	switch[#switch+1] = "}"
	testModelOpen[#testModelOpen+1] = "\treturn "..count..";"
	testModelOpen[#testModelOpen+1] = "}"
	--print(GetTableString(testModelOpen))
end
Server:LoadSrcCode()
--print(string.find("[((","%("))
--print(GetTableString(string.split("sdfsd a,fsfxcvb,d,eafdsf,zcv",",")))