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
function Server:MemHandler(tab,TypeName)
	local ret = {}
	local index,endindex = nil,nil
	local s = nil
	local Name,Type = nil,nil
	print(TypeName)
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
		elseif (string.len(v)-2) > string.len(TypeName) then
			if string.byte(v,1) ~= string.byte("~",1) then
				index,endindex = string.find(s,TypeName)
				if index then
					ret[TypeName] = ret[TypeName] or {}
					ret[TypeName][#ret[TypeName] + 1] = string.sub(s,string.len(TypeName)+1,-1)
				end
			end
		end
	end
	if ret[TypeName] == nil then
		ret[TypeName] = {"()"}
	end
	return ret
end
function Server:RegisterStructGetVal(StructName,Type,Name,FuncName)
	self.WriteList[#self.WriteList + 1] = "static int "..FuncName.."(lua_State* L);"
	local tab = {
		"static int "..FuncName.."(lua_State* L){",
		"\t"..StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, \""..StructName.."\");",
		"\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
		"\treturn "..PutDataPrve..Type.."((*P)->"..Name..");",
		"}"
	}
	return tab
end
function Server:RegisterStructGetFunc(StructName,Name,List,FuncName)
	self.WriteList[#self.WriteList + 1] = "static int "..FuncName.."(lua_State* L);"
	local Ret = List.Ret
	local s = "static struct "..FuncName.."FuncList {"
	local tab = {
		"static int "..FuncName.."(lua_State* L){",
		"\t"..StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, \""..StructName.."\");",
		"\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
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
		if #tab == 1 and tab == "" then
			tab = {}
		end
		for k,vv in pairs(tab) do
			local ps = self:DelFirstEndNILChar(vv) --string.split(vv)
			if ps ~= "" then
				ParamLists[i][#ParamLists[i] + 1] = ps
			end
		end
	end
	if ParamLists == nil then
		return 
	end
	local Getdataprve = string.sub(GetDataPrve,string.len(GetLuaManage)+1,-1)
	if #ParamLists == 1 then--直接调用
		local s = ""
		if #ParamLists[1] == 0 then
			s = "\t(*P)->"..Name.."();"
		else
			s = "\tauto LM = "..GetLuaManage..";\n\t(*P)->"..Name.."("
			
			for k,v in pairs(ParamLists[1]) do
				s = s.."LM"..Getdataprve..v.."(),"
			end
			s = string.sub(s,1,-2)
			s = s..");"
		end
		tab[#tab + 1] = s
	else
		local s = "\tauto LM = "..GetLuaManage..";\n"
		s = s.."\tint len = lua_gettop("..GetLuaManage.."->L)-1;\n\tswitch(len){"
		local strlastRet = ""
		if Ret ~= "void" then
			strlastRet = "auto ret = "
		end
		table.sort(ParamLists,function (a,b)
			if #a ~= #b then
				return #a < #b
			end
			return false
		end)
		local min,max = #ParamLists[1],#ParamLists[#ParamLists]
		local StructFuncAddr = StructName.."::"..Name
		for i=min,max do
			s = s.."\n\tcase "..i..":{\n"
			if i == 0 then
				s = s.."\t\t(*P)->"..Name.."();\n"
				table.remove(ParamLists,1)
			else
				local len = #ParamLists
				local tabvarnames = {}
				for j = 1,len do
					if #ParamLists[j] ~= i then
						break
					end
					s = s.."\t\tstd::tuple<"
					local varname = "t"
					local funtype = Ret.."("..StructName.."::*)("
					for kk,vv in pairs(ParamLists[j]) do
						s = s..vv..","
						if string.sub(vv,-1,-1) == "*" then
							varname = varname..string.sub(vv,1,-2)
						else
							varname = varname..vv
						end
						funtype = funtype..vv..","
					end
					funtype = string.sub(funtype,1,-2)
					tabvarnames[funtype] = varname
					s = string.sub(s,1,-2)
					s=s.."> "..varname..";\n"
				end
				local flg = false
				local format = "\t\t"
				for k,v in pairs(tabvarnames) do
					local ps = "LM->getFunAllParam(\n\t"..format.."[&](int len){\n\t\t"..format.."if ("..len.." < len) return false;\n\t\t"..format
					for endi = 1,len-1 do
						ps = ps.."std::get<"..(endi-1)..">("..v..") = LM->getParam<decltype(std::get<"..(endi-1)..">("..v.."))>(std::get<"..(endi-1)..">("..v.."),"..endi..");\n\t\t"..format
					end
					ps = ps.."return true;\n\t"..format.."},\n\t"..format.."[&](){\n\t\t"..format..""
					ps = ps..strlastRet.."public2ThisFunc"..i.."<decltype(*P),"..k.."),decltype("..v..")>(*P,&"..StructFuncAddr..","..v..");\n\t"..format..""
					ps = ps.."})"
					s = s..format.."if ("..ps.." == false)\n"
					table.remove(ParamLists,1)
					format =format.."\t"
				end
				s = s ..format.. "assert(false);//传入的参数和所有重载函数都不匹配\n"
			end
			s = s.."\t};break;"
		end
		--参数数量错误没有该重载函数
		s = s.."\n\tdefault:{\n\t\tprintf(\"\");\n\t\treturn 0;\n\t}\n"
		s = s.."}"
		tab[#tab + 1] = s
	end
	tab[#tab+1] = wrret
	tab[#tab + 1] = "}"
	return tab
end
function Server:RegisterLua(Name,tab,varname,funcname,CSFN)
	local vartab = {"static const struct luaL_Reg "..varname.."[] = {"}
	local Funtab = {"static const struct luaL_Reg "..funcname.."[] = {"}
	local FuncName = nil
	local Members = {}
	local newFuncs = {}
	local index,endindex = nil,nil
	for k,v in pairs(tab[Name]) do
		if string.len(v) > 2 then
			local ntab = string.split(string.sub(v,2,-2),",")
			local nntab = {}
			for k,v in pairs(ntab) do
				index,endindex = string.find(v,"*")
				if index then
					nntab[k] = self:DelFirstEndNILChar(string.sub(v,1,index-1))
				else
					nntab[k] = self:DelFirstEndNILChar(v)
				end
			end
			newFuncs[#newFuncs + 1] = nntab
		elseif string.len(v) == 2 then
			newFuncs[#newFuncs + 1] = {}
		end
	end
	tab[Name] = nil
	for k,v in pairs(tab) do
		if type(v) == "string" then
			FuncName = "AutoRegister_"..Name.."_Val_"..k
			vartab[#vartab+1] = "\t{\""..k.."\","..FuncName.."},"
			Members[#Members+1] = self:RegisterStructGetVal(Name,v,k,FuncName)
		else
			FuncName = "AutoRegister_"..Name.."_Func_"..k
			Funtab[#Funtab+1] = "\t{\""..k.."\","..FuncName.."},"
			Members[#Members+1] = self:RegisterStructGetFunc(Name,k,v,FuncName)
		end
	end
	vartab[#vartab+1] = "\t{NULL,NULL},"
	vartab[#vartab+1] = "};"	
	Funtab[#Funtab+1] = "\t{NULL,NULL},"
	Funtab[#Funtab+1] = "};"
	self.WriteList[#self.WriteList + 1] = "static int "..CSFN.."(lua_State* L);"
	local RegisterClass = {"static int "..CSFN.."(lua_State* L){","\t"..Name.."** PP = ("..Name.."**)lua_newuserdata(L,sizeof("..Name.."*));"}
	local Getdataprve = string.sub(GetDataPrve,string.len(GetLuaManage)+1,-1)
	if #newFuncs == 1 then
		if #newFuncs[1] == 0 then
			RegisterClass[#RegisterClass + 1] = "\t*PP = new "..Name..";"
		else
			local s = "\tauto LM = "..GetLuaManage..";\n\t*P = new "..Name.."("
			for k,v in pairs(newFuncs[1]) do
				s = s.."LM"..Getdataprve..v.."(),"
			end
			RegisterClass[#RegisterClass + 1] = string.sub(s,1,-2)..");"
		end
	else
		table.sort(newFuncs,function (a,b)
			if #a ~= #b then
				return #a >  #b
			end
			return false
		end)
		local max,min = #newFuncs[1],#newFuncs[#newFuncs]
		RegisterClass[#RegisterClass + 1] = "\tauto LM = "..GetLuaManage..";"
		RegisterClass[#RegisterClass + 1] = "\tint len = lua_gettop(L)-1;"
		--RegisterClass[#RegisterClass + 1] = "\t"..Name.."** PP = ("..Name.."**)lua_newuserdata(L, sizeof("..Name.."*))"
		RegisterClass[#RegisterClass + 1] = "\tif ( len < "..min.." && len > "..max.." ) { \n\t\tassert(false);//参数个数错误\n\t\treturn 0;\n\t}"
		RegisterClass[#RegisterClass + 1] = "\tswitch(len){"
		local tabnvarnames = {}
		local lambda = ""
		local s = ""
		for i = min,max do
			s = "\t\t case "..i..":{\n\t\t\t"
			if i == 0 then
				s = s.."*PP = new "..Name..";"
				newFuncs[#newFuncs] = nil
			else
				local types = {}
				local values = {}
				for j = #newFuncs,1,-1 do
					if #newFuncs[j] ~= i then
						s = s..""
						break
					end
					s = s.."std::tuple<"
					types[#types + 1] = newFuncs[j]
					local value = "New"		
					for k,v in pairs(newFuncs[j]) do
						if v == "char" then
							s = s.."char*"..","
						else
							s = s..v..","
						end
						value = value..v
					end
					s = string.sub(s,1,-2).."> "..value..";\n\t\t\t"
					values[#values + 1] = value
					newFuncs[j] = nil
				end
				local PFormat = "\n\t\t"
				for k,v in pairs(values) do
					local format = PFormat.."\t"
					s = s.."if ( LM->getFunAllParam([&](int plen){"..format.."\t"
					for kk,vv in pairs(types[k]) do
						if vv == "char" then
							vv = "char*"
						end
						s = s.."std::get<"..(kk-1)..">("..v..") = LM->getParam<"..vv..">(std::get<"..(kk-1)..">("..v.."),"..kk..");"..format.."\t"
					end
					s = s.."return true;"..format.."},[&](){"..format.."\t"
					s = s.."*PP = new "..Name.."("
					for kk,vv in pairs(types[k]) do
						s = s.."std::get<"..(kk-1)..">("..v.."),"
					end
					s = string.sub(s,1,-2)..");"..format.."}) == false)"..format.."\t"
					PFormat = PFormat.."\t"
				end
				s = s.."assert(false);//传入的参数和所有重载函数都不匹配;"
			end
			s = s.."\n\t\t}break;"
			RegisterClass[#RegisterClass + 1] = s
		end
		RegisterClass[#RegisterClass + 1] = "\t}"
	end
	RegisterClass[#RegisterClass + 1] = "\tluaL_getmetatable(L,\""..Name.."\");"
	RegisterClass[#RegisterClass + 1] = "\tlua_setmetatable(L,-2);"
	RegisterClass[#RegisterClass + 1] = "\treturn 1;\n}"
	return {vartab,Funtab,Members,RegisterClass}
end
function Server:Write()
	if self.WriteBuffer then
		local temp = nil
		temp = function (tab,back)
			if type(tab) == "string" then
				back(tab)
			else
				for k,v in pairs(tab) do
					temp(v,back)
				end
			end
		end
		local f = io.open(Write2File,"w")
		if f == nil then
			return false
		end
		temp(self.WriteBuffer,function (s)
			--print(s)
			f:write(s.."\n")
		end)
		f:close()
		return true
	end
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
		self.temptab[k] = self:MemHandler(v,k)
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
	local switch = {"int AutoRegister_Model(lua_State *L){","\tint top = lua_gettop(L);",
	"\tluaL_requiref(L, \""..ModelName.."\", testModelOpen, 0);","\ttop=lua_gettop(L);"}
	local ValUserStructName = ""
	local FucUserStructName = ""
	local AllRegistDatas = {}
	local ClassTableNames = {"static const struct luaL_Reg AutoRegister_ClassName_News[] = {"}
	local CreateStructFuncName = ""
	--print(GetTableString(self.temptab))
	for k,v in pairs(self.temptab) do
		if next(v) then
			ValUserStructName = "AutoRegister_"..k.."_Struct"
			FucUserStructName = "AutoRegister_"..k.."_Funcs"
			switch[#switch+1] = "\tluaL_newmetatable(L, \""..k.."\");"
			switch[#switch+1] = "\tlua_pushvalue(L, -1);"
			switch[#switch+1] = "\tlua_setfield(L, -2, \"__index\");"
			switch[#switch+1] = "\tluaL_setfuncs(L, "..ValUserStructName..", 0);"
			switch[#switch+1] = "\tlua_pop(L,1);"
			switch[#switch+1] = "\ttop=lua_gettop(L);"
			CreateStructFuncName = "AutoRegister_CreateStruct"..k
			ClassTableNames[#ClassTableNames+1] = "\t{\""..k.."\","..CreateStructFuncName.."},"
			local ft = self:RegisterLua(k,v,ValUserStructName,FucUserStructName,CreateStructFuncName)
			AllRegistDatas[k] = ft
		end
	end
	switch[#switch+1] = "\treturn 1;"
	switch[#switch+1] = "}"
	testModelOpen[#testModelOpen + 1] = "\tluaL_newlib(L, AutoRegister_ClassName_News);"
	testModelOpen[#testModelOpen+1] = "\treturn 1;"
	testModelOpen[#testModelOpen+1] = "}"
	ClassTableNames[#ClassTableNames+1] = "\t{NULL,NULL}"
	ClassTableNames[#ClassTableNames+1] = "};"
	self.temptab = nil
	self.WriteBuffer = {self.WriteList,AllRegistDatas,ClassTableNames,testModelOpen,switch}
	self.WriteList = nil
	--print(GetTableString(ClassTableNames))
	self:Write()
end
Server:LoadSrcCode()
--print(string.find("[((","%("))
--print(GetTableString(string.split("sdfsd a,fsfxcvb,d,eafdsf,zcv",",")))