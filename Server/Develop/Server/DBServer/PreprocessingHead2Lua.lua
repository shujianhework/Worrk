local HeadFile = "LuaInterFace.h"
local Write2File = "LuaInterFace.cpp"
local GetLuaManage = "SJH::LuaManage::getInstance()"
local GetDataPrve = GetLuaManage.."->get"
local PutDataPrve = GetLuaManage.."->push"
local BeginStructFlg = "namespace LUANOUSER"
local ModelName = "testModel"
local programInterfaceName = "AutoRegister_Model"
local Server = {WriteList = {}}
local CType2LuaType = {
	["int"] = "TNUMBER",
	["float"] = "TNUMBER",
	["double"] = "TNUMBER",
	["std::string"] = "TSTRING",
	["string"] = "TSTRING",
	["char"] = "TSTRING",
	["bool"] = "TBOOLEAN",
	["void"] = "TNIL",
	["LUAFUNC"] = "TFUNCTION",
	["MAPSS"]	= "TTABLE",
	["MAPSI"]	= "TTABLE",
	["MAPIS"]	= "TTABLE",
	["MAPII"]	= "TTABLE",
	["MAPSB"]	= "TTABLE",
	["MAPIB"]	= "TTABLE",
	["VECS"]	= "TTABLE",
	["VECI"]	= "TTABLE",
	["VECB"]	= "TTABLE",
}
local Type2GetValue = {
	["TNUMBER"] = "Tonumber(%d)",
	["TSTRING"] = "Tostring(%d)",
	["TBOOLEAN"] = "Toboolean(%d)",
	["TNIL"] = "",
	["TFUNCTION"] = "ToFunction(%d)",
	["TTABLE"] 	= "ToTable<%s>(%d)",
}
local Type2SetValue = {
	["TNUMBER"] = "Pushnumber(%s)",
	["TSTRING"] = "Pushstring(%s)",
	["TBOOLEAN"] = "Pushboolean(%s)",
	["TNIL"] = "Pushnil(%s)",
	["TFUNCTION"] = "Pushnil(%s)",
	["TTABLE"] = "PushTable(%s)",
}
require "initLua"
function Server:DelTailNILChar(s)
	return DelTailNullChat(s)
end
function Server:DelHeadNILChar(s)
	return DelPrveNullChat(s)
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
function Server:Write()
	if self.WriteBuffer then
		local temp = nil
		temp = function (k,tab,back)
			if type(tab) == "string" then
				back(tab)
			elseif type(tab) == "function" then
				back(tab())
			else
				for k,v in pairs(tab) do
					temp(k,v,back)
				end
			end
		end
		local f = io.open(Write2File,"w")
		if f == nil then
			return false
		end
		temp(nil,self.WriteBuffer,function (s)
			f:write(s.."\n")
		end)
		f:close()
		return true
	end
end
function Server:CreateFuncListTable(StructName,FuncName,RetType,ParamString,new)

	self.FuncLists = self.FuncLists or {}
	self.FuncLists[StructName] = self.FuncLists[StructName] or {}
	self.FuncLists[StructName][FuncName] = self.FuncLists[StructName][FuncName] or {}
	local t = self.FuncLists[StructName][FuncName]
	local ParamList = string.split(ParamString,",")
	local NewKey = FuncName
	ParamList[1] = self:DelFirstEndNILChar(string.sub(ParamList[1],2,-1))
	ParamList[#ParamList] = self:DelFirstEndNILChar(string.sub(ParamList[#ParamList],1,-2))
	for i = 2,#ParamList-1 do
		ParamList[i] = self:DelFirstEndNILChar(ParamList[i])
	end
	for k,v in pairs(ParamList) do
		NewKey = NewKey.."|"..v
	end
	NewKey = NewKey.."=>"..RetType
	if t[NewKey] then
		return nil,nil
	else
		self.FuncLists[StructName][FuncName][NewKey] = ParamList
		if self.FuncLists[StructName][FuncName].Ret == nil then
			self.FuncLists[StructName][FuncName].Ret = RetType
		end
		return NewKey,ParamList
	end
end
function Server:HandlerParents(StructTable,MapTable)
end
function Server:CTypeTranceLuaType(Type)
	if Type == "" then--无类型
		return ""
	end
	local _,c = string.gsub(Type,"%*"," ")
	if c ~= nil and c > 0 then
		Type = string.sub(Type,1,0-c-1)
	end
	if Type == "char" and c  then
		c = c - 1
	end
	assert(CType2LuaType[Type] and Type2GetValue[CType2LuaType[Type]])
	Type = CType2LuaType[Type]
	for i = 1,c do
		assert(false,"暂不支持指针也没有道理")
		Type = Type.."_P"
	end
	return Type
end
function Server:ValHandler(StructName,Name,Type)
	local RegisterFuncName = "static int AutoRegister_"..StructName.."_Val_%s_"..Name.."(lua_State *L)%s"
	self.WriteList[#self.WriteList + 1] = string.format(RegisterFuncName,"set",";")
	self.WriteList[#self.WriteList + 1] = string.format(RegisterFuncName,"get",";")
	self.StructFunLists[StructName][#self.StructFunLists[StructName] + 1] = "\t{\"set"..Name.."\",AutoRegister_"..StructName.."_Val_set_"..Name.."},"
	self.StructFunLists[StructName][#self.StructFunLists[StructName] + 1] = "\t{\"get"..Name.."\",AutoRegister_"..StructName.."_Val_get_"..Name.."},"
	local tab = {
		{
			string.format(RegisterFuncName,"set","{"),
			"\t"..StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, \""..StructName.."\");",
			"\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
			"\tauto LM = "..GetLuaManage..";",
			"\t(*P)->"..Name.." = ("..Type..")LM->"..string.format(Type2GetValue[self:CTypeTranceLuaType(Type)],2)..";",
			"\treturn 0;",
			"}"
		},{
			string.format(RegisterFuncName,"get","{"),
			"\t"..StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, \""..StructName.."\");",
			"\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
			"\tauto LM = "..GetLuaManage..";",
			"\treturn LM->"..string.format(Type2SetValue[self:CTypeTranceLuaType(Type)],"("..Type..")(*P)->"..Name)..";",
			"}"
		}
	}
	return tab
end
function Server:FuncHandler(StructName,FuncName,pv,new,IsNewFunc)
	local LuaFuncName = FuncName
	if IsNewFunc then
		self.NewStructFunLists[#self.NewStructFunLists + 1] = "\t{\""..StructName.."\",AutoRegister_"..StructName.."_Func_new},"
		LuaFuncName = "new"
	else
		self.StructFunLists[StructName][#self.StructFunLists[StructName] + 1] = "\t{\""..FuncName.."\",AutoRegister_"..StructName.."_Func_"..FuncName.."},"
	end
	local RegisterFuncName = "static int  AutoRegister_"..StructName.."_Func_"..LuaFuncName.."(lua_State *L)"
	self.WriteList[#self.WriteList + 1] = RegisterFuncName..";"
	local tab = {RegisterFuncName.."{"}
	tab[#tab + 1] = "\tconst static std::string ParamTypeLists[] = {"
	--print(StructName,FuncName)
	local t = self.FuncLists[StructName][FuncName] or pv
	local LuaTypes = {}
	local i = 0
	local RetStr = ""
	for k,v in pairs(t) do
		if k ~= "Ret" then
			i = i + 1
			LuaTypes[i] = {}
			local s = ""
			if type(v) == "table" then
				for kk,vv in pairs(v) do
					LuaTypes[i][kk] = {vv,self:CTypeTranceLuaType(vv)}
					s = s..","..LuaTypes[i][kk][2]
				end
			end
			if s ~= "," and s ~= "" then
				if LuaFuncName ~= "new" then
					s = "TUSERDATA"..s
				end
			end
			tab[#tab + 1] = "\t\t\""..s.."\","
		else
			RetStr = self:CTypeTranceLuaType(v)
		end
	end
	tab[#tab + 1] = "\t\t\"\""
	tab[#tab + 1] = "\t};"
	tab[#tab + 1] = "\tauto LM = "..GetLuaManage..";"
	tab[#tab + 1] = "\tLM->GetParamTypes();"
	if LuaFuncName ~= "new" then
		tab[#tab + 1] = "\t"..StructName.."** P = ("..StructName.."**)luaL_checkudata(L, 1, \""..StructName.."\");"
		tab[#tab + 1] = "\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");"
	else
		tab[#tab + 1] = "\t"..StructName.."** P = ("..StructName.."**)lua_newuserdata(L,sizeof("..StructName.."*));"
	end
	local StrongType = ""
	if new == "new" then
		new = new.." "..StructName.." "
	end
	for k,v in pairs(LuaTypes) do
		tab[#tab + 1] = "\tif (true == LM->CheckParams(ParamTypeLists["..(k-1).."])){"
		local RealReferenceS = ""
		for kk,vv in pairs(v) do
			if vv[2] ~= "" and vv[2] ~= "void" then
				if vv[1] == "" then
					StrongType = ""
				else
					StrongType = "("..vv[1]..")"
				end
				if vv[2] == "TTABLE" then
					RealReferenceS = RealReferenceS..StrongType.."LM->"..string.format(Type2GetValue[vv[2]],vv[1],LuaFuncName == "new" and kk or kk + 1)..","
				else
					RealReferenceS = RealReferenceS..StrongType.."LM->"..string.format(Type2GetValue[vv[2]],LuaFuncName == "new" and kk or kk + 1)..","
				end
			end
		end
		if RealReferenceS ~= "" then
			RealReferenceS = string.sub(RealReferenceS,1,-2)
		end
		local s = ""
		if LuaFuncName == "new" then
			s = "\t\t*P = "..new.."("..RealReferenceS..");\n\t\tluaL_getmetatable(L,\""..StructName.."\");\n\t\tlua_setmetatable(L,-2);\n\t\treturn 1;"
		elseif RetStr == "" or RetStr == "TNIL" then
			s = "\t\t(*P)->"..FuncName.."("..RealReferenceS..");\n\t\treturn 0;"
		else
			s = "\t\tauto ret = (*P)->"..FuncName.."("..RealReferenceS..");\n\t\treturn LM->"..string.format(Type2SetValue[RetStr],"ret")..";"
		end
		tab[#tab + 1] = s
		tab[#tab + 1] = "\t}"
	end
	tab[#tab + 1] = "\treturn 0;"
	tab[#tab + 1] = "}"
	return tab
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
	self.WriteList[#self.WriteList + 1] = "int "..programInterfaceName.."(lua_State *L);"
	self.WriteList[#self.WriteList + 1] = "static int AutoRegister_Func(lua_State *L);"
	--AutoRegister_Model
	local outkeys = {["{"] = true,["}"] = true,["public:"] = true}
	local ErrorFlg = false

	local switchList = {}
	local Flg = false
	local newtab = TabLoopBackNewTab(temptab,function (k,v)
		local newFuncLists = {}
		local instans = {}
		local nnewtab = TabLoopBackNewTab(v,function (kk,vv)
			if Flg == false then
				if vv == "private:" or vv == "private" then
					Flg = true
					return nil,nil
				end
				local i,l = string.find(vv,"static "..k.."%* ")
				if i and i == 1 then
					local ss = string.sub(vv,l+1,-1)
					if string.find(ss,k) == nil then
						i,l = string.find(ss,"%(");
						assert(i)
						ss = string.sub(ss,1,i-1)
						instans[#instans + 1] = ss
						vv = string.sub(vv,8,-1)
						return kk,vv
					end
				end
				return kk,vv
			else
				if vv == "public" or vv == "public:" then
					Flg = false
				end
				local i = string.find(vv,k.."%(")
				if i and i == 1 then
					newFuncLists[#newFuncLists + 1] = 1
				end
				return nil,nil
			end
		end)
		nnewtab.new = "new"
		if #instans > 0 then
			assert(#instans == 1,"不能获取到对象或者，获取到的方式太多new不知处理方式")
			nnewtab.new = k.."::"..instans[1]
		end
		return k,nnewtab
	end)
	temptab = newtab
	local newtab = TabLoopBackNewTab(temptab,function (k,v)
		if ErrorFlg == true then
			return nil,nil
		end
		if type(v) ~= "table" then
			return nil,nil
		end
		local i = 1
		local new = v.new
		v.new = nil
		local vv = TabLoopBackNewTab(v,function (kkk,vvv)
			if ErrorFlg then return nil,nil end
			if kkk == "switch" then
				switchList[k] = vvv
				return nil,nil
			end
			if type(vvv) ~= "string" or outkeys[vvv] == true then
				return nil,nil
			end
			local index,endindex = string.find(vvv,"%(")
			if index == nil then
				index,endindex = string.find(vvv," ")
				if index then
					return self:DelFirstEndNILChar(string.sub(vvv,index+1,-2) or ""),self:DelFirstEndNILChar(string.sub(vvv,1,index-1) or "")
				else
					ErrorFlg = true--出现了异常 不是关键字也不是函数还不是变量
					return
				end
			else
				--函数处理方式？
				local prve = self:DelFirstEndNILChar(string.sub(vvv,1,index-1))
				local tail = self:DelFirstEndNILChar(string.sub(vvv,index,-1))
				index,endindex = string.find(tail,"%)")
				if index == nil then
					ErrorFlg = true
					return 
				end
				tail = self:DelFirstEndNILChar(string.sub(tail,1,index) or "")
				index,endindex = string.find(vvv," ") or 1,1
				local FuncName,retType = self:DelFirstEndNILChar(string.sub(prve,index,-1) or ""),self:DelFirstEndNILChar(string.sub(prve,1,index-1) or "")
				if index == 1 and endindex == 1 and retType == "" and FuncName ~= k then--构造函数
					if FuncName ~= "~"..k then--析构函数
						--print(vvv)
						ErrorFlg = true--出现了异常 不是关键字也不是函数还不是变量
						return
					else
						return
					end
				end				
				if FuncName == "" or (retType == "" and FuncName ~= k) or tail == "" then
					ErrorFlg = true
					return 
				end
				local newkey,newvalues = self:CreateFuncListTable(k,FuncName,retType,tail)
				if newkey then
					return FuncName,newvalues
				end
				return nil,nil
			end
		end)
		vv.new = new
		return k,vv
	end)
	if ErrorFlg then
		return
	end
	local tab = {}
	self.StructFunLists = {}
	self.NewStructFunLists = {"static const struct luaL_Reg AutoRegister_Struct[] {"}
	local RootFuncCode = {
		"static int AutoRegister_Func(lua_State *L){",
		"\tluaL_newlib(L, AutoRegister_Struct);",
		"\treturn 1;",
		"}",
		"int "..programInterfaceName.."(lua_State* L) {",
		"\tluaL_requiref(L, \""..ModelName.."\",AutoRegister_Func,0);",
		""
	}
	local DelectFunc = function (k,new)
		self.WriteList[#self.WriteList + 1] = "static int AutoRegister_Delete_"..k.."(lua_State *L);"
		local DelFunCode = {
			"static int AutoRegister_Delete_"..k.."(lua_State *L){",
			"\t"..k.."** P = ("..k.."**)luaL_checkudata(L, 1, \""..k.."\");",
			"\tluaL_argcheck(L, P != NULL, 1, \"invalid user data\");",
		}
		if new == "new" then
			DelFunCode[#DelFunCode + 1] = "\tdelete *P;"
		end
		DelFunCode[#DelFunCode + 1] = "\treturn 0;"
		DelFunCode[#DelFunCode + 1] = "}"
		return "AutoRegister_Delete_"..k,DelFunCode
	end
	for k,v in pairs(newtab) do
		tab[k] = {}
		self.StructFunLists[k] = {"static const struct luaL_Reg AutoRegister_Struct_"..k.."[] {"}
		if v[k] == nil and new == "new" then
			v[k] = {{}}
		end
		local new = v.new
		local getObjFunc = k
		if new ~= "new" then
			getObjFunc = string.sub(new,string.len(k..":: "),-1)
		end
		v.new = nil
		for kk,vv in pairs(v) do
			if type(vv) == "table" then
				if getObjFunc == kk then
					print(kk)
					tab[k].class = self:FuncHandler(k,k,vv,new,true)
				else
					tab[k][#tab[k] + 1] = self:FuncHandler(k,kk,vv,new,false)
				end
			else
				--添加get和set函数
				 tab[k][#tab[k] + 1] = self:ValHandler(k,kk,vv)
			end
		end
		local len = #self.StructFunLists[k]
		RootFuncCode[#RootFuncCode + 1] = "\tluaL_newmetatable(L,\""..k.."\");"
		RootFuncCode[#RootFuncCode + 1] = "\tlua_pushvalue(L,-1);"
		RootFuncCode[#RootFuncCode + 1] = "\tlua_setfield(L, -2, \"__index\");"
		RootFuncCode[#RootFuncCode + 1] = "\tluaL_setfuncs(L, AutoRegister_Struct_"..k..", 0);"
		RootFuncCode[#RootFuncCode + 1] = "\tlua_pop(L,1);"
		RootFuncCode[#RootFuncCode + 1] = ""
		local Name,Code = DelectFunc(k,new)
		tab[k][#tab[k] + 1] = Code
		self.StructFunLists[k][len+1] = "\t{\"__gc\","..Name.."},"
		self.StructFunLists[k][len+2] = "\t{NULL,NULL}"		
		self.StructFunLists[k][len+3] = "};"
	end
	self.NewStructFunLists[#self.NewStructFunLists + 1] = "\t{NULL,NULL}"
	self.NewStructFunLists[#self.NewStructFunLists + 1] = "};"
	RootFuncCode[#RootFuncCode + 1] = "\treturn 0;"
	RootFuncCode[#RootFuncCode + 1] = "}"
	self.WriteBuffer = {self.WriteList,self.StructFunLists,tab,self.NewStructFunLists,RootFuncCode}
	self:Write()
	--函数和变量分开，函数兼容重载以及参数列表分开
	if ErrorFlg == true then
		return
	end
	--重载处理？
end
Server:LoadSrcCode()
--print(string.find("[((","%("))
--print(GetTableString(string.split("sdfsd a,fsfxcvb,d,eafdsf,zcv",",")))