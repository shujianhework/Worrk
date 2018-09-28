local tm = require "testModel"
local Scheduler = tm:LSchedulerCell()
local tab = {a = "123q234",c = "dsfsd"}
print(Scheduler:testmap(tab))
print(Scheduler:testmap({a=1,b=100,c=100,f = 124},true))