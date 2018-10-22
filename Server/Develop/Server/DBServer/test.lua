local tm = require "testModel"
require "initLua"
_G.tool = tm:Tool()
if _G.tool and _G.tool.print then
	--[[print = function(...)
		local s = GetTableString(...)
		_G.tool:print(0,s)
	end]]--
end
_G.Scheduler = tm:LSchedulerCell()
Scheduler:setHandlerBack(function (id,dt,...)
	print(id,dt,...)
end)
Scheduler:setTimer(100,true)
Scheduler:setTimer(100,true)
Scheduler:setTimer(100,true)
Scheduler:setTimer(100,true)
_G.db = tm:SJH_DB_SQL_DBTaskEvent()
db:Run()
local l = 0
db:setLuaBack(function ( ... )
	print(...)
end,function ( ... )
	print(...)
end,function ( ... )
	print(...)
end,function ( ... )
	local s = ""..l..","
	local tab = {...}
	for k,v in pairs(tab) do
		s = s..","..type(v)
		if type(v) == "table" then
			s = s.."{"
			for kk,vv in pairs(v) do
				s = s..kk..","..vv
			end
			s = s.."}"
		elseif type(v) == type(true) then
			s = s..(v and "true" or "false")
		else
			s = s..v
		end
	end
	print(s)
	l = l + 1
end)
db:push("Driver={sql server};server=47.75.78.46;uid=sa;pwd=shu110SHUsa;database=ply_web","ply_web")
db:push("select * from Area ")